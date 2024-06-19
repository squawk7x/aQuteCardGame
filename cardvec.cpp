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
        card->loadImage(isCardFaceVisible_);
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
    // Directly clear the layout and the cards vector
    foreach (const auto& card, cards_) {
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
    }
    cards_.clear();
    layout_->update();
    update();
}

bool CardVec::isCardInCards(const QSharedPointer<Card>& card)
{
    foreach (const auto& c, cards_) {
        if (card == c)
            return true;
    }
    return false;
}

QString CardVec::cardsAsString() const
{
    QString cardsStr;
    foreach (const auto& card, cards_) {
        cardsStr += card->str() + " ";
    }
    return cardsStr.trimmed();
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

QSharedPointer<Card> CardVec::topCard()
{
    if (!cards_.isEmpty()) {
        return cards_.last();
    }
    return nullptr;
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

    return mostCommonSuit;
}

void CardVec::sortCardsByPattern(int pattern)
{
    const std::vector<std::vector<QString>> patterns
        = {{"9", "7", "8", "10", "Q", "K", "J", "A", "6"},
           {"J", "9", "7", "8", "10", "Q", "K", "A", "6"},
           {"6", "A", "K", "Q", "10", "8", "7", "9", "J"},
           {"6", "7", "8", "9", "10", "Q", "K", "A", "J"},
           {"J", "A", "K", "Q", "10", "6", "7", "8", "9"}};

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
                  return posA > posB;
              });

    updateLayout();
}

void CardVec::updateLayout()
{
    foreach (const auto& card, cards_) {
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
    layout_->update();
    update();
}
