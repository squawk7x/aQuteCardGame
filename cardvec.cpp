#include "cardvec.h"
#include <QDebug>
#include <algorithm>

CardVec::CardVec(QWidget* parent, QVector<QSharedPointer<Card>> rhs)
    : QWidget(parent)
    , cards_(std::move(rhs))
    , isVisible_(true)
{
    layout_ = new QHBoxLayout(this);
    setLayout(layout_);

    for (const auto& card : cards_) {
        card->setParent(this);
        layout_->addWidget(card.data());
    }
    layout_->update();
    update();
}

CardVec::~CardVec()
{
    clearCards();
}

void CardVec::addCard(QSharedPointer<Card> card)
{
    if (card) {
        card->setParent(this);
        cards_.append(card);
        layout_->addWidget(card.data());
        layout_->update();
        update();
    }
}

void CardVec::removeCard(QSharedPointer<Card> card)
{
    if (cards_.contains(card)) {
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
        cards_.removeOne(card);
        layout_->update();
        update();
    }
}

void CardVec::clearCards()
{
    for (const auto& card : cards_) {
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
    }
    cards_.clear();
    layout_->update();
    update();
}

QString CardVec::cardsAsString() const
{
    QString cardsStr;
    for (const auto& card : cards_) {
        cardsStr += card->str() + " ";
    }
    return cardsStr.trimmed();
}

void CardVec::moveCardTo(QSharedPointer<Card> card, CardVec* targetVec)
{
    if (cards_.contains(card) && targetVec != nullptr) {
        removeCard(card);
        targetVec->addCard(card);
    }
}

void CardVec::moveTopCardTo(CardVec* targetVec)
{
    if (!cards_.isEmpty() && targetVec != nullptr) {
        QSharedPointer<Card> topCard = cards_.last();
        moveCardTo(topCard, targetVec);
    }
}

void CardVec::copyCardTo(QSharedPointer<Card> card, CardVec* targetVec)
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

QSharedPointer<Card> CardVec::drawTopCard()
{
    if (!cards_.isEmpty()) {
        QSharedPointer<Card> card = cards_.takeLast();
        card->setParent(nullptr);
        layout_->removeWidget(card.data());
        layout_->update();
        update();
        return card;
    }
    return nullptr;
}

QString CardVec::mostCommonSuit() const
{
    QMap<QString, int> suitCounts;

    for (const auto& card : cards_) {
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

void CardVec::toggleIsVisible()
{
    isVisible_ = !isVisible_;
    setVisible(isVisible_);
}

void CardVec::sortCards(int pattern)
{
    const std::vector<std::vector<QString>> patterns
        = {{"6", "A", "J", "K", "Q", "10", "8", "7", "9"},
           {"J", "9", "7", "8", "10", "Q", "K", "A", "6"},
           {"J", "A", "K", "Q", "10", "9", "8", "7", "6"},
           {"9", "8", "7", "6", "10", "Q", "K", "A", "J"}};

    if (pattern < 0 || pattern >= patterns.size()) {
        qWarning() << "Invalid pattern index.";
        return;
    }

    const auto& selected_pattern = patterns[pattern];

    std::sort(cards_.begin(),
              cards_.end(),
              [&selected_pattern](QSharedPointer<Card> a, QSharedPointer<Card> b) {
                  auto posA = std::find(selected_pattern.begin(), selected_pattern.end(), a->rank());
                  auto posB = std::find(selected_pattern.begin(), selected_pattern.end(), b->rank());
                  return posA < posB;
              });

    for (auto& card : cards_) {
        layout_->removeWidget(card.data());
        layout_->addWidget(card.data());
    }

    layout_->update();
    update();
}

// Getters
QVector<QSharedPointer<Card>>& CardVec::cards()
{
    return cards_;
}

bool CardVec::isVisible() const
{
    return isVisible_;
}

// Setters
void CardVec::setIsVisible(bool isVisible)
{
    isVisible_ = isVisible;
    setVisible(isVisible_);
}

// Slots
void CardVec::onCardClicked(const QSharedPointer<Card>& card)
{
    // To be implemented by subclasses
    qDebug() << "onCardClicked received in CardVec: " << card->str();
}
