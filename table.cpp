#include "table.h"
#include <QDebug>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include "ui_table.h"

Table::Table(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Table)
    , game(QSharedPointer<Game>::create()) // Changed from std::make_unique to QSharedPointer::create
{
    ui->setupUi(this);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Set maximum size constraints based on the available screen size
    setMaximumSize(screenWidth, screenHeight);

    // Find all QHBoxLayouts
    QHBoxLayout* layout_player2 = findChild<QHBoxLayout*>("player2");
    QHBoxLayout* layout_player3 = findChild<QHBoxLayout*>("player3");
    QHBoxLayout* layout_monitor = findChild<QHBoxLayout*>("monitor");

    QHBoxLayout* layout_eightsChooser = findChild<QHBoxLayout*>("eightsChooser");
    QHBoxLayout* layout_quteChooser = findChild<QHBoxLayout*>("quteChooser");
    QHBoxLayout* layout_jpointsChooser = findChild<QHBoxLayout*>("jpointsChooser");
    QHBoxLayout* layout_roundChooser = findChild<QHBoxLayout*>("roundChooser");

    QHBoxLayout* layout_played = findChild<QHBoxLayout*>("played");
    QHBoxLayout* layout_drawn = findChild<QHBoxLayout*>("drawn");
    QHBoxLayout* layout_shuffles = findChild<QHBoxLayout*>("shuffles");
    QHBoxLayout* layout_blind = findChild<QHBoxLayout*>("blind");
    QHBoxLayout* layout_jsuitChooser = findChild<QHBoxLayout*>("jsuitChooser");
    QHBoxLayout* layout_stack = findChild<QHBoxLayout*>("stack");
    QHBoxLayout* layout_playable = findChild<QHBoxLayout*>("playable");
    QHBoxLayout* layout_scores = findChild<QHBoxLayout*>("scores");
    QHBoxLayout* layout_player1 = findChild<QHBoxLayout*>("player1");

    // Add widgets to their respective layouts
    layout_player2->addWidget(game->player2()->handdeck());
    layout_player3->addWidget(game->player3()->handdeck());
    layout_monitor->addWidget(game->monitor().get());

    layout_eightsChooser->addWidget(game->eightsChooser().get());
    layout_quteChooser->addWidget(game->quteChooser().get());
    layout_jpointsChooser->addWidget(game->jpointsChooser().get());
    layout_roundChooser->addWidget(game->roundChooser().get());

    layout_played->addWidget(game->played().get());
    layout_drawn->addWidget(game->drawn().get());

    layout_shuffles->addWidget(game->lcdShuffles().get());
    layout_blind->addWidget(game->blind().get());
    layout_jsuitChooser->addWidget(game->jsuitChooser().get());
    layout_stack->addWidget(game->stack().get());
    layout_playable->addWidget(game->playable().get());
    layout_scores->addWidget(game->lcd1().get());
    layout_scores->addWidget(game->lcd2().get());
    layout_scores->addWidget(game->lcd3().get());
    layout_player1->addWidget(game->player1()->handdeck());

    // // Add group boxes to their respective layouts
    QGroupBox* groupBoxPlayer2 = findChild<QGroupBox*>("Player2");
    groupBoxPlayer2->setLayout(layout_player2);
    QGroupBox* groupBoxPlayer3 = findChild<QGroupBox*>("Player3");
    groupBoxPlayer3->setLayout(layout_player3);

    QGroupBox* groupBoxMonitor = findChild<QGroupBox*>("Monitor");
    groupBoxMonitor->setLayout(layout_monitor);
    QGroupBox* groupBoxEightsChooser = findChild<QGroupBox*>("EightsChooser");
    groupBoxEightsChooser->setLayout(layout_eightsChooser);
    QGroupBox* groupBoxQuteChooser = findChild<QGroupBox*>("QuteChooser");
    groupBoxQuteChooser->setLayout(layout_quteChooser);
    QGroupBox* groupBoxJpointsChooser = findChild<QGroupBox*>("JpointsChooser");
    groupBoxJpointsChooser->setLayout(layout_jpointsChooser);
    QGroupBox* groupBoxRoundChooser = findChild<QGroupBox*>("RoundChooser");
    groupBoxRoundChooser->setLayout(layout_roundChooser);

    QGroupBox* groupBoxPlayed = findChild<QGroupBox*>("Played");
    groupBoxPlayed->setLayout(layout_played);
    QGroupBox* groupBoxDrawn = findChild<QGroupBox*>("Drawn");
    groupBoxDrawn->setLayout(layout_drawn);

    QGroupBox* groupBoxShuffles = findChild<QGroupBox*>("Shuffles");
    QGroupBox* groupBoxBlind = findChild<QGroupBox*>("Blind");
    groupBoxBlind->setLayout(layout_blind);
    QGroupBox* groupBoxJsuitChooser = findChild<QGroupBox*>("JsuitChooser");
    groupBoxJsuitChooser->setLayout(layout_jsuitChooser);
    QGroupBox* groupBoxStack = findChild<QGroupBox*>("Stack");
    groupBoxStack->setLayout(layout_stack);

    QGroupBox* groupBoxPlayable = findChild<QGroupBox*>("Playable");
    groupBoxPlayable->setLayout(layout_playable);

    // QGroupBox* groupBoxScores = findChild<QGroupBox*>("Scores");
    // groupBoxScores->setLayout(layout_scores);

    QGroupBox* groupBoxPlayer1 = findChild<QGroupBox*>("Player1");
    groupBoxPlayer1->setLayout(layout_player1);

    connect(this, &Table::rightMouseClicked, game.get(), &Game::activateNextPlayer);
}

Table::~Table()
{
    delete ui;
}

void Table::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        emit rightMouseClicked();
    }
    QWidget::mousePressEvent(event);
}

void Table::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_R) {
        game->startNewRound();
    }

    if (event->key() == Qt::Key_G) {
        game->startNewGame();
    }

    if (event->key() == Qt::Key_6) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "6");
            game->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_D) {
        game->player->handdeck()->removeFirstCard();
    }

    if (event->key() == Qt::Key_F) {
        game->player->handdeck()->removeLastCard();
    }

    if (event->key() == Qt::Key_8) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "8");
            game->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_J) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "J");
            game->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_S) {
        qDebug() << "* * * * *  Scores * * * * *";
        for (const auto& player : game->playerList_) {
            qDebug() << player->name() << player->score() << player->handdeck()->cardsAsString();
        }
    }

    if (event->key() == Qt::Key_0) {
        game->playable()->sortCards(0);
    }
    if (event->key() == Qt::Key_1) {
        game->playable()->sortCards(1);
    }
    if (event->key() == Qt::Key_2) {
        game->playable()->sortCards(2);
    }
    if (event->key() == Qt::Key_3) {
        game->playable()->sortCards(3);
    }

    QWidget::keyPressEvent(event);
}
