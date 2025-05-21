#ifndef MYRECT_H
#define MYRECT_H

#include<QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <QBrush>
#include <QGraphicsPixmapItem>


class Player: public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Player() {
        setRect(0,0, 50, 50);
        setBrush(Qt::green);  // Obstacle color
        movingUp=false, movingDown=false,movingRight=false,movingLeft=false;
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Player::move);
        timer->start(16);
    }
    void move();
    void createBullet();
    void keyPressEvent(QKeyEvent * event) override;
    void keyReleaseEvent(QKeyEvent *event)override;

signals:
    void gameOver();
private:
    int speed=4;
    qreal lastDirection;
    QTimer *timer;
    bool movingUp, movingDown,movingRight,movingLeft;

};

#endif // MYRECT_H
