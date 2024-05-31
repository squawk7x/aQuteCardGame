#include "handdeck.h"

Handdeck::Handdeck(QWidget* parent)
    : CardVec(parent)
{}

void Handdeck::addCard(QSharedPointer<Card> card)
{
    if (card) {
        connect(card.data(), &Card::cardClicked, this, [this, card]() {
            this->onCardClicked(card);
        });
        card->setParent(this);
        cards_.append(card);
        layout_->addWidget(card.data());
        layout_->update();
        update();
    }
}

void Handdeck::removeCard(QSharedPointer<Card> card)
{
    if (card && cards_.contains(card)) {
        disconnect(card.data(), &Card::cardClicked, this, nullptr);
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
        cards_.removeOne(card);
        layout_->update();
        update();
    }
}

void Handdeck::onCardClicked(const QSharedPointer<Card>& card)
{
    if (cards().contains(card)) {
        emit handCardClicked(card);
    }
}
