#ifndef INGAMEMENUE_H
#define INGAMEMENUE_H

#include <QObject>
#include <QPoint>
#include <qrect.h>
#include <QTimer>
#include <QThread>

#include "oxygine-framework.h"

#include "objects/cursor.h"

#include "oxygine/KeyEvent.h"

#include "gameinput/mapmover.h"

class InGameMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit InGameMenue();
    explicit InGameMenue(qint32 width, qint32 heigth, QString map = "");
    virtual ~InGameMenue();

    Cursor* getCursor();
    bool getFocused() const;
    void setFocused(bool Focused);
    void calcNewMousePosition(qint32 x, qint32 y);
    QPoint getMousePos(qint32 x, qint32 y);
    void MoveMap(qint32 x, qint32 y);
    virtual void autoScroll();
signals:
    void sigMouseWheel(qint32 direction);
    void sigMoveMap(qint32 x, qint32 y);
    void sigRightClick(qint32 x, qint32 y);
    void sigRightClickDown(qint32 x, qint32 y);
    void sigRightClickUp(qint32 x, qint32 y);
    void sigLeftClick(qint32 x, qint32 y);
    void sigLeftClickDown(qint32 x, qint32 y);
    void sigLeftClickUp(qint32 x, qint32 y);
    void sigMouseMove(qint32 x, qint32 y);
public slots:
    virtual void keyInput(oxygine::KeyEvent event);
    virtual void keyUp(oxygine::KeyEvent event);
    void centerMapOnCursor();
protected:
    void loadBackground();
    void loadHandling();
    void connectMapCursor();
    bool m_moveMap{false};
    bool m_Focused{true};
    QPoint m_MoveMapMousePoint;

    spCursor m_Cursor{new Cursor()};
    QRect autoScrollBorder{300, 50, 300, 50};

    QThread m_MapMoveThread;
    spMapMover m_MapMover;

    bool handlingLoaded{false};
};

#endif // INGAMEMENUE_H
