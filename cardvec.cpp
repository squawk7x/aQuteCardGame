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

QString CardVec::cardsAsString() const
{
    QString cardsStr;
    foreach (const auto& card, cards_) {
        cardsStr += card->str() + " ";
    }
    return cardsStr.trimmed();
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

void CardVec::clearCards()
{
    for (int i = 0; i < cards_.size(); ++i) {
        layout_->removeWidget(cards_[i].data());
        cards_[i]->setParent(nullptr);
    }
    cards_.clear();
}

bool CardVec::isCardInCards(const QSharedPointer<Card>& card)
{
    return std::any_of(cards_.cbegin(), cards_.cend(), [&](const auto& c) { return card == c; });
}

bool CardVec::isSuitInCards(const QString& suit)
{
    for (const QSharedPointer<Card>& card : std::as_const(cards_)) {
        if (card->suit() == suit)
            return true;
    }
    return false;
}

bool CardVec::isRankInCards(const QString& rank)
{
    for (const QSharedPointer<Card>& card : std::as_const(cards_)) {
        if (card->rank() == rank)
            return true;
    }
    return false;
}

void CardVec::sortCardsByPattern(const QVector<QString>& pattern)
{
    std::sort(cards_.begin(), cards_.end(), [&](QSharedPointer<Card> a, QSharedPointer<Card> b) {
        auto posA = std::find(pattern.begin(), pattern.end(), a->rank());
        auto posB = std::find(pattern.begin(), pattern.end(), b->rank());
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

QString CardVec::suitOfRankWithMostPoints() const
{
    QMap<QString, int> rankPoints;

    foreach (const auto& card, cards_) {
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

    foreach (const auto& card, cards_) {
        if (card->rank() == rankWithMaxPoints) {
            return card->suit();
        }
    }
    return QString();
}

void CardVec::updateLayout()
{
    foreach (const auto& card, cards_) {
        layout_->removeWidget(card.data());
        layout_->addWidget(card.data());
    }
}

// void CardVec::showEvent(QShowEvent* event)
// {
//     QWidget::showEvent(event);

//     // After the CardVec is shown, load images for all cards
//     foreach (const auto& card, cards_) {
//         card->loadImage(isCardFaceVisible_);
//     }
// }

QVector<QSharedPointer<Card>>& CardVec::cards()
{
    return cards_;
}

void CardVec::onCardClicked(const QSharedPointer<Card>& card)
{
    qDebug() << "onCardClicked received in CardVec: " << card->str();
}

void CardVec::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;

    foreach (const auto& card, cards_) {
        card->loadImage(isVisible);
    }
}
