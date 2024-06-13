#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{
}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::onCardMustFromBlind(const QSharedPointer<Card>& card)
{
    addCard(card->clone(this));
}

// void Drawn::onToggleIsCardFaceVisible(bool isVisible)
// {
//     isCardFaceVisible_ = isVisible;
//     for (const auto& card : cards_) {
//         card->loadImage(isCardFaceVisible_);
//     }
//     layout_->update();
//     update();
// }
