#ifndef GAME_H
#define GAME_H

#include <QAudioOutput>
#include <QLCDNumber>
#include <QMediaPlayer>
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
    int numberOfPlayers_;
    bool isCardsVisible_;
    bool isSoundOn_;
    QSharedPointer<QMediaPlayer> mediaPlayer_;
    QSharedPointer<QAudioOutput> audioOutput_;

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

    explicit Game(int numberOfPlayers, QObject* parent = nullptr);
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
    void refillBlindFromStack();
    bool isThisCardPlayable(const QSharedPointer<Card>& card);
    bool isMustDrawCard();
    bool isNextPlayerPossible();
    void updatePlayable();
    void drawCardFromBlind(DrawOption option);
    void handleChoosers();
    void handleSpecialCards();
    void rotatePlayerList();
    void autoplay();
    bool isRoundFinished();
    void countRound();
    void updateDisplay();
    void togglePlayerListToScore(bool highest);
    void collectAllCardsToBlind();

signals:
    void cardAddedToStack(const QSharedPointer<Card>& card);
    void cardMustFromBlind(const QSharedPointer<Card>& card);
    void cardBadFromBlind(const QSharedPointer<Card>& card);
    void cardsPlayed(int numCardsPlayed);
    void countPoints(int shuffles);
    void setBlindRed(bool red);

    // Controls and Settings:
    void setCbVisible(bool isVisible);
    void toggleCardsVisible(bool isVisible);

public slots:
    void onHandCardClicked(const QSharedPointer<Card>& card);
    void activateNextPlayer();
    // void onBlindClicked();
    void onCbVisible(int state);
    void onCbVisibleStatus(int state);
    void onCbSound(int state);
    void onRbSuit();
    void onRbRank();
    void onNewRound();
    void onNewGame();
};

#endif // GAME_H
