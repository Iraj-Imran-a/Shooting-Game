#include "Bullet.h"
#include <QTimer>
#include "Enemy.h"
#include "Player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QtMath>
#include <QDebug>

Bullet::Bullet(qreal angle,bool isEnemyBullet)
{
    if (isEnemyBullet){
        setRect(0,0,40,40);
        setBrush(Qt::red);
        setPen(Qt::NoPen);
    }
    else{
        setRect(0,0,5,5);
        setBrush(Qt::black);
    }
    setFlag(QGraphicsItem::ItemIsFocusable, false);

    direction = angle;
    this->isEnemyBullet = isEnemyBullet;

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this, &Bullet::move); //connecting a timer to move bullet
    if(isEnemyBullet)
        timer->start(16);
    timer->start(50);


}

void Bullet::move()
{
    if(isEnemyBullet)
        speed= 20;

    setPos(x()+speed*qCos(direction),y()+speed *qSin(direction));//moving in components of the angle

    QList<QGraphicsItem *> collidingItemsList = collidingItems(); //checking for collision in the array
    for (QGraphicsItem *item : collidingItemsList)
    {              // which has all collisions stored

        Enemy *enemy = dynamic_cast<Enemy *>(item);
        if (enemy&& !isEnemyBullet)
        {
            enemy->decreaseHealth(); //reducing enemy health on collision
            scene()->removeItem(this); //removing the bullet
            delete this; //deleting the bullet after collision
            return;
        }
        Player *player = dynamic_cast<Player *>(item);
        if (player&&isEnemyBullet){
            emit gameOver();

            scene()->removeItem(this);
            delete this;
            return;
        }
    }
    if (pos().y()<0 || pos().y()>720||pos().x()>1080||pos().x()<0) //deleting bullet when off screen
    {
        scene()->removeItem(this);
        delete this;
    }
}

void Bullet::setDirection(qreal angle)
{
    direction = angle;

}
