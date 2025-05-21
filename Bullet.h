#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsRectItem>
#include <QObject>
#include <QKeyEvent>


class Bullet:public QObject ,public QGraphicsRectItem{
    Q_OBJECT

public:
    Bullet(qreal angle,bool isEnemyBullet=false);
    void move();
    void setDirection(qreal angle);
signals:
    void gameOver();
private:
    qreal direction;
    bool isEnemyBullet;
    int speed =13;
};


#endif // BULLET_H
