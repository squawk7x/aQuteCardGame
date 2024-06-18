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
{
    ui->setupUi(this);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    setMaximumSize(screenWidth, screenHeight);

    initializeGame(numberOfPlayers);
}

void Table::initializeGame(int numberOfPlayers)
{
    if (game_) {
        game_.reset();
    }

    game_ = QSharedPointer<Game>::create(numberOfPlayers);

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

    QGroupBox* groupBoxPlayer3 = findChild<QGroupBox*>("gbPlayer3");
    if (numberOfPlayers == 3) {
        groupBoxPlayer3->setLayout(layoutPlayer3);
        groupBoxPlayer3->show(); // Ensure the group box is visible
    } else {
        groupBoxPlayer3->hide(); // Hide the group box for player 3 if 2 players only
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
    if (numberOfPlayers == 3) {
        groupBoxGot2->setLayout(layoutGot2);
        groupBoxGot2->show();
    } else {
        groupBoxGot2->hide(); // Hide the group box for overnext player if 2 players only
    }

    QGroupBox* groupBoxPlayed = findChild<QGroupBox*>("gbPlayed");
    groupBoxPlayed->setLayout(layoutPlayed);
    QGroupBox* groupBoxDrawn = findChild<QGroupBox*>("gbDrawn");
    groupBoxDrawn->setLayout(layoutDrawn);

    QGroupBox* groupBoxShuffles = findChild<QGroupBox*>("gbShuffles");
    groupBoxShuffles->setLayout(layoutShuffles);
    QGroupBox* groupBoxBlind = findChild<QGroupBox*>("gbBlind");
    groupBoxBlind->setLayout(layoutBlind);
    // groupBoxBlind->setStyleSheet("QGroupBox QWidget { background-color: red; }");
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

    // Settings
    connect(game_.get(), &Game::setCbVisible, this, [this](bool checked) {
        ui->cbVisible->setChecked(checked);
    });
    connect(game_.get(), &Game::setRbUnsorted, this, [this](bool checked) {
        ui->rbUnsorted->setChecked(checked);
    });

    connect(this, &Table::cbVisibleStatus, game_.get(), &Game::onCbVisibleStatus);
    connect(ui->cbVisible, &QCheckBox::stateChanged, game_.get(), &Game::onCbVisible);
    connect(ui->cbSound, &QCheckBox::stateChanged, game_.get(), &Game::onCbSound);
    connect(ui->rbSuit, &QRadioButton::pressed, game_.get(), &Game::onRbSuit);
    connect(ui->rbRank, &QRadioButton::pressed, game_.get(), &Game::onRbRank);
    connect(ui->rbNumPlayers2, &QRadioButton::pressed, this, &Table::onRbNumPlayers2);
    connect(ui->rbNumPlayers3, &QRadioButton::pressed, this, &Table::onRbNumPlayers3);

    // if must draw a card from blind turn blind red
    // connect(game_.get(), &Game::setBlindRed, this, &Table::onSetBlindRed);

    // connect(game_->blind().get(),
    //         &Blind::blindClicked,
    //         game_.get(),
    //         &Game::onBlindClicked); // Connect the click signal to the Game slot

    emit cbVisibleStatus(ui->cbVisible->isChecked());
}

Table::~Table()
{
    delete ui;
}

void Table::addSpecialCardsToHand(QKeyEvent* event, const QVector<QString>& suits)
{
    QMap<int, QString> keyToCard = {{Qt::Key_6, "6"},
                                    {Qt::Key_7, "7"},
                                    {Qt::Key_8, "8"},
                                    {Qt::Key_9, "9"},
                                    {Qt::Key_K, "K"},
                                    {Qt::Key_A, "A"},
                                    {Qt::Key_J, "J"}};

    if (keyToCard.contains(event->key())) {
        QString cardValue = keyToCard[event->key()];
        for (const auto& suit : suits) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, cardValue);
            game_->player->handdeck()->addCard(newCard);
        }
    }
}

void Table::sortCardsByPattern(QKeyEvent* event)
{
    int key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_4) {
        int pattern = key - Qt::Key_0;
        game_->player->handdeck()->sortCardsByPattern(pattern);
    }
}

void Table::onRbNumPlayers2()
{
    initializeGame(2);
}

void Table::onRbNumPlayers3()
{
    initializeGame(3);
}

// void Table::onSetBlindRed(bool red)
// {
//     QGroupBox* groupBoxBlind = findChild<QGroupBox*>("gbBlind");
//     if (groupBoxBlind) {
//         if (red)
//             groupBoxBlind->setStyleSheet("QGroupBox QWidget { background-color: red; }");
//         else
//             groupBoxBlind->setStyleSheet("");
//     }
// }

void Table::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        emit rightMouseClicked();
    }

    QWidget::mousePressEvent(event);
}

void Table::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F1) {
        openReadmeFile();
    }

    if (event->key() == Qt::Key_G) {
        game_->onNewGame();
    }

    //For Testing:
    if (event->key() == Qt::Key_D) {
        game_->player->handdeck()->moveTopCardTo(game_->blind().get());
    }

    addSpecialCardsToHand(event, suits);
    sortCardsByPattern(event);

    QWidget::keyPressEvent(event);
}

void Table::openReadmeFile()
{
    QString readmePath = "/home/andreas/Qt/Projects/aQuteCardGame/README.md";
    QDesktopServices::openUrl(QUrl::fromLocalFile(readmePath));
}
