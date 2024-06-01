#include "game.h"
#include <QDebug>

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
    , playable_(new Playable())
    , drawn_(new Drawn())
    , player1_(new Player(nullptr, "Player1", false, 0, new Handdeck(nullptr)))
    , player2_(new Player(nullptr, "Player2", true, 0, new Handdeck(nullptr)))
    , player3_(new Player(nullptr, "Player3", true, 0, new Handdeck(nullptr)))
{
    playerList_.push_back(player1_);
    playerList_.push_back(player2_);
    playerList_.push_back(player3_);

    connect(player1()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);
    connect(player2()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);
    connect(player3()->handdeck(), &Handdeck::handCardClicked, this, &Game::onHandCardClicked);

    connect(this, &Game::cardAddedToStack, played_.get(), &Played::onCardAddedToStack);
    connect(this, &Game::cardAddedToStack, monitor_.get(), &Monitor::onCardAddedToStack);
    connect(this, &Game::cardAddedToStack, jsuitChooser_.get(), &JsuitChooser::onCardAddedToStack);

    connect(this, &Game::cardDrawnFromBlind, drawn_.get(), &Drawn::onCardDrawnFromBlind);

    for (const auto& player : playerList_) {
        connect(this, &Game::countPoints, player.get(), &Player::onCountPoints);
    }
    connect(monitor_.get(),
            &Monitor::eightsInMonitor,
            eightsChooser_.get(),
            &EightsChooser::onEightsInMonitor);
    connect(monitor_.get(),
            &Monitor::fourCardsInMonitor,
            quteChooser_.get(),
            &QuteChooser::onFourCardsInMonitor);

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
    collectAllCardsToBlind();

    if (!monitor()->cards().isEmpty())
        monitor_->clearCards();
    if (!played()->cards().isEmpty())
        played_->clearCards();
    if (!drawn()->cards().isEmpty())
        drawn_->clearCards();

    // shuffle the cards
    blind_->shuffle();

    // Sort the player list by scores in descending order
    // std::sort(playerList_.begin(), playerList_.end(), &Game::comparePlayersByScore);
    std::sort(playerList_.begin(),
              playerList_.end(),
              [](const QSharedPointer<Player>& a, const QSharedPointer<Player>& b) {
                  return a->score() > b->score();
              });

    // std::sort(playerList_.begin(), playerList_.end(), &Game::comparePlayersByScore);

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
}

void Game::onHandCardClicked(const QSharedPointer<Card>& card)
{
    if (isCardPlayable(card)) {
        emit cardAddedToStack(card);
        player->handdeck()->moveCardTo(card, stack().get());

        // JsuitChooser
        if (card->rank() == "J") {
            jsuitChooser()->toggle_to(player->handdeck()->mostCommonSuit());
            if (!player->isRobot())
                jsuitChooser()->setEnabled(true);
            jsuitChooser()->show();
        } else {
            jsuitChooser()->hide();
            jsuitChooser()->setEnabled(false);
        }

        // EightsChooser
        if (card->rank() == "8" && monitor()->cards().size() >= 2 && played()->cards().size() >= 2) {
            emit monitor()->eightsInMonitor();
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
            emit monitor()->fourCardsInMonitor();
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
        if (card->rank() == "J"
            && (player->handdeck()->cards().isEmpty() || quteChooser()->isEnabled())) {
            if (!player->isRobot())
                jpointsChooser()->setEnabled(true);
            else
                jpointsChooser()->toggleRandom();
            jpointsChooser()->show();
        } else {
            jpointsChooser()->hide();
            jpointsChooser()->setEnabled(false);
        }

        player = playerList_.front();
        player->handdeck()->removeCard(card);
        updatePlayable();
    }
}

void Game::updatePlayable()
{
    playable()->clearCards();

    player = playerList_.front();

    for (const auto& card : player->handdeck()->cards()) { // Use const auto& to avoid copying
        if (isCardPlayable(card)) {
            player->handdeck()->copyCardTo(card, playable().get());
        }
    }
}

bool Game::isCardPlayable(const QSharedPointer<Card>& card)

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

    // for playing first card by
    // player1_->handdeck()->cards().last()->click();
    if (!stackCard) {
        return true;
    }

    // 1st move: if stack card is 'J'
    if (played()->cards().isEmpty() && stackCard->rank() == "J") {
        if (card->suit() == jsuitChooser()->suit() || card->rank() == "J") {
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

    // 2nd move: if stack card is '6'
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
    }

    return false;
}

bool Game::mustDrawCard()
{
    /*
        must draw card, if:
        ---------------------------------
         card   playable  card    must  next player
        played    card    drawn   draw  playable
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
            1       0       .       Y       N   <-- must draw card
    */

    QSharedPointer<Card> stackCard = stack()->topCard();

    // played card '6' must be covered
    if (stackCard->rank() == "6" && !played()->cards().isEmpty() && playable()->cards().isEmpty()) {
        return true;
    }

    // at least one card must be played or drawn (if no playable card on hand)
    if (played()->cards().isEmpty() && playable()->cards().isEmpty() && drawn()->cards().isEmpty()) {
        return true;
    }

    return false;
}

bool Game::isNextPlayerPossible()
{
    QSharedPointer<Card> stackCard = stack()->topCard();

    // If quteChooser is enabled and the decision is "y", the next player is possible
    if (quteChooser()->isEnabled() && quteChooser()->decision() == "y") {
        return true;
    }

    getPlayableCard();

    // If the stack card's rank is "6", the next player is not possible
    if (stackCard->rank() == "6") {
        return false;
    }

    // Next player is possible if there are played cards or if there are no playable cards but drawn cards exist
    return !played()->cards().isEmpty()
           || (playable()->cards().isEmpty() && !drawn()->cards().isEmpty());
}

void Game::getPlayableCard()
{
    player = playerList_.front();

    while (mustDrawCard()) {
        if (blind()->cards().isEmpty())
            refillBlindFromStack();
        emit cardDrawnFromBlind(blind()->topCard());
        blind()->moveTopCardTo(player->handdeck());
        updatePlayable();
    }
}

void Game::rotatePlayerList()
{
    played()->clearCards();
    drawn()->clearCards();

    eightsChooser()->setEnabled(false);
    jsuitChooser()->setEnabled(false);
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

    for (auto player : playerList_) {
        player->handdeck()->setEnabled(false);
    }

    player = playerList_.front();
    player->handdeck()->setEnabled(true);

    updatePlayable();
}

void Game::activateNextPlayer()
{
    qDebug() << "--------------------------------------------------";
    qDebug() << "player" << player->name() << "has played:" << played()->cardsAsString();
    qDebug() << "stack:" << stack()->cardsAsString();
    qDebug() << "blind:" << blind()->cardsAsString();

    bool isRoundFinished = false;

    if (!isNextPlayerPossible())
        return;

    // Count occurrences of sevens, eights, and aces
    int aces = 0;
    int sevens = 0;
    int eights = 0;

    for (const auto& card : played()->cards()) {
        if (card->rank() == "7") {
            sevens++;
        } else if (card->rank() == "8") {
            eights++;
        } else if (card->rank() == "A") {
            aces++;
        }
    }

    if (player->handdeck()->cards().isEmpty())
        isRoundFinished = true;

    if (quteChooser()->isEnabled() && quteChooser()->decision() == "y") {
        isRoundFinished = true;
    } else if (quteChooser()->isEnabled())
        quteChooser()->setEnabled(false);

    if (isRoundFinished && jpointsChooser()->isEnabled()) {
        player->setScore(player->score()
                         - shuffles * 20
                               * qMin(played()->cards().size(), monitor()->cards().size()));

        if (jpointsChooser()->decision() == "p") {
            for (const auto& player : playerList_) {
                player->setScore(player->score()
                                 + shuffles * 20
                                       * qMin(played()->cards().size(), monitor()->cards().size()));
            }
        }
    }

    // Rotate the player list normally first
    rotatePlayerList();

    // Apply effects of '7' (next player must draw a card)
    for (int i = 0; i < sevens; ++i) {
        if (blind()->cards().isEmpty())
            refillBlindFromStack();
        emit cardDrawnFromBlind(blind()->topCard());
        blind()->moveTopCardTo(player->handdeck());
    }

    // Apply effects of 'A' (skip next player(s))
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

    // If there are eights and decision is 'cards to all'
    if (eights >= 2 && eightsChooser()->decision() == "a") {
        int leap = 1;
        while (leap <= eights) {
            if (leap % playerList_.size() != 0) {
                if (blind()->cards().isEmpty())
                    refillBlindFromStack();
                emit cardDrawnFromBlind(blind()->topCard());
                blind()->moveTopCardTo(player->handdeck());
                if (blind()->cards().isEmpty())
                    refillBlindFromStack();
                emit cardDrawnFromBlind(blind()->topCard());
                blind()->moveTopCardTo(player->handdeck());
                // drawn()->cards().clear();
                rotatePlayerList();
            } else {
                rotatePlayerList();
                eights++;
            }
            leap++;
        }
        eights = 0;
    }

    // If there are eights and decision is 'cards to next player'
    if (eights > 0) {
        for (int i = 0; i < eights; ++i) {
            if (blind()->cards().isEmpty())
                refillBlindFromStack();
            emit cardDrawnFromBlind(blind()->topCard());
            blind()->moveTopCardTo(player->handdeck());
            if (blind()->cards().isEmpty())
                refillBlindFromStack();
            emit cardDrawnFromBlind(blind()->topCard());
            blind()->moveTopCardTo(player->handdeck());
        }
        rotatePlayerList();
    }

    if (isRoundFinished) {
        emit countPoints(shuffles);

        // TODO: log scores

        for (const auto& player : playerList_) {
            if (player->score() > 125)

                startNewGame();
            else
                startNewRound();
        }
    }

    if (player->isRobot()) {
        autoplay();
    } else {
        player->handdeck()->setEnabled(true);
    }

    qDebug() << "player" << player->name() << "has played:" << played()->cardsAsString();
    for (auto player : playerList_)
        qDebug() << "player" << player->name() << "cards:" << player->handdeck()->cardsAsString();
    qDebug() << "-------------------------------------";
}

void Game::autoplay()
{
    player->handdeck()->setEnabled(true);

    do {
        // Play all playable cards in the handdeck
        for (const auto& card : player->handdeck()->cards()) {
            if (isCardPlayable(card)) {
                card->click(); // Simulate click on playable card
                updatePlayable();
            }
        }

        // Draw a card if necessary and update playable cards
        if (mustDrawCard()) {
            blind()->moveTopCardTo(player->handdeck());
            updatePlayable();
        }
    } while (!playable()->cards().isEmpty() || !isNextPlayerPossible());
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

void Game::startNewRound()
{
    // Sort the player list by scores in descending order
    std::sort(playerList_.begin(),
              playerList_.end(),
              [](const QSharedPointer<Player>& a, const QSharedPointer<Player>& b) {
                  return a->score() > b->score();
              });

    // std::sort(playerList_.begin(), playerList_.end(), &Game::comparePlayersByScore);

    if (playerList_.front()->score() > 125) {
        startNewGame();
    }

    qDebug() << "Round finished. The winner is: " << playerList_.last()->name();

    rounds += 1;
    initializeGame();

    qDebug() << "New round started.";
}

void Game::startNewGame()
{
    for (const auto& player : playerList_) {
        player->setScore(0);
    }

    games += 1;
    rounds = 1;
    initializeGame();

    qDebug() << "New game started.";
}
