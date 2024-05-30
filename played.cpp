#include "played.h"
#include <QDebug>

Played::Played(QWidget* parent)
    : CardVec(parent)
{
    isVisible_ = true; // Default visibility can be set here
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
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

void Played::onCardAddedToStack(const QSharedPointer<Card> &card)
{
    addCard(card->clone(this));
}
