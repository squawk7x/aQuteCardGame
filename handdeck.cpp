#include "handdeck.h"
#include <QGroupBox>

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

        QGroupBox* handdeck = qobject_cast<QGroupBox*>(parent());

        if (handdeck && handdeck->objectName() == "gbPlayer1") {
            card->loadImage(true);
        } else {
            card->loadImage(isCardFaceVisible_);
        }

        cards_.append(card);
        layout_->addWidget(card.data());
        card->show();
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

void Handdeck::sortCardsBy(SortOption option)
{
    if (option == SortOption::Suit) {
        std::sort(cards_.begin(),
                  cards_.end(),
                  [](const QSharedPointer<Card>& a, const QSharedPointer<Card>& b) {
                      if (a->suit() == b->suit()) {
                          return ranks.indexOf(a->rank()) < ranks.indexOf(b->rank());
                      }
                      return suits.indexOf(a->suit()) < suits.indexOf(b->suit());
                  });
    } else if (option == SortOption::Rank) {
        std::sort(cards_.begin(),
                  cards_.end(),
                  [](const QSharedPointer<Card>& a, const QSharedPointer<Card>& b) {
                      if (a->rank() == b->rank()) {
                          return suits.indexOf(a->suit()) < suits.indexOf(b->suit());
                      }
                      return ranks.indexOf(a->rank()) < ranks.indexOf(b->rank());
                  });
    }

    updateLayout();
}

void Handdeck::onCardClicked(const QSharedPointer<Card>& card)
{
    if (cards().contains(card)) {
        emit handCardClicked(card);
    }
}

void Handdeck::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = isVisible;

    QGroupBox* handdeck = qobject_cast<QGroupBox*>(parent());

    if (!handdeck) {
        return; // Safeguard against invalid parent casting
    }

    for (const auto& card : std::as_const(cards_)) { // Using as_const() to prevent detachment
        if (handdeck && handdeck->objectName() == "gbPlayer1") {
            card->loadImage(true);
        } else {
            card->loadImage(isCardFaceVisible_);
        }
    }
    layout_->update();
    update();
}
