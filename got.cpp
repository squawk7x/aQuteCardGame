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
        card->setStyleSheet("background-color: white; border: 1px solid black; border-radius: 4px; "
                            "padding: 1px 2px; margin: 0px; font-size: 16px;");
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        addCard(card->clone(this)); // Clone and add the card, setting this as parent
    }
}
