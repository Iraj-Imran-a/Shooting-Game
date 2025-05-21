#include "Enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QObject>
#include "Bullet.h"

Enemy::Enemy(QGraphicsItem *targetItem, QGraphicsItem *parent)
: QGraphicsRectItem(parent), target(targetItem)
{
    setRect(0,0,75,75); //initializing enemy
    setBrush(Qt::red);
    setPos(800,20);

    QTimer * timer = new QTimer();
    connect(timer, &QTimer::timeout,this, &Enemy::move); //connecting a timer to change enemy mvmnt
    timer->start(50);
    health = 20;
    QTimer * attacktimer = new QTimer();
    connect(attacktimer, &QTimer::timeout,this, &Enemy::spreadAttack); //connecting a timer to change enemy mvmnt
    attacktimer->start(3000);

}


void Enemy::move()
{
    if (isPaused) return;
    emit positionChanged();
    if (!target) return;

    QPointF enemyPos = pos();

    //getting the target's position
    QPointF targetPos = target->pos();

    //calculating the direction vector
    QPointF direction = targetPos - enemyPos;

    //normalizing the vector
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length == 0) return; // Avoid division by zero
    QPointF normalized = direction / length;

    //moving the enemy toward the player(target)
    setPos(enemyPos + normalized * speed);

    if (collidesWithItem(target))
    {
        emit gameOver();

    }
}

void Enemy::decreaseHealth(){
    health--;
    emit healthChanged();
    if (health<=0){
        emit gameWon();
        scene()->removeItem(this);
        delete this;

    }
}

void Enemy::spreadAttack()
{

        // Set the center of the enemy's rectangle
        int enemyCenterX = this->x() + this->rect().width() / 2;
        int enemyCenterY = this->y() + this->rect().height() / 2;

        // Define the spread angles for the bullets (in radians)
        QVector<double> directions = {-M_PI / 4, 0, M_PI / 4,-M_PI/2,M_PI/2,M_PI,-3*M_PI/4,3*M_PI/4};  // 45 degrees left, center, and right

        for (double direction : directions) {
            Bullet * bullet =new Bullet(direction,true);
            bullet->setPos(enemyCenterX - bullet->rect().width() / 2, enemyCenterY - bullet->rect().height() / 2); // Center the bullet
            connect(bullet, &Bullet::gameOver, [this]() {
                emit gameOver(); // Pass the signal up to the MainWindow
            });
            scene()->addItem(bullet);
        }
        isPaused = true;  // Set the flag to pause the enemy

        // Set up a timer to resume the movement after 1 second (1000 milliseconds)
        QTimer::singleShot(1000, this, [this]() {
            isPaused = false;  // Resume movement after the pause
        });
}

