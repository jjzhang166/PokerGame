#ifndef POKERGAME_H
#define POKERGAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include "pokerscene.h"
#include <QAction>
#include <QList>
#include "pokercard.h"
#include <QTcpSocket>
#include <QListWidget>


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

private:
    static PokerCard *backCard_;

    QGraphicsView *view_;
    PokerScene *scene_;
    QListWidget *infoList_;

    QAction *actConnectServer_;
    QList<PokerCard*> cardInMyHands;
    QList<PokerCard*> cardInHandsTop_;
    QList<PokerCard*> cardInHandsLeft_;
    QList<PokerCard*> cardInHandsRight_;
    QList<PokerCard*>* cardListArray_[4];

    QList<PokerCard*> cardOutOfMyHands_;
    QList<PokerCard*> cardOutOfHandsTop_;
    QList<PokerCard*> cardOutOfHandsLeft_;
    QList<PokerCard*> cardOutOfHandsRight_;



    QTcpSocket *tcpSocket_;
    GameStatus gameStatus_;
    QSvgRenderer *backRenderer_;
    int playerId_;

    QGraphicsSvgItem *playImageItem_;
    QGraphicsSvgItem *passImageItem_;

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
    void arrangeMyInHandCards();

    void arrangeMyOutOfHandCards(QList<PokerCard *> &newCardsOnDeck);
    void arrangeTopOutOfHandCards();
    void arrangeLeftOutOfHandCards();
    void arrangeRightOutOfHandCards();

    void doSomething();

    void receiveCardsFromPlayers(QStringList &strlist);

    void leftPlayerPlayCards(QStringList &strlist);
    void topPlayerPlayCards(QStringList &strlist);
    void rightPlayerPlayCards(QStringList &strlist);
private slots:
    void playCards();
};

#endif // POKERGAME_H
