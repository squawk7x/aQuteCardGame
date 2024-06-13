#include "table.h"
#include <QDebug>
#include <QDesktopServices>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScreen> // Include QScreen header
#include "card.h"
#include "ui_table.h"

Table::Table(int numberOfPlayers, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Table)
    , game_(QSharedPointer<Game>::create(numberOfPlayers))
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
    QHBoxLayout* layoutPlayer2 = findChild<QHBoxLayout*>("layoutPlayer2");
    QHBoxLayout* layoutPlayer3 = nullptr; // Initialize to nullptr
    if (numberOfPlayers == 3) {
        layoutPlayer3 = findChild<QHBoxLayout*>("layoutPlayer3");
    }

    QHBoxLayout* layoutMonitor = findChild<QHBoxLayout*>("layoutMonitor");
    QHBoxLayout* layoutEightsChooser = findChild<QHBoxLayout*>("layoutEightsChooser");
    QHBoxLayout* layoutQuteChooser = findChild<QHBoxLayout*>("layoutQuteChooser");
    QHBoxLayout* layoutJpointsChooser = findChild<QHBoxLayout*>("layoutJpointsChooser");
    QHBoxLayout* layoutRoundChooser = findChild<QHBoxLayout*>("layoutRoundChooser");
    QHBoxLayout* layoutGot1 = findChild<QHBoxLayout*>("layoutGot1");
    QHBoxLayout* layoutGot2 = findChild<QHBoxLayout*>("layoutGot2");
    QHBoxLayout* layoutPlayed = findChild<QHBoxLayout*>("layoutPlayed");
    QHBoxLayout* layoutDrawn = findChild<QHBoxLayout*>("layoutDrawn");

    QHBoxLayout* layoutShuffles = findChild<QHBoxLayout*>("layoutShuffles");
    QHBoxLayout* layoutBlind = findChild<QHBoxLayout*>("layoutBlind");
    QHBoxLayout* layoutJsuitChooser = findChild<QHBoxLayout*>("layoutJsuitChooser");
    QHBoxLayout* layoutStack = findChild<QHBoxLayout*>("layoutStack");
    QHBoxLayout* layoutPlayable = findChild<QHBoxLayout*>("layoutPlayable");
    QHBoxLayout* layoutRound = findChild<QHBoxLayout*>("layoutRound");
    QHBoxLayout* layoutScores = findChild<QHBoxLayout*>("layoutScores");
    QHBoxLayout* layoutPlayer1 = findChild<QHBoxLayout*>("layoutPlayer1");

    // Add widgets to their respective layouts
    layoutPlayer2->addWidget(game_->player2()->handdeck().get());
    if (numberOfPlayers == 3) {
        layoutPlayer3->addWidget(game_->player3()->handdeck().get());
    }

    layoutMonitor->addWidget(game_->monitor().get());
    layoutEightsChooser->addWidget(game_->eightsChooser().get());
    layoutQuteChooser->addWidget(game_->quteChooser().get());
    layoutJpointsChooser->addWidget(game_->jpointsChooser().get());
    layoutRoundChooser->addWidget(game_->roundChooser().get());

    layoutGot1->addWidget(game_->got1().get());
    layoutGot2->addWidget(game_->got2().get());
    layoutPlayed->addWidget(game_->played().get());
    layoutDrawn->addWidget(game_->drawn().get());

    layoutShuffles->addWidget(game_->lcdShuffles().get());
    layoutBlind->addWidget(game_->blind().get());
    layoutJsuitChooser->addWidget(game_->jsuitChooser().get());
    layoutStack->addWidget(game_->stack().get());

    layoutPlayable->addWidget(game_->playable().get());
    layoutRound->addWidget(game_->lcdRound().get());
    layoutScores->addWidget(game_->lcdP1().get());
    layoutScores->addWidget(game_->lcdP2().get());
    if (numberOfPlayers == 3) {
        layoutScores->addWidget(game_->lcdP3().get());
    }

    layoutPlayer1->addWidget(game_->player1()->handdeck().get());

    // Add group boxes to their respective layouts
    QGroupBox* groupBoxPlayer2 = findChild<QGroupBox*>("gbPlayer2");
    groupBoxPlayer2->setLayout(layoutPlayer2);

    QGroupBox* groupBoxPlayer3 = nullptr;
    if (numberOfPlayers == 3) {
        QGroupBox* groupBoxPlayer3 = findChild<QGroupBox*>("gbPlayer3");
        groupBoxPlayer3->setLayout(layoutPlayer3);
    }

    QGroupBox* groupBoxMonitor = findChild<QGroupBox*>("gbMonitor");
    groupBoxMonitor->setLayout(layoutMonitor);
    QGroupBox* groupBoxEightsChooser = findChild<QGroupBox*>("gbEightsChooser");
    groupBoxEightsChooser->setLayout(layoutEightsChooser);
    QGroupBox* groupBoxQuteChooser = findChild<QGroupBox*>("gbQuteChooser");
    groupBoxQuteChooser->setLayout(layoutQuteChooser);
    QGroupBox* groupBoxJpointsChooser = findChild<QGroupBox*>("gbJpointsChooser");
    groupBoxJpointsChooser->setLayout(layoutJpointsChooser);
    QGroupBox* groupBoxRoundChooser = findChild<QGroupBox*>("gbRoundChooser");
    groupBoxRoundChooser->setLayout(layoutRoundChooser);

    QGroupBox* groupBoxGot1 = findChild<QGroupBox*>("gbGot1");
    groupBoxGot1->setLayout(layoutGot1);
    QGroupBox* groupBoxGot2 = findChild<QGroupBox*>("gbGot2");
    groupBoxGot2->setLayout(layoutGot2);
    QGroupBox* groupBoxPlayed = findChild<QGroupBox*>("gbPlayed");
    groupBoxPlayed->setLayout(layoutPlayed);
    QGroupBox* groupBoxDrawn = findChild<QGroupBox*>("gbDrawn");
    groupBoxDrawn->setLayout(layoutDrawn);

    QGroupBox* groupBoxShuffles = findChild<QGroupBox*>("gbShuffles");
    groupBoxShuffles->setLayout(layoutShuffles);
    QGroupBox* groupBoxBlind = findChild<QGroupBox*>("gbBlind");
    groupBoxBlind->setLayout(layoutBlind);
    QGroupBox* groupBoxJsuitChooser = findChild<QGroupBox*>("gbJsuitChooser");
    groupBoxJsuitChooser->setLayout(layoutJsuitChooser);
    QGroupBox* groupBoxStack = findChild<QGroupBox*>("gbStack");
    groupBoxStack->setLayout(layoutStack);

    QGroupBox* groupBoxPlayable = findChild<QGroupBox*>("gbPlayable");
    groupBoxPlayable->setLayout(layoutPlayable);
    QGroupBox* groupBoxRound = findChild<QGroupBox*>("gbRound");
    groupBoxRound->setLayout(layoutRound);
    QGroupBox* groupBoxScores = findChild<QGroupBox*>("gbScores");
    groupBoxScores->setLayout(layoutScores);

    QGroupBox* groupBoxPlayer1 = findChild<QGroupBox*>("gbPlayer1");
    groupBoxPlayer1->setLayout(layoutPlayer1);

    connect(this, &Table::rightMouseClicked, game_.get(), &Game::activateNextPlayer);

    connect(this,
            &Table::toggleIsCardFaceVisible,
            game_->player2()->handdeck().get(),
            &Handdeck::onToggleIsCardFaceVisible);

    if (numberOfPlayers == 3) {
        connect(this,
                &Table::toggleIsCardFaceVisible,
                game_->player3()->handdeck().get(),
                &Handdeck::onToggleIsCardFaceVisible);
    }

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

    if (event->key() == Qt::Key_0) {
        game_->player->handdeck()->sortCardsByPattern(0);
    }
    if (event->key() == Qt::Key_1) {
        game_->player->handdeck()->sortCardsByPattern(1);
    }
    if (event->key() == Qt::Key_2) {
        game_->player->handdeck()->sortCardsByPattern(2);
    }
    if (event->key() == Qt::Key_3) {
        game_->player->handdeck()->sortCardsByPattern(3);
    }
    if (event->key() == Qt::Key_4) {
        game_->player->handdeck()->sortCardsByPattern(4);
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
        game_->player->handdeck()->moveTopCardTo(game_->blind().get());
    }

    QWidget::keyPressEvent(event);
}
