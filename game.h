#ifndef GAME_H
#define GAME_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include "Enemy.h"

class game :public QMainWindow
{
    Q_OBJECT

public:
    game(QWidget *parent = nullptr);
    void createGameElements(QGraphicsScene *scene, Enemy *enemy);
    void startGame();


private:

    QStackedWidget *stackedWidget; // Holds different screens (menu, game, win/lose)
    QWidget *mainMenu;             // Main menu screen
    QWidget *gameScreen;           // Game screen
    QWidget *endScreen;            // Win/Lose screen
    QGraphicsScene *gameScene;     // Game scene
    QGraphicsView *gameView;

public slots:

    void showWinScreen();
    void showLoseScreen();

};
#endif // GAME_H
