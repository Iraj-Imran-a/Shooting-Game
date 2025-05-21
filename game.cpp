#include "game.h"
#include "Player.h"
#include <QApplication>

game::game(QWidget *parent):QMainWindow(parent)
    ,stackedWidget(new QStackedWidget(this))
    , gameScreen(nullptr)
    ,endScreen(nullptr)

{

        // Main Menu
        mainMenu = new QWidget(this);
        QVBoxLayout *menuLayout = new QVBoxLayout(mainMenu);

        QLabel *titleLabel = new QLabel("Horror AI", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 100px; font-weight: bold;");

        QPushButton *startButton = new QPushButton("Start Game", this);
        connect(startButton, &QPushButton::clicked, this, &game::startGame);

        menuLayout->addWidget(titleLabel);
        menuLayout->addWidget(startButton);
        mainMenu->setLayout(menuLayout);

        // Add Main Menu to Stacked Widget
        stackedWidget->addWidget(mainMenu);

        // Set Main Window Layout
        setCentralWidget(stackedWidget);
        resize(1080, 720);
}
void game::createGameElements(QGraphicsScene *scene, Enemy *enemy) {
        // Set up the grid dimensions and obstacle probability
        int rows = 12, cols = 18, cellSize = 80;
        float obstacleProbability = 0.2f;

        // Side walls
        QGraphicsRectItem *rect = new QGraphicsRectItem(-150, -20, 150, 750);
        rect->setBrush(Qt::darkBlue);
        rect->setPen(Qt::NoPen);
        scene->addItem(rect);

        QGraphicsRectItem *rect1 = new QGraphicsRectItem(1080, -20, 150, 750);
        rect1->setBrush(Qt::darkBlue);
        rect1->setPen(Qt::NoPen);
        scene->addItem(rect1);

        QGraphicsRectItem *rect2 = new QGraphicsRectItem(-150, -20, 1300, 20);
        rect2->setBrush(Qt::darkBlue);
        rect2->setPen(Qt::NoPen);
        scene->addItem(rect2);

        QGraphicsRectItem *rect3 = new QGraphicsRectItem(-150, 720, 1500, 20);
        rect3->setBrush(Qt::darkBlue);
        rect3->setPen(Qt::NoPen);
        scene->addItem(rect3);

        // Hardcoded middle rectangle
        QGraphicsRectItem *rect4 = new QGraphicsRectItem(640, 240, 80, 180);
        rect4->setBrush(Qt::darkBlue);
        rect4->setPen(Qt::NoPen);
        scene->addItem(rect4);

        // Loop through each cell in the grid to randomly place obstacles
        for (int row = 3; row < rows; ++row) {
            for (int col = 3; col < cols; ++col) {
                // Randomly decide if this cell should be an obstacle
                if (rand() % 100 < obstacleProbability * 100) {
                    QGraphicsRectItem *rect = new QGraphicsRectItem(col * cellSize, row * cellSize, cellSize, cellSize);
                    rect->setBrush(Qt::darkBlue);
                    rect->setPen(Qt::NoPen);
                    scene->addItem(rect);
                }
            }
        }

        // Create a QGraphicsTextItem to display the health as text
        QGraphicsTextItem *healthDisplay = new QGraphicsTextItem();
        healthDisplay->setPlainText(QString("health: %1").arg(enemy->getHealth()));
        healthDisplay->setDefaultTextColor(Qt::green);
        healthDisplay->setFont(QFont("comic sans", 15));
        healthDisplay->setZValue(2); // Ensure it renders above other items

        // Position the health display near the enemy
        healthDisplay->setPos(enemy->x() + 3, enemy->y() - 20);
        scene->addItem(healthDisplay);

        // Connect the enemy's healthChanged signal to update the text display
        QObject::connect(enemy, &Enemy::healthChanged, [scene, healthDisplay, enemy]() {
            healthDisplay->setPlainText(QString("health: %1").arg(enemy->getHealth()));
            if (enemy->getHealth() > 15) {
                healthDisplay->setDefaultTextColor(Qt::green); // high health
            } else if (enemy->getHealth() > 5) {
                healthDisplay->setDefaultTextColor(Qt::yellow); // medium health
            } else {
                healthDisplay->setDefaultTextColor(Qt::black); // low health
            }

            if (enemy->getHealth() == 0) {
                scene->removeItem(healthDisplay);
                delete healthDisplay;
            }
        });

        // Update the position of the text as the enemy moves
        QObject::connect(enemy, &Enemy::positionChanged, [healthDisplay, enemy]() {
            healthDisplay->setPos(enemy->x() + 3, enemy->y() - 20); // Keep it above the enemy
        });
    }
void game::startGame() {
        // Initialize Game Screen
        if (gameScreen) {
            delete gameScreen;  // Delete the existing game screen if it exists
            gameScreen = nullptr;  // Reset the pointer to nullptr
        }
        if (endScreen) {
            delete endScreen;
            endScreen = nullptr;
        }
        gameScreen = new QWidget(this); // Creating gameScreen

        QGraphicsScene * scene =new QGraphicsScene(); // Creating scene, player, enemy
        scene->setSceneRect(0,0,1080,720);

        Player *player = new Player();
        player->setPos(10,10);
        player->setFlag(QGraphicsItem::ItemIsFocusable);
        player->setFocus();
        scene->addItem(player);

        Enemy *enemy = new Enemy(player);
        scene->addItem(enemy);

        QGraphicsView * view = new QGraphicsView(scene);

        view->setRenderHint(QPainter::Antialiasing); //smooth rendering
        createGameElements(scene,enemy);

        // Set up the layout for the game screen
        QVBoxLayout *gameLayout = new QVBoxLayout(gameScreen);
        gameLayout->addWidget(view);  // Add the view to the layout
        gameScreen->setLayout(gameLayout);

        // Connect player to win/lose signals
        connect(enemy, &Enemy::gameOver, this, &game::showLoseScreen);
        connect(enemy, &Enemy::gameWon, this, &game::showWinScreen);


        // Add Game Screen to Stacked Widget
        stackedWidget->addWidget(gameScreen);



        // Switch to Game Screen
        stackedWidget->setCurrentWidget(gameScreen);
    }

    void game::showWinScreen() {
        if (endScreen) {
            delete endScreen;  // Cleanup if already exists
            endScreen = nullptr;  // Reset the pointer
        }

        endScreen = new QWidget(this);  // Create new end screen widget
        QVBoxLayout *endLayout = new QVBoxLayout(endScreen);

        QLabel *winLabel = new QLabel("You defeated the A.I!", this);
        winLabel->setAlignment(Qt::AlignCenter);
        winLabel->setStyleSheet("font-size: 70px; font-weight: bold; color: green;");



        QPushButton *quitButton = new QPushButton("Quit", this);
        connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);

        endLayout->addWidget(winLabel);

        endLayout->addWidget(quitButton);
        endScreen->setLayout(endLayout);

        stackedWidget->addWidget(endScreen);
        stackedWidget->setCurrentWidget(endScreen);  // Ensure we switch to the end screen
    }

void game::showLoseScreen() {
        if (endScreen) {
            delete endScreen;  // Cleanup if already exists
            endScreen = nullptr;  // Reset the pointer
        }

        endScreen = new QWidget(this);  // Create new end screen widget
        QVBoxLayout *endLayout = new QVBoxLayout(endScreen);

        QLabel *loseLabel = new QLabel("You were defeated by A.I!", this);
        loseLabel->setAlignment(Qt::AlignCenter);
        loseLabel->setStyleSheet("font-size: 70px; font-weight: bold; color: red;");



        QPushButton *quitButton = new QPushButton("Quit", this);
        connect(quitButton, &QPushButton::clicked, this, &QApplication::quit);

        endLayout->addWidget(loseLabel);

        endLayout->addWidget(quitButton);
        endScreen->setLayout(endLayout);

        stackedWidget->addWidget(endScreen);
        stackedWidget->setCurrentWidget(endScreen);  // Ensure we switch to the end screen
    }


