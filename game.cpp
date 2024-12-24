#include "game.h"
#include <QDebug>
#include <QTimer>
#include <algorithm> // std::next_permutation

extern std::vector<std::vector<QString>> patterns;

Game::Game(int numberOfPlayers, QObject* parent)
    : QObject(parent)
    , numberOfPlayers_(numberOfPlayers)
    , mediaPlayer_(QSharedPointer<QMediaPlayer>::create())
    , audioOutput_(QSharedPointer<QAudioOutput>::create())
    , isCardsVisible_(false)
    , isSoundOn_(false)
    , monitor_(QSharedPointer<Monitor>::create())
    , eightsChooser_(QSharedPointer<EightsChooser>::create())
    , quteChooser_(QSharedPointer<QuteChooser>::create())
    , jpointsChooser_(QSharedPointer<JpointsChooser>::create())
    , roundChooser_(QSharedPointer<RoundChooser>::create())
    , played_(QSharedPointer<Played>::create())
    , blind_(QSharedPointer<Blind>::create())
    , jsuitChooser_(QSharedPointer<JsuitChooser>::create())
    , stack_(QSharedPointer<Stack>::create())
    , got1_(QSharedPointer<Got>::create())
    , got2_(QSharedPointer<Got>::create())
    , playable_(QSharedPointer<Playable>::create())
    , drawn_(QSharedPointer<Drawn>::create())
    , lcdRound_(QSharedPointer<QLCDNumber>::create())
    , lcdP1_(QSharedPointer<QLCDNumber>::create())
    , lcdP2_(QSharedPointer<QLCDNumber>::create())
    , lcdP3_(QSharedPointer<QLCDNumber>::create())
    , lcdShuffles_(QSharedPointer<QLCDNumber>::create())
{
    player1_ = QSharedPointer<Player>::create(1, "Player1", false, 0);
    player2_ = QSharedPointer<Player>::create(2, "Player2", true, 0);
    player3_ = nullptr; // or create a dummy player for the third slot

    playerList_.clear();
    playerList_.push_back(player1_);
    playerList_.push_back(player2_);

    connect(player1()->handdeck().get(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);
    connect(player2()->handdeck().get(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);

    if (numberOfPlayers_ == 3) {
        player3_ = QSharedPointer<Player>::create(3, "Player3", true, 0);
        playerList_.push_back(player3_);
        connect(player3()->handdeck().get(),
                &Handdeck::handCardClicked,
                this,
                &Game::onHandCardClicked);
    }

    connect(this, &Game::cardAddedToStack, played_.get(), &Played::onCardAddedToStack);
    connect(this, &Game::cardAddedToStack, monitor_.get(), &Monitor::onCardAddedToStack);
    connect(this, &Game::cardMustFromBlind, drawn_.get(), &Drawn::onCardMustFromBlind);
    connect(this, &Game::cardsPlayed, stack_.get(), &Stack::onCardsPlayed);

    foreach (const auto& player, playerList_) {
        connect(this, &Game::countPoints, player.get(), &Player::onCountPoints);
    }

    connect(this, &Game::cardAddedToStack, jsuitChooser_.get(), &JsuitChooser::onCardAddedToStack);

    connect(quteChooser().get(),
            &QuteChooser::quteDecisionChanged,
            roundChooser().get(),
            &RoundChooser::onQuteDecisionChanged);

    connect(roundChooser().get(), &RoundChooser::finishRound, this, &Game::handleSpecialCards);
    connect(roundChooser().get(), &RoundChooser::newRound, this, &Game::onNewRound);
    connect(roundChooser().get(), &RoundChooser::newGame, this, &Game::onNewGame);

    connect(this,
            &Game::toggleCardsVisible,
            player2()->handdeck().get(),
            &Handdeck::onToggleCardsVisible);

    if (numberOfPlayers == 3) {
        connect(this,
                &Game::toggleCardsVisible,
                player3()->handdeck().get(),
                &Handdeck::onToggleCardsVisible);
    }

    connect(this, &Game::toggleCardsVisible, got1().get(), &Got::onToggleCardsVisible);
    connect(this, &Game::toggleCardsVisible, got2().get(), &Got::onToggleCardsVisible);
    connect(this, &Game::toggleCardsVisible, blind().get(), &Blind::onToggleCardsVisible);
    connect(this, &Game::toggleCardsVisible, playable().get(), &Playable::onToggleCardsVisible);
    connect(this, &Game::toggleCardsVisible, drawn().get(), &Drawn::onToggleCardsVisible);

    // in handdeck of player 1 card face always shown
    connect(this,
            &Game::toggleCardsVisible,
            player1()->handdeck().get(),
            &Handdeck::onToggleCardsVisible);

    mediaPlayer_->setAudioOutput(audioOutput_.data());

    initializeRound();
}

Game::~Game()
{
    // Unique pointers will automatically clean up the resources
}

// Getters
QSharedPointer<Player> Game::player1() const
{
    return player1_;
}

QSharedPointer<Player> Game::player2() const
{
    return player2_;
}

QSharedPointer<Player> Game::player3() const
{
    return player3_;
}

QSharedPointer<Monitor> Game::monitor()
{
    return monitor_;
}

QSharedPointer<EightsChooser> Game::eightsChooser()
{
    return eightsChooser_;
}

QSharedPointer<QuteChooser> Game::quteChooser()
{
    return quteChooser_;
}

QSharedPointer<JpointsChooser> Game::jpointsChooser()
{
    return jpointsChooser_;
}

QSharedPointer<RoundChooser> Game::roundChooser()
{
    return roundChooser_;
}

QSharedPointer<Played> Game::played()
{
    return played_;
}

QSharedPointer<QLCDNumber> Game::lcdRound()
{
    return lcdRound_;
}
QSharedPointer<QLCDNumber> Game::lcdP1()
{
    return lcdP1_;
}

QSharedPointer<QLCDNumber> Game::lcdP2()
{
    return lcdP2_;
}

QSharedPointer<QLCDNumber> Game::lcdP3()
{
    return lcdP3_;
}

QSharedPointer<QLCDNumber> Game::lcdShuffles()
{
    return lcdShuffles_;
}

QSharedPointer<Blind> Game::blind()
{
    return blind_;
}

QSharedPointer<JsuitChooser> Game::jsuitChooser()
{
    return jsuitChooser_;
}

QSharedPointer<Stack> Game::stack()
{
    return stack_;
}

QSharedPointer<Got> Game::got1()
{
    return got1_;
}

QSharedPointer<Got> Game::got2()
{
    return got2_;
}

QSharedPointer<Playable> Game::playable()
{
    return playable_;
}

QSharedPointer<Drawn> Game::drawn()
{
    return drawn_;
}

// Methods
void Game::initializeRound()
{
    jpointsChooser()->setDisabled(true);
    jpointsChooser()->hide();
    roundChooser()->setEnabled(false);
    roundChooser()->hide();

    // emit setRbNumPlayers(numberOfPlayers_);

    collectAllCardsToBlind();

    monitor_->clearCards();
    played_->clearCards();
    drawn_->clearCards();
    got1_->clearCards();
    got2_->clearCards();

    // shuffle the cards
    if (isSoundOn_) {
        mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":/sounds/shuffling.wav"));
        mediaPlayer_->play();
    }
    blind_->shuffle();
    shuffles = 1;
    lcdShuffles()->display(shuffles);
    lcdRound()->display(rounds);

    // Player with highest score will start
    togglePlayerListToScore(true);

    // Distribute cards
    for (int k = 1; k <= 5; k++) {
        blind_->moveTopCardTo(player2_->handdeck().get());
    }

    if (numberOfPlayers_ == 3) {
        for (int k = 1; k <= 5; k++) {
            blind_->moveTopCardTo(player3_->handdeck().get());
        }
    }

    for (int k = 1; k <= 5; k++) {
        blind_->moveTopCardTo(player1_->handdeck().get());
    }

    for (const auto& player : std::as_const(playerList_)) {
        player->handdeck()->setEnabled(false);
    }

    // active player is first player in playerList_
    player = playerList_.front();
    player->handdeck()->setEnabled(true);
    player->handdeck()->cards().last()->click();

    emit setCbVisible(false);
    // if (forAndroid)
    //     emit resetCbVisible(false);

    // in case a robot player starts a new round all playable cards are played
    autoplay();
}

void Game::onHandCardClicked(const QSharedPointer<Card>& card)
{
    if (isThisCardPlayable(card)) {
        if (isSoundOn_) {
            mediaPlayer_->stop(); // Stop any previous playback
            mediaPlayer_->setSource(QUrl("qrc:/sounds/put_card_on_stack.wav"));
            mediaPlayer_->play();
        }
        emit cardAddedToStack(card);
        player->handdeck()->moveCardTo(card, stack().get());

        updatePlayable();
        handleChoosers();
    }
}

void Game::handleChoosers()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    // JsuitChooser
    if (stackCard->rank() == "J") {
        if (!player->handdeck()->cards().isEmpty() && !played()->cards().isEmpty())
            // KI toggle JSuit to rank with most points
            // jsuitChooser()->toggle_to(player->handdeck()->mostCommonSuit());
            jsuitChooser()->toggle_to(player->handdeck()->suitOfRankWithMostPoints());
        // end KI toggle JSuit to rank with most points

        // Do not allow Player1 toggle JSuitChooser:
        jsuitChooser()->setDisabled(player->isRobot());
        jsuitChooser()->show();
        // JsuitChooser is shown until next player's card is added to stack
        // (slot: onCardAddedToStack)
    }

    // EightsChooser
    if (stackCard->rank() == "8" && monitor()->cards().size() >= 2
        && played()->cards().size() >= 2) {
        // 2 Players
        if (numberOfPlayers_ == 2) {
            // if 2 players not allowing toggling to "a"
            eightsChooser()->toggle_to("n");
            eightsChooser()->setDisabled(true);
            eightsChooser()->show();
        }

        // 3 Players
        if (numberOfPlayers_ == 3) {
            // KI EightsChooser

            /*      number of cards hold by ...
             *      next overnext player        decision
             *      == 1   >  1                     n
             *      >  1   == 1                     a
             *      last 8's suit in handdeck       a
             *         else                         n
            */

            if (player->isRobot()) {
                if (playerList_[1]->handdeck()->cards().size() == 1
                    && playerList_[2]->handdeck()->cards().size() > 1) {
                    eightsChooser()->toggle_to("n");
                }

                else if (playerList_[1]->handdeck()->cards().size() > 1
                         && playerList_[2]->handdeck()->cards().size() == 1) {
                    eightsChooser()->toggle_to("a");
                }

                // if suit of last played 8 in handdeck toggle_to('a')
                else if (player->handdeck()->isSuitInCards(stackCard->suit())) {
                    eightsChooser()->toggle_to("a");
                }

                else
                    // eightsChooser()->toggleRandom();
                    eightsChooser()->toggle_to("n");
            }
        }
        // end KI EightsChooser

        eightsChooser()->setDisabled(player->isRobot());
        eightsChooser()->show();
    }

    // no Eights condition:
    else {
        eightsChooser()->hide();
        eightsChooser()->setEnabled(false);
    }

    // QuteChooser
    if (monitor()->cards().size() == 4 && played()->cards().size() > 0) {
        int jacks = 0;
        if (stackCard->rank() == "J") {
            for (const auto& card : std::as_const(played()->cards())) {
                if (card->rank() == "J") {
                    jacks++;
                }
            }
            connect(quteChooser().get(),
                    &QuteChooser::quteDecisionChanged,
                    jpointsChooser().get(),
                    &JpointsChooser::onQuteDecisionChanged);
        }

        // KI QuteChooser

        /*                                          decision
         *  points on hand - jacks * 20 <    0         y (JpointsChooser = m)
         *  score                       == 125         y (JpointsChooser = auto m)
         *              else                           n
        */

        if ((player->handdeck()->pointsOnHand() - jacks * 20) <= 0) {
            quteChooser()->toggle_to("y");
        }

        // score == 125
        else if (player->handdeck()->pointsOnHand() * shuffles + player->score() == 125) {
            quteChooser()->toggle_to("y");
        }

        // score == 125
        else if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                 == 125) {
            quteChooser()->toggle_to("y");
        }

        else
            quteChooser()->toggle_to("n");

        // end KI QuteChooser

        quteChooser()->setDisabled(player->isRobot());
        quteChooser()->show();
    }

    // No Qute Condition
    else {
        disconnect(quteChooser().get(),
                   &QuteChooser::quteDecisionChanged,
                   jpointsChooser().get(),
                   &JpointsChooser::onQuteDecisionChanged);
        quteChooser()->hide();
        quteChooser()->setDisabled(true);
        quteChooser()->toggle_to("y"); // prepare for next 'Qute'
    }

    // JPointsChooser
    if (stackCard->rank() == "J" && player->handdeck()->cards().isEmpty()
        || (stackCard->rank() == "J")
               && (quteChooser()->isVisible() && quteChooser()->decision() == "y")) {
        // KI JPointsChooser
        if (player->isRobot()) {
            int jacks = 0;
            for (const auto& card : std::as_const(played()->cards())) {
                if (card->rank() == "J") {
                    jacks++;
                }
            }

            if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                == 125) {
                jpointsChooser()->toggle_to("m");
            }

            else if (player->handdeck()->pointsOnHand() * shuffles + player->score() == 125) {
                jpointsChooser()->toggle_to("p");
            }

            else if ((player->handdeck()->pointsOnHand() - jacks * 20) <= 0) {
                jpointsChooser()->toggle_to("m");
            } else
                jpointsChooser()->toggleRandom();
        }
        // end KI JPointsChooser

        jpointsChooser()->toggle_to(jpointsChooser()->decision());
        jpointsChooser()->setDisabled(player->isRobot());
        jpointsChooser()->show();
    }

    // No Jpoints Condition
    else {
        jpointsChooser()->hide();
        jpointsChooser()->setEnabled(false);
    }

    // RoundChooser
    if (player->handdeck()->cards().isEmpty() && stack()->topCard()->rank() != '6'
        || quteChooser()->isVisible() && quteChooser()->decision() == "y") {
        roundChooser()->setDecision("f");
        roundChooser()->setEnabled(true);
        roundChooser()->show();
    } else {
        roundChooser()->hide();
        roundChooser()->setEnabled(false);
    }
}

void Game::updatePlayable()
{
    playable()->clearCards();

    foreach (const auto& card, player->handdeck()->cards()) {
        if (isThisCardPlayable(card)) {
            player->handdeck()->copyCardTo(card, playable().get());
        }
    }
}

bool Game::isThisCardPlayable(const QSharedPointer<Card>& card)

/*
    1st move:
    ---------
    if stack_card == 'J': Jsuit   rank=J
    general rule:          suit   rank     J

    2nd move:
    ---------
    if stack_card == '6': suit    rank     J
    general rule:                 rank
    */

{
    QSharedPointer<Card> stackCard = stack()->topCard();

    // for playing first card when stack is empty
    // player1_->handdeck()->cards().last()->click();
    if (!stackCard) {
        return true;
    }

    // 1st move:
    // if stack card is 'J'
    if (played()->cards().isEmpty() && stackCard->rank() == "J") {
        if (card->suit() == jsuitChooser()->suit() || card->rank() == stackCard->rank()) {
            return true;
        }
        return false;
    }

    // General rule for first move
    if (played()->cards().isEmpty()) {
        if (card->suit() == stackCard->suit() || card->rank() == stackCard->rank()
            || card->rank() == "J") {
            return true;
        }
        return false;
    }

    // 2nd move:
    // if stack card is '6'
    if (!played()->cards().isEmpty() && stackCard->rank() == "6") {
        if (card->suit() == stackCard->suit() || card->rank() == stackCard->rank()
            || card->rank() == "J") {
            return true;
        }
        return false;
    }

    // General rule for other moves
    if (!played()->cards().isEmpty()) {
        if (card->rank() == stackCard->rank()) {
            return true;
        }
        return false;
    }

    return false;
}

bool Game::isMustDrawCard()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    updatePlayable();

    // played card '6' must be covered
    if (stackCard->rank() == "6" && playable()->cards().isEmpty()) {
        // emit setBlindRed(true);
        return true;
    }

    // at least one card must be played or drawn (if no playable card on hand)
    if (played()->cards().isEmpty() && playable()->cards().isEmpty() && drawn()->cards().isEmpty()) {
        // emit setBlindRed(true);
        return true;
    }

    return false;
}

/*      
            ----------------------------------------------------
             card   playable  card    next player
            played    card    drawn   possible
                1       1       1         Y
                1       1       0         Y
                1       0       1         Y
                1       0       0         Y
                0       1       1         N
                0       1       0         N
                0       0       1         Y
                0       0       0         N   <-- must draw card
            ----------------------------------------------------
            '6' on stack:
                6       1      0||1       N
                6       0      0||1       N   <-- must draw card
*/

bool Game::isNextPlayerPossible()
{
    if (isRoundFinished()) {
        return true;
    }

    // while (isMustDrawCard()) {
    if (isMustDrawCard()) {
        drawCardFromBlind(Game::DrawOption::MustCard);
        updatePlayable(); // needed for if, not for while
        // human player must draw card by card
    }

    QSharedPointer<Card> stackCard = stack()->topCard();
    if (stackCard->rank() == "6") {
        return false;
    }

    updatePlayable();

    return !played()->cards().isEmpty()
           || (playable()->cards().isEmpty() && !drawn()->cards().isEmpty());
}

void Game::drawCardFromBlind(DrawOption option)
{
    if (blind()->cards().isEmpty())
        refillBlindFromStack();

    if (option == DrawOption::MustCard) {
        emit cardMustFromBlind(blind()->topCard());
    } else if (option == DrawOption::BadCard) {
        emit cardBadFromBlind(blind()->topCard());
    }

    if (isSoundOn_) {
        mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":/sounds/draw_card_from_blind.wav"));
        mediaPlayer_->play();
    }

    blind()->moveTopCardTo(player->handdeck().get());
}

void Game::rotatePlayerList()
{
    played()->clearCards();
    drawn()->clearCards();

    // jsuitChooser()->setEnabled(false); // handled by slot onCardAddedToStack

    eightsChooser()->setEnabled(false);
    jpointsChooser()->setEnabled(false);
    quteChooser()->setEnabled(false);
    eightsChooser()->hide();
    // jpointsChooser()->hide();    // don't hide when counting points
    quteChooser()->hide();

    if (!playerList_.isEmpty()) {
        player = playerList_.front();
        playerList_.pop_front();
        playerList_.push_back(player);
    }

    for (const auto& player : std::as_const(playerList_)) {
        player->handdeck()->setEnabled(false);
    }

    player = playerList_.front();
    player->handdeck()->setEnabled(true);

    updatePlayable();
}

void Game::togglePlayerListToScore(bool highest)
{
    if (playerList_.empty())
        return;

    // Find the player with the extreme score (highest or lowest)
    auto extremePlayerIt = highest ? std::max_element(playerList_.begin(),
                                                      playerList_.end(),
                                                      [](const QSharedPointer<Player>& a,
                                                         const QSharedPointer<Player>& b) {
                                                          return *a < *b;
                                                      })
                                   : std::min_element(playerList_.begin(),
                                                      playerList_.end(),
                                                      [](const QSharedPointer<Player>& a,
                                                         const QSharedPointer<Player>& b) {
                                                          return *a < *b;
                                                      });

    // Rotate the list so the extreme player is at the front
    std::rotate(playerList_.begin(), extremePlayerIt, playerList_.end());

}

bool Game::isRoundFinished()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    if (roundChooser()->isEnabled()) {
        return true;
    }

    if (quteChooser()->isEnabled() && quteChooser()->decision() == "y") {
        return true;
    }

    if (player->handdeck()->cards().isEmpty() && stackCard->rank() != "6") {
        return true;
    }
    return false;
}

void Game::handleSpecialCards()
{
    // Count occurrences of sevens, eights, and aces
    int jacks = 0;
    int sevens = 0;
    int aces = 0;
    int eights = 0;

    for (const auto& card : std::as_const(played()->cards())) {
        if (card->rank() == "7") {
            sevens++;
        } else if (card->rank() == "8") {
            eights++;
        } else if (card->rank() == "A") {
            aces++;
        } else if (card->rank() == "J") {
            jacks++;
        }
    }

    // shuffles are considered when players count their points
    if (jpointsChooser()->isVisible()) {
        // Calculate points based on the minimum size of played and monitor card sets
        int points = 20 * jacks;

        // Set points for the current player
        if (jpointsChooser()->decision() == "m") {
            player->setJpoints(-points);
        }

        // If the decision is "p", set points for all players in the player list
        else if (jpointsChooser()->decision() == "p") {
            for (const auto& player : std::as_const(playerList_)) {
                player->setJpoints(points);
            }
            player->setJpoints(0); // active Player
        }
    }

    QSharedPointer<CardVec> cardVec(new CardVec(nullptr, played_->cards()));

    bool isFinished = isRoundFinished();

    // Rotate the player list normally first
    rotatePlayerList();

    got1()->clearCards();
    got2()->clearCards();

    for (int i = 0; i < sevens; ++i) {
        connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
        drawCardFromBlind(Game::DrawOption::BadCard);
        // if Player1 gets a card, cards are unsorted
        if (!player->isRobot())
            emit setRbUnsorted(true);
        disconnect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
    }

    if (aces > 0) {
        int leap = 1;
        while (leap <= aces) {
            if (leap % playerList_.size() != 0) {
                rotatePlayerList();
            } else {
                rotatePlayerList();
                aces++;
            }
            leap++;
        }
    }

    if (eights >= 2 && eightsChooser()->decision() == "a") {
        int leap = 1;
        int num = 0;
        while (leap <= eights) {
            if (leap % playerList_.size() != 0) {
                if (num % 2 == 0)
                    connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
                else
                    connect(this, &Game::cardBadFromBlind, got2_.get(), &Got::onCardBadFromBlind);
                drawCardFromBlind(Game::DrawOption::BadCard);
                drawCardFromBlind(Game::DrawOption::BadCard);
                // if Player1 gets a card, cards are unsorted
                if (!player->isRobot())
                    emit setRbUnsorted(true);
                disconnect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
                disconnect(this, &Game::cardBadFromBlind, got2_.get(), &Got::onCardBadFromBlind);
                rotatePlayerList();
                num++;
            } else {
                rotatePlayerList();
                eights++;
            }
            leap++;
        }
        eights = 0;
    }

    if (eights > 0) {
        connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
        for (int i = 0; i < eights; ++i) {
            drawCardFromBlind(Game::DrawOption::BadCard);
            drawCardFromBlind(Game::DrawOption::BadCard);
            // if Player1 gets a card, cards are unsorted
            if (!player->isRobot())
                emit setRbUnsorted(true);
        }
        disconnect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
        rotatePlayerList();
    }

    // when active player draws for '7' got1 is setEnabled(true)
    got1()->setEnabled(sevens > 0);
    got2()->setEnabled(false);

    played()->clearCards();
    drawn()->clearCards();

    if (isFinished) {
        // make all cards visible for counting points
        if (forAndroid) {
            for (const auto& player : playerList_) {
                player->handdeck()->setEnabled(true);
            }
        }
        //
        emit countPoints(shuffles);
        emit setCbVisible(true);
        playable()->clearCards();
        updateDisplay();
    }
}

void Game::activateNextPlayer()
{
    if (!isNextPlayerPossible())
        return;

    if (isRoundFinished())
        return;

    // // Delay activation of next player for:
    // if (playerList_[0]->name() == "Player1" and drawn()->cards().size() > 0
    //     and playable()->cards().size() == 0) {
    //     qDebug() << "pausing for 2 seconds...";
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    // }

    // // Check that 36 cards are in the game for (auto& player : playerList_)
    // int cardsInGame = 0;

    // for (auto player : playerList_) {
    //     cardsInGame += player->handdeck()->cards().size();
    // }

    // cardsInGame += blind()->cards().size() + stack()->cards().size();

    // qDebug() << "Cards in game:" << cardsInGame;
    // // end of check

    emit cardsPlayed(played()->cards().size());

    handleSpecialCards();

    updatePlayable();

    autoplay();
    if (forAndroid and isCardsVisible_) {
        emit resetCbVisible(isCardsVisible_);
    }
}

void Game::autoplay()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    if (player->isRobot() && !roundChooser()->isEnabled()) {
        player->handdeck()->setEnabled(true);

        while (!isNextPlayerPossible()) {
            // TODO: KI make robots play all possible ranks at first move
            while (!playable()->cards().isEmpty()) { // play all cards with same rank

                QString stackSuit = stackCard->suit();
                if (stackCard->rank() == "J")
                    stackSuit = jsuitChooser()->suit();

                QVector<QString> pattern = player->handdeck()->patternByRankPoints();

                // KI rankPoints 6, 7, 8, J, A
                // the other player holds only one card
                if (playerList_[1]->handdeck()->cards().size() == 1) {
                    player->handdeck()->prependRank(pattern, "7");
                    player->handdeck()->prependRank(pattern, "8");
                } else if (player->score()
                               + player->handdeck()->countCardsOfRank("A") * 15 * shuffles
                           == 125)
                    player->handdeck()->appendRank(pattern, "A");
                else {
                    player->handdeck()->prependRank(pattern, "6");
                    player->handdeck()->appendRank(pattern, "J");
                }
                // end KI rankPoints 6, 7, 8, J, A

                player->handdeck()->sortCardsByPattern(pattern);
                playable()->sortCardsByPattern(pattern);

                // qDebug() << "pattern used:" << pattern;

                // KI permute ranks
                player->handdeck()->permuteRanks(playable()->cards().front()->rank(),
                                                 stackCard,
                                                 stackSuit);
                // end KI permute ranks

                for (const auto& card : std::as_const(player->handdeck()->cards())) {
                    card->click();
                }
                updatePlayable();
            }
        }
        emit cardsPlayed(played()->cards().size());
        handleChoosers();
    }
}

void Game::refillBlindFromStack()
{
    if (stack()->cards().size() <= 1) {
        qDebug() << "Not enough cards in the stack to refill the blind.";
        return;
    }

    // Reverse the order of cards in the stack
    std::reverse(stack()->cards().begin(), stack()->cards().end());

    // Move all but the original last card (now first) from stack to blind
    while (stack()->cards().size() > 1) {
        stack()->moveTopCardTo(blind().get());
    }

    // Shuffle the blind deck
    if (isSoundOn_) {
        mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":/sounds/shuffling.wav"));
        mediaPlayer_->play();
    }
    blind()->shuffle();
    shuffles += 1;
    lcdShuffles()->display(shuffles);

    // qDebug() << "Blind refilled and shuffled.";
}

void Game::collectAllCardsToBlind()
{
    // Move all cards from the stack to the blind
    while (!stack_->cards().isEmpty()) {
        stack()->moveTopCardTo(blind().get());
    }

    // Move all cards from each player's handdeck to the blind
    for (auto& player : playerList_) {
        while (!player->handdeck()->cards().isEmpty()) {
            player->handdeck()->moveTopCardTo(blind().get());
        }
    }
}

void Game::updateDisplay()
{
    for (const auto& player : std::as_const(playerList_)) {
        if (player->id() == 1) {
            lcdP1()->display(player->score());
        } else if (player->id() == 2) {
            lcdP2()->display(player->score());
        } else if (player->id() == 3) {
            lcdP3()->display(player->score());
        }
    }
}

void Game::onCbVisible(bool isVisible)
{
    isCardsVisible_ = isVisible;
    emit toggleCardsVisible(isVisible);
}


void Game::onCbSound(int state)
{
    isSoundOn_ = state;
}

void Game::onRbSuit()
{
    player->handdeck()->sortCardsBy(Handdeck::SortOption::Suit);
}
void Game::onRbRank()
{
    player->handdeck()->sortCardsBy(Handdeck::SortOption::Rank);
}

// void Game::onBlindClicked()
// {
//     if (!player->isRobot() && isMustDrawCard()) {
//         drawCardFromBlind(Game::DrawOption::MustCard);
//         updatePlayable();
//         // emit setBlindRed(false);
//     }
// }

void Game::onNewRound()
{
    for (const auto& player : std::as_const(playerList_)) {
        player->setJpoints(0);
    }

    togglePlayerListToScore(true);
    if (playerList_.front()->score() <= 125) {
        // qDebug() << "Starting new Round ...";
        rounds += 1;
        initializeRound();
    } else {
        // qDebug() << "Game is over...";
        roundChooser()->setDecision("g");
    }
}

void Game::onNewGame()
{
    emit countPoints(0);

    games += 1;
    rounds = 1;

    updateDisplay();

    // qDebug() << "Starting new game ...";

    initializeRound();
}
