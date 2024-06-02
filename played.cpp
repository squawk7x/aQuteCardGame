#include "played.h"
#include <QDebug>

Played::Played(QWidget* parent)
    : CardVec(parent)
{
    isCardFacesVisible_ = true; // Default visibility can be set here
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
        card->setIsFaceVisible(true);
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

void Played::onCardAddedToStack(const QSharedPointer<Card> &card)
{
    addCard(card->clone(this));
}

void Played::onToggleCardVisibility()
{
    for (const auto& card : cards_) {
        card->setIsFaceVisible(true);
    }
    layout_->update();
    update();
}
