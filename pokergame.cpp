#include "pokergame.h"
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBrush>
#include <QMenu>
#include <QMenuBar>
#include <QHostAddress>

PokerGame::PokerGame(QWidget *parent)
    : QMainWindow(parent)
{
    view_ = new QGraphicsView;
    scene_ = new PokerScene;
    scene_->setSceneRect(QRectF(-400, -300, 800, 600));
    scene_->setBackgroundBrush(QBrush(QColor(0, 128, 0)));


    view_->setScene(scene_);
    view_->resize(900, 700);

    tcpSocket_ = NULL;

    QWidget *cwidget = new QWidget;
    QPushButton *startGameBtn = new QPushButton("Connect Server");
    connect(startGameBtn, SIGNAL(clicked()), this, SLOT(slotConnectServer()));
    startGameBtn->setFixedHeight(80);


    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(startGameBtn);
    infoList_ = new QListWidget;
    infoList_->setFixedWidth(150);
    infoList_->addItem("Test");
    rightLayout->addWidget(infoList_);

    layout->addWidget(view_);
    layout->addLayout(rightLayout);
    //layout->addWidget(startGameBtn);
    layout->setMargin(0);
    layout->setSpacing(0);

    cwidget->setLayout(layout);
    setCentralWidget(cwidget);


    resize(1000, 700);



    //backCard_ = new PokerCard;
//    backCard_->loadImage();
    //pokerVec_ = std::vector<PokerCard *>(cardNum, NULL);

    createActions();
    createMenus();


}

PokerGame::~PokerGame()
{



}

void PokerGame::slotConnectServer()
{
    if(tcpSocket_ && tcpSocket_->isValid())
        return;
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));



    QHostAddress *serverIP = new QHostAddress;
    serverIP->setAddress("127.0.0.1");

    tcpSocket_->connectToHost(*serverIP, 9009);

}

void PokerGame::slotConnected()
{
    QString info = QString("%1 : %2").arg(tcpSocket_->localAddress().toString()).arg(tcpSocket_->localPort());
    infoList_->addItem(info);

    int length = 0;
    QString msg= tr(":Enter Chat Room");
    if((length=tcpSocket_->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        return;
    }
}

void PokerGame::slotDisconnected()
{

}

void PokerGame::slotReadyRead()
{
    while(tcpSocket_->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket_->bytesAvailable());

        tcpSocket_->read(datagram.data(),datagram.size());
        QString msg=datagram.data();

        processData(msg);

        infoList_->addItem(msg.left(datagram.size()));
    }
}

void PokerGame::processData(QString msg)
{
    QStringList strlist = msg.split(",");
    if(strlist[0].toInt() == 1)
    {
        for(int i = 0; i < strlist[1].toInt(); i++)
        {
            int curCard = strlist[i + 2].toInt();
            PokerCard *card = new PokerCard;
            if(curCard == 52)
            {
                card->setSuit(BlackJoker);
            }
            else if(curCard == 53)
            {
                card->setSuit(RedJoker);
            }
            else
            {
                card->setRank(Rank(curCard%13 + 1));
                card->setSuit(Suit(curCard / 13 + 1));
            }

            card->loadImage();
            card->item()->setPos(-250 + i * 24, 130);
            card->item()->setScale(0.7);
            card->item()->setClicked(false);
            scene_->addItem(card->item());
            //pokerVec[i] = card;
        }
    }
}

void PokerGame::createActions()
{
    actConnectServer_ = new QAction("Connect Server", this);
    connect(actConnectServer_, SIGNAL(triggered()), this, SLOT(slotConnectServer()));

}

void PokerGame::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("Operation");
    fileMenu->addAction(actConnectServer_);
}
