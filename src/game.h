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

enum class DrawOption { NoCard, BadCard, MustCard };
enum class NextOption { Possible, NotPossible };
// enum class ButtonColor { Grey, Yellow };

class Game : public QObject
{
    Q_OBJECT

private:
    int numberOfPlayers_;
    bool isCardsVisible_;
    bool isSoundOn_;
    bool isLoggingOn_;
    int games = 1;
    int rounds = 1;
    int shuffles = 0;
    QSharedPointer<QMediaPlayer> mediaPlayer_;
    QSharedPointer<QAudioOutput> audioOutput_;

    void initializeGame();
    void initializeRound();
    void refillBlindFromStack();
    bool isThisCardPlayable(const QSharedPointer<Card>& card);
    bool isMustDrawCard();
    void updatePlayable();
    void drawCardFromBlind(DrawOption option);
    void handleChoosers();
    void handleSpecialCards();
    void rotatePlayerList();
    void autoplay();
    bool isRoundFinished();
    void countRound();
    void updateLcdDisplays();
    void togglePlayerListToScore(bool highest);
    void collectAllCardsToBlind();
    void logData();

public:
    QSharedPointer<Player> player2_;
    QSharedPointer<Player> player3_;

    QSharedPointer<Monitor> monitor_;

    // Choosers
    QSharedPointer<Chooser> chooser_;
    QSharedPointer<EightsChooser> eightsChooser_;
    QSharedPointer<QuteChooser> quteChooser_;
    QSharedPointer<RoundChooser> roundChooser_;
    QSharedPointer<JpointsChooser> jpointsChooser_;
    QSharedPointer<JsuitChooser> jsuitChooser_;
    //

    QSharedPointer<Got> got1_;
    QSharedPointer<Got> got2_;
    QSharedPointer<Played> played_;
    QSharedPointer<Drawn> drawn_;

    QSharedPointer<QLCDNumber> lcdShuffles_;
    QSharedPointer<Blind> blind_;
    QSharedPointer<Stack> stack_;

    QSharedPointer<Playable> playable_;
    QSharedPointer<QLCDNumber> lcdRound_;
    QSharedPointer<QLCDNumber> lcdP1_;
    QSharedPointer<QLCDNumber> lcdP2_;
    QSharedPointer<QLCDNumber> lcdP3_;

    QSharedPointer<Player> player1_;

    explicit Game(int numberOfPlayers, QObject* parent = nullptr);
    ~Game();

    // Getters
    QSharedPointer<Player> player2() const;
    QSharedPointer<Player> player3() const;

    QSharedPointer<Monitor> monitor();

    // Choosers
    QSharedPointer<Chooser> chooser();
    QSharedPointer<QuteChooser> quteChooser();
    QSharedPointer<EightsChooser> eightsChooser();
    QSharedPointer<JpointsChooser> jpointsChooser();
    QSharedPointer<RoundChooser> roundChooser();
    QSharedPointer<JsuitChooser> jsuitChooser();
    //

    QSharedPointer<Got> got1();
    QSharedPointer<Got> got2();
    QSharedPointer<Played> played();
    QSharedPointer<Drawn> drawn();

    QSharedPointer<QLCDNumber> lcdShuffles();
    QSharedPointer<Blind> blind();
    QSharedPointer<Stack> stack();

    QSharedPointer<Playable> playable();
    QSharedPointer<QLCDNumber> lcdRound();
    QSharedPointer<QLCDNumber> lcdP1();
    QSharedPointer<QLCDNumber> lcdP2();
    QSharedPointer<QLCDNumber> lcdP3();

    QSharedPointer<Player> player1() const;

    QVector<QSharedPointer<Player>> playerList_;
    QSharedPointer<Player> player = nullptr;

    bool isNextPlayerPossible();
    void setButtonColors();

signals:
    void cardAddedToStack(const QSharedPointer<Card>& card);
    void cardMustFromBlind(const QSharedPointer<Card>& card);
    void cardBadFromBlind(const QSharedPointer<Card>& card);
    void cardsPlayed(int numCardsPlayed);
    void countPoints(int shuffles);

    // Controls and Settings:
    void setRbNumPlayers(int num);
    void setCbVisible(bool isVisible);
    void setRbCardType(CardType);
    void setCardText();
    void toggleCardsVisible(bool isVisible);
    void resetCbVisible();
    void clickToTable(); // to Table
    void paintDrawButton(DrawOption drawOption);
    void paintNextButton(NextOption nextOption);

public slots:
    void activateNextPlayer();
    void onHandCardClicked(const QSharedPointer<Card>& card);
    void onNewRound();
    void onNewGame();
    void onCbSound(int state);
    void onCbLogging(int state);
    void onCbVisible(bool isVisible);
    void onRbCardType(CardType type);
    void onRbSuit();
    void onRbRank();
    void onBlindClicked();
};

#endif // GAME_H
