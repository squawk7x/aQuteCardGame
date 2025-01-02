#include "played.h"
#include <QDebug>

Played::Played(QWidget* parent)
    : CardVec(parent)
{
    cardFace_ = CardFace::Open;
}

Played::~Played()
{
    // No need to delete cards_, as CardVec's destructor handles it with QSharedPointer
}

void Played::addCard(QSharedPointer<Card> card)
{
    if (card) {
        card->setParent(this);                 // Ensure the card's parent is this Played instance
        card->setStyleSheet("background-color: white; border: 1px solid black; border-radius: 4px; "
                            "padding: 1px 2px; margin: 0px; font-size: 16px;");
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        cards_.prepend(card);                  // Add card to the beginning of the list
        layout_->insertWidget(0, card.data()); // Insert widget at the beginning of the layout
    }
}

void Played::onCardAddedToStack(const QSharedPointer<Card>& card)
{
    card->loadImage(true);      // Ensure the card's face is visible
    addCard(card->clone(this)); // Clone the card and add it to the stack with this as parent
}

void Played::onToggleCardsVisible(bool isVisible)
{
    // isCardFaceVisible_ = true; // Always make card faces visible
    isCardFaceVisible_ = isVisible; // Always make card faces visible
    foreach (const auto& card, cards_) {
        // card->loadImage(true); // Load the card image to ensure the face is visible
        card->loadImage(isVisible); // Load the card image to ensure the face is visible
    }
}
