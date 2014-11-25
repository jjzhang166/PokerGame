#ifndef POKERGAME_H
#define POKERGAME_H

#include <QMainWindow>

class PokerGame : public QMainWindow
{
    Q_OBJECT

public:
    PokerGame(QWidget *parent = 0);
    ~PokerGame();
};

#endif // POKERGAME_H
