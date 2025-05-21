#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QImage>
#include <QGraphicsScene>
#include <QPixmap>
#include <QApplication>
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Create the main window (assuming you have a MainWindow class)
    game Game;
    Game.startGame();
    // Pass the scene to the MainWindow constructor

    // Show the main window
    Game.show();


    return a.exec();
}
