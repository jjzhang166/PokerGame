#ifndef POKERSCENE_H
#define POKERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PokerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PokerScene(QObject *parent = 0);

signals:

public slots:

protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
};

#endif // POKERSCENE_H
