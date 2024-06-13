#include "table.h"
#include <QDebug>
#include <QDesktopServices>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include "card.h"
#include "ui_table.h"

Table::Table(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Table)
    , game_(QSharedPointer<Game>::create())
    , isCardFaceVisible_(true)
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
    QHBoxLayout* layout_got1 = findChild<QHBoxLayout*>("got1");
    QHBoxLayout* layout_got2 = findChild<QHBoxLayout*>("got2");
    QHBoxLayout* layout_played = findChild<QHBoxLayout*>("played");
    QHBoxLayout* layout_drawn = findChild<QHBoxLayout*>("drawn");

    QHBoxLayout* layout_shuffles = findChild<QHBoxLayout*>("shuffles");
    QHBoxLayout* layout_blind = findChild<QHBoxLayout*>("blind");
    QHBoxLayout* layout_jsuitChooser = findChild<QHBoxLayout*>("jsuitChooser");
    QHBoxLayout* layout_stack = findChild<QHBoxLayout*>("stack");
    QHBoxLayout* layout_playable = findChild<QHBoxLayout*>("playable");
    QHBoxLayout* layout_round = findChild<QHBoxLayout*>("round");
    QHBoxLayout* layout_scores = findChild<QHBoxLayout*>("scores");
    QHBoxLayout* layout_player1 = findChild<QHBoxLayout*>("player1");

    // Add widgets to their respective layouts
    layout_player2->addWidget(game_->player2()->handdeck().get());
    layout_player3->addWidget(game_->player3()->handdeck().get());
    layout_monitor->addWidget(game_->monitor().get());

    layout_eightsChooser->addWidget(game_->eightsChooser().get());
    layout_quteChooser->addWidget(game_->quteChooser().get());
    layout_jpointsChooser->addWidget(game_->jpointsChooser().get());
    layout_roundChooser->addWidget(game_->roundChooser().get());

    layout_got1->addWidget(game_->got1().get());
    layout_got2->addWidget(game_->got2().get());
    layout_played->addWidget(game_->played().get());
    layout_drawn->addWidget(game_->drawn().get());

    layout_shuffles->addWidget(game_->lcdShuffles().get());
    layout_blind->addWidget(game_->blind().get());
    layout_jsuitChooser->addWidget(game_->jsuitChooser().get());
    layout_stack->addWidget(game_->stack().get());

    layout_playable->addWidget(game_->playable().get());
    layout_round->addWidget(game_->lcdRound().get());
    layout_scores->addWidget(game_->lcdP1().get());
    layout_scores->addWidget(game_->lcdP2().get());
    layout_scores->addWidget(game_->lcdP3().get());

    layout_player1->addWidget(game_->player1()->handdeck().get());

    // Add group boxes to their respective layouts
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

    QGroupBox* groupBoxGot1 = findChild<QGroupBox*>("Got1");
    groupBoxGot1->setLayout(layout_got1);
    QGroupBox* groupBoxGot2 = findChild<QGroupBox*>("Got2");
    groupBoxGot2->setLayout(layout_got2);
    QGroupBox* groupBoxPlayed = findChild<QGroupBox*>("Played");
    groupBoxPlayed->setLayout(layout_played);
    QGroupBox* groupBoxDrawn = findChild<QGroupBox*>("Drawn");
    groupBoxDrawn->setLayout(layout_drawn);

    QGroupBox* groupBoxShuffles = findChild<QGroupBox*>("Shuffles");
    groupBoxShuffles->setLayout(layout_shuffles);
    QGroupBox* groupBoxBlind = findChild<QGroupBox*>("Blind");
    groupBoxBlind->setLayout(layout_blind);
    QGroupBox* groupBoxJsuitChooser = findChild<QGroupBox*>("JsuitChooser");
    groupBoxJsuitChooser->setLayout(layout_jsuitChooser);
    QGroupBox* groupBoxStack = findChild<QGroupBox*>("Stack");
    groupBoxStack->setLayout(layout_stack);

    QGroupBox* groupBoxPlayable = findChild<QGroupBox*>("Playable");
    groupBoxPlayable->setLayout(layout_playable);
    QGroupBox* groupBoxRound = findChild<QGroupBox*>("Round");
    groupBoxRound->setLayout(layout_round);
    QGroupBox* groupBoxScores = findChild<QGroupBox*>("Scores");
    groupBoxScores->setLayout(layout_scores);

    QGroupBox* groupBoxPlayer1 = findChild<QGroupBox*>("Player1");
    groupBoxPlayer1->setLayout(layout_player1);

    connect(this, &Table::rightMouseClicked, game_.get(), &Game::activateNextPlayer);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->player2()->handdeck().get(),
            &Handdeck::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->player3()->handdeck().get(),
            &Handdeck::onToggleIsCardFaceVisible);

    // in Monitor card face always shown
    // connect(this,
    //         &Table::toggleIsCardFaceVisible,
    //         game_->monitor().get(),
    //         &Monitor::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->got1().get(),
            &Got::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->got2().get(),
            &Got::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->blind().get(),
            &Blind::onToggleIsCardFaceVisible);

    // in Stack card face always shown
    // connect(this,
    //         &Table::toggleIsCardFaceVisible,
    //         game_->stack().get(),
    //         &Stack::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->playable().get(),
            &Playable::onToggleIsCardFaceVisible);

    // in played cards card face always shown
    // connect(this,
    //         &Table::toggleIsCardFaceVisible,
    //         game_->played().get(),
    //         &Played::onToggleIsCardFaceVisible);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->drawn().get(),
            &Drawn::onToggleIsCardFaceVisible);

    // in handdeck of player 1 card face always shown
    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->player1()->handdeck().get(),
            &Handdeck::onToggleIsCardFaceVisible);

    // does not work as expected:
    // emit toggleIsCardFaceVisible(isCardFaceVisible_);

    // this makes player 1 visible == true and opponent players visible == false:
    QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::NoModifier);
    QApplication::postEvent(this, event);

    connect(game_.get(), &Game::setIsCardVisible, this, &Table::onSetIsCardVisible);
}

Table::~Table()
{
    delete ui;
}

void Table::openReadmeFile()
{
    QString readmePath = "/home/andreas/Qt/Projects/aQuteCardGame/README.md";
    QDesktopServices::openUrl(QUrl::fromLocalFile(readmePath));
}

void Table::onSetIsCardVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;
    emit toggleIsCardFaceVisible(isCardFaceVisible_);
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
    if (event->key() == Qt::Key_V) {
        isCardFaceVisible_ = !isCardFaceVisible_;
        qDebug() << "isCardFaceVisible:" << isCardFaceVisible_;
        emit toggleIsCardFaceVisible(isCardFaceVisible_);
    }

    if (event->key() == Qt::Key_F1) {
        openReadmeFile();
    }

    if (event->key() == Qt::Key_R) {
        game_->player->handdeck()->sortCards(Handdeck::SortOption::Rank);
    }

    if (event->key() == Qt::Key_S) {
        game_->player->handdeck()->sortCards(Handdeck::SortOption::Suit);
    }

    if (event->key() == Qt::Key_1) {
        game_->player->handdeck()->sortCardsByPattern(0);
    }
    if (event->key() == Qt::Key_1) {
        game_->player->handdeck()->sortCardsByPattern(1);
    }
    if (event->key() == Qt::Key_1) {
        game_->player->handdeck()->sortCardsByPattern(2);
    }
    if (event->key() == Qt::Key_1) {
        game_->player->handdeck()->sortCardsByPattern(3);
    }

    if (event->key() == Qt::Key_F) {
        emit game_->roundChooser()->finishRound();
    }

    if (event->key() == Qt::Key_N) {
        emit game_->roundChooser()->newRound();
    }

    if (event->key() == Qt::Key_G) {
        emit game_->roundChooser()->newGame();
    }

    if (event->key() == Qt::Key_6) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "6");
            game_->player->handdeck()->addCard(newCard);
        }
    }

    // Special Cards for Testing
    if (event->key() == Qt::Key_7) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "7");
            game_->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_8) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "8");
            game_->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_A) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "A");
            game_->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_J) {
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, "J");
            game_->player->handdeck()->addCard(newCard);
        }
    }

    if (event->key() == Qt::Key_D) {
        emit game_->player->handdeck()->moveTopCardTo(game_->blind().get());
    }

    QWidget::keyPressEvent(event);
}
