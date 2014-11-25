#ifndef POKERGAME_H
#define POKERGAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include "pokerscene.h"
#include <QAction>
#include <vector>
#include "pokercard.h"
#include <QTcpSocket>
#include <QListWidget>


class PokerGame : public QMainWindow
{
    Q_OBJECT

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
    std::vector<PokerCard *> pokerVec_;

    QTcpSocket *tcpSocket_;

private:
    void createMenus();
    void createActions();
    void processData(QString msg);
};

#endif // POKERGAME_H
