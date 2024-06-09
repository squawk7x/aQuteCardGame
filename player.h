#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "handdeck.h"

class Player : public QWidget
{
    Q_OBJECT

private:
    int id_;
    QString name_;
    bool isRobot_;
    int score_;
    int jpoints_;
    Handdeck* handdeck_;

public:
    explicit Player(QWidget* parent,
                    int id,
                    const QString& name = "",
                    bool isRobot = true,
                    int score = 0,
                    Handdeck* handdeck = nullptr);
    ~Player();

    friend bool operator<(const Player& lhs, const Player& rhs);
    friend bool operator>(const Player& lhs, const Player& rhs);
    friend bool operator==(const Player& lhs, const Player& rhs);

    // Getters
    int id() const;
    QString name() const;
    bool isRobot() const;
    int jpoints() const;
    int score() const;
    Handdeck* handdeck() const;

    // Methods
    int countHand(int shuffles = 1);

    // Setters
    void setName(const QString& name);
    void setIsRobot(bool isRobot);
    // void setScore(int score);
    void setJpoints(int jpoints);
    void setHanddeck(Handdeck* handdeck);

public slots:
    void onCountPoints(int shuffles);
};

#endif // PLAYER_H
