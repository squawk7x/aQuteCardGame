#include "drawn.h"

Drawn::Drawn(QWidget* parent)
    : CardVec(parent)
{
    cardFace_ = CardFace::Closed;
}

Drawn::~Drawn()
{
    // CardVec's destructor will handle cleaning up the cards using smart pointers
}

void Drawn::addCard(QSharedPointer<Card> card)
{
    if (card) {
        card->setParent(this);                 // Ensure the card's parent is this Played instance
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        cardFace_ = CardFace::Closed;
        cards_.prepend(card);                  // Add card to the beginning of the list
        layout_->insertWidget(0, card.data()); // Insert widget at the beginning of the layout
    }
}

void Drawn::onCardMustFromBlind(const QSharedPointer<Card>& card)
{
    if (card) {
        addCard(card->clone(this)); // Clone and add the card, setting this as parent

        for (auto const& c : std::as_const(cards_))
            c->loadImage(isCardFaceVisible_);
    }
}
