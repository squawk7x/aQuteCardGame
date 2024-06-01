#ifndef GAME_H
#define GAME_H

#include <QMouseEvent>
#include <QObject>
#include <QVector>
#include "blind.h"
#include "choosers.h"
#include "drawn.h"
#include "monitor.h"
#include "playable.h"
#include "played.h"
#include "player.h"
#include "stack.h"

class Game : public QObject
{
    Q_OBJECT

private:
    QSharedPointer<Monitor> monitor_;
    QSharedPointer<EightsChooser> eightsChooser_;
    QSharedPointer<QuteChooser> quteChooser_;
    QSharedPointer<JpointsChooser> jpointsChooser_;
    QSharedPointer<RoundChooser> roundChooser_;
    QSharedPointer<Played> played_;
    QSharedPointer<Blind> blind_;
    QSharedPointer<JsuitChooser> jsuitChooser_;
    QSharedPointer<Stack> stack_;
    QSharedPointer<Playable> playable_;
    QSharedPointer<Drawn> drawn_;
    QSharedPointer<Player> player1_;
    QSharedPointer<Player> player2_;
    QSharedPointer<Player> player3_;
    int games = 1;
    int rounds = 1;
    int shuffles = 1;

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void initializeGame();

    // Getters
    QVector<QSharedPointer<Player>> playerList_;
    QSharedPointer<Player> player1() const;
    QSharedPointer<Player> player2() const;
    QSharedPointer<Player> player3() const;
    QSharedPointer<Player> player = nullptr;
    QSharedPointer<Monitor> monitor();
    QSharedPointer<EightsChooser> eightsChooser();
    QSharedPointer<QuteChooser> quteChooser();
    QSharedPointer<JpointsChooser> jpointsChooser();
    QSharedPointer<RoundChooser> roundChooser();
    QSharedPointer<Played> played();
    QSharedPointer<Drawn> drawn();
    QSharedPointer<Blind> blind();
    QSharedPointer<JsuitChooser> jsuitChooser();
    QSharedPointer<Stack> stack();
    QSharedPointer<Playable> playable();
    bool isCardPlayable(const QSharedPointer<Card>& card);
    bool isNextPlayerPossible();
    void updatePlayable();
    bool mustDrawCard();
    void autoplay();
    void getPlayableCard();
    void refillBlindFromStack();
    void collectAllCardsToBlind();
    void countRound();
    bool comparePlayersByScore(const QSharedPointer<Player>& a, const QSharedPointer<Player>& b);
    void startNewRound();
    void startNewGame();

signals:
    void cardAddedToStack(const QSharedPointer<Card>& card);
    void cardDrawnFromBlind(const QSharedPointer<Card>& card);
    void countPoints(int shuffles);

public slots:
    void onHandCardClicked(const QSharedPointer<Card>& card);
    void rotatePlayerList();
    void activateNextPlayer();
};

#endif // GAME_H
