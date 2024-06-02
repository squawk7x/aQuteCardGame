#include "player.h"

Player::Player(QWidget* parent, const QString& name, bool isRobot, int score, Handdeck* handdeck)
    : QWidget(parent)
    , name_(name)
    , isRobot_(isRobot)
    , score_(score)
    , jpoints_(0)
    , handdeck_(handdeck ? handdeck : new Handdeck(this))

{}

Player::~Player()
{
    delete handdeck_;
}

QString Player::name() const
{
    return name_;
}

bool Player::isRobot() const
{
    return isRobot_;
}

int Player::score() const
{
    return score_;
}

Handdeck* Player::handdeck() const
{
    return handdeck_;
}

// Methods
int Player::countHand(int shuffles)
{
    int pointsOnHand = 0;
    for (const auto& card : handdeck_->cards()) {
        pointsOnHand += card->value();
    }
    pointsOnHand += jpoints_;
    return pointsOnHand * shuffles;
}

// Setters
void Player::setName(const QString& name)
{
    name_ = name;
}

void Player::setIsRobot(bool isRobot)
{
    isRobot_ = isRobot;
}

void Player::setScore(int score)
{
    score_ += score;
}

void Player::setJpoints(int jpoints)
{
    jpoints_ += jpoints;
}

void Player::setHanddeck(Handdeck* handdeck)
{
    if (handdeck_ != handdeck) {
        delete handdeck_;
        handdeck_ = handdeck;
    }
}

// Slots:
void Player::onCountPoints(int shuffles)
{
    if (shuffles == 0) {
        score_ = 0;
        jpoints_ = 0;
    } else {
        score_ += countHand(shuffles);

        if (score_ == 125)
            score_ = 0;
    }

    qDebug() << name() << score();
}
