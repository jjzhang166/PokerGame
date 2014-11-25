#include "pokergame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PokerGame w;
    w.show();

    return a.exec();
}
