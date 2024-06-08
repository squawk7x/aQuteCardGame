#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{
    isCardFacesVisible_ = true; // Ensure the widget is visible if needed
}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::onCardMustFromBlind(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}
