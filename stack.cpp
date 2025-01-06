#include "stack.h"
#include <QDebug>

Stack::Stack(QWidget* parent, const QVector<QSharedPointer<Card>>& rhs)
    : CardVec(parent, rhs)
{
    // Initialization code specific to Stack can go here if needed
    cardFace_ = CardFace::Open;
}

Stack::~Stack()
{
    clearCards(); // Ensure clean-up is handled via CardVec
}

void Stack::addCard(QSharedPointer<Card> card)
{
    if (card) {
        disconnect(card.data(), &Card::cardClicked, this, nullptr);
        card->setParent(this);
        card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        card->loadImage(true);
        cards_.append(card);
        layout_->insertWidget(0, card.data());
    }
}

void Stack::updateLayout()
{
    {
        int maxCards = 5; // Limit to 8 cards
        int count = 0;    // Counter for added cards

        for (const auto& card : cards_) {
            if (count >= std::min<int>(cards_.size(), maxCards))
                break;

            layout_->removeWidget(card.data());
            layout_->addWidget(card.data());

            ++count; // Increment the counter
        }
    }
}

void Stack::onCardsPlayed(int numCardsPlayed)
{
    int maxCardsToShow = numCardsPlayed + 10;

    // Ensure all cards are hidden initially
    for (auto& card : cards_) {
        card->hide();
    }

    // Show up to maxCardsToShow cards
    int startIdx = qMax(0, cards_.size() - maxCardsToShow);
    for (int i = startIdx; i < cards_.size(); ++i) {
        cards_[i]->show();
    }

    // Update the layout and widget
}

void Stack::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = true;

    for (const auto& card : cards_) {
        card->loadImage(true);
    }
}
