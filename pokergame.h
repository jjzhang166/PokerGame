#ifndef POKERGAME_H
#define POKERGAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include "pokerscene.h"
#include <QAction>
#include <QList>

#include "pokercarditem.h"
#include <QTcpSocket>
#include <QListWidget>
#include "pokercardmodel.h"
#include "settingstab.h"

class PokerGame : public QMainWindow
{
    Q_OBJECT
    enum GameStatus {BeforeGame = -2, ConnectedAndWait, WaitPlayer0 = 0, WaitPlayer1 , WaitPlayer2, WaitPlayer3, OtherWin, SelfWin};
public:
    PokerGame(QWidget *parent = 0);
    ~PokerGame();
public slots:
    void slotConnectServer();
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();
    void slotSocketError(QAbstractSocket::SocketError socketError);

private:
    //static PokerCard *backCard_;

    QGraphicsView *view_;
    PokerScene *scene_;
    QListWidget *infoList_;

    SettingsTab *ui_tab;

    QAction *actConnectServer_;
    QList<PokerCardItem*> cardInHandsBottom_;
    QList<PokerCardItem*> cardInHandsTop_;
    QList<PokerCardItem*> cardInHandsLeft_;
    QList<PokerCardItem*> cardInHandsRight_;
    QList<PokerCardItem*>* cardListArray_[4];

    QList<PokerCardItem*> cardOutOfHandsBottom_;
    QList<PokerCardItem*> cardOutOfHandsTop_;
    QList<PokerCardItem*> cardOutOfHandsLeft_;
    QList<PokerCardItem*> cardOutOfHandsRight_;

    QList<PokerCardItem*> lastCardList_;
    int lastCardOwner_;
    CardModel lastCardModel_;

    QTcpSocket *tcpSocket_;
    GameStatus gameStatus_;
    QSvgRenderer *backRenderer_;
    int playerId_;

    QGraphicsSvgItem *playImageItem_;
    QGraphicsSvgItem *passImageItem_;

    QGraphicsSvgItem *clockLeftItem_;
    QGraphicsSvgItem *clockRightItem_;
    QGraphicsSvgItem *clockTopItem_;
    QGraphicsSvgItem *clockBottomItem_;


    QGraphicsSvgItem *passLeftItem_;
    QGraphicsSvgItem *passRightItem_;
    QGraphicsSvgItem *passTopItem_;
    QGraphicsSvgItem *passBottomItem_;


private:
    void createMenus();
    void createActions();
    void loadImages();
    void processData(QString msg);
    void receiveInitialCards(QStringList &strlist);
    void addInitialCardsToScene();

    void arrangeTopInHandCards();
    void arrangeLeftInHandCards();
    void arrangeRightInHandCards();
    void arrangeBottomInHandCards();

    void arrangeBottomOutOfHandCards();
    void arrangeTopOutOfHandCards();
    void arrangeLeftOutOfHandCards();
    void arrangeRightOutOfHandCards();

    void doSomething();

    void receiveCardsFromPlayers(QStringList &strlist);

    void bottomPlayerPlayCards(QStringList &strlist);
    void leftPlayerPlayCards(QStringList &strlist);
    void topPlayerPlayCards(QStringList &strlist);
    void rightPlayerPlayCards(QStringList &strlist);

    void clearTable();
    void clearBottomTable();
    void clearLeftTable();
    void clearRightTable();
    void clearTopTable();

private slots:
    void playCards();
    void passRound();
};

#endif // POKERGAME_H
