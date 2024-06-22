#include "card.h"
#include <QDebug>

// Definitions for the global card properties
QVector<QString> suits = {"♦", "♠", "♥", "♣"};
QVector<QString> ranks = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
QVector<QString> ranknames = {"6", "7", "8", "9", "10", "jack", "queen", "king", "ace"};
QVector<QString> suitnames = {"diamonds", "spades", "hearts", "clubs"};

void Card::initCard()
{
    setSuitname(suit_);
    setRankname(rank_);
    setValue(rank_);
    setStr();
    loadImage();

    this->setToolTip(QString("%1 of %2").arg(rankname_, suitname_));

    connect(this, &QPushButton::clicked, this, [this]() { emit cardClicked(this->clone()); });
}

// Standard Constructor
Card::Card(const QString& suit, const QString& rank, QWidget* parent)
    : QPushButton(parent)
    , suit_(suit)
    , rank_(rank)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setText("");
    initCard();
}

// Constructor by str
Card::Card(const QString& cardStr, QWidget* parent)
    : QPushButton(parent)
{
    if (cardStr.length() >= 2) {
        if (suits.contains(cardStr.at(0)) && ranks.contains(cardStr.mid(1))) {
            suit_ = cardStr.at(0);
            rank_ = cardStr.mid(1);
            initCard();
        }
    } else {
        qDebug() << "Invalid card string format:" << cardStr;
    }
}

// Copy Constructor
Card::Card(const Card& other)
    : QPushButton(other.parentWidget())
    , suit_(other.suit_)
    , rank_(other.rank_)
{
    initCard();
}

// Copy Assignment Operator
Card& Card::operator=(const Card& other)
{
    if (this != &other) {
        this->setParent(other.parentWidget());
        suit_ = other.suit_;
        rank_ = other.rank_;
        initCard();
    }
    return *this;
}

// Move Constructor
Card::Card(Card&& other) noexcept
    : QPushButton(other.parentWidget())
    , suit_(std::move(other.suit_))
    , rank_(std::move(other.rank_))
{
    //Set the parent of the moved-from object to nullptr to prevent it from accessing the old parent:
    other.setParent(nullptr);
    initCard();
}

// Move Assignment Operator
Card& Card::operator=(Card&& other) noexcept
{
    if (this != &other) {
        this->setParent(other.parentWidget());
        suit_ = std::move(other.suit_);
        rank_ = std::move(other.rank_);
        other.setParent(nullptr);
        initCard();
    }
    return *this;
}

// Operator==
bool Card::operator==(const Card& other) const
{
    return suit_ == other.suit_ && rank_ == other.rank_;
}

// Clone
QSharedPointer<Card> Card::clone(QWidget* parent) const
{
    QSharedPointer<Card> clonedCard = QSharedPointer<Card>::create(*this);
    if (parent) {
        clonedCard->setParent(parent);
    }
    return clonedCard;
}

// Getters
QString Card::suit() const
{
    return suit_;
}
QString Card::rank() const
{
    return rank_;
}
QString Card::suitname() const
{
    return suitname_;
}
QString Card::rankname() const
{
    return rankname_;
}
QString Card::str() const
{
    return str_;
}
int Card::value() const
{
    return value_;
}

// Setters
void Card::setSuitname(const QString& suit)
{
    int index = suits.indexOf(suit);
    if (index != -1) {
        suitname_ = suitnames[index];
    }
}

void Card::setRankname(const QString& rank)
{
    int index = ranks.indexOf(rank);
    if (index != -1) {
        rankname_ = ranknames[index];
    }
}

void Card::setStr()
{
    str_ = suit_ + rank_;
}

void Card::setValue(const QString& rank)
{
    if (rank == "10" || rank == "Q" || rank == "K") {
        value_ = 10;
    } else if (rank == "A") {
        value_ = 15;
    } else if (rank == "J") {
        value_ = 20;
    } else {
        value_ = 0;
    }
}

void Card::loadImage(bool isCardFaceVisible)
{
    QString imagePath;
    if (isCardFaceVisible)
        imagePath = QString(":/images/cards/%1_of_%2.png").arg(rankname_, suitname_);
    else {
        imagePath = QString(":/images/cards/backside_blue.png");
    }

    QIcon icon(imagePath);
    if (!icon.isNull()) {
        this->setIcon(icon);
        this->setIconSize(QSize(50, 75));

        // Ensure there are no margins or padding around the icon
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");

        // Resize the button to fit the icon size
        this->resize(iconSize());
    } else {
        this->setText(str_);
        qDebug() << "Failed to load card image:" << imagePath;
    }
}
