#include "playable.h"

Playable::Playable(QWidget* parent)
    : CardVec(parent)
{}

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
        card->loadImage(true);
        cards_.append(card);
        layout_->addWidget(card.data());
    }
}

void Playable::onCardClicked(const QSharedPointer<Card>& card)
{
    // to make the card played when clicking on card in playable:
    // if (cards().contains(card)) {
    //     emit handCardClicked(card);
    // }
}

void Playable::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = true;

    foreach (const auto& card, cards_) {
        card->loadImage(true);
    }
}
