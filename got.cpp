#include "got.h"

Got::Got(QWidget* parent)
    : CardVec(parent)
{
}

Got::~Got()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Got::onCardBadFromBlind(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}
