#include "blind.h"
#include <QRandomGenerator>
#include <algorithm>
#include <random>

extern QVector<QString> suits; // Assuming these are defined globally
extern QVector<QString> ranks;

Blind::Blind(QWidget* parent, const QVector<QSharedPointer<Card>>& rhs)
    : CardVec(parent, rhs.isEmpty() ? QVector<QSharedPointer<Card>>() : rhs)
{
    if (rhs.isEmpty()) {
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                auto card = QSharedPointer<Card>::create(suit, rank);
                addCard(card);
                // cards_.append(card);
                // layout_->addWidget(card.data()); // Add the card to the existing layout
            }
        }
    }

    // shuffle();      // Shuffle cards after initialization
    // this->update(); // Request an update to trigger paintEvent
}

Blind::~Blind()
{
    // CardVec's destructor handles cleaning up the cards
}

void Blind::shuffle()
{
    auto rng = QRandomGenerator::global();
    std::shuffle(cards_.begin(), cards_.end(), std::mt19937(rng->generate()));
}

QSharedPointer<Card> Blind::drawCardFromBlind()
{
    if (!cards_.isEmpty()) {
        QSharedPointer<Card> card = cards_.takeLast();
        this->update();
        return card;
    }
    return nullptr;
}
