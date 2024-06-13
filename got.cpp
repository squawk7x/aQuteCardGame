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

// void Got::onToggleIsCardFaceVisible(bool isVisible)
// {
//     isCardFaceVisible_ = isVisible;

//     for (const auto& card : cards_) {
//         card->loadImage(isCardFaceVisible_);
//     }
//     layout_->update();
//     update();
// }
