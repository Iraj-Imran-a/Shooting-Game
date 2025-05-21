#include "Player.h"
#include <QDebug>
#include <QKeyEvent>
#include "Bullet.h"
#include <QTimer>
#include <QGraphicsScene>



void Player::keyPressEvent(QKeyEvent * event)
{
    if (event -> key()==Qt::Key_Left) //moving player according to key press
        movingLeft =true;
    else if (event -> key()==Qt::Key_Right)
        movingRight =true;
    else if (event -> key()==Qt::Key_Up)
        movingUp=true;
    else if (event -> key()==Qt::Key_Down)
        movingDown=true;

    if (event->key() == Qt::Key_W)
    { //determining direction of bullet
        lastDirection = -M_PI / 2;  //setting -pi/2 or upwards
        Bullet * bullet = new Bullet(lastDirection);
        bullet->setPos(x()+25,y()+25);
        scene()->addItem(bullet);

    }
    else if (event->key() == Qt::Key_S)
    {
        lastDirection = M_PI / 2;  //down
        Bullet * bullet = new Bullet(lastDirection);
        bullet->setPos(x()+25,y()+25);
        scene()->addItem(bullet);

    }
    else if (event->key() == Qt::Key_A)
    {
        lastDirection = M_PI;  //left
        Bullet * bullet = new Bullet(lastDirection);
        bullet->setPos(x()+25,y()+25);
        scene()->addItem(bullet);

    }
    else if (event->key() == Qt::Key_D)
    {
        lastDirection = 0;  //right
        Bullet * bullet = new Bullet(lastDirection);
        bullet->setPos(x()+25,y()+25);
        scene()->addItem(bullet);

    }
    if (event->key()==Qt::Key_Space)
    {  //shooting the bullet

        Bullet * bullet = new Bullet(lastDirection,false);
        bullet->setPos(x()+25,y()+25);
        scene()->addItem(bullet);

    }

}
void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) movingUp = false;
    else if (event->key() == Qt::Key_Down) movingDown = false;
    else if (event->key() == Qt::Key_Left) movingLeft = false;
    else if (event->key() == Qt::Key_Right) movingRight = false;
}

void Player::move()
{
    // Calculate the new position based on movement flags
    qreal newX = x();
    qreal newY = y();

    if (movingUp) newY -= speed;
    if (movingDown) newY += speed;
    if (movingLeft) newX -= speed;
    if (movingRight) newX += speed;

    // Create a temporary rectangle for the new position
    QRectF newPlayerRect(newX, newY, boundingRect().width(), boundingRect().height());


    // Check for potential collisions at the new position
    QList<QGraphicsItem *> itemsAtNewPosition = scene()->items(newPlayerRect);
    for (QGraphicsItem *item : itemsAtNewPosition) {
        // Skip checking collisions with the player itself
        if (item == this) continue;
        Bullet* bullet = dynamic_cast<Bullet*>(item);
        if (bullet) {
            // Skip collision if the bullet is newly created or doesn't interact with the player
            continue;
        }
        // If there's an item at the new position, abort movement
        if (item->type() == QGraphicsRectItem::Type) { // Example: Checking if the item is an obstacle
            return;
        }
    }

    // No collisions detected; move the player to the new position
    setPos(newX, newY);
}
