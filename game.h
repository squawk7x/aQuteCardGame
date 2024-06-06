#ifndef GAME_H
#define GAME_H

#include <QLCDNumber>
#include <QMouseEvent>
#include <QObject>
#include <QVector>
#include "blind.h"
#include "choosers.h"
#include "drawn.h"
#include "got.h"
#include "monitor.h"
#include "playable.h"
#include "played.h"
#include "player.h"
#include "stack.h"

class Game : public QObject
{
    Q_OBJECT

private:
    QSharedPointer<Player> player2_;
    QSharedPointer<Player> player3_;

    QSharedPointer<Monitor> monitor_;
    QSharedPointer<EightsChooser> eightsChooser_;
    QSharedPointer<QuteChooser> quteChooser_;
    QSharedPointer<RoundChooser> roundChooser_;
    QSharedPointer<JpointsChooser> jpointsChooser_;
    QSharedPointer<Got> got1_;
    QSharedPointer<Got> got2_;
    QSharedPointer<Played> played_;
    QSharedPointer<Drawn> drawn_;

    QSharedPointer<QLCDNumber> lcdShuffles_;
    QSharedPointer<Blind> blind_;
    QSharedPointer<JsuitChooser> jsuitChooser_;
    QSharedPointer<Stack> stack_;

    QSharedPointer<Playable> playable_;
    QSharedPointer<QLCDNumber> lcdRound_;
    QSharedPointer<QLCDNumber> lcdP1_;
    QSharedPointer<QLCDNumber> lcdP2_;
    QSharedPointer<QLCDNumber> lcdP3_;

    QSharedPointer<Player> player1_;

    int games = 1;
    int rounds = 1;
    int shuffles = 0;

public:
    enum class DrawOption { MustCard, BadCard };

    explicit Game(QObject* parent = nullptr);
    ~Game();

    void initializeGame();

    // Getters
    QSharedPointer<Player> player2() const;
    QSharedPointer<Player> player3() const;

    QSharedPointer<Monitor> monitor();
    QSharedPointer<EightsChooser> eightsChooser();
    QSharedPointer<QuteChooser> quteChooser();
    QSharedPointer<RoundChooser> roundChooser();
    QSharedPointer<JpointsChooser> jpointsChooser();
    QSharedPointer<Got> got1();
    QSharedPointer<Got> got2();
    QSharedPointer<Played> played();
    QSharedPointer<Drawn> drawn();

    QSharedPointer<QLCDNumber> lcdShuffles();
    QSharedPointer<Blind> blind();
    QSharedPointer<JsuitChooser> jsuitChooser();
    QSharedPointer<Stack> stack();

    QSharedPointer<Playable> playable();
    QSharedPointer<QLCDNumber> lcdRound();
    QSharedPointer<QLCDNumber> lcdP1();
    QSharedPointer<QLCDNumber> lcdP2();
    QSharedPointer<QLCDNumber> lcdP3();

    QSharedPointer<Player> player1() const;

    QVector<QSharedPointer<Player>> playerList_;
    QSharedPointer<Player> player = nullptr;

    void initializeRound();
    bool isThisCardPlayable(const QSharedPointer<Card>& card);
    void handleSpecialCards();
    bool isNextPlayerPossible();
    bool isRoundFinished();
    void updatePlayable();
    bool mustDrawCard();
    void drawCardFromBlind(DrawOption option);
    void autoplay();
    void refillBlindFromStack();
    void handleChoosers();
    void rotatePlayerList();
    void countRound();
    void updateDisplay();
    void collectAllCardsToBlind();
    void togglePlayerListToScore(bool highest);

signals:
    void cardAddedToStack(const QSharedPointer<Card>& card);
    void cardDrawnFromBlind(const QSharedPointer<Card>& card);
    void cardBadFromBlind(const QSharedPointer<Card>& card);
    void countPoints(int shuffles);

public slots:
    void onHandCardClicked(const QSharedPointer<Card>& card);
    void activateNextPlayer();
    void onNewRound();
    void onNewGame();
};

#endif // GAME_H
