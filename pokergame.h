#ifndef POKERGAME_H
#define POKERGAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include "pokerscene.h"
#include <QAction>
#include <vector>
#include "pokercard.h"
#include <QTcpSocket>


class PokerGame : public QMainWindow
{
    Q_OBJECT

public:
    PokerGame(QWidget *parent = 0);
    ~PokerGame();

private:
    static PokerCard *backCard_;

    QGraphicsView *view_;
    PokerScene *scene_;

    QAction *actStartGame_;
    std::vector<PokerCard *> pokerVec_;

    QTcpSocket *tcpSocket_;

private:
    void createMenus();
    void createActions();
};

#endif // POKERGAME_H
