#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{
    setVisible(true); // Ensure the widget is visible if needed
}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::onCardDrawnFromBlind(const QSharedPointer<Card> &card)
{
    addCard(card->clone());
}
