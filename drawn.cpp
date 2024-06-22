#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::onCardMustFromBlind(const QSharedPointer<Card>& card)
{
    if (card) {
        addCard(card->clone(this)); // Clone and add the card, setting this as parent
    }
}
