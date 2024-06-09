#include "played.h"
#include <QDebug>

Played::Played(QWidget* parent)
    : CardVec(parent)
{
    isCardVecVisible_ = true; // Default visibility can be set here
}

Played::~Played()
{
    // No need to delete cards_, as CardVec's destructor handles it with QSharedPointer
}

void Played::addCard(QSharedPointer<Card> card)
{
    if (card) {
        card->setParent(this);
        cards_.prepend(card);
        // card->setIsCardFaceVisible(true);
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

void Played::onCardAddedToStack(const QSharedPointer<Card> &card)
{
    card->loadImage(true);
    addCard(card->clone(this));
}

void Played::onToggleIsTableCardsVisible(bool isTableCardsVisible)
{
    isCardVecVisible_ = true;
    for (const auto& card : cards_) {
        card->loadImage(true);
    }
    layout_->update();
    update();
}
