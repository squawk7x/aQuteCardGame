#include "got.h"

Got::Got(QWidget* parent)
    : CardVec(parent)
{
    cardFace_ = CardFace::Closed;
}

Got::~Got()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Got::onCardBadFromBlind(const QSharedPointer<Card>& card)
{
    if (card) {
        addCard(card->clone(this)); // Clone and add the card, setting this as parent
    }
}
