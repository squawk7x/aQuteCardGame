#include "game.h"
#include <QDebug>
#include <QTimer>

Game::Game(QObject* parent)
    : QObject(parent)
    , monitor_(new Monitor())
    , eightsChooser_(new EightsChooser())
    , quteChooser_(new QuteChooser())
    , jpointsChooser_(new JpointsChooser())
    , roundChooser_(new RoundChooser())
    , played_(new Played())
    , blind_(new Blind())
    , jsuitChooser_(new JsuitChooser())
    , stack_(new Stack())
    , got1_(new Got())
    , got2_(new Got())
    , playable_(new Playable())
    , drawn_(new Drawn())
    , player1_(new Player(nullptr, 1, "Player1", false, 0, new Handdeck(nullptr)))
    , player2_(new Player(nullptr, 2, "Player2", true, 0, new Handdeck(nullptr)))
    , player3_(new Player(nullptr, 3, "Player3", true, 0, new Handdeck(nullptr)))
    , lcd1_(new QLCDNumber())
    , lcd2_(new QLCDNumber())
    , lcd3_(new QLCDNumber())
    , lcdShuffles_(new QLCDNumber())
{
    lcdShuffles_->setDigitCount(2);

    playerList_.push_back(player1_);
    playerList_.push_back(player2_);
    playerList_.push_back(player3_);

    connect(player1()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);
    connect(player2()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);
    connect(player3()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);

    // connect(this, &Game::cardBadFromBlind, got1_.get(), &Got::oncardBadFromBlind);
    // connect(this, &Game::cardBadFromBlind, got2_.get(), &Got::oncardBadFromBlind);
    // connect(this, &Game::cardAddedToStack, got_.get(), &Got::clearGotCards);
    connect(this, &Game::cardAddedToStack, played_.get(), &Played::onCardAddedToStack);
    connect(this, &Game::cardAddedToStack, monitor_.get(), &Monitor::onCardAddedToStack);

    connect(this, &Game::cardDrawnFromBlind, drawn_.get(), &Drawn::onCardDrawnFromBlind);

    for (const auto& player : playerList_) {
        connect(this, &Game::countPoints, player.get(), &Player::onCountPoints);
    }

    // connect(this, &Game::cardAddedToStack, jsuitChooser_.get(), &JsuitChooser::onCardAddedToStack);

    // connect(monitor_.get(),
    //         &Monitor::eightsInMonitor,
    //         eightsChooser_.get(),
    //         &EightsChooser::onEightsInMonitor);

    // connect(monitor_.get(),
    //         &Monitor::fourCardsInMonitor,
    //         quteChooser_.get(),
    //         &QuteChooser::onFourCardsInMonitor);

    connect(quteChooser().get(),
            &QuteChooser::quteDecisionChanged,
            roundChooser().get(),
            &RoundChooser::onQuteDecisionChanged);

    connect(quteChooser().get(),
            &QuteChooser::quteDecisionChanged,
            jpointsChooser().get(),
            &JpointsChooser::onQuteDecisionChanged);

    connect(roundChooser().get(), &RoundChooser::newRound, this, &Game::startNewRound);
    connect(roundChooser().get(), &RoundChooser::newGame, this, &Game::startNewGame);

    initializeGame();
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

QSharedPointer<QLCDNumber> Game::lcd1()
{
    return lcd1_;
}

QSharedPointer<QLCDNumber> Game::lcd2()
{
    return lcd2_;
}

QSharedPointer<QLCDNumber> Game::lcd3()
{
    return lcd3_;
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
void Game::initializeGame()
{
    // jpointsChooser()->setDisabled(true);
    roundChooser()->setDisabled(true);
    roundChooser()->hide();

    collectAllCardsToBlind();

    if (!monitor()->cards().isEmpty())
        monitor_->clearCards();
    if (!played()->cards().isEmpty())
        played_->clearCards();
    if (!drawn()->cards().isEmpty())
        drawn_->clearCards();

    // shuffle the cards
    blind_->shuffle();
    shuffles = 1;
    lcdShuffles()->display(shuffles);

    // Player with highest score will start
    togglePlayerListToScore(true);

    // Distribute cards
    for (int k = 1; k <= 5; k++) {
        blind_->moveTopCardTo(player2_->handdeck());
    }

    for (int k = 1; k <= 5; k++) {
        blind_->moveTopCardTo(player3_->handdeck());
    }

    for (int k = 1; k <= 5; k++) {
        blind_->moveTopCardTo(player1_->handdeck());
    }

    for (const auto& player : playerList_) {
        player->handdeck()->setEnabled(false);
    }

    // active player is first player in playerList_
    player = playerList_.front();
    player->handdeck()->setEnabled(true);
    player->handdeck()->cards().last()->click();

    // case a robot player starts a new round
    autoplay();
}

void Game::onHandCardClicked(const QSharedPointer<Card>& card)
{
    if (isThisCardPlayable(card)) {
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
        jsuitChooser()->toggle_to(player->handdeck()->mostCommonSuit());
        if (!player->isRobot())
            jsuitChooser()->setEnabled(true);
        jsuitChooser()->show();
    } else {
        jsuitChooser()->hide();
        jsuitChooser()->setEnabled(false);
    }

    // EightsChooser
    if (stackCard->rank() == "8" && monitor()->cards().size() >= 2
        && played()->cards().size() >= 2) {
        // emit monitor()->eightsInMonitor();
        if (!player->isRobot())
            eightsChooser()->setEnabled(true);
        else
            eightsChooser()->toggleRandom();

        eightsChooser()->show();
    } else {
        eightsChooser()->hide();
        eightsChooser()->setEnabled(false);
    }

    // QuteChooser
    if (monitor()->cards().size() == 4 && played()->cards().size() > 0) {
        // emit monitor()->fourCardsInMonitor();
        if (!player->isRobot())
            quteChooser()->setEnabled(true);
        else {
            quteChooser()->toggleRandom();
        }
        quteChooser()->show();
    } else {
        quteChooser()->hide();
        quteChooser()->setEnabled(false);
    }

    // JPointsChooser
    if (stackCard->rank() == "J"
        && (player->handdeck()->cards().isEmpty() || quteChooser()->isEnabled())) {
        if (!player->isRobot())
            jpointsChooser()->setEnabled(true);
        else {
            jpointsChooser()->toggleRandom();
        }
        jpointsChooser()->show();
    } else {
        jpointsChooser()->hide();
        jpointsChooser()->setEnabled(false);
    }

    // RoundChooser
    if (player->handdeck()->cards().isEmpty() && stack()->topCard()->rank() != '6'
        || quteChooser()->isEnabled() && quteChooser()->decision() == "y") {
        roundChooser()->setDecision("r");
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

    for (const auto& card : player->handdeck()->cards()) {
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
        if (card->rank() == stackCard->rank() || card->suit() == stackCard->suit()
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

bool Game::mustDrawCard()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    updatePlayable();

    // played card '6' must be covered
    if (!played()->cards().isEmpty() && playable()->cards().isEmpty() && stackCard->rank() == "6") {
        return true;
    }

    // at least one card must be played or drawn (if no playable card on hand)
    if (played()->cards().isEmpty() && playable()->cards().isEmpty() && drawn()->cards().isEmpty()) {
        return true;
    }

    return false;
}

bool Game::isNextPlayerPossible()

/*      
        ---------------------------------
         card   playable  card    must  next player
        played    card    drawn   draw  possible
            1       1       1       N       Y
            1       1       0       N       Y
            1       0       1       N       Y
            1       0       0       N       Y
            0       1       1       N       N
            0       1       0       N       N
            0       0       1       N       Y
            0       0       0       Y       N   <-- must draw card
        '6' on stack:
        -------------
            1       0      0||1     Y       N   <-- must draw card
    */
{
    // If quteChooser is enabled and the decision is "y", the next player is possible
    if (quteChooser()->isEnabled() && quteChooser()->decision() == "y") {
        return true;
    }

    updatePlayable();

    while (mustDrawCard()) {
        drawCardFromBlind(Game::DrawOption::MustCard);
        updatePlayable();
    }

    QSharedPointer<Card> stackCard = stack()->topCard();

    // If the stack card's rank is "6", the next player is not possible
    // except on quteChooser()->decision() == "y"
    // player himself has played a '6'
    if (!played()->cards().isEmpty() && stackCard->rank() == "6") {
        return false;
    }

    // Next player is possible if there are played cards or if there are no playable cards but drawn cards exist
    return !played()->cards().isEmpty()
           || (playable()->cards().isEmpty() && !drawn()->cards().isEmpty());
}

void Game::drawCardFromBlind(DrawOption option)
{
    if (blind()->cards().isEmpty())
        refillBlindFromStack();

    if (option == DrawOption::MustCard) {
        emit cardDrawnFromBlind(blind()->topCard());
    } else if (option == DrawOption::BadCard) {
        emit cardBadFromBlind(blind()->topCard());
    }

    blind()->moveTopCardTo(player->handdeck());
}

void Game::rotatePlayerList()
{
    played()->clearCards();
    drawn()->clearCards();

    jsuitChooser()->setEnabled(false);

    eightsChooser()->setEnabled(false);
    jpointsChooser()->setEnabled(false);
    quteChooser()->setEnabled(false);
    eightsChooser()->hide();
    jpointsChooser()->hide();
    quteChooser()->hide();

    if (!playerList_.isEmpty()) {
        player = playerList_.front();
        playerList_.pop_front();
        playerList_.push_back(player);
    }

    for (const auto& player : playerList_) {
        player->handdeck()->setEnabled(false);
    }

    player = playerList_.front();
    player->handdeck()->setEnabled(true);

    updatePlayable();
}

void Game::togglePlayerListToScore(bool highest)
{
    auto compare = [highest](const QSharedPointer<Player>& a, const QSharedPointer<Player>& b) {
        return highest ? a->score() < b->score() : a->score() > b->score();
    };

    auto maxIt = std::max_element(playerList_.begin(), playerList_.end(), compare);

    if (maxIt != playerList_.end()) {
        std::rotate(playerList_.begin(), maxIt, maxIt + 1);
    }
}

void Game::activateNextPlayer()
{
    if (!isNextPlayerPossible())
        return;

    // Count occurrences of sevens, eights, and aces
    int jacks = 0;
    int sevens = 0;
    int aces = 0;
    int eights = 0;

    for (const auto& card : played()->cards()) {
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
    if (jpointsChooser()->isEnabled()) {
        // Calculate points based on the minimum size of played and monitor card sets
        int points = 20 * jacks;

        // Set points for the current player
        player->setJpoints(-points);

        // If the decision is "p", set points for all players in the player list
        if (jpointsChooser()->decision() == "p") {
            for (const auto& player : playerList_) {
                player->setJpoints(points);
            }
        }
    } else {
        // If jpointsChooser is not enabled, set points to 0 for all players
        for (const auto& player : playerList_) {
            player->setJpoints(0);
        }
    }

    QSharedPointer<CardVec> cardVec(new CardVec(nullptr, played_->cards()));

    // Rotate the player list normally first
    rotatePlayerList();

    got1()->clearCards();
    got2()->clearCards();

    for (int i = 0; i < sevens; ++i) {
        connect(this, &Game::cardBadFromBlind, got2_.get(), &Got::onCardBadFromBlind);
        drawCardFromBlind(Game::DrawOption::BadCard);
        disconnect(this, &Game::cardBadFromBlind, got2_.get(), &Got::onCardBadFromBlind);
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
        }
        disconnect(this, &Game::cardBadFromBlind, got1_.get(), &Got::onCardBadFromBlind);
        rotatePlayerList();
    }

    got1()->setEnabled(false);
    got2()->setEnabled(got1()->cards().isEmpty());

    played()->clearCards();
    drawn()->clearCards();

    autoplay();
}

void Game::autoplay()
{
    if (!roundChooser()->isEnabled() && player->isRobot()) {
        player->handdeck()->setEnabled(true);

        updatePlayable();

        while (!isNextPlayerPossible() || !playable()->cards().isEmpty()) {
            for (auto& card : playable()->cards()) {
                for (auto& card : player->handdeck()->cards()) {
                    card->click();
                    updatePlayable();
                    handleChoosers();
                }
            }
        }
    }
}

void Game::refillBlindFromStack()
{
    if (stack()->cards().size() <= 1) {
        qDebug() << "Not enough cards in the stack to refill the blind.";
        return;
    }

    // Move all but the top card from stack to blind
    while (stack()->cards().size() > 1) {
        stack()->moveTopCardTo(blind().get());
    }

    // Shuffle the blind deck
    blind()->shuffle();
    shuffles += 1;
    lcdShuffles()->display(shuffles);

    qDebug() << "Blind refilled and shuffled.";
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
    for (const auto& player : playerList_) {
        if (player->id() == 1) {
            lcd1()->display(player->score());
        } else if (player->id() == 2) {
            lcd2()->display(player->score());
        } else if (player->id() == 3) {
            lcd3()->display(player->score());
        }
    }
}

void Game::startNewRound()
{
    activateNextPlayer();

    // make the players count their points and update their scores
    emit countPoints(shuffles);
    updateDisplay();

    togglePlayerListToScore(false);
    qDebug() << "The winner is: " << playerList_.front()->name();

    togglePlayerListToScore(true);
    if (playerList_.front()->score() <= 125) {
        rounds += 1;
        qDebug() << "Starting new Round ...";
        initializeGame();
    } else {
        roundChooser()->setDecision("g");
    }
}

void Game::startNewGame()
{
    emit countPoints(0);

    games += 1;
    rounds = 1;

    updateDisplay();

    qDebug() << "Starting new game ...";

    initializeGame();
}
