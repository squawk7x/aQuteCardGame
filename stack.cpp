#include "stack.h"
#include <QDebug>

Stack::Stack(QWidget* parent, const QVector<QSharedPointer<Card>>& rhs)
    : CardVec(parent, rhs)
{
    // Initialization code specific to Stack can go here if needed
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
        card->loadImage(true);
        cards_.append(card);
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

void Stack::removeFirstCard()
{
    if (!cards_.isEmpty()) {
        QSharedPointer<Card> firstCard = cards_.takeFirst();
        layout_->removeWidget(firstCard.data());
        firstCard->setParent(nullptr);
        firstCard->hide();
        layout_->update();
        update();
    }
}

void Stack::onNumberCardsPlayed(int numCardsPlayed)
{
    // Check if the number of cards exceeds the limit
    while (cards_.size() > numCardsPlayed + 1) {
        removeFirstCard(); // Remove the first card if the limit is exceeded
    }
}

void Stack::onToggleIsTableCardsVisible(bool isTableCardsVisible)
{
    isCardVecVisible_ = true;
    for (const auto& card : cards_) {
        card->loadImage(true);
    }
    layout_->update();
    update();
}
