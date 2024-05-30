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

        cards_.append(card);
        layout_->insertWidget(0, card.data());
        layout_->update();
        update();
    }
}

QSharedPointer<Card>& Stack::topCardFromStack()
{
    if (!cards_.isEmpty()) {
        return cards_.last(); // Access the last card using CardVec's storage
    }
    static QSharedPointer<Card> nullPointer; // Create a static null pointer
    return nullPointer;                      // Return null QSharedPointer if no cards are present
}
