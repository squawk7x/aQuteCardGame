#include "table.h"
#include <QDebug>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QShortcut>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include "card.h"
#include "playable.h"
#include "ui_table.h"
#include <qstyle.h>

Table::Table(int numberOfPlayers, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Table)
    , mainWindow_(parent)
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

    // Find all QHBoxLayouts defined in table.ui
    // QHBoxLayout* layoutPlayer2 = findChild<QHBoxLayout*>("layoutPlayer2");
    QHBoxLayout* layoutPlayer3 = nullptr;
    if (numberOfPlayers == 3) {
      layoutPlayer3 = findChild<QHBoxLayout*>("layoutPlayer3");
    }

    // Add widgets to their respective layouts
    ui->gbPlayer2Layout->addWidget(game_->player2()->handdeck().get());
    if (numberOfPlayers == 3) {
        layoutPlayer3->addWidget(game_->player3()->handdeck().get());
    }
    ui->gbMonitorLayout->addWidget(game_->monitor_.get());
    ui->gbEightsChooserLayout->addWidget(game_->eightsChooser().get());
    ui->gbQuteChooserLayout->addWidget(game_->quteChooser().get());
    ui->gbJpointsChooserLayout->addWidget(game_->jpointsChooser().get());
    ui->gbRoundChooserLayout->addWidget(game_->roundChooser().get());
    ui->gbGot1Layout->addWidget(game_->got1().get());
    ui->gbGot2Layout->addWidget(game_->got2().get());
    ui->gbPlayedLayout->addWidget(game_->played().get());
    ui->gbDrawnLayout->addWidget(game_->drawn().get());
    ui->gbShufflesLayout->addWidget(game_->lcdShuffles().get());
    ui->gbBlindLayout->addWidget(game_->blind().get());
    ui->gbJsuitChooserLayout->addWidget(game_->jsuitChooser().get());
    ui->gbStackLayout->addWidget(game_->stack().get());
    ui->gbPlayableLayout->addWidget(game_->playable().get());
    ui->gbRoundLayout->addWidget(game_->lcdRound().get());
    ui->gbScoresLayout->addWidget(game_->lcdP1().get());
    ui->gbScoresLayout->addWidget(game_->lcdP2().get());
    if (numberOfPlayers == 3) {
      ui->gbScoresLayout->addWidget(game_->lcdP3().get());
    }
    ui->gbPlayer1Layout->addWidget(game_->player1()->handdeck().get());

    if (numberOfPlayers == 3) {
      ui->gbPlayer3->show();
      ui->gbGot2->show();
    } else {
      ui->gbPlayer3->hide();
      ui->gbGot2->hide();
    }

    connect(game_.get(), &Game::setRbNumPlayers, this, [&](int num) {
        if (num == 2) {
            ui->rbNumPlayers2->setChecked(true);
        } else {
            ui->rbNumPlayers3->setChecked(true);
        }
    });

    connect(game_.get(), &Game::setCbVisible, this, [&](bool checked) {
        ui->cbVisible->setChecked(checked);
    });

    // connect(game_.get(), &Game::onCbLogging, this, [&](bool checked) {
    //     ui->cbLogging->setChecked(checked);
    // });

    connect(game_.get(), &Game::setRbCardType, this, [&](CardType type) {
        if (type == CardType::Small) {
            ui->rbCardTypeSmall->setChecked(true); // Ensures the button is checked
            ui->rbCardTypeSmall->click();          // Simulate the click
        } else if (type == CardType::Normal) {
            ui->rbCardTypeNormal->setChecked(true); // Ensures the button is checked
            ui->rbCardTypeNormal->click();          // Simulate the click
        }
    });

    connect(ui->rbNumPlayers2, &QRadioButton::pressed, this, &Table::onRbNumPlayers2);
    connect(ui->rbNumPlayers3, &QRadioButton::pressed, this, &Table::onRbNumPlayers3);
    connect(ui->cbSound, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbSound);
    connect(ui->cbLogging, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbLogging);
    connect(ui->cbVisible, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbVisible);
    connect(ui->cbLogging, &QCheckBox::checkStateChanged, game_.get(), &Game::onCbLogging);
    connect(ui->rbSuit, &QRadioButton::pressed, game_.get(), &Game::onRbSuit);
    connect(ui->rbRank, &QRadioButton::pressed, game_.get(), &Game::onRbRank);
    connect(ui->rbCardTypeSmall, &QRadioButton::clicked, game_.get(), [&]() {
        game_->onRbCardType(CardType::Small);
        if (mainWindow_) {
            mainWindow_->adjustSize();
        }
    });
    connect(ui->rbCardTypeNormal, &QRadioButton::clicked, game_.get(), [&]() {
        game_->onRbCardType(CardType::Normal);
    });

    QObject::connect(game_.get(), &Game::resetCbVisible, this, &Table::onResetCbVisible);

    // onChooserToggled -> update() is needed for ANDROID
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

    // needed to toggle Button Colors
    QObject::connect(game_.get()->quteChooser().get(),
                     &QuteChooser::quteDecisionChanged,
                     this,
                     &::Table::onDecisionChanged);

    QObject::connect(game_.get()->roundChooser().get(),
                     &RoundChooser::roundDecisionChanged,
                     this,
                     &::Table::onDecisionChanged);

    // Paint Next and Draw Button
    QObject::connect(game_.get(), &Game::paintNextButton, this, &Table::onPaintNextButton);
    QObject::connect(game_.get(), &Game::paintDrawButton, this, &Table::onPaintDrawButton);

    // Pushbuttons
    QObject::connect(ui->pbNext, &QPushButton::clicked, this,
                     &Table::onNextClicked);
    QObject::connect(ui->pbDraw, &QPushButton::clicked, this,
                     &Table::onDrawClicked);
    QObject::connect(ui->pbInfo, &QPushButton::clicked, this,
                     &Table::onInfoClicked);

    ui->rbCardTypeSmall->click();
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
        for (const auto& suit : std::as_const(suits)) {
            QSharedPointer<Card> newCard = QSharedPointer<Card>::create(suit, rank);
            game_->player->handdeck()->addCard(newCard);
        }
    }
}

void Table::keyPressEvent(QKeyEvent* event)
// Add cards for testing
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() >= Qt::Key_6 && event->key() <= Qt::Key_Q) {
            addSpecialCardsToHand(event);
            onResetCbVisible();
        }
    }

    //     4 QUTE               5 EIGHTS/JPOINTS    6 FINISH/ROUND/GAME
    //     1 PLAYABLE/JSUIT     2 PLAY              3 NEXT
    //     0 DRAW

    // NUM 0
    if (event->key() == Qt::Key_0) {
        ui->pbDraw->click();
        event->accept();

        // NUM 1 2 3
    } else if (game_.get()->jsuitChooser()->isEnabled() && event->key() == Qt::Key_1) {
        game_.get()->jsuitChooser()->toggle();
        event->accept();
    } else if (event->key() == Qt::Key_1) {
        game_.get()->playable()->togglePlayableCards();
        event->accept();
    } else if (!game_.get()->playable()->cards().isEmpty() && event->key() == Qt::Key_2) {
        auto card = game_.get()->playable()->cards().first();
        (game_.get()->player->handdeck()->playThisCard(*card));
        event->accept();
        // BUGFIX infinite loop:
    } else if (game_.get()->isNextPlayerPossible() && event->key() == Qt::Key_3) {
        ui->pbNext->click();
        event->accept();
    }

    // NUM 4 5 6
    else if (game_.get()->quteChooser()->isEnabled() && event->key() == Qt::Key_4) {
        game_.get()->quteChooser()->toggle();
        event->accept();
    } else if (game_.get()->eightsChooser()->isEnabled() && event->key() == Qt::Key_5) {
        game_.get()->eightsChooser()->toggle();
        event->accept();
    } else if (game_.get()->jpointsChooser()->isEnabled() && event->key() == Qt::Key_5) {
        game_.get()->jpointsChooser()->toggle();
        event->accept();
    } else if (game_.get()->roundChooser()->isEnabled() && event->key() == Qt::Key_6) {
        game_.get()->roundChooser()->click();
        event->accept();
    } else {
        // event->accept();
        // Table::keyPressEvent(event);
    }
}

void Table::onInfoClicked()
{
    // Load markdown content from a resource file
    QString resourcePath = ":README.md";

    QFile resourceFile(resourcePath);
    if (!resourceFile.exists()) {
        QMessageBox::warning(this, "Error", "File does not exist in resources: " + resourcePath);
        return;
    }

    if (!resourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open resource file: " + resourcePath);
        return;
    }

    QTextStream stream(&resourceFile);
    QString fileContent = stream.readAll();
    resourceFile.close();

    // Create a custom dialog
    QDialog dialog(this);
    dialog.setWindowTitle("Markdown Reader");
    dialog.resize(800, 600); // Set an initial size for the dialog

    // Create a text editor for markdown
    QTextEdit* editor = new QTextEdit(&dialog);
    editor->setMarkdown(fileContent); // Display markdown content
    editor->setReadOnly(true);        // Make the editor read-only to disable editing

    // Prevent the keyboard from showing up on Android by setting focus policy to NoFocus
    // editor->setFocusPolicy(Qt::NoFocus);

    // Create a scroll area to contain the editor
    QScrollArea* scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidget(editor);        // Set the editor as the scrollable widget
    scrollArea->setWidgetResizable(true); // Allow widget resizing within the scroll area
    // Always show the vertical scrollbar
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Ensure that the scroll area handles touch gestures properly on Android
    // scrollArea->setHorizontalScrollBarPolicy(
    //     Qt::ScrollBarAlwaysOn); // Disable horizontal scrollbar for markdown content

    // Create a layout for the dialog
    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
    mainLayout->addWidget(scrollArea); // Add the scroll area to the layout

    // Create a "Close" button and make sure it's always visible
    QPushButton* closeButton = new QPushButton("Close", &dialog);

    // Ensure the button stays at the bottom of the dialog
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    // Connect the Close button to the dialog's reject() method
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // Execute the dialog
    dialog.exec();
}

void Table::onResetCbVisible()
{
    bool isVisible = ui->cbVisible->isChecked();
    ui->cbVisible->setChecked(!isVisible);
    ui->cbVisible->setChecked(isVisible);
    // emit cbVisible(ui->cbVisible->isChecked());
}

void Table::onDrawClicked()
{
    game_->onBlindClicked();
}

void Table::onNextClicked()
{
    game_->activateNextPlayer();
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
void Table::onCbLogging() {} // Transfer to Game
void Table::onRbSuit() {} // Transfer to Game
void Table::onRbRank() {} // Transfer to Game

void Table::onChooserToggled()
{
    update();
}

void Table::onPaintDrawButton(DrawOption drawOption)
{
    ui->pbDraw->setProperty("control",
                            drawOption == DrawOption::MustCard
                                && !game_.get()->roundChooser()->isEnabled());
    ui->pbDraw->style()->unpolish(ui->pbDraw);
    ui->pbDraw->style()->polish(ui->pbDraw);
}

void Table::onPaintNextButton(NextOption nextOption)
{
    ui->pbNext->setProperty("control",
                            nextOption == NextOption::Possible
                                && !game_.get()->roundChooser()->isEnabled());
    ui->pbNext->style()->unpolish(ui->pbNext);
    ui->pbNext->style()->polish(ui->pbNext);
}

void Table::onDecisionChanged(const QString& dec)
{
    // qDebug() << "dec transmitted:" << dec;

    if (game_.get()->roundChooser()->isEnabled()) {
        onPaintNextButton(NextOption::NotPossible);
        onPaintDrawButton(DrawOption::NoCard);
    } else if (game_.get()->quteChooser()->isEnabled())
        game_->setButtonColors();
}
