#include "blind.h"
#include <QRandomGenerator>
#include <algorithm>
#include <random>

extern QVector<QString> suits;
extern QVector<QString> ranks;

Blind::Blind(QWidget* parent, const QVector<QSharedPointer<Card>>& rhs)
    : CardVec(parent, rhs)
{
    if (rhs.isEmpty()) {
        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                auto card = QSharedPointer<Card>::create(suit, rank);
                addCard(card);
            }
        }
    } else
        cards_ = rhs;
    this->shuffle();

    if (!cards_.isEmpty()) {
        for (int i = 0; i < cards_.size() - 1; ++i) {
            cards_[i]->hide();
        }
    }

    layout_->update();
    update();
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
