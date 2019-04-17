#include "qmlvector.h"

#include "coreengine/mainapp.h"

QmlVectorPoint::QmlVectorPoint()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

QmlVectorUnit::QmlVectorUnit()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void QmlVectorUnit::randomize()
{
    QVector<Unit*> randVector;
    while (m_Vector.size() > 0)
    {
        qint32 value = Mainapp::randInt(0, m_Vector.size() - 1);
        randVector.push_back(m_Vector[value]);
        m_Vector.removeAt(value);
    }
    m_Vector.swap(randVector);
}

QmlVectorBuilding::QmlVectorBuilding()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void QmlVectorBuilding::randomize()
{
    QVector<Building*> randVector;
    while (m_Vector.size() > 0)
    {
        qint32 value = Mainapp::randInt(0, m_Vector.size() - 1);
        randVector.push_back(m_Vector[value]);
        m_Vector.removeAt(value);
    }
    m_Vector.swap(randVector);
}
