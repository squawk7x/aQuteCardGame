#include "choosers.h"
#include <QDebug>
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
    QString imagePath = QString(":images/cards/jsuit_of_%1.png").arg(suitname_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
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
    } else {
        qDebug() << "Current suit not found in the list.";
    }
}

void JsuitChooser::toggle_to(const QString& target_suit)
{
    while (suit_ != target_suit) {
        toggle();
    }
}

// Slots:
void JsuitChooser::onCardAddedToStack(const QSharedPointer<Card>& card)
{
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
    QString imagePath = QString(":images/cards/chooser_eights_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
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
}

// Slots:
void EightsChooser::onEightsInMonitor() {}

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
    QString imagePath = QString(":images/cards/chooser_qute_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
    }
}

void QuteChooser::toggle()
{
    if (decision_ == "y") {
        decision_ = "n";
    } else
        decision_ = "y";
    loadImage();
}

void QuteChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
}

// Slots:
void QuteChooser::onFourCardsInMonitor() {}

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
        QString imagePath = QString(":images/cards/chooser_jpoints_%1.png").arg(decision_);
        QIcon icon((QString(imagePath)));
        if (!icon.isNull()) {
            setIcon(icon);
            setIconSize(QSize(50, 75));
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
}

// Slots:
void JpointsChooser::onJpoints() {}

/* ************************************************************************* */

RoundChooser::RoundChooser(QString decision, QWidget* parent)
    : decision_{decision}
{
    loadImage();

    connect(this, &QPushButton::clicked, this, &RoundChooser::toggle);
}

// Methods
void RoundChooser::loadImage()
{
    QString imagePath = QString(":images/cards/chooser_round_%1.png").arg(decision_);
    QIcon icon((QString(imagePath)));
    if (!icon.isNull()) {
        setIcon(icon);
        setIconSize(QSize(50, 75));
    }
}

// Getters
QString RoundChooser::decision()
{
    return decision_;
}

void RoundChooser::toggle()
{
    if (decision_ == "c") {
        decision_ = "f";
    } else
        decision_ = "c";
    loadImage();
}

void RoundChooser::toggle_to(const QString& target_decision)
{
    while (decision_ != target_decision) {
        toggle();
    }
}

// Slots:
void RoundChooser::onNewRound() {}

/* ************************************************************************* */
