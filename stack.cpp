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
    int maxCardsToShow = 9;

    for (int i = cards_.length(); i >= 0; i--) {
        for (auto& card : std::as_const(cards_)) {
            card->hide();
        }

        int startIdx = qMax(0, cards_.size() - maxCardsToShow);
        for (int i = startIdx; i < cards_.size(); ++i) {
            cards_[i]->show();
        }
    }
}
void Stack::onCardsPlayed(int numCardsPlayed)
{
    updateLayout();
}

void Stack::onToggleCardsVisible(bool isVisible)
{
    isCardFaceVisible_ = true;

    for (const auto& card : std::as_const(cards_)) {
        card->loadImage(true);
    }
}
