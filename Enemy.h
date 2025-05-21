#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsRectItem>
#include <QObject>

class Enemy : public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    Enemy(QGraphicsItem *target, QGraphicsItem *parent = nullptr);
    void decreaseHealth();
    int getHealth() const {return health;}
    void spreadAttack();
public slots:
    void move();
signals:
    void healthChanged();
    void positionChanged();
    void gameOver();
    void gameWon();
private:
    QGraphicsItem * target;
    int health;
    int speed =10;
    bool isPaused =false;
};

#endif // ENEMY_H
