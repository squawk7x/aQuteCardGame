#include "monitor.h"
#include <QDebug>

Monitor::Monitor(QWidget* parent)
    : CardVec(parent)
{
    isVisible_ = false; // Default visibility
}

Monitor::~Monitor()
{
    // CardVec's destructor will handle cleaning up the cards using QSharedPointer
}

void Monitor::addCard(QSharedPointer<Card> card)
{
    if (!card)
        return;
    card->setParent(this);

    QString rank = card->rank();

    // Remove all cards that do not match the rank of the new card
    QVector<QSharedPointer<Card>> filteredCards;

    for (const auto& existingCard : cards_) {
        if (existingCard->rank() == rank) {
            filteredCards.append(existingCard);
        } else {
            layout_->removeWidget(existingCard.data());
            existingCard->setParent(nullptr);
        }
    }

    // Update the internal card vector
    cards_ = filteredCards;

    // Add the new card to the layout and vector
    cards_.prepend(card);
    layout_->insertWidget(0, card.data());
    layout_->update();
    update();
}

void Monitor::onCardAddedToStack(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}
