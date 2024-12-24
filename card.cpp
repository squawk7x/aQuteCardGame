#include "card.h"
#include <QApplication>
#include <QDebug>
#include <QScreen>

// Definitions for the global card properties
QVector<QString> suits = {"♦", "♠", "♥", "♣"};
QVector<QString> ranks = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
QVector<QString> ranknames = {"6", "7", "8", "9", "10", "jack", "queen", "king", "ace"};
QVector<QString> suitnames = {"diamonds", "spades", "hearts", "clubs"};

bool forAndroid = false;

// Private Methods
void Card::initCard()
{
    setSuitname(suit_);
    setRankname(rank_);
    setValue(rank_);
    loadImage();

    connect(this, &QPushButton::clicked, this, [this]() { emit cardClicked(this->clone()); });
}

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

// Constructors
Card::Card(const QString& suit, const QString& rank, QWidget* parent)
    : QPushButton(parent)
    , suit_(suit)
    , rank_(rank)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    setText("");
    initCard();
}

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

Card::Card(const Card& other)
    : QPushButton(other.parentWidget())
    , suit_(other.suit_)
    , rank_(other.rank_)
{
    initCard();
}

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

Card::Card(Card&& other) noexcept
    : QPushButton(other.parentWidget())
    , suit_(std::move(other.suit_))
    , rank_(std::move(other.rank_))
{
    other.setParent(nullptr);
    initCard();
}

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

// Comparison Operators
bool Card::operator==(const Card& other) const
{
    return suit_ == other.suit_ && rank_ == other.rank_;
}

bool Card::operator<(const Card& other) const
{
    if (rank_ != other.rank_)
        return rank_ < other.rank_;
    else
        return suit_ < other.suit_;
}

bool Card::operator>(const Card& other) const
{
    if (rank_ != other.rank_)
        return rank_ > other.rank_;
    else
        return suit_ > other.suit_;
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
// void Card::loadImage(bool isCardFaceVisible)
// {
//     QString imagePath;
//     if (isCardFaceVisible)
//         imagePath = QString(":/cards/%1_of_%2.png").arg(rankname_, suitname_);
//     else {
//         imagePath = QString(":/cards/backside_blue.png");
//     }

//     QIcon icon(imagePath);
//     if (!icon.isNull()) {
//         this->setIcon(icon);
//         this->setIconSize(QSize(50, 75));
//         this->setStyleSheet("padding: 0px; margin: 0px; border: none;");
//         this->resize(iconSize());
//     } else {
//         this->setText(str_);
//         qDebug() << "Failed to load card image:" << imagePath;
//     }
// }

void Card::loadImage(bool isCardFaceVisible)
{
    setStr();

    QString imagePath;
    if (isCardFaceVisible)
        imagePath = QString(":/cards/%1_of_%2.png").arg(rankname_, suitname_);
    else {
        imagePath = QString(":/cards/backside_blue.png");
    }

    QPixmap pixmap(imagePath); // Load the image as a QPixmap
    if (!pixmap.isNull() and not forAndroid) {
        // Fetch the size of the application’s primary screen
        QSize screenSize = QApplication::primaryScreen()->size(); // Get the size of the primary screen
        QSize maxSize;

        // Calculate max size as a percentage of the screen size
        int height = screenSize.height() * 0.15; // 15% of the screen height
        int width = height * 0.5;                // 50% of height for the width
        maxSize = QSize(width, height);

        // Scale the pixmap to fit within the calculated size
        QPixmap scaledPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QIcon icon(scaledPixmap); // Create an icon from the scaled pixmap
        this->setIcon(icon);
        this->setIconSize(scaledPixmap.size()); // Set the icon size to the scaled image size

        // Resize the widget to fit the scaled image size
        this->resize(scaledPixmap.size());

        // Optional: Remove padding and margins
        this->setStyleSheet("padding: 0px; margin: 0px; border: none;");

        // forAndoid or pixmap.isNull()
    } else {
        if (isCardFaceVisible and isEnabled()) {
            setText(str_);
        } else
            setText("▓▓");
    }
}
