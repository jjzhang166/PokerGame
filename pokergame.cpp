#include "pokergame.h"
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBrush>
#include <QMenu>
#include <QMenuBar>
#include <QHostAddress>
#include <QtAlgorithms>

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

    gameStatus_ = BeforeGame;
    resize(1000, 700);

    connect(scene_, SIGNAL(playCards()), this, SLOT(playCards()));


    //backCard_ = new PokerCard;
//    backCard_->loadImage();
    //pokerVec_ = std::vector<PokerCard *>(cardNum, NULL);

    createActions();
    createMenus();
    loadImages();


    backRenderer_ = new QSvgRenderer(QString(":/cards/back.svg"));



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

    gameStatus_ = ConnectedAndWait;

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
    //infoList_->addItem(QString("SlotReadyRead"));
    while(tcpSocket_->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket_->bytesAvailable());

        tcpSocket_->read(datagram.data(),datagram.size());
        QString msg=datagram.data();
        infoList_->addItem(msg.left(datagram.size()));
        processData(msg);


    }
}

void PokerGame::processData(QString msg)
{

    QStringList strlist = msg.split(",");
    if(strlist[0].toInt() == 1)
    {
        receiveInitialCards(strlist);
    }
    if(strlist[0].toInt() == 2)
    {
        //infoList_->addItem(QString("processData"));
        receiveCardsFromPlayers(strlist);
        gameStatus_ = GameStatus((int(gameStatus_) + 1) % 4);
        doSomething();
    }

}

void PokerGame::receiveCardsFromPlayers(QStringList &strlist)
{
    if(strlist[1].toInt() == playerId_)
    {
        QList<PokerCard*> myCardOnDeck;
        for(int i  = 0; i < strlist[2].toInt(); i++)
        {
            for(int j = 0; j < cardInMyHands.length(); j++)
            {
                if(cardInMyHands.at(j)->cardID() == strlist[3 + i].toInt())
                {
                    myCardOnDeck.append(cardInMyHands.at(j));
                    cardInMyHands.removeAt(j);
                    j--;
                }
            }
        }
        arrangeMyInHandCards();
        arrangeMyOutOfHandCards(myCardOnDeck);

    }
    else
    {
        infoList_->addItem(QString("receive data"));
        int num = (playerId_ + 4 - strlist[1].toInt()) % 4;

        if(num == 3)
        {
            leftPlayerPlayCards(strlist);
        }
        if(num == 2)
        {
            topPlayerPlayCards(strlist);
        }
        if(num == 1)
        {
            rightPlayerPlayCards(strlist);
        }
    }
}
void PokerGame::leftPlayerPlayCards(QStringList &strlist)
{
    infoList_->addItem(QString("leftPlayerPlayCards"));
    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsLeft_.at(i)->item());
        //cardInHandsLeft_.at(i)->deleteLater();
    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsLeft_.removeFirst();
    }
    arrangeLeftInHandCards();

    for(int i = 0; i < cardOutOfHandsLeft_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsLeft_.at(i)->item());
        //cardOutOfHandsLeft_.at(i)->deleteLater();
    }
    cardOutOfHandsLeft_.clear();


    for(int i = 0; i < cardNum; i++)
    {
        PokerCard* card = new PokerCard(strlist[3 + i].toInt());
        card->loadImage();
        scene_->addItem(card->item());
        cardOutOfHandsLeft_.append(card);
    }

    arrangeLeftOutOfHandCards();
}
void PokerGame::topPlayerPlayCards(QStringList &strlist)
{
    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsTop_.at(i)->item());
        //cardInHandsTop_.at(i)->deleteLater();

    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsTop_.removeFirst();
    }
    arrangeLeftInHandCards();
    qDebug() << "Card Num = " << cardNum;
    for(int i = 0; i < cardOutOfHandsTop_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsTop_.at(i)->item());
        //cardOutOfHandsTop_.at(i)->deleteLater();
    }
    cardOutOfHandsTop_.clear();

    qDebug() << "Card Num = " << cardNum;
    for(int i = 0; i < cardNum; i++)
    {
        PokerCard* card = new PokerCard(strlist[3 + i].toInt());
        card->loadImage();
        scene_->addItem(card->item());
        cardOutOfHandsTop_.append(card);
    }

    arrangeTopOutOfHandCards();
}
void PokerGame::rightPlayerPlayCards(QStringList &strlist)
{
    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsRight_.at(i)->item());
        //cardInHandsLeft_.at(i)->deleteLater();
    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsRight_.removeFirst();
    }
    arrangeLeftInHandCards();

    for(int i = 0; i < cardOutOfHandsRight_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsRight_.at(i)->item());
       // cardOutOfHandsRight_.at(i)->deleteLater();
    }
    cardOutOfHandsRight_.clear();


    for(int i = 0; i < cardNum; i++)
    {
        PokerCard* card = new PokerCard(strlist[3 + i].toInt());
        card->loadImage();
        //card->item()->hide();
        card->item()->setScale(0.35);
        scene_->addItem(card->item());
        cardOutOfHandsRight_.append(card);
    }

    arrangeRightOutOfHandCards();
}

void PokerGame::receiveInitialCards(QStringList &strlist)
{
    playerId_ = strlist[1].toInt();
    QString info = QString("I am No. %1 player.").arg(playerId_);
    infoList_->addItem(info);
    for(int i = 0; i < strlist[2].toInt(); i++)
    {
        int curCard = strlist[i + 3].toInt();
        PokerCard *card = new PokerCard(curCard);

        card->loadImage();
        cardInMyHands.append(card);
        //card->item()->setPos(-250 + i * 24, 130);
        card->item()->setScale(0.6);
        card->item()->setClicked(false);

        //pokerVec[i] = card;
    }
    qSort(cardInMyHands.begin(), cardInMyHands.end(), PokerCard::compareCard);
    addInitialCardsToScene();
    gameStatus_ = WaitPlayer1;
    doSomething();

}
void PokerGame::doSomething()
{
    if(gameStatus_ == GameStatus(playerId_))
    {
        playImageItem_->show();
        passImageItem_->show();
    }
    else
    {

        playImageItem_->hide();
        passImageItem_->hide();
    }
}

void PokerGame::playCards()
{
    if(gameStatus_ == GameStatus(playerId_))
    {
        QList<int> playCardList;
        QString info = QString("2,%1").arg(playerId_);
        for(int i = 0; i < cardInMyHands.length(); i++)
        {
            if(cardInMyHands.at(i)->item()->isClicked())
            {
                playCardList.append(cardInMyHands.at(i)->cardID());
            }
        }
        info.append(QString(",%1").arg(playCardList.length()));
        for(int i = 0; i < playCardList.length(); i++)
        {
            info.append(QString(",%1").arg(playCardList.at(i)));
        }
        int length = 0;

        if((length=tcpSocket_->write(info.toLatin1(),info.length()))!=info.length())
        {
            return;
        }
    }
}

void PokerGame::addInitialCardsToScene()
{
    if(cardInMyHands.length() > 0)
    {
        for(int i = 0; i < cardInMyHands.length(); i++)
            scene_->addItem(cardInMyHands.at(i)->item());
    }
    for(int i = 0; i < 13; i++)
    {
        PokerCard* badCard = new PokerCard(-1);
        badCard->loadImageByRenderer(backRenderer_);
        badCard->item()->setScale(0.45);
        //badCard->item()->setPos(-300, -200 + 15 * i);
        cardInHandsTop_.append(badCard);
        scene_->addItem(badCard->item());

    }
    for(int i = 0; i < 13; i++)
    {
        PokerCard* badCard = new PokerCard(-1);
        badCard->loadImageByRenderer(backRenderer_);
        badCard->item()->setScale(0.45);
        //badCard->item()->setPos(-300, -200 + 15 * i);
        badCard->item()->setRotation(90);
        cardInHandsLeft_.append(badCard);
        scene_->addItem(badCard->item());

    }
    for(int i = 0; i < 13; i++)
    {
        PokerCard* badCard = new PokerCard(-1);
        badCard->loadImageByRenderer(backRenderer_);
        badCard->item()->setScale(0.45);
        badCard->item()->setRotation(-90);
        //badCard->item()->setPos(300, -200 + 15 * i + badCard->item()->sceneBoundingRect().height());
        cardInHandsRight_.append(badCard);
        scene_->addItem(badCard->item());
    }

    arrangeLeftInHandCards();
    arrangeMyInHandCards();
    arrangeRightInHandCards();
    arrangeTopInHandCards();
}

void PokerGame::arrangeTopInHandCards()
{
    if(cardInHandsTop_.length() > 0)
    {
        int curCardNum = cardInHandsTop_.length();
        int step = 15;
        int start = 0 - (step * (curCardNum - 1) + cardInHandsTop_.at(0)->item()->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardInHandsTop_.at(i)->item()->setPos( start + i * step, -320);
            //scene_->addItem(cardTop_.at(i)->item());
        }
    }
}

void PokerGame::arrangeLeftInHandCards()
{
    int curCardNum = cardInHandsLeft_.length();
    if(curCardNum == 0)
        return;
    int step = 15;
    for(int i = 0; i < curCardNum; i++)
    {
        cardInHandsLeft_.at(i)->item()->setPos(-300, -200 + step * i);;
        //scene_->addItem(cardTop_.at(i)->item());
    }
}

void PokerGame::arrangeRightInHandCards()
{
    int curCardNum = cardInHandsRight_.length();
    if(curCardNum == 0)
        return;
    int height = cardInHandsRight_.at(0)->item()->sceneBoundingRect().height();
    int step = 15;
    for(int i = 0; i < curCardNum; i++)
    {
        cardInHandsRight_.at(i)->item()->setPos(300, -200 + step * i + height);
        //scene_->addItem(cardTop_.at(i)->item());
    }
}

void PokerGame::arrangeMyInHandCards()
{
    if(cardInMyHands.length() > 0)
    {
        int curCardNum = cardInMyHands.length();
        int step = 20;
        int start = 0 - (step * (curCardNum - 1) + cardInMyHands.at(0)->item()->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardInMyHands.at(i)->item()->setPos( start + i * 20, 190);
            //scene_->addItem(cardInMyHands.at(i)->item());
        }
    }
}

void PokerGame::arrangeMyOutOfHandCards(QList<PokerCard*> &newCardsOnDeck)
{
    for(int i = 0; i < cardOutOfMyHands_.length();i++)
    {
        scene_->removeItem(cardOutOfMyHands_.at(i)->item());
        cardOutOfMyHands_.at(0)->item()->hide();
        //cardOutOfMyHands_.at(0)->deleteLater();
    }
    cardOutOfMyHands_.clear();
    if(newCardsOnDeck.length() > 0)
    {
        int curCardNum = newCardsOnDeck.length();
        int step = 10;
        int start = 0 - (step * (curCardNum - 1) + newCardsOnDeck.at(0)->item()->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            newCardsOnDeck.at(i)->item()->setScale(0.35);
            newCardsOnDeck.at(i)->item()->setPos( start + i * step, 50);
            cardOutOfMyHands_.append(newCardsOnDeck.at(i));
            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }

}

void PokerGame::arrangeTopOutOfHandCards()
{
    if(cardOutOfHandsTop_.length() > 0)
    {
        int curCardNum = cardOutOfHandsTop_.length();
        int step = 10;
        cardOutOfHandsTop_.at(0)->item()->setScale(0.35);
        int start = 0 - (step * (curCardNum - 1) + cardOutOfHandsTop_.at(0)->item()->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsTop_.at(i)->item()->setScale(0.35);
            cardOutOfHandsTop_.at(i)->item()->setPos( start + i * step, -200);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
}

void PokerGame::arrangeLeftOutOfHandCards()
{
    if(cardOutOfHandsLeft_.length() > 0)
    {
        int curCardNum = cardOutOfHandsLeft_.length();
        int step = 10;
        int start = -280;
        cardOutOfHandsLeft_.at(0)->item()->setScale(0.35);
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsLeft_.at(i)->item()->setScale(0.35);
            cardOutOfHandsLeft_.at(i)->item()->setPos( i * step + start, -90);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
}

void PokerGame::arrangeRightOutOfHandCards()
{
    if(cardOutOfHandsRight_.length() > 0)
    {
        int curCardNum = cardOutOfHandsRight_.length();
        int step = 10;

        int start = 280 - (step * (curCardNum - 1) + cardOutOfHandsRight_.at(0)->item()->sceneBoundingRect().width()) ;
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsRight_.at(i)->item()->setScale(0.35);
            cardOutOfHandsRight_.at(i)->item()->setPos( start + i * step , -90);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
}

void PokerGame::createActions()
{
    actConnectServer_ = new QAction("Connect Server", this);
    connect(actConnectServer_, SIGNAL(triggered()), this, SLOT(slotConnectServer()));

}

void PokerGame::loadImages()
{
    QString strName = QString(":/images/play.svg");
    playImageItem_ = new QGraphicsSvgItem();
    QSvgRenderer *playrenderer = new QSvgRenderer(strName);
    playImageItem_->setSharedRenderer(playrenderer);
    playImageItem_->setScale(0.10);
    playImageItem_->setPos(-70-playImageItem_->sceneBoundingRect().width(), 140);
    playImageItem_->hide();
   // playImageItem_->setElementId(QString("Play"));
    scene_->addItem(playImageItem_);

    strName = QString(":/images/pass.svg");
    passImageItem_ = new QGraphicsSvgItem();
    QSvgRenderer *passrenderer = new QSvgRenderer(strName);
    passImageItem_->setSharedRenderer(passrenderer);
    passImageItem_->setScale(0.10);
    passImageItem_->setPos(70, 140);
    passImageItem_->hide();
    //passImageItem_->setElementId(QString("Pass"));
    scene_->addItem(passImageItem_);

}

void PokerGame::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("Operation");
    fileMenu->addAction(actConnectServer_);
}
