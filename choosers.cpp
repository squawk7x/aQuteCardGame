#include "choosers.h"
#include <QDebug>
#include <QRandomGenerator>
#include "card.h"

/* ************************************************************************* */
// JsuitChooser Implementation

JsuitChooser::JsuitChooser(QString suit, QWidget* parent)
    : suit_{suit}
{
    setSuitname(suit_);
    setStr();
    loadImage();

    connect(this, &QPushButton::clicked, this, &JsuitChooser::toggle);
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

// Methods
void JsuitChooser::toggle()
{
    int index = suits.indexOf(suit_);
    if (index != -1) {
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
    int toggle_count = 0;
    const int max_toggles = 4;

    while (suit_ != target_suit && toggle_count <= max_toggles) {
        toggle();
        toggle_count++;
    }

    loadImage();
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

// Slots
void JsuitChooser::onCardAddedToStack(const QSharedPointer<Card>& card)
{
    hide();
    setEnabled(false);
}

/* ************************************************************************* */
// EightsChooser Implementation

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
void EightsChooser::toggle()
{
    decision_ = (decision_ == "a") ? "n" : "a";
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
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

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

/* ************************************************************************* */
// QuteChooser Implementation

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
void QuteChooser::toggle()
{
    decision_ = (decision_ == "y") ? "n" : "y";
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
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

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

/* ************************************************************************* */
// JpointsChooser Implementation

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
void JpointsChooser::toggle()
{
    decision_ = (decision_ == "m") ? "p" : "m";
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
    decision_ = (QRandomGenerator::global()->bounded(2) == 0) ? dec1 : dec2;
    loadImage();
}

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

// Slots
void JpointsChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
    loadImage();
}

/* ************************************************************************* */
// RoundChooser Implementation

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

// Getters
QString RoundChooser::decision()
{
    return decision_;
}

// Setters
void RoundChooser::setDecision(const QString& target_decision)
{
    decision_ = target_decision;
    loadImage();
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

// Slots
void RoundChooser::onQuteDecisionChanged(const QString& dec)
{
    if (dec == "y") {
        setEnabled(true);
        show();
    } else {
        hide();
        setEnabled(false);
    }
}

/* ************************************************************************* */
