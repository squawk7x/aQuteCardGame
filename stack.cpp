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
        card->setIsFaceVisible(true);
        cards_.append(card);
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

void Stack::onToggleCardVisibility()
{
    for (const auto& card : cards_) {
        card->setIsFaceVisible(true);
    }
    layout_->update();
    update();
}
