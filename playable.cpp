#include "playable.h"

Playable::Playable(QWidget* parent)
    : CardVec(parent)
{
    isCardFacesVisible_ = true;
}

Playable::~Playable()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Playable::addCard(QSharedPointer<Card> card)
{
    if (card) {
        // connect(card.data(), &Card::cardClicked, this, [this, card]() {
        //     this->onCardClicked(card);
        // });
        card->setParent(this);
        cards_.append(card);
        layout_->addWidget(card.data());
        layout_->update();
        update();
    }
}

void Playable::onCardClicked(const QSharedPointer<Card> &card)
{
    // want to make the card played when clicking on card in playable:
    if (cards().contains(card)) {
        emit handCardClicked(card->clone());
    }
}
