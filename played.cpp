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
        cards_.append(card); // Use the protected cards_ vector from CardVec
        layout_->addWidget(card.data());
        layout_->update();
        update();
    }
}

void Played::onCardAddedToStack(const QSharedPointer<Card> &card)
{
    addCard(card->clone()); // Simply call the addCard method to handle the layout update
}
