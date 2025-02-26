#include "cardvec.h"

#include <qevent.h>

#include <QDebug>
#include <algorithm>

CardVec::CardVec(QWidget* parent, QVector<QSharedPointer<Card>> rhs)
    : QWidget(parent), cards_(std::move(rhs)), isCardFaceVisible_(false) {
  cardFace_ = CardFace::Closed;

  layout_ = new QHBoxLayout(this);
  setLayout(layout_);
  // setFocusPolicy(Qt::NoFocus);

  for (const auto& card : std::as_const(cards_)) {
    card->setParent(this);
    card->loadImage(isCardFaceVisible_);
    layout_->addWidget(card.data());
  }
}

CardVec::~CardVec() { clearCards(); }

// CardVec as range
// auto
QVector<QSharedPointer<Card>>::iterator CardVec::begin() {
  return cards_.begin();
}

// auto
QVector<QSharedPointer<Card>>::iterator CardVec::end() { return cards_.end(); }

void CardVec::addCard(QSharedPointer<Card> card) {
  if (card) {
    card->setParent(this);
    card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    card->loadImage(isCardFaceVisible_);
    cards_.append(card);
    layout_->addWidget(card.data());
  }
}

void CardVec::removeCard(QSharedPointer<Card> card) {
  if (cards_.contains(card)) {
    layout_->removeWidget(card.data());
    card->setParent(nullptr);
    cards_.removeOne(card);
  }
}

QString CardVec::cardsAsString() const {
  QString cardsStr;
  for (const auto& card : std::as_const(cards_)) {
    cardsStr += card->str() + " ";
  }
  return cardsStr.trimmed();
}

void CardVec::copyCardTo(const QSharedPointer<Card>& card, CardVec* targetVec) {
  if (card && targetVec != nullptr) {
    QSharedPointer<Card> newCard = card->clone(targetVec);
    targetVec->addCard(newCard);
  }
}

void CardVec::copyTopCardTo(CardVec* targetVec) {
  if (!cards_.isEmpty() && targetVec != nullptr) {
    QSharedPointer<Card> topCard = cards_.last();
    copyCardTo(topCard, targetVec);
  }
}

void CardVec::moveCardTo(QSharedPointer<Card> card, CardVec* targetVec) {
  if (cards_.contains(card) && targetVec != nullptr) {
    removeCard(card);
    targetVec->addCard(std::move(card));
  }
}

void CardVec::moveTopCardTo(CardVec* targetVec) {
  if (!cards_.isEmpty() && targetVec != nullptr) {
    QSharedPointer<Card> topCard = cards_.last();
    moveCardTo(topCard, targetVec);
  }
}

QSharedPointer<Card> CardVec::topCard() {
  if (!cards_.isEmpty()) {
    return cards_.last();
  }
  return nullptr;
}

void CardVec::clearCards() {
  for (int i = 0; i < cards_.size(); ++i) {
    layout_->removeWidget(cards_[i].data());
    cards_[i]->setParent(nullptr);
  }
  cards_.clear();
}

bool CardVec::isCardInCards(const Card& cardToCheck) {
  return std::any_of(cards_.cbegin(), cards_.cend(), [&](const auto& card) {
    return cardToCheck.str() == card->str();
  });
}

bool CardVec::isSuitInCards(const QString& suit) {
  for (const QSharedPointer<Card>& card : std::as_const(cards_)) {
    if (card->suit() == suit) return true;
  }
  return false;
}

bool CardVec::isRankInCards(const QString& rank) {
  for (const QSharedPointer<Card>& card : std::as_const(cards_)) {
    if (card->rank() == rank) return true;
  }
  return false;
}

void CardVec::sortCardsByPattern(const QVector<QString>& pattern) {
  std::sort(cards_.begin(), cards_.end(),
            [&](QSharedPointer<Card> a, QSharedPointer<Card> b) {
              auto posA = std::find(pattern.begin(), pattern.end(), a->rank());
              auto posB = std::find(pattern.begin(), pattern.end(), b->rank());
              return posA < posB;
            });

  updateLayout();
}

int CardVec::countCardsOfRank(const QString& rank) const {
  int count = 0;
  for (const auto& card : std::as_const(cards_)) {
    if (card->rank() == rank) {
      count++;
    }
  }
  return count;
}

QString CardVec::mostCommonSuit() const {
  QMap<QString, int> suitCounts;

  for (const auto& card : std::as_const(cards_)) {
    suitCounts[card->suit()]++;
  }

  QString mostCommonSuit;
  int maxCount = 0;
  for (auto it = suitCounts.constBegin(); it != suitCounts.constEnd(); ++it) {
    if (it.value() > maxCount) {
      maxCount = it.value();
      mostCommonSuit = it.key();
    }
  }
  return mostCommonSuit;
}

QString CardVec::suitOfRankWithMostPoints() const {
  QMap<QString, int> rankPoints;

  for (const auto& card : std::as_const(cards_)) {
    rankPoints[card->rank()] += card->value();

    if (card->rank() == "J") {
      rankPoints[card->rank()] = -1;
    }
  }

  QString rankWithMaxPoints;
  int maxPoints = -1;
  for (auto it = rankPoints.constBegin(); it != rankPoints.constEnd(); ++it) {
    if (it.value() > maxPoints) {
      maxPoints = it.value();
      rankWithMaxPoints = it.key();
    }
  }

  for (const auto& card : std::as_const(cards_)) {
    if (card->rank() == rankWithMaxPoints) {
      return card->suit();
    }
  }
  return QString("♦");  // suspect program crash here when only 'J' in handdeck
                        // and finish round
}

void CardVec::updateLayout() {
  for (const auto& card : std::as_const(cards_)) {
    layout_->removeWidget(card.data());
    layout_->addWidget(card.data());
  }
}

QVector<QSharedPointer<Card>>& CardVec::cards() { return cards_; }

void CardVec::onCardClicked(const QSharedPointer<Card>& card) {
  qDebug() << "onCardClicked received in CardVec: " << card->str();
}

void CardVec::onToggleCardsVisible(bool isVisible) {
  isCardFaceVisible_ = isVisible;

  for (const auto& card : std::as_const(cards_)) {
    card->loadImage(isVisible);
  }
}
