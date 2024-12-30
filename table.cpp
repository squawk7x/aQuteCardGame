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
    QPushButton* pbNext = findChild<QPushButton*>("pbNext");
    QPushButton* pbDraw = findChild<QPushButton*>("pbDraw");
    QPushButton* pbHelp = findChild<QPushButton*>("pbHelp");

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

    connect(ui->rbNumPlayers2, &QRadioButton::pressed, this, &Table::onRbNumPlayers2);
    connect(ui->rbNumPlayers3, &QRadioButton::pressed, this, &Table::onRbNumPlayers3);
    connect(ui->cbSound, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbSound);
    connect(ui->cbVisible, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbVisible);
    connect(ui->rbSuit, &QRadioButton::pressed, game_.get(), &Game::onRbSuit);
    connect(ui->rbRank, &QRadioButton::pressed, game_.get(), &Game::onRbRank);
    connect(ui->rbCardsSmall, &QRadioButton::pressed, this, &Table::onRbCardsSmall);   // ToDo
    connect(ui->rbCardsNormal, &QRadioButton::pressed, this, &Table::onRbCardsNormal); // ToDo

    // Eventloop in Android Version different to PC Version
    if (isAndroidVersion) {
        QObject::connect(game_.get(), &Game::resetCbVisible, this, &Table::onResetCbVisible);
        QObject::connect(game_.get()->jsuitChooser().get(),
                         &JsuitChooser::chooserToggled,
                         this,
                         &Table::onChooserToggled);
        QObject::connect(game_.get()->quteChooser().get(),
                         &QuteChooser::chooserToggled,
                         this,
                         &Table::onChooserToggled);
        QObject::connect(game_.get()->eightsChooser().get(),
                         &EightsChooser::chooserToggled,
                         this,
                         &Table::onChooserToggled);
        QObject::connect(game_.get()->jpointsChooser().get(),
                         &JpointsChooser::chooserToggled,
                         this,
                         &Table::onChooserToggled);
        QObject::connect(game_.get()->roundChooser().get(),
                         &RoundChooser::chooserToggled,
                         this,
                         &Table::onChooserToggled);
    }

    // Pushbuttons
    QObject::connect(pbNext, &QPushButton::clicked, this, &Table::onNextClicked);
    QObject::connect(pbDraw, &QPushButton::clicked, this, &Table::onDrawClicked);
    // QObject::connect(pbHelp, &QPushButton::clicked, this, &Table::onHelpClicked);

    onResetCbVisible(false);
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

void Table::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() >= Qt::Key_6 && event->key() <= Qt::Key_Q) {
            addSpecialCardsToHand(event);
        }
    }
}

#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>

void Table::onHelpClicked()
{
    // Using qrc path
    QString resourcePath = ":README.md";

    QFile resourceFile(resourcePath);
    if (!resourceFile.exists()) {
        qDebug() << "File does not exist in resources:" << resourcePath;
        return;
    }

    // Open the resource file
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open resource file:" << resourcePath;
        return;
    }

    // Create a temporary file to hold the resource content
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qDebug() << "Failed to create a temporary file";
        return;
    }

    // Write the resource content to the temporary file
    tempFile.write(resourceFile.readAll());
    tempFile.flush(); // Ensure content is written to disk
    resourceFile.close();

    // Get the path of the temporary file
    QString tempFilePath = tempFile.fileName();

    // Try opening with QDesktopServices
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath))) {
        // If it fails, fall back to a text editor
        QProcess::startDetached("xdg-open", QStringList() << tempFilePath);
    }

    // Temporary file will automatically delete when it goes out of scope
}

void Table::onResetCbVisible(bool isVisible)
{
    ui->cbVisible->setChecked(!isVisible);
    ui->cbVisible->setChecked(isVisible);

    emit cbVisible(ui->cbVisible->isChecked());
}

void Table::onNextClicked()
{
    game_->activateNextPlayer();
}

void Table::onDrawClicked()
{
    game_->onBlindClicked();
}

void Table::onRbNumPlayers2()
{
    initializeGame(2);
}

void Table::onRbNumPlayers3()
{
    initializeGame(3);
}

void Table::onCbSound() {} // Transfer to Game

void Table::onCbVisible() {} // Transfer to Game

void Table::onRbSuit() {} // Transfer to Game

void Table::onRbRank() {} // Transfer to Game

void Table::onRbCardsSmall()
{
    qDebug() << "rbCardsSmall clicked"; // ToDo
}

void Table::onRbCardsNormal()
{
    qDebug() << "rbCardsNormal clicked"; // ToDo
}

void Table::onChooserToggled()
{
    if (isAndroidVersion)
        update();
}
