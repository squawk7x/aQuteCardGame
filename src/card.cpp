#include "card.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QScreen>
#include <QStyle>

// Definitions for the global card properties
QVector<QString> suits = {"♦", "♠", "♥", "♣"};
QVector<QString> ranks = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
QVector<QString> ranknames = {"6",    "7",     "8",    "9",  "10",
                              "jack", "queen", "king", "ace"};
QVector<QString> suitnames = {"diamonds", "spades", "hearts", "clubs"};

// Private Methods
void Card::initCard() {
  pair_.first = suit_;
  pair_.second = rank_;
  setSuitname(suit_);
  setRankname(rank_);
  setValue(rank_);
  setCardType(CardType::Small);
  loadImage();

  connect(this, &QPushButton::clicked, this,
          [&]() { emit cardClicked(*this); });
}

void Card::setSuitname(const QString& suit) {
  int index = suits.indexOf(suit);
  if (index != -1) {
    suitname_ = suitnames[index];
  }
}

void Card::setRankname(const QString& rank) {
  int index = ranks.indexOf(rank);
  if (index != -1) {
    rankname_ = ranknames[index];
  }
}

void Card::setStr() { str_ = suit_ + rank_; }

void Card::setValue(const QString& rank) {
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

void Card::setCardType(CardType type) {
  cardType_ = type;
  setProperty("card-type", type == CardType::Small ? "Small" : "Normal");
}

// Constructors
Card::Card(const QString& suit, const QString& rank, QWidget* parent)
    : QPushButton(parent), suit_(suit), rank_(rank) {
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
  setText("");
  initCard();
}

Card::Card(const QString& cardStr, QWidget* parent) : QPushButton(parent) {
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

Card::Card(const std::pair<QString, QString>& pair, QWidget* parent)
    : Card(pair.first, pair.second) {}

Card::Card(const Card& other)
    : QPushButton(other.parentWidget()),
      suit_(other.suit_),
      rank_(other.rank_) {
  // setFocusPolicy(Qt::NoFocus);
  initCard();
}

Card& Card::operator=(const Card& other) {
  if (this != &other) {
    this->setParent(other.parentWidget());
    suit_ = other.suit_;
    rank_ = other.rank_;
    initCard();
  }
  return *this;
}

Card::Card(Card&& other) noexcept
    : QPushButton(other.parentWidget()),
      suit_(std::move(other.suit_)),
      rank_(std::move(other.rank_)) {
  other.setParent(nullptr);
  initCard();
}

Card& Card::operator=(Card&& other) noexcept {
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
bool Card::operator==(const Card& other) const {
  return suit_ == other.suit_ && rank_ == other.rank_;
}

bool Card::operator<(const Card& other) const {
  if (rank_ != other.rank_)
    return rank_ < other.rank_;
  else
    return suit_ < other.suit_;
}

bool Card::operator>(const Card& other) const {
  if (rank_ != other.rank_)
    return rank_ > other.rank_;
  else
    return suit_ > other.suit_;
}

// Clone
QSharedPointer<Card> Card::clone(QWidget* parent) const {
  QSharedPointer<Card> clonedCard = QSharedPointer<Card>::create(*this);
  if (parent) {
    clonedCard->setParent(parent);
  }
  return clonedCard;
}

// Getters
QString Card::suit() const { return suit_; }

QString Card::rank() const { return rank_; }

QString Card::suitname() const { return suitname_; }

QString Card::rankname() const { return rankname_; }

QString Card::str() const { return str_; }

int Card::value() const { return value_; }

void Card::loadImage(bool isCardFaceVisible) {
  setStr();  // Update the string representation of the card

  QString imagePath;

  // Determine the image path based on visibility
  if (isCardFaceVisible) {
    imagePath = QString(":res/cards/%1_of_%2.png").arg(rankname_, suitname_);
  } else {
    imagePath = QString(":res/cards/backside_blue.png");
  }

  setProperty("card-type", cardType_ == CardType::Small ? "Small" : "Normal");

  QPixmap pixmap(imagePath);  // Load the image as a QPixmap

  if (!pixmap.isNull() && cardType_ == CardType::Normal) {
    setText("");             // Remove any text
    setIcon(QIcon(pixmap));  // Set the icon

    QSize buttonSize = QSize(500, 726) * 0.2;
    setIconSize(buttonSize);
  } else {
    setIcon(QIcon());  // Clear any existing icon
    setText(isCardFaceVisible && isEnabled() ? str_ : "▓▓");
  }

  // Notify Qt to reapply the stylesheet
  style()->unpolish(this);
  style()->polish(this);
}
