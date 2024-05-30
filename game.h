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
#include <memory>

class Game : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<Monitor> monitor_;
    std::unique_ptr<EightsChooser> eightsChooser_;
    std::unique_ptr<QuteChooser> quteChooser_;
    std::unique_ptr<JpointsChooser> jpointsChooser_;
    std::unique_ptr<RoundChooser> roundChooser_;
    std::unique_ptr<Played> played_;
    std::unique_ptr<Blind> blind_;
    std::unique_ptr<JsuitChooser> jsuitChooser_;
    std::unique_ptr<Stack> stack_;
    std::unique_ptr<Playable> playable_;
    std::unique_ptr<Drawn> drawn_;
    std::unique_ptr<Player> player1_;
    std::unique_ptr<Player> player2_;
    std::unique_ptr<Player> player3_;
    int games = 1;
    int rounds = 1;
    int shuffles = 1;

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void initializeGame();

    // Getters
    QVector<Player*> playerList_;
    Player* player1() const;
    Player* player2() const;
    Player* player3() const;
    Player* player = nullptr;
    std::unique_ptr<Monitor>& monitor();
    std::unique_ptr<EightsChooser>& eightsChooser();
    std::unique_ptr<QuteChooser>& quteChooser();
    std::unique_ptr<JpointsChooser>& jpointsChooser();
    std::unique_ptr<RoundChooser>& roundChooser();
    std::unique_ptr<Played>& played();
    std::unique_ptr<Drawn>& drawn();
    std::unique_ptr<Blind>& blind();
    std::unique_ptr<JsuitChooser>& jsuitChooser();
    std::unique_ptr<Stack>& stack();
    std::unique_ptr<Playable>& playable();
    bool isCardPlayable(const QSharedPointer<Card>& card);
    bool isNextPlayerPossible();
    void updatePlayable();
    bool mustDrawCard();
    void autoplay();
    void getPlayableCard();
    void refillBlindFromStack();
    void collectAllCardsToBlind();
    void countRound();
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
