#include "playable.h"
#include "player.h"

Playable::Playable(QWidget* parent)
    : CardVec(parent)
{
    isCardVecVisible_ = true;
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
        cards_.append(card);
        layout_->addWidget(card.data());
        layout_->update();
        update();
    }
}

void Playable::onCardClicked(const QSharedPointer<Card> &card)
{
    // want to make the card played when clicking on card in playable:
    if (cards().contains(card)) {
        emit handCardClicked(card->clone());
    }
}

void Playable::onToggleIsTableCardsVisible(bool isTableCardsVisible)
{
    isCardVecVisible_ = isTableCardsVisible;
    // Safely cast the parent to a Player object
    Player* playableParent = qobject_cast<Player*>(parent());

    if (playableParent) {
        for (const auto& card : cards_) {
            if (playableParent->isRobot())
                card->loadImage(isTableCardsVisible);
            else
                card->loadImage(true);
        }
        layout_->update();
        update();
    } else {
        qWarning() << "Parent is not of type Player";
    }
}
