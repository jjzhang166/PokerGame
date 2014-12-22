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
#include <QLabel>
#include <QLineEdit>



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

    ui_tab = new SettingsTab;
    rightLayout->addWidget(ui_tab);
    ui_tab->setFixedHeight(100);
    ui_tab->setFixedWidth(180);
    ui_tab->getUi()->lineEditHostAddress->setText("127.0.0.1");
    ui_tab->getUi()->lineEditPort->setText("9009");
    ui_tab->getUi()->lineEditPassword->setEchoMode(QLineEdit::Password);

    infoList_ = new QListWidget;
    infoList_->setFixedWidth(180);
    rightLayout->addWidget(infoList_);

    QFrame *buttonFrame = new QFrame;
    QHBoxLayout *blayout = new QHBoxLayout;
    QPushButton *connectBtn = new QPushButton(QString("Connect"));
    QPushButton *disconnectBtn = new QPushButton(QString("Disconnect"));
    blayout->addWidget(connectBtn);
    blayout->addWidget(disconnectBtn);
    buttonFrame->setLayout(blayout);
    buttonFrame->setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255,255,255));
    buttonFrame->setPalette(palette);

    buttonFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    buttonFrame->setLineWidth(1);
    buttonFrame->setFixedWidth(180);

    rightLayout->addWidget(buttonFrame);
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(slotConnectServer()));
    connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(slotDisconnected()));
    //rightLayout->addWidget(startGameBtn);


    layout->addWidget(view_);
    layout->addLayout(rightLayout);

    layout->setMargin(0);
    layout->setSpacing(0);

    cwidget->setLayout(layout);
    setCentralWidget(cwidget);

    gameStatus_ = BeforeGame;
    resize(1100, 700);

    connect(scene_, SIGNAL(playCards()), this, SLOT(playCards()));
    connect(scene_, SIGNAL(passRound()), this, SLOT(passRound()));


    createActions();
    createMenus();
    loadImages();


    backRenderer_ = new QSvgRenderer(QString(":/cards/back.svg"));
    lastCardOwner_ = -1;





}

PokerGame::~PokerGame()
{
    clearAllItemsOnTable();
    int length = 0;
    QString msg= tr("7\n");
    if((length=tcpSocket_->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        return;
    }



}

void PokerGame::slotConnectServer()
{
    if(tcpSocket_ == NULL)
        tcpSocket_ = new QTcpSocket(this);
    else
        tcpSocket_->close();
    connect(tcpSocket_, SIGNAL(connected()), this, SLOT(slotConnected()));
    //connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError(QAbstractSocket::SocketError)));



    QHostAddress *serverIP = new QHostAddress;
    serverIP->setAddress(ui_tab->getUi()->lineEditHostAddress->text());

    tcpSocket_->connectToHost(*serverIP, ui_tab->getUi()->lineEditPort->text().toInt());
    //if(tcpSocket_->waitForConnected(2000))

}

void PokerGame::slotConnected()
{
    QString info = QString("%1 : %2").arg(tcpSocket_->localAddress().toString()).arg(tcpSocket_->localPort());
    infoList_->addItem(info);

    gameStatus_ = ConnectedAndWait;

    int length = 0;
    QString msg= tr("Connected to the server!!! Wait for opening game!!!\n");
    if((length=tcpSocket_->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        return;
    }
}

void PokerGame::slotDisconnected()
{
    //qDebug() << "Disconnect~~" << endl;

    //infoList_->addItem(QString("Disconnect"));

    if(tcpSocket_ && tcpSocket_->isValid())
    {
        QString msg= tr("7\n");
        tcpSocket_->write(msg.toLatin1(),msg.length());
        tcpSocket_->close();
    }

    clearAllItemsOnTable();

}
void PokerGame::clearGame()
{
    //scene_->clear();
    clearOutOfHandCardsOnTable();

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

void PokerGame::slotSocketError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError :
    {
        QString info("Remote Host Closed~~");
        infoList_->addItem(info);
        slotEndGame();
        break;
    }
    case QAbstractSocket::HostNotFoundError :
    {
        QString info("Host Not Found~~");
        infoList_->addItem(info);
        //slotEndGame();
        break;
    }
    case QAbstractSocket::ConnectionRefusedError :
    {
        QString info("Connection Refused~~");
        infoList_->addItem(info);
        //slotEndGame();
        break;
    }
    default:
    {
        QString info("Unknown Error");
        infoList_->addItem(info);
        slotEndGame();
        break;
    }
    }
    tcpSocket_->close();

}

void PokerGame::slotEndGame()
{
    slotDisconnected();
}

void PokerGame::processData(QString msg)
{
    //infoList_->addItem(msg);
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
    if(strlist[0].toInt() == 3)
    {
        //infoList_->addItem(QString("processData"));
        receiveCardsFromPlayers(strlist);
        gameStatus_ = GameStatus((int(gameStatus_) + 1) % 4);
        doSomething();
    }
    if(strlist[0].toInt() == 9)
    {
        clearOutOfHandCardsOnTable();
    }
    if(strlist[0].toInt() == 7)
    {
        slotDisconnected();
    }


}

void PokerGame::receiveCardsFromPlayers(QStringList &strlist)
{
    if(strlist[1].toInt() == playerId_)
    {
        bottomPlayerPlayCards(strlist);

    }
    else
    {
        //infoList_->addItem(QString("receive data"));
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

void PokerGame::bottomPlayerPlayCards(QStringList &strlist)
{

    for(int i  = 0; i < strlist[2].toInt(); i++)
    {
        for(int j = 0; j < cardInHandsBottom_.length(); j++)
        {
            if(cardInHandsBottom_.at(j)->cardID() == strlist[3 + i].toInt())
            {
                scene_->removeItem(cardInHandsBottom_.at(j));
                cardOutOfHandsBottom_.append(cardInHandsBottom_.at(j));
                cardInHandsBottom_.removeAt(j);
                j--;
            }
        }
    }

    arrangeBottomInHandCards();
    arrangeBottomOutOfHandCards();
}
void PokerGame::leftPlayerPlayCards(QStringList &strlist)
{

    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsLeft_.at(i));
        delete cardInHandsLeft_.at(i);
    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsLeft_.removeFirst();
    }
    arrangeLeftInHandCards();

    clearOutOfHandCardsOnLeftTable();


    for(int i = 0; i < cardNum; i++)
    {
        PokerCardItem* card = new PokerCardItem;
        card->setCardID(strlist[3 + i].toInt());
        card->setClickable(false);
        card->loadImage();
        scene_->addItem(card);
        cardOutOfHandsLeft_.append(card);
    }

    if(cardOutOfHandsLeft_.length() > 0)
    {
        CardModel model = PokerCardModel::checkModel(cardOutOfHandsLeft_);
        lastCardModel_ = model;
        lastCardList_ = cardOutOfHandsLeft_;
        lastCardOwner_ = strlist[1].toInt();
    }


    arrangeLeftOutOfHandCards();
}
void PokerGame::topPlayerPlayCards(QStringList &strlist)
{
    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsTop_.at(i));
        delete cardInHandsTop_.at(i);

    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsTop_.removeFirst();
    }
    arrangeLeftInHandCards();

    clearOutOfHandCardsOnTopTable();


    for(int i = 0; i < cardNum; i++)
    {
        PokerCardItem* card = new PokerCardItem;
        card->setCardID(strlist[3 + i].toInt());
        card->setClickable(false);
        card->loadImage();
        scene_->addItem(card);
        cardOutOfHandsTop_.append(card);
    }

    if(cardOutOfHandsTop_.length() > 0)
    {
        CardModel model = PokerCardModel::checkModel(cardOutOfHandsTop_);
        lastCardModel_ = model;
        lastCardList_ = cardOutOfHandsTop_;
        lastCardOwner_ = strlist[1].toInt();
    }

    arrangeTopOutOfHandCards();
}
void PokerGame::rightPlayerPlayCards(QStringList &strlist)
{
    int cardNum = strlist[2].toInt();

    for(int i = 0; i < cardNum; i++)
    {
        scene_->removeItem(cardInHandsRight_.at(i));
        delete cardInHandsRight_.at(i);
    }
    for(int i = 0; i < cardNum; i++)
    {
        cardInHandsRight_.removeFirst();
    }
    arrangeLeftInHandCards();

    clearOutOfHandCardsOnRightTable();


    for(int i = 0; i < cardNum; i++)
    {
        PokerCardItem* card = new PokerCardItem;
        card->setCardID(strlist[3 + i].toInt());
        card->setClickable(false);
        card->loadImage();
        //card->item()->hide();
        card->setScale(0.35);
        scene_->addItem(card);
        cardOutOfHandsRight_.append(card);
    }

    if(cardOutOfHandsRight_.length() > 0)
    {
        CardModel model = PokerCardModel::checkModel(cardOutOfHandsRight_);
        lastCardModel_ = model;
        lastCardList_ = cardOutOfHandsRight_;
        lastCardOwner_ = strlist[1].toInt();
    }

    arrangeRightOutOfHandCards();
}

void PokerGame::clearAllItemsOnTable()
{
    clearInHandCardsOnTable();
    clearOutOfHandCardsOnTable();
    clearOtherItemsOnTable();
}

void PokerGame::clearOutOfHandCardsOnTable()
{
    clearOutOfHandCardsOnTopTable();
    clearOutOfHandCardsOnLeftTable();
    clearOutOfHandCardsOnRightTable();
    clearOutOfHandCardsOnBottomTable();

}

void PokerGame::clearOutOfHandCardsOnBottomTable()
{
    for(int i = 0; i < cardOutOfHandsBottom_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsBottom_.at(i));
        delete cardOutOfHandsBottom_.at(i);
    }
    cardOutOfHandsBottom_.clear();
    passBottomItem_->hide();
}

void PokerGame::clearOutOfHandCardsOnLeftTable()
{
    for(int i = 0; i < cardOutOfHandsLeft_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsLeft_.at(i));
        delete cardOutOfHandsLeft_.at(i);
    }
    cardOutOfHandsLeft_.clear();
    passLeftItem_->hide();
}

void PokerGame::clearOutOfHandCardsOnRightTable()
{
    for(int i = 0; i < cardOutOfHandsRight_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsRight_.at(i));
        delete cardOutOfHandsRight_.at(i);
    }
    cardOutOfHandsRight_.clear();
    passRightItem_->hide();
}

void PokerGame::clearOutOfHandCardsOnTopTable()
{
    for(int i = 0; i < cardOutOfHandsTop_.length(); i++)
    {
        scene_->removeItem(cardOutOfHandsTop_.at(i));
        delete cardOutOfHandsTop_.at(i);
    }
    cardOutOfHandsTop_.clear();
    passTopItem_->hide();
}

void PokerGame::clearInHandCardsOnTable()
{
    clearInHandCardsOnBottomTable();
    clearInHandCardsOnLeftTable();
    clearInHandCardsOnRightTable();
    clearInHandCardsOnTopTable();
}

void PokerGame::clearInHandCardsOnBottomTable()
{
    for(int i = 0; i < cardInHandsBottom_.length(); i++)
    {
        scene_->removeItem(cardInHandsBottom_.at(i));
        delete cardInHandsBottom_.at(i);
    }
    cardInHandsBottom_.clear();
    //passBottomItem_->hide();
}

void PokerGame::clearInHandCardsOnLeftTable()
{
    for(int i = 0; i < cardInHandsLeft_.length(); i++)
    {
        scene_->removeItem(cardInHandsLeft_.at(i));
        delete cardInHandsLeft_.at(i);
    }
    cardInHandsLeft_.clear();
}

void PokerGame::clearInHandCardsOnRightTable()
{
    for(int i = 0; i < cardInHandsRight_.length(); i++)
    {
        scene_->removeItem(cardInHandsRight_.at(i));
        delete cardInHandsRight_.at(i);
    }
    cardInHandsRight_.clear();
}

void PokerGame::clearInHandCardsOnTopTable()
{
    for(int i = 0; i < cardInHandsTop_.length(); i++)
    {
        scene_->removeItem(cardInHandsTop_.at(i));
        delete cardInHandsTop_.at(i);
    }
    cardInHandsTop_.clear();
}

void PokerGame::clearOtherItemsOnTable()
{
    playImageItem_->hide();
    passImageItem_->hide();
    clockLeftItem_->hide();
    clockBottomItem_->hide();
    clockTopItem_->hide();
    clockRightItem_->hide();
}

void PokerGame::receiveInitialCards(QStringList &strlist)
{
    playerId_ = strlist[1].toInt();
    QString info = QString("I am No. %1 player.").arg(playerId_);
    infoList_->addItem(info);
    for(int i = 0; i < strlist[2].toInt(); i++)
    {
        int curCard = strlist[i + 3].toInt();
        PokerCardItem *card = new PokerCardItem;
        card->setCardID(curCard);

        card->loadImage();
        cardInHandsBottom_.append(card);
        //card->setPos(-250 + i * 24, 130);
        card->setScale(0.6);
        card->setClickable(true);
        card->setClicked(false);

        //pokerVec[i] = card;
    }
    qSort(cardInHandsBottom_.begin(), cardInHandsBottom_.end(), PokerCardItem::compareCard);
    addInitialCardsToScene();
    gameStatus_ = WaitPlayer0;
    doSomething();

}
void PokerGame::doSomething()
{
    if(gameStatus_ == GameStatus(playerId_))
    {
        playImageItem_->show();
        passImageItem_->show();

        clockLeftItem_->hide();
        clockRightItem_->hide();
        clockTopItem_->hide();
        clockBottomItem_->show();

        if(lastCardOwner_ == int(gameStatus_))
        {
            QString info = QString("9, %1\n").arg(playerId_);
            int length = 0;
            if((length=tcpSocket_->write(info.toLatin1(),info.length()))!=info.length())
            {
                return;
            }
        }
        else
            clearOutOfHandCardsOnBottomTable();
    }
    else
    {

        playImageItem_->hide();
        passImageItem_->hide();
        int num = (playerId_ + 4 - int(gameStatus_)) % 4;

        if(num == 3)
        {
            clockLeftItem_->show();
            clockRightItem_->hide();
            clockTopItem_->hide();
            clockBottomItem_->hide();
            clearOutOfHandCardsOnLeftTable();
        }
        if(num == 2)
        {
            clockLeftItem_->hide();
            clockRightItem_->hide();
            clockTopItem_->show();
            clockBottomItem_->hide();
            clearOutOfHandCardsOnTopTable();
        }
        if(num == 1)
        {
            clockLeftItem_->hide();
            clockRightItem_->show();
            clockTopItem_->hide();
            clockBottomItem_->hide();
            clearOutOfHandCardsOnRightTable();
        }
    }


}

void PokerGame::playCards()
{
//    for(int i = 0; i < cardInMyHands.length(); i++)
//    {
//        scene_->removeItem(cardInMyHands.at(i));
//        delete cardInMyHands.at(i);
//    }
    if(gameStatus_ == GameStatus(playerId_))
    {
        QList<int> playCardList;
        QList<PokerCardItem*> list;

        for(int i = 0; i < cardInHandsBottom_.length(); i++)
        {
            if(cardInHandsBottom_.at(i)->isClicked())
            {
                playCardList.append(cardInHandsBottom_.at(i)->cardID());
                list.append(cardInHandsBottom_.at(i));
            }
        }
        CardModel model = PokerCardModel::checkModel(list);
        if(model == Illegal)
        {
            return;
        }
        if(!(lastCardOwner_ == -1 || lastCardOwner_ == playerId_))
        {
            if(!PokerCardModel::greaterThan(list, model, lastCardList_, lastCardModel_))
                return;

        }


        QString info = QString("2,%1").arg(playerId_);
        info.append(QString(",%1").arg(playCardList.length()));
        for(int i = 0; i < list.length(); i++)
        {
            info.append(QString(",%1").arg(list.at(i)->cardID()));
        }
        int length = 0;
        info.append("\n");
        if((length=tcpSocket_->write(info.toLatin1(),info.length()))!=info.length())
        {
            return;
        }
    }
}

void PokerGame::passRound()
{
    if(gameStatus_ == GameStatus(playerId_))
    {

        for(int i = 0; i < cardInHandsBottom_.length(); i++)
        {
            cardInHandsBottom_.at(i)->setClicked(false);

        }
        arrangeBottomInHandCards();

        QString info = QString("3,%1,0\n").arg(playerId_);

        int length = 0;

        if((length=tcpSocket_->write(info.toLatin1(),info.length()))!=info.length())
        {
            return;
        }
    }
}

void PokerGame::addInitialCardsToScene()
{
    if(cardInHandsBottom_.length() > 0)
    {
        for(int i = 0; i < cardInHandsBottom_.length(); i++)
            scene_->addItem(cardInHandsBottom_.at(i));
    }
    arrangeBottomInHandCards();
    for(int i = 0; i < 13; i++)
    {
        PokerCardItem* badCard = new PokerCardItem;
        badCard->setCardID(54);
        badCard->loadImageByRenderer(backRenderer_);
        badCard->setScale(0.45);
        cardInHandsTop_.append(badCard);
        badCard->setClickable(false);
        scene_->addItem(badCard);
    }
    arrangeTopInHandCards();

    for(int i = 0; i < 13; i++)
    {
        PokerCardItem* badCard = new PokerCardItem;
        badCard->loadImageByRenderer(backRenderer_);
        badCard->setCardID(54);
        badCard->setScale(0.45);
        badCard->setRotation(90);
        badCard->setClickable(false);
        cardInHandsLeft_.append(badCard);
        scene_->addItem(badCard);
    }
    arrangeLeftInHandCards();

    for(int i = 0; i < 13; i++)
    {
        PokerCardItem* badCard = new PokerCardItem;
        badCard->setCardID(54);
        badCard->loadImageByRenderer(backRenderer_);
        badCard->setScale(0.45);
        badCard->setRotation(-90);
        //badCard->item()->setPos(300, -200 + 15 * i + badCard->item()->sceneBoundingRect().height());
        cardInHandsRight_.append(badCard);
        badCard->setClickable(false);
        scene_->addItem(badCard);
    }


    arrangeRightInHandCards();

}

void PokerGame::arrangeTopInHandCards()
{
    if(cardInHandsTop_.length() > 0)
    {
        int curCardNum = cardInHandsTop_.length();
        int step = 15;
        int start = 0 - (step * (curCardNum - 1) + cardInHandsTop_.at(0)->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardInHandsTop_.at(i)->setPos( start + i * step, -320);
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
        cardInHandsLeft_.at(i)->setPos(-300, -200 + step * i);;
        //scene_->addItem(cardTop_.at(i)->item());
    }
}

void PokerGame::arrangeRightInHandCards()
{
    int curCardNum = cardInHandsRight_.length();
    if(curCardNum == 0)
        return;
    int height = cardInHandsRight_.at(0)->sceneBoundingRect().height();
    int step = 15;
    for(int i = 0; i < curCardNum; i++)
    {
        cardInHandsRight_.at(i)->setPos(300, -200 + step * i + height);
        //scene_->addItem(cardTop_.at(i)->item());
    }
}

void PokerGame::arrangeBottomInHandCards()
{
    if(cardInHandsBottom_.length() > 0)
    {
        int curCardNum = cardInHandsBottom_.length();
        int step = 20;
        int start = 0 - (step * (curCardNum - 1) + cardInHandsBottom_.at(0)->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardInHandsBottom_.at(i)->setPos( start + i * 20, 190);
            //scene_->addItem(cardInMyHands.at(i)->item());
        }
    }
}

void PokerGame::arrangeBottomOutOfHandCards()
{

    if(cardOutOfHandsBottom_.length() > 0)
    {
        int curCardNum = cardOutOfHandsBottom_.length();
        int step = 10;
        int start = 0 - (step * (curCardNum - 1) + cardOutOfHandsBottom_.at(0)->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsBottom_.at(i)->setScale(0.35);
            cardOutOfHandsBottom_.at(i)->setPos( start + i * step, 50);
            scene_->addItem(cardOutOfHandsBottom_.at(i));

        }
    }
    else
    {
        passBottomItem_->show();
    }

    if(cardOutOfHandsBottom_.length() > 0)
    {
        CardModel model = PokerCardModel::checkModel(cardOutOfHandsBottom_);
        lastCardModel_ = model;
        lastCardList_ = cardOutOfHandsBottom_;
        lastCardOwner_ = playerId_;
    }

}

void PokerGame::arrangeTopOutOfHandCards()
{
    if(cardOutOfHandsTop_.length() > 0)
    {
        int curCardNum = cardOutOfHandsTop_.length();
        int step = 10;
        cardOutOfHandsTop_.at(0)->setScale(0.35);
        int start = 0 - (step * (curCardNum - 1) + cardOutOfHandsTop_.at(0)->sceneBoundingRect().width()) / 2;
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsTop_.at(i)->setScale(0.35);
            cardOutOfHandsTop_.at(i)->setPos( start + i * step, -200);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
    else
    {
        passTopItem_->show();
    }
}

void PokerGame::arrangeLeftOutOfHandCards()
{
    if(cardOutOfHandsLeft_.length() > 0)
    {
        int curCardNum = cardOutOfHandsLeft_.length();
        int step = 10;
        int start = -280;
        cardOutOfHandsLeft_.at(0)->setScale(0.35);
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsLeft_.at(i)->setScale(0.35);
            cardOutOfHandsLeft_.at(i)->setPos( i * step + start, -90);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
    else
        passLeftItem_->show();
}

void PokerGame::arrangeRightOutOfHandCards()
{
    if(cardOutOfHandsRight_.length() > 0)
    {
        int curCardNum = cardOutOfHandsRight_.length();
        int step = 10;

        int start = 280 - (step * (curCardNum - 1) + cardOutOfHandsRight_.at(0)->sceneBoundingRect().width()) ;
        for(int i = 0; i < curCardNum; i++)
        {
            cardOutOfHandsRight_.at(i)->setScale(0.35);
            cardOutOfHandsRight_.at(i)->setPos( start + i * step , -90);

            //scene_->addItem(newCardsOnDeck.at(i)->item());
        }
    }
    else
        passRightItem_->show();
}

void PokerGame::createActions()
{
    actConnectServer_ = new QAction("Connect Server", this);
    actEndGame_ = new QAction("Clear Table", this);
    connect(actConnectServer_, SIGNAL(triggered()), this, SLOT(slotConnectServer()));
    connect(actEndGame_, SIGNAL(triggered()), this, SLOT(slotEndGame()));

}

void PokerGame::loadImages()
{
    QString strName = QString(":/images/play.svg");
    playImageItem_ = new QGraphicsSvgItem();
    QSvgRenderer *playrenderer = new QSvgRenderer(strName);
    playImageItem_->setSharedRenderer(playrenderer);
    playImageItem_->setScale(0.10);
    playImageItem_->setPos(-70-playImageItem_->sceneBoundingRect().width(), 135);
    playImageItem_->hide();
    QVariant playID("Play");
    playImageItem_->setData(0, playID);
    scene_->addItem(playImageItem_);

    strName = QString(":/images/pass.svg");
    passImageItem_ = new QGraphicsSvgItem();
    QSvgRenderer *passrenderer = new QSvgRenderer(strName);
    passImageItem_->setSharedRenderer(passrenderer);
    passImageItem_->setScale(0.10);
    passImageItem_->setPos(70, 135);
    passImageItem_->hide();
    QVariant passID("Pass");
    passImageItem_->setData(0, passID);
    scene_->addItem(passImageItem_);



    strName = QString(":/images/pass2.svg");
    QSvgRenderer *passIconRenderer = new QSvgRenderer(strName);

    passLeftItem_ = new QGraphicsSvgItem;
    passLeftItem_->setSharedRenderer(passIconRenderer);
    passLeftItem_->setScale(1.2);
    int passWidth = passLeftItem_->sceneBoundingRect().width();
    int passHeight = passLeftItem_->sceneBoundingRect().height();

    passRightItem_ = new QGraphicsSvgItem;
    passRightItem_->setSharedRenderer(passIconRenderer);
    passRightItem_->setScale(1.2);

    passTopItem_ = new QGraphicsSvgItem;
    passTopItem_->setSharedRenderer(passIconRenderer);
    passTopItem_->setScale(1.2);

    passBottomItem_ = new QGraphicsSvgItem;
    passBottomItem_->setSharedRenderer(passIconRenderer);
    passBottomItem_->setScale(1.2);

    passLeftItem_->setPos(-200, - passHeight/2);
    passRightItem_->setPos(200 - passWidth, - passHeight/2);

    passTopItem_->setPos(-passWidth/2, -120);
    passBottomItem_->setPos(-passWidth/2, 120 - passHeight);

    passLeftItem_->hide();
    passRightItem_->hide();
    passTopItem_->hide();
    passBottomItem_->hide();

    scene_->addItem(passLeftItem_);
    scene_->addItem(passRightItem_);
    scene_->addItem(passTopItem_);
    scene_->addItem(passBottomItem_);



    strName = QString(":/images/clock.svg");
    QSvgRenderer *clockIconRenderer = new QSvgRenderer(strName);

    clockLeftItem_ = new QGraphicsSvgItem;
    clockLeftItem_->setSharedRenderer(clockIconRenderer);
    clockLeftItem_->setScale(0.1);
    int clockWidth = clockLeftItem_->sceneBoundingRect().width();
    int clockHeight = clockLeftItem_->sceneBoundingRect().height();

    clockRightItem_ = new QGraphicsSvgItem;
    clockRightItem_->setSharedRenderer(clockIconRenderer);
    clockRightItem_->setScale(0.1);

    clockTopItem_ = new QGraphicsSvgItem;
    clockTopItem_->setSharedRenderer(clockIconRenderer);
    clockTopItem_->setScale(0.1);

    clockBottomItem_ = new QGraphicsSvgItem;
    clockBottomItem_->setSharedRenderer(clockIconRenderer);
    clockBottomItem_->setScale(0.1);

    clockLeftItem_->setPos(-200, - clockHeight/2);
    clockRightItem_->setPos(200 - clockWidth, - clockHeight/2);

    clockTopItem_->setPos(-clockWidth/2, -120);
    clockBottomItem_->setPos(-clockWidth/2, 120 - clockHeight);

    clockLeftItem_->hide();
    clockRightItem_->hide();
    clockTopItem_->hide();
    clockBottomItem_->hide();

    scene_->addItem(clockLeftItem_);
    scene_->addItem(clockRightItem_);
    scene_->addItem(clockTopItem_);
    scene_->addItem(clockBottomItem_);



}

void PokerGame::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("Operation");
    fileMenu->addAction(actConnectServer_);
    fileMenu->addAction(actEndGame_);
}
