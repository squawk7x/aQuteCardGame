#include "blind.h"
#include <QRandomGenerator>
#include "qevent.h"
#include <algorithm>
#include <random>

extern QVector<QString> suits;
extern QVector<QString> ranks;

Blind::Blind(QWidget* parent, const QVector<QSharedPointer<Card>>& rhs)
    : CardVec(parent, rhs)
{
    cardFace_ = CardFace::Closed;

    if (rhs.isEmpty()) {
        foreach (const auto& suit, suits) {
            foreach (const auto& rank, ranks) {
                auto card = QSharedPointer<Card>::create(suit, rank);
                card->setStyleSheet(
                    "background-color: white; border: 1px solid black; border-radius: 4px; "
                    "padding: 1px 2px; margin: 0px; font-size: 16px;");
                card->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
                addCard(card);
            }
        }
    } else
        cards_ = rhs;

    this->shuffle();
}

Blind::~Blind()
{
    // CardVec's destructor handles cleaning up the cards
}

void Blind::removeCard(QSharedPointer<Card> card)
{
    if (cards_.contains(card)) {
        layout_->removeWidget(card.data());
        card->setParent(nullptr);
        cards_.removeOne(card);
        showTopCard();
    }
}

void Blind::shuffle()
{
    auto rng = QRandomGenerator::global();
    std::shuffle(cards_.begin(), cards_.end(), std::mt19937(rng->generate()));

    updateLayout();
}

void Blind::showTopCard()
{
    if (!cards_.isEmpty()) {
        for (int i = 0; i < cards_.size() - 1; ++i) {
            cards_[i]->hide();
        }
        cards_.last()->show();
    }
}

// void Blind::onToggleCardsVisible(bool isVisible)
// {
//     isCardFaceVisible_ = isVisible;

//     foreach (const auto& card, cards_) {
//         card->loadImage(isVisible);
//     }
// }

void Blind::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit blindClicked();
    }
    QWidget::mousePressEvent(event);
}
