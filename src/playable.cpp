#include "playable.h"

Playable::Playable(QWidget* parent)
    : CardVec(parent)
{
    // cardFaces for Player 2+3 not shown in autoplay;
    cardFace_ = CardFace::Open;
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
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        // card->setFocusPolicy(Qt::StrongFocus);
        card->loadImage(true);
        cards_.append(card);
        layout_->addWidget(card.data());
    }
}

void Playable::togglePlayableCards()
{
    if (!cards_.isEmpty()) {
        // Take the last element from the vector
        QSharedPointer<Card> firstCard = cards_.takeFirst();

        // Insert the last card at the front of the vector
        cards_.append(firstCard);

        // Update layout or UI if needed
        updateLayout();
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

    for (const auto& card : std::as_const(cards_)) {
        card->loadImage(true);
    }
}
