#include "pokergame.h"
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QBrush>
#include <QMenu>
#include <QMenuBar>

PokerGame::PokerGame(QWidget *parent)
    : QMainWindow(parent)
{
    view_ = new QGraphicsView;
    scene_ = new PokerScene;
    scene_->setSceneRect(QRectF(-400, -300, 800, 600));
    scene_->setBackgroundBrush(QBrush(QColor(0, 128, 0)));


    view_->setScene(scene_);
    view_->resize(900, 700);

    QWidget *cwidget = new QWidget;
    QPushButton *startGameBtn = new QPushButton("Start Game");
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(view_);
    layout->addWidget(startGameBtn);
    layout->setMargin(0);
    layout->setSpacing(0);
    cwidget->setLayout(layout);
    setCentralWidget(view_);


    resize(900, 700);



    //backCard_ = new PokerCard;
//    backCard_->loadImage();
    //pokerVec_ = std::vector<PokerCard *>(cardNum, NULL);

    createActions();
    createMenus();

}

PokerGame::~PokerGame()
{



}

void PokerGame::createActions()
{
    actStartGame_ = new QAction("Start Game", this);
    connect(actStartGame_, SIGNAL(triggered()), this, SLOT(slotStartGame()));

}

void PokerGame::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("Operation");
    fileMenu->addAction(actStartGame_);
}
