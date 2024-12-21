#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include "handdeck.h"

class Player : public QObject
{
    Q_OBJECT

private:
    int id_;
    QString name_;
    bool isRobot_;
    int score_;
    int jpoints_;
    QSharedPointer<Handdeck> handdeck_;

public:
    explicit Player(int id = 0,
                    const QString &name = "",
                    bool isRobot = true,
                    int score = 0,
                    QSharedPointer<Handdeck> handdeck = QSharedPointer<Handdeck>(nullptr),
                    QObject *parent = nullptr); // QObject instead of QWidget
    ~Player() = default;

    friend bool operator<(const Player &lhs, const Player &rhs);
    friend bool operator>(const Player &lhs, const Player &rhs);
    friend bool operator==(const Player &lhs, const Player &rhs);

    // Getters
    int id() const;
    QString name() const;
    bool isRobot() const;
    int jpoints() const;
    int score() const;
    QSharedPointer<Handdeck> handdeck() const;

    // Methods
    int pointsOnHand();

    // Setters
    void setName(const QString &name);
    void setIsRobot(bool isRobot);
    void setJpoints(int jpoints);
    void setHanddeck(QSharedPointer<Handdeck> handdeck);

public slots:
    void onCountPoints(int shuffles = 1);

};

#endif // PLAYER_H
