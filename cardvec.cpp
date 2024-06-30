#include "cardvec.h"
#include <QDebug>
#include <algorithm>
#include <qevent.h>

CardVec::CardVec(QWidget* parent, QVector<QSharedPointer<Card>> rhs)
    : QWidget(parent)
    , cards_(std::move(rhs))
    , isCardFaceVisible_(true)
{
    layout_ = new QHBoxLayout(this);
    setLayout(layout_);

    foreach (const auto& card, cards_) {
        card->setParent(this);
        card->loadImage(isCardFaceVisible_);
        layout_->addWidget(card.data());
    }
}

CardVec::~CardVec()
{
    clearCards();
}

void CardVec::addCard(QSharedPointer<Card> card)
{
    if (card) {
        card->setParent(this);
        card->loadImage(isCardFaceVisible_);
        cards_.append(card);
        layout_->addWidget(card.data());
    }
}

void CardVec::removeCard(QSharedPointer<Card> card)
{
    if (cards_.contains(card)) {
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
        cards_.removeOne(card);
    }
}

void CardVec::copyCardTo(const QSharedPointer<Card>& card, CardVec* targetVec)
{
    if (card && targetVec != nullptr) {
        QSharedPointer<Card> newCard = card->clone(targetVec);
        targetVec->addCard(newCard);
    }
}

void CardVec::copyTopCardTo(CardVec* targetVec)
{
    if (!cards_.isEmpty() && targetVec != nullptr) {
        QSharedPointer<Card> topCard = cards_.last();
        copyCardTo(topCard, targetVec);
    }
}

void CardVec::moveCardTo(QSharedPointer<Card> card, CardVec* targetVec)
{
    if (cards_.contains(card) && targetVec != nullptr) {
        removeCard(card);
        targetVec->addCard(std::move(card));
    }
}

void CardVec::moveTopCardTo(CardVec* targetVec)
{
    if (!cards_.isEmpty() && targetVec != nullptr) {
        QSharedPointer<Card> topCard = cards_.last();
        moveCardTo(topCard, targetVec);
    }
}

QSharedPointer<Card> CardVec::topCard()
{
    if (!cards_.isEmpty()) {
        return cards_.last();
    }
    return nullptr;
}

QString CardVec::cardsAsString() const
{
    QString cardsStr;
    foreach (const auto& card, cards_) {
        cardsStr += card->str() + " ";
    }
    return cardsStr.trimmed();
}

void CardVec::clearCards()
{
    // Using index-based loop to avoid detachment
    for (int i = 0; i < cards_.size(); ++i) {
        layout_->removeWidget(cards_[i].data());
        cards_[i]->setParent(nullptr);
    }
    cards_.clear();
}

bool CardVec::isCardInCards(const QSharedPointer<Card>& card)
{
    return std::any_of(cards_.cbegin(), cards_.cend(), [&](const auto& c) { return card == c; });

    // foreach (const auto& c, cards_) {
    //     if (card == c)
    //         return true;
    // }
    // return false;
}

bool CardVec::isSuitInCards(const QString& suit)
{
    for (const QSharedPointer<Card>& card : cards_) {
        if (card->suit() == suit)
            return true;
    }
    return false;
}

bool CardVec::isRankInCards(const QString& rank)
{
    for (const QSharedPointer<Card>& card : cards_) {
        if (card->rank() == rank)
            return true;
    }
    return false;
}

QString CardVec::mostCommonSuit() const
{
    QMap<QString, int> suitCounts;

    foreach (const auto& card, cards_) {
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
    // qDebug() << "mostCommonSuit:" << mostCommonSuit;
    return mostCommonSuit;
}

void CardVec::sortCardsByPattern(int pattern)
{
    const std::vector<std::vector<QString>> patterns
        = {{"8", "7", "6", "J", "A", "K", "Q", "10", "9"},
           {"6", "A", "K", "Q", "10", "8", "7", "9", "J"},
           {"6", "A", "8", "7", "K", "Q", "10", "9", "J"}};

    if (pattern < 0 || pattern >= patterns.size()) {
        qWarning() << "Invalid pattern index.";
        return;
    }

    const auto& selected_pattern = patterns[pattern];

    // qDebug() << "pattern used:" << pattern << ":" << patterns[pattern];

    std::sort(cards_.begin(),
              cards_.end(),
              [&selected_pattern](QSharedPointer<Card> a, QSharedPointer<Card> b) {
                  auto posA = std::find(selected_pattern.begin(), selected_pattern.end(), a->rank());
                  auto posB = std::find(selected_pattern.begin(), selected_pattern.end(), b->rank());
                  return posA < posB;
              });

    updateLayout();
}

int CardVec::countCardsOfRank(const QString& rank) const
{
    int count = 0;
    for (const auto& card : cards_) {
        if (card->rank() == rank) {
            count++;
        }
    }
    return count;
}

void CardVec::updateLayout()
{
    foreach (const auto& card, cards_) {
        layout_->removeWidget(card.data());
        layout_->addWidget(card.data());
    }
}

// Getters
QVector<QSharedPointer<Card>>& CardVec::cards()
{
    return cards_;
}

bool CardVec::isCardFaceVisible() const
{
    return isCardFaceVisible_;
}

// Setters
void CardVec::setIsCardFaceVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;
}

// Slots
void CardVec::onCardClicked(const QSharedPointer<Card>& card)
{
    // To be implemented by subclasses
    qDebug() << "onCardClicked received in CardVec: " << card->str();
}

void CardVec::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;

    foreach (const auto& card, cards_) {
        card->loadImage(isCardFaceVisible_);
    }
    //
}
