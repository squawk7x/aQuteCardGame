#include "table.h"
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QProcess>
#include <QScreen>
#include <QShortcut>
#include "card.h"
#include "ui_table.h"

Table::Table(int numberOfPlayers, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Table)
{
    ui->setupUi(this);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    setMaximumSize(screenGeometry.width(), screenGeometry.height());

    initializeGame(numberOfPlayers);
}

Table::~Table()
{
    delete ui;
}

void Table::initializeGame(int numberOfPlayers)
{
    if (game_) {
        game_.reset();
    }

    game_ = QSharedPointer<Game>::create(numberOfPlayers);

    // Find all QHBoxLayouts
    QHBoxLayout* layoutPlayer2 = findChild<QHBoxLayout*>("layoutPlayer2");
    QHBoxLayout* layoutPlayer3 = nullptr;
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
        groupBoxPlayer3->show();
    } else {
        groupBoxPlayer3->hide();
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
        groupBoxGot2->hide();
    }
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

    connect(this, &Table::mouseClicked, game_.get(), &Game::activateNextPlayer);

    connect(game_.get(), &Game::setRbNumPlayers, this, [this](int num) {
        if (num == 2) {
            ui->rbNumPlayers2->setChecked(true);
        } else {
            ui->rbNumPlayers3->setChecked(true);
        }
    });
    connect(game_.get(), &Game::setCbVisible, this, [this](bool checked) {
        ui->cbVisible->setChecked(checked);
    });
    connect(game_.get(), &Game::setRbUnsorted, this, [this](bool checked) {
        ui->rbRank->setChecked(checked);
    });

    connect(this, &Table::cbVisible, game_.get(), &Game::onCbVisible);
    connect(ui->cbVisible, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbVisible);
    connect(ui->cbSound, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbSound);
    connect(ui->rbSuit, &QRadioButton::pressed, game_.get(), &Game::onRbSuit);
    connect(ui->rbRank, &QRadioButton::pressed, game_.get(), &Game::onRbRank);
    connect(ui->rbNumPlayers2, &QRadioButton::pressed, this, &Table::onRbNumPlayers2);
    connect(ui->rbNumPlayers3, &QRadioButton::pressed, this, &Table::onRbNumPlayers3);

    // Add F1 shortcut
    QShortcut *f1Shortcut = new QShortcut(QKeySequence("F1"), this);
    connect(f1Shortcut, &QShortcut::activated, this, &Table::openHelpFile);
    QObject::connect(game_.get(), &Game::resetCbVisible, this, &Table::onResetCbVisible);

    emit cbVisible(ui->cbVisible->isChecked());
}

void Table::addSpecialCardsToHand(QKeyEvent* event)
{
    QMap<int, QString> keyToCard = {{Qt::Key_6, "6"},
                                    {Qt::Key_7, "7"},
                                    {Qt::Key_8, "8"},
                                    {Qt::Key_9, "9"},
                                    {Qt::Key_T, "10"},
                                    {Qt::Key_J, "J"},
                                    {Qt::Key_Q, "Q"},
                                    {Qt::Key_K, "K"},
                                    {Qt::Key_A, "A"}};

    if (keyToCard.contains(event->key())) {
        QString rank = keyToCard[event->key()];
        for (const auto &suit : std::as_const(suits)) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, rank);
            game_->player->handdeck()->addCard(newCard);
        }
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

void Table::mousePressEvent(QMouseEvent* event)
{
    // if (event->button() == Qt::RightButton) {
    if (event->button() == Qt::LeftButton) {
        emit mouseClicked();
    }
    QWidget::mousePressEvent(event);
}

void Table::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() >= Qt::Key_6 && event->key() <= Qt::Key_Q) {
            addSpecialCardsToHand(event);
        }
    }
}

void Table::openHelpFile()
{
    // Print the current working directory
    // qDebug() << "Current working directory:" << QDir::currentPath();

    // Using a relative path
    QString relativeFilePath = "../../README.md";

    // Convert relative path to absolute path
    QString absoluteFilePath = QDir::current().absoluteFilePath(relativeFilePath);
    // qDebug() << "Absolute path resolved to:" << absoluteFilePath;

    QFileInfo fileInfo(absoluteFilePath);

    if (!fileInfo.exists()) {
        qDebug() << "File does not exist:" << absoluteFilePath;
        return;
    }

    // Try opening with QDesktopServices
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(absoluteFilePath))) {
        // If it fails, fall back to a text editor
        QProcess::startDetached("xdg-open", QStringList() << absoluteFilePath);
    }
}

void Table::onResetCbVisible(bool isVisible)
{
    ui->cbVisible->setChecked(!isVisible);
    ui->cbVisible->setChecked(isVisible);

    emit cbVisible(ui->cbVisible->isChecked());
}
