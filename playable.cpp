#include "playable.h"

Playable::Playable(QWidget* parent)
    : CardVec(parent)
{
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
        card->loadImage(true);
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

void Playable::onToggleIsCardFaceVisible(bool isVisible)
{
    isCardFaceVisible_ = true;

    for (const auto& card : cards_) {
        card->loadImage(true);
    }
    layout_->update();
    update();
}
