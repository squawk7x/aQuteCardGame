#include "game.h"
#include <QDebug>
#include <QTimer>
#include <algorithm> // std::next_permutation
#include <qforeach.h>

extern std::vector<std::vector<QString>> patterns;

Game::Game(int numberOfPlayers, QObject* parent)
    : QObject(parent)
    , numberOfPlayers_(numberOfPlayers)
    , mediaPlayer_(QSharedPointer<QMediaPlayer>::create())
    , audioOutput_(QSharedPointer<QAudioOutput>::create())
    , isSoundOn_(false)
    , isCardsVisible_(false)
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

    // Handdeck of player 1 card face always isVisible == true
    // connect(this,
    //         &Game::toggleCardsVisible,
    //         player1()->handdeck().get(),
    //         &Handdeck::onToggleCardsVisible);

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
        // mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":res/sounds/shuffling.wav"));
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

    emit setCbVisible(isCardsVisible_);
    emit toggleCardsVisible(isCardsVisible_);
    emit setRbCardType(cardType::small);
    onRbCardType(cardType::small);

    autoplay();
}

void Game::onHandCardClicked(const QSharedPointer<Card>& card)
{
    if (isThisCardPlayable(card)) {
        if (isSoundOn_) {
            // mediaPlayer_->stop(); // Stop any previous playback
            mediaPlayer_->setSource(QUrl("qrc:res/sounds/put_card_on_stack.wav"));
            mediaPlayer_->play();
        }
        player->handdeck()->moveCardTo(card, stack().get());
        emit cardAddedToStack(card);

        updatePlayable();
        handleChoosers();
    }
}

void Game::handleChoosers()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    // -----------------------------------------------------------------------

    // JsuitChooser
    if (stackCard->rank() == "J") {
        if (!player->handdeck()->cards().isEmpty() && !played()->cards().isEmpty())
            // jsuitChooser()->toggle_to(player->handdeck()->mostCommonSuit());
            jsuitChooser()->toggle_to(player->handdeck()->suitOfRankWithMostPoints());
        // end KI toggle JSuit to rank with most points

        // Do not allow Player1 toggle JSuitChooser:
        jsuitChooser()->setDisabled(player->isRobot());
        jsuitChooser()->show();
        // JsuitChooser is shown until next player's card is added to stack
        // and removed by cardAddedToStack -> onCardAddedToStack

        // Eventloop in Android is different to eventloop on PC.
        emit jsuitChooser()->chooserToggled();
    }

    // -----------------------------------------------------------------------

    // EightsChooser
    if (stackCard->rank() == "8" && monitor()->cards().size() >= 2
        && played()->cards().size() >= 2) {
        // 2 Players
        if (numberOfPlayers_ == 2) {
            // if 2 players -> NOT allowing toggling to "ALL"
            eightsChooser()->toggle_to(QString("NEXT"));

            emit eightsChooser()->chooserToggled();
        }

        // 3 Players
        if (numberOfPlayers_ == 3) {
            // KI EightsChooser

            /* number of cards hold by next overnext player    decision
             *                         == 1        >  1          NEXT
             *                         >  1        == 1          ALL
             *                     last 8's suit in handdeck     ALL
             *                               else                NEXT / RANDOM
            */

            if (player->isRobot()) {
                if (playerList_[1]->handdeck()->cards().size() == 1
                    && playerList_[2]->handdeck()->cards().size() > 1) {
                    eightsChooser()->toggle_to(QString("NEXT"));
                }

                else if (playerList_[1]->handdeck()->cards().size() > 1
                         && playerList_[2]->handdeck()->cards().size() == 1) {
                    eightsChooser()->toggle_to(QString("ALL"));
                }

                // if suit of last played 8 in handdeck toggle_to('a')
                else if (player->handdeck()->isSuitInCards(stackCard->suit())) {
                    eightsChooser()->toggle_to(QString("ALL"));
                }

                else
                    // eightsChooser()->toggleRandom();
                    eightsChooser()->toggle_to(QString("NEXT"));
            }
        }
        // end KI EightsChooser

        eightsChooser()->setDisabled(player->isRobot());
        eightsChooser()->show();

        emit eightsChooser()->chooserToggled();
    }

    // no Eights condition:
    else {
        eightsChooser()->hide();
        eightsChooser()->setEnabled(false);
    }

    // -----------------------------------------------------------------------

    // QuteChooser

    // Qute Condition -> connect Q -> J
    if (monitor()->cards().size() == 4 && played()->cards().size() > 0) {
        connect(quteChooser().get(),
                &QuteChooser::quteDecisionChanged,
                jpointsChooser().get(),
                &JpointsChooser::onQuteDecisionChanged);

        if (stackCard->rank() == "J") {
            // jpointsChooser()->setDisabled(player->isRobot());
            // jpointsChooser()->show();
            // Qute && Jpoints
            //           Condition                            Decision
            // score by PLUS or MINUS Jpoints == 125      QUTE      PLUS / MINUS
            // score by PLUS or MINUS Jpoints >  100      QUTE         MINUS
            // points on hand - jacks * 20 <=    0        QUTE         MINUS
            //             else                           CONTINUE      hide
            //
            int jacks = 0;
            for (const auto& card : std::as_const(played()->cards())) {
                if (card->rank() == "J") {
                    jacks++;
                }
            }

            // score == 125 -> score = 0
            if ((player->handdeck()->pointsOnHand()) * shuffles + player->score() == 125) {
                jpointsChooser()->toggle_to("PLUS");
                quteChooser()->toggle_to(QString("QUTE"));
            }

            // score == 125 -> score = 0
            else if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                     == 125) {
                jpointsChooser()->toggle_to("MINUS");
                quteChooser()->toggle_to(QString("QUTE"));
            }

            // better chances to reach 125 in next round
            else if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                     > 100) {
                jpointsChooser()->toggle_to("MINUS");
                quteChooser()->toggle_to(QString("QUTE"));
            }

            // if score will not grow
            else if ((player->handdeck()->pointsOnHand() - jacks * 20) <= 0) {
                jpointsChooser()->toggle_to("MINUS");
                quteChooser()->toggle_to(QString("QUTE"));
            } else {
                quteChooser()->toggle_to(QString("CONTINUE"));
                jpointsChooser()->hide();
            }
        }
        if (stackCard->rank() != "J") {
            jpointsChooser()->toggle_to(QString(""));
            jpointsChooser()->hide();
            // Qute && !Jpoints
            //           Condition                      Decision
            //             score == 125                   QUTE
            //             score >  125                 CONTINUE
            //       points on hand <= 10                 QUTE
            //             else                         CONTINUE
            //
            // score == 125 -> score = 0
            if ((player->handdeck()->pointsOnHand()) * shuffles + player->score() == 125) {
                quteChooser()->toggle_to(QString("QUTE"));
            }

            else if ((player->handdeck()->pointsOnHand()) * shuffles + player->score() > 125) {
                quteChooser()->toggle_to(QString("CONTINUE"));
            }

            else if ((player->handdeck()->pointsOnHand()) * shuffles <= 10) {
                quteChooser()->toggle_to(QString("QUTE"));
            }

            else {
                quteChooser()->toggle_to(QString("CONTINUE"));
            }
        }
        quteChooser()->setDisabled(player->isRobot());
        quteChooser()->show();
    }

    // No Qute Condition -> disconnect Q -> J
    else {
        disconnect(quteChooser().get(),
                   &QuteChooser::quteDecisionChanged,
                   jpointsChooser().get(),
                   &JpointsChooser::onQuteDecisionChanged);

        quteChooser()->setEnabled(false);
        quteChooser()->hide();
    }
    emit quteChooser()->quteDecisionChanged(quteChooser()->decision());

    emit quteChooser()->chooserToggled();

    // -----------------------------------------------------------------------

    // JpointsChooser
    // KI

    /*           Condition                           decision
     *  score by PLUS or MINUS Jpoints == 125      PLUS / MINUS   
     *  score by PLUS or MINUS Jpoints >  100         MINUS
     *  points on hand - jacks * 20 <=    0           MINUS
     *              else                              PLUS
    */

    // Qute Situation see KI Qute
    if (stackCard->rank() == "J" && player->handdeck()->cards().isEmpty()) {
        int jacks = 0;
        for (const auto& card : std::as_const(played()->cards())) {
            if (card->rank() == "J") {
                jacks++;
            }
        }
        // score == 125 -> score = 0
        if ((player->handdeck()->pointsOnHand()) * shuffles + player->score() == 125) {
            jpointsChooser()->toggle_to("PLUS");
        }

        // score == 125 -> score = 0
        else if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                 == 125) {
            jpointsChooser()->toggle_to("MINUS");
        }

        // better chances to reach 125 in next round
        else if ((player->handdeck()->pointsOnHand() - jacks * 20) * shuffles + player->score()
                 > 100) {
            jpointsChooser()->toggle_to("MINUS");
        }

        // if score will not grow
        else if ((player->handdeck()->pointsOnHand() - jacks * 20) <= 0) {
            jpointsChooser()->toggle_to("MINUS");
        } else {
            jpointsChooser()->toggle_to("PLUS");
        }
        jpointsChooser()->setDisabled(player->isRobot());
        jpointsChooser()->show();
    }
    // end KI JPointsChooser

    // Qute situation
    else if (stackCard->rank() == "J" && quteChooser()->isEnabled()) {
        jpointsChooser()->setDisabled(player->isRobot());
        jpointsChooser()->show();

        // No Jpoints Condition
    } else {
        jpointsChooser()->toggle_to("");
        jpointsChooser()->hide();
    }

    emit jpointsChooser()->chooserToggled();

    // -----------------------------------------------------------------------

    // RoundChooser
    if (player->handdeck()->cards().isEmpty() && stack()->topCard()->rank() != '6'
        || quteChooser()->isVisible() && quteChooser()->decision() == QString("QUTE")) {
        roundChooser()->toggle_to(QString("FINISH"));
        roundChooser()->setEnabled(true);
        roundChooser()->show();

        emit roundChooser()->chooserToggled();
    } else {
        roundChooser()->hide();
        roundChooser()->setEnabled(false);
    }

    // -----------------------------------------------------------------------
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
    if (isMustDrawCard() && player->isRobot()) {
        drawCardFromBlind(DrawOption::MustCard);
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
        // mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":res/sounds/draw_card_from_blind.wav"));
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

    if (quteChooser()->isEnabled() && quteChooser()->decision() == QString("QUTE")) {
        return true;
    }

    if (player->handdeck()->cards().isEmpty() && stackCard->rank() != "6") {
        return true;
    }
    return false;
}

void Game::handleSpecialCards()
{
    QSharedPointer<CardVec> cardVec(new CardVec(nullptr, played_->cards()));
    bool isFinished = isRoundFinished();

    // Count occurrences of sevens, eights, aces and jacks in played cards
    int sevens = 0;
    int eights = 0;
    int aces = 0;
    int jacks = 0;

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
        if (jpointsChooser()->decision() == QString("MINUS")) {
            player->setJpoints(-points);
        }

        // If the decision is "PLUS", set points for all players in the player list
        else if (jpointsChooser()->decision() == QString("PLUS")) {
            for (const auto& player : std::as_const(playerList_)) {
                player->setJpoints(points);
            }
            player->setJpoints(0); // active Player
        }
    }

    // Rotate the player list normally first
    rotatePlayerList();

    got1()->clearCards();
    got2()->clearCards();

    for (int i = 0; i < sevens; ++i) {
        connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
        drawCardFromBlind(DrawOption::BadCard);
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

    if (eights >= 2 && eightsChooser()->decision() == QString("ALL")) {
        int leap = 1;
        int num = 0;
        while (leap <= eights) {
            if (leap % playerList_.size() != 0) {
                if (num % 2 == 0)
                    connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
                else
                    connect(this, &Game::cardBadFromBlind, got2_.get(), &Got::onCardBadFromBlind);
                drawCardFromBlind(DrawOption::BadCard);
                drawCardFromBlind(DrawOption::BadCard);
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
            drawCardFromBlind(DrawOption::BadCard);
            drawCardFromBlind(DrawOption::BadCard);
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
        for (const auto& player : std::as_const(playerList_)) {
            player->handdeck()->setEnabled(true);
        }
        //
        emit countPoints(shuffles);
        emit setCbVisible(true);
        emit resetCbVisible();
        playable()->clearCards();
        updateLcdDisplays();
    }
}

void Game::activateNextPlayer()
{
    if (!isNextPlayerPossible())
        return;

    if (isRoundFinished())
        return;

    emit cardsPlayed(played()->cards().size());

    handleSpecialCards();

    updatePlayable();

    autoplay();
}

void Game::autoplay()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    if (!roundChooser()->isEnabled())
        player->handdeck()->setEnabled(true);

    emit resetCbVisible();

    while (player->isRobot()) {
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
            } else if (player->score() + player->handdeck()->countCardsOfRank("A") * 15 * shuffles
                       == 125)
                player->handdeck()->appendRank(pattern, "A");
            else {
                player->handdeck()->prependRank(pattern, "6");
                player->handdeck()->appendRank(pattern, "J");
            }
            // end KI rankPoints 6, 7, 8, J, A

            player->handdeck()->sortCardsByPattern(pattern);
            playable()->sortCardsByPattern(pattern);

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
        if (isNextPlayerPossible())
            break;
        // updatePlayable(); // executed in isNextPlayerPossible()
    }
    handleChoosers();
    emit cardsPlayed(played()->cards().size());

    // cardvecs and choosers need to be refreshed.
    // JsuitChooser must be refreshed separately when toggling suit
    emit resetCbVisible();
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
        // mediaPlayer_->stop(); // Stop any previous playback
        mediaPlayer_->setSource(QUrl(":res/sounds/shuffling.wav"));
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

void Game::updateLcdDisplays()
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

void Game::onRbCardType(cardType type)
{
    for (auto& card : player2()->handdeck()->cards()) {
        card->setCardType(type);
        card->loadImage(isCardsVisible_);
    }
    for (auto& card : player3()->handdeck()->cards()) {
        card->setCardType(type);
        card->loadImage(isCardsVisible_);
    }
    // Always small cards for monitor
    // for (auto& card : monitor()->cards()) {
    //     card->setCardType(type);
    // }
    for (auto& card : blind()->cards()) {
        card->setCardType(type);
        card->loadImage(isCardsVisible_);
    }
    for (auto& card : stack()->cards()) {
        card->setCardType(type);
        card->loadImage(true);
    }
    // Always small cards for drawn, playable, played
    // for (auto& card : drawn()->cards()) {
    //     card->setCardType(type);
    // }
    // for (auto& card : playable()->cards()) {
    //     card->setCardType(type);
    // }
    // for (auto& card : played()->cards()) {
    //     card->setCardType(type);
    // }
    for (auto& card : player1()->handdeck()->cards()) {
        card->setCardType(type);
        card->loadImage(true);
    }
    emit resetCbVisible();
}

void Game::onCbSound(int state)
{
    isSoundOn_ = state;
}

void Game::onRbSuit()
{
    player->handdeck()->sortCardsBy(SortOption::Suit);
}
void Game::onRbRank()
{
    player->handdeck()->sortCardsBy(SortOption::Rank);
}

// human player must draw card from blind manually, not autoplay
void Game::onBlindClicked()
{
    if (!player->isRobot() && isMustDrawCard()) {
        drawCardFromBlind(DrawOption::MustCard);
        updatePlayable();
    }
}

void Game::onNewRound()
{
    for (const auto& player : std::as_const(playerList_)) {
        player->setJpoints(0);
    }

    togglePlayerListToScore(true);
    if (playerList_.front()->score() <= 125) {
        roundChooser()->toggle_to(QString("NEW"));
        rounds += 1;
        initializeRound();
    } else {
        roundChooser()->toggle_to(QString("GAME"));
    }
}

void Game::onNewGame()
{
    emit countPoints(0);

    games += 1;
    rounds = 1;

    updateLcdDisplays();

    initializeRound();
}
