#include "got.h"

Got::Got(QWidget* parent)
    : CardVec(parent)
{
    isCardFacesVisible_ = true; // Ensure the widget is visible if needed
}

Got::~Got()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Got::onCardBadFromBlind(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}

// void Got::clearGotCards()
// {
//     // if (!cards_.isEmpty())
//     //     clearCards();
// }
