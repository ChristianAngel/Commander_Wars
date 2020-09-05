#include "cursor.h"

#include "resource_management/objectmanager.h"
#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "game/gamemap.h"

Cursor::Cursor()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    changeCursor("cursor+default");
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Cursor));
    connect(this, &Cursor::sigUpdatePosition, this, &Cursor::updatePosition, Qt::QueuedConnection);
}

void Cursor::changeCursor(QString spriteID, qint32 xOffset, qint32 yOffset, float scale)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (m_CurrentCursor.get() != nullptr)
    {
        m_CurrentCursor->detach();
        m_CurrentCursor = nullptr;
    }
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteID);
    m_CurrentCursor = new oxygine::Sprite();
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * 200), -1);
        m_CurrentCursor->addTween(tween);
    }
    else
    {
        m_CurrentCursor->setResAnim(pAnim);
    }
    m_CurrentCursor->setScale(scale * static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize));
    m_CurrentCursor->setPosition(xOffset, yOffset);
    this->addChild(m_CurrentCursor);
    pApp->continueThread();
}

void Cursor::updatePosition(qint32 mousePosX, qint32 mousePosY)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 x = (mousePosX - pMap->getPosition().x) / (GameMap::getImageSize() * pMap->getZoom());
        qint32 y = (mousePosY - pMap->getPosition().y) / (GameMap::getImageSize() * pMap->getZoom());
        onMap = pMap->onMap(x, y);
        if (onMap)
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            // play tick sound when changing the field
            if ((x != m_MapPointX) ||
                (y != m_MapPointY))
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchfield.wav");
            }

            m_MapPointX = x;
            m_MapPointY = y;
            this->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            // provide cursor move signal
            emit sigCursorMoved(m_MapPointX, m_MapPointY);
            pApp->continueThread();
        }
    }
}

void Cursor::addCursorRangeOutline(qint32 range, QColor color)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (qint32 i = 0; i <= range; i++)
    {
        if (i == 0)
        {
            createOuterLeftRightOutline(range, color);
        }
        else if (i == range)
        {
            createOuterTopBottomOutline(range, color);
        }
        else
        {
            createOutline(i, range, color);
        }
    }
    pApp->continueThread();
}

void Cursor::createOuterLeftRightOutline(qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim("cursor+border+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim("cursor+border+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim("cursor+border+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim("cursor+border+right");

    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimLeft);
    pSprite->setX(-range * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimTop);
    pSprite->setX(-range * GameMap::getImageSize());
    pSprite->setY(-2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBottom);
    pSprite->setX(-range * GameMap::getImageSize());
    pSprite->setY(2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimRight);
    pSprite->setX(range * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimTop);
    pSprite->setX(range * GameMap::getImageSize());
    pSprite->setY(-2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBottom);
    pSprite->setX(range * GameMap::getImageSize());
    pSprite->setY(2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);
}

void Cursor::createOuterTopBottomOutline(qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim("cursor+border+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim("cursor+border+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim("cursor+border+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim("cursor+border+right");

    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimTop);
    pSprite->setY(-range * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimLeft);
    pSprite->setX(-2);
    pSprite->setY(-range * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimRight);
    pSprite->setY(-range * GameMap::getImageSize());
    pSprite->setX(2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBottom);
    pSprite->setY(range * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimLeft);
    pSprite->setX(-2);
    pSprite->setY(range * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimRight);
    pSprite->setY(range * GameMap::getImageSize());
    pSprite->setX(2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);
}

void Cursor::createOutline(qint32 i, qint32 range, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnimBottom = pObjectManager->getResAnim("cursor+border+bottom");
    oxygine::ResAnim* pAnimTop = pObjectManager->getResAnim("cursor+border+top");
    oxygine::ResAnim* pAnimLeft = pObjectManager->getResAnim("cursor+border+left");
    oxygine::ResAnim* pAnimRight = pObjectManager->getResAnim("cursor+border+right");

    // left top
    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimLeft);
    pSprite->setX(-(range - i) * GameMap::getImageSize() - 2);
    pSprite->setY(-i * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimTop);
    pSprite->setX(-(range - i) * GameMap::getImageSize());
    pSprite->setY(-i * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    // left bottom
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimLeft);
    pSprite->setX(-(range - i) * GameMap::getImageSize() - 2);
    pSprite->setY(i * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBottom);
    pSprite->setX(-(range - i) * GameMap::getImageSize());
    pSprite->setY((i) * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    // right top
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimRight);
    pSprite->setX((range - i) * GameMap::getImageSize() + 2);
    pSprite->setY(-i * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimTop);
    pSprite->setX((range - i) * GameMap::getImageSize());
    pSprite->setY(-i * GameMap::getImageSize() - 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    // right bottom
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimRight);
    pSprite->setX((range - i) * GameMap::getImageSize() + 2);
    pSprite->setY(i * GameMap::getImageSize());
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);

    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBottom);
    pSprite->setX((range - i) * GameMap::getImageSize());
    pSprite->setY((i) * GameMap::getImageSize() + 2);
    pSprite->setColor(color);
    addChild(pSprite);
    m_cursorRangeOutline.append(pSprite);
}

void Cursor::resetCursorRangeOutline()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (auto & outline : m_cursorRangeOutline)
    {
        outline->detach();
    }
    m_cursorRangeOutline.clear();
    pApp->continueThread();
}
