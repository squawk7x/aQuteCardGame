#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{
    isCardVecVisible_ = true; // Ensure the widget is visible if needed
}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::onCardMustFromBlind(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}

void Drawn::onToggleIsTableCardsVisible(bool isTableCardsVisible)
{
    isCardVecVisible_ = isTableCardsVisible;
    for (const auto& card : cards_) {
        card->loadImage(isTableCardsVisible);
    }
    layout_->update();
    update();
}
