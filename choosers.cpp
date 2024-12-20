#include "choosers.h"
#include <QDebug>
#include <QRandomGenerator>
#include "card.h"

/* ************************************************************************* */

JsuitChooser::JsuitChooser(QString suit, QWidget* parent)
    : suit_{suit}
{
    setSuitname(suit_);
    setStr();
    loadImage();

    connect(this, &QPushButton::clicked, this, &JsuitChooser::toggle);
}

// Setters
void JsuitChooser::setSuit(const QString& suit)
{
    suit_ = suit;
}

void JsuitChooser::setSuitname(const QString& suit)
{
    suitname_ = suitnames[suits.indexOf(suit)];
}

void JsuitChooser::setStr()
{
    str_ = suit_ + suit_;
}

// Getters
QString JsuitChooser::suit()
{
    return suit_;
}

QString JsuitChooser::suitname()
{
    return suitname_;
}

QString JsuitChooser::str()
{
    return str_;
}

void JsuitChooser::loadImage()
{
    QString imagePath = QString(":images/choosers/jsuit_of_%1.png").arg(suitname_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
        this->resize(iconSize());
    } else {
        qDebug() << "Failed to load card image:" << imagePath;
    }
}

void JsuitChooser::toggle()
{
    // Find the current suit's index
    int index = suits.indexOf(suit_);
    if (index != -1) {
        // Move to the next suit in the list, wrapping around if necessary
        index = (index + 1) % suits.size();
        suit_ = suits[index];
        suitname_ = suitnames[index];
        setStr();
        loadImage();
        // qDebug() << "toggle:" << suit_ << suitname_;
    } else {
        qDebug() << "Current suit not found in the list.";
    }
}

void JsuitChooser::toggle_to(
    const QString& target_suit)
{
    int toggle_count = 0;      // Initialize a counter to track toggles
    const int max_toggles = 4; // Define the maximum number of toggles allowed

    while (suit_ != target_suit && toggle_count <= max_toggles) {
        toggle();       // Perform the toggle
        toggle_count++; // Increment the toggle counter
    }

    loadImage(); // Load the image after toggling
    // qDebug() << "toggle_to:" << suit_ << suitname_ << ", toggles performed:" << toggle_count;
}

// Slots:
void JsuitChooser::onCardAddedToStack(const QSharedPointer<Card>& card)
{
    hide();
    setEnabled(false);
}

/* ************************************************************************* */

EightsChooser::EightsChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &EightsChooser::toggle);
}


// Getters

QString EightsChooser::decision()
{
    return decision_;
}

// Methods
void EightsChooser::loadImage()
{
    QString imagePath = QString(":images/choosers/chooser_eights_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
        this->resize(iconSize());
    }
}

void EightsChooser::toggle()
{
    if (decision_ == "a") {
        decision_ = "n";
    } else
        decision_ = "a";
    loadImage();
}

void EightsChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
    loadImage();
}

void EightsChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    // Generate a random number between 0 and 1
    int randomNumber = QRandomGenerator::global()->bounded(2);

    // If the random number is 0, return dec1, otherwise return dec2
    (randomNumber == 0) ? decision_ = dec1 : decision_ = dec2;

    loadImage();
}

/* ************************************************************************* */

QuteChooser::QuteChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &QuteChooser::toggle);
}

// Getters

QString QuteChooser::decision()
{
    return decision_;
}

// Methods
void QuteChooser::loadImage()
{
    QString imagePath = QString(":images/choosers/chooser_qute_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
        this->resize(iconSize());
    }
}

void QuteChooser::toggle()
{
    if (decision_ == "y") {
        decision_ = "n";
    } else
        decision_ = "y";
    emit quteDecisionChanged(decision_);
    loadImage();
}

void QuteChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
    loadImage();
}

void QuteChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    // Generate a random number between 0 and 1
    int randomNumber = QRandomGenerator::global()->bounded(2);

    // If the random number is 0, return dec1, otherwise return dec2
    (randomNumber == 0) ? decision_ = dec1 : decision_ = dec2;

    loadImage();
}

// Slots:
// void QuteChooser::onFourCardsInMonitor() {}

/* ************************************************************************* */

JpointsChooser::JpointsChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &JpointsChooser::toggle);
}

// Getters
QString JpointsChooser::decision()
{
    return decision_;
}

// Methods
void JpointsChooser::loadImage()
{
    QString imagePath = QString(":images/choosers/chooser_jpoints_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
        this->resize(iconSize());
    }
}

void JpointsChooser::toggle()
{
    if (decision_ == "m") {
        decision_ = "p";
    } else
        decision_ = "m";
    loadImage();
}

void JpointsChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
    loadImage();
}

void JpointsChooser::toggleRandom(const QString& dec1, const QString& dec2)
{
    // Generate a random number between 0 and 1
    int randomNumber = QRandomGenerator::global()->bounded(2);

    // If the random number is 0, return dec1, otherwise return dec2
    (randomNumber == 0) ? decision_ = dec1 : decision_ = dec2;

    loadImage();
}

// Slots:
// void JpointsChooser::onJpoints() {}

// Slots:
void JpointsChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        this->setEnabled(true);
        this->show();
    } else {
        this->hide();
        this->setEnabled(false);
    }
    loadImage();
}

/* ************************************************************************* */
RoundChooser::RoundChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, [this]() {
        if (decision_ == "f") {
            emit finishRound();
            setDecision("r");
        } else if (decision_ == "r") {
            emit newRound();
        } else if (decision_ == "g") {
            emit newGame();
        }
    });
}

// Methods
void RoundChooser::loadImage()
{
    QString imagePath = QString(":images/choosers/chooser_new_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
        this->resize(iconSize());
    }
}

// Getters
QString RoundChooser::decision()
{
    return decision_;
}

void RoundChooser::setDecision(const QString& target_decision)
{
    decision_ = target_decision;
    loadImage();
}

// Slots:
void RoundChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        this->setEnabled(true);
        this->show();
    } else {
        this->hide();
        this->setEnabled(false);
    }
}

/* ************************************************************************* */
