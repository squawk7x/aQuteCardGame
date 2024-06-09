#include "handdeck.h"
#include "player.h"

Handdeck::Handdeck(QWidget* parent)
    : CardVec(parent)
{}

void Handdeck::addCard(QSharedPointer<Card> card)
{
    // Player* handdeckParent = qobject_cast<Player*>(parent());

    if (card) {
        connect(card.data(), &Card::cardClicked, this, [this, card]() {
            this->onCardClicked(card);
        });
        card->setParent(this);
        // if (handdeckParent->isRobot())
        //     card->loadImage(isCardVecVisible_);
        // else
        //     card->loadImage(true);
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

void Handdeck::sortCards(Handdeck::SortOption option)
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

void Handdeck::onToggleIsTableCardsVisible(bool isTableCardsVisible)
{
    isCardVecVisible_ = isTableCardsVisible;
    // Debugging statement to check the type of the parent object
    qDebug() << "Parent object type:" << parent()->metaObject()->className();

    // Safely cast the parent to a Player object
    Player* handdeckParent = qobject_cast<Player*>(parent());

    if (handdeckParent) {
        // Parent object successfully cast to Player
        for (const auto& card : cards_) {
            if (handdeckParent->isRobot())
                card->loadImage(isTableCardsVisible);
            else
                card->loadImage(true);
        }
        layout_->update();
        update();
    } else {
        // Parent object is not of type Player
        qWarning() << "Parent is not of type Player";
    }
}
