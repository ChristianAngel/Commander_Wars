#include "humanplayerinputmenu.h"

#include "game/gamemap.h"

#include "game/unit.h"
#include "wiki/fieldinfo.h"

#include "menue/gamemenue.h"
#include "objects/cursor.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "resource_management/gamemanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/unitspritemanager.h"

#include "wiki/wikidatabase.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spActor> icons,
                                           QVector<qint32> costList, QVector<bool> enabledList)
    : m_ActionIDs(actionIDs),
      m_CostList(costList),
      m_EnabledList(enabledList)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    connect(pApp, &Mainapp::sigKeyDown, this, &HumanPlayerInputMenu::keyInput, Qt::QueuedConnection);
    qint32 width = 0;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    style.fontSize = 20;
    oxygine::spTextField testText = new oxygine::TextField();
    for (qint32 i = 0; i < texts.size(); i++)
    {

        testText->setStyle(style);
        testText->setHtmlText(texts[i]);
        qint32 newWidth = testText->getTextRect().getWidth() * static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize);
        if (newWidth > width )
        {
            width = newWidth;
        }
    }
    width += GameMap::getImageSize() + GameMap::getImageSize() * 3 / 4 ;
    itemWidth = width;
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    qint32 heigth = createTopSprite(0, width);

    qint32 y = heigth;
    startY = y;
    m_Cursor = new oxygine::Sprite();
    pAnim = pGameManager->getResAnim("cursor+menu");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
        m_Cursor->addTween(tween);
    }
    else
    {
        m_Cursor->setResAnim(pAnim);
    }
    m_Cursor->setPosition(width - m_Cursor->getScaledWidth() / 2, y + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
    m_Cursor->setScale(GameMap::getImageSize() / pAnim->getWidth());
    qint32 x = 0;

    qint32 xCount = 0;

    qint32 maxY = 0;
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        if (i > 0 && i % Settings::getMenuItemCount() == 0)
        {
            xCount++;
            createBottomSprite(x, y, width);
            x += width;
            maxY = y;
            y = startY;
            createTopSprite(x, width);
        }
        bool enabled = true;
        if (enabledList.size() > 0)
        {
            enabled = enabledList[i];
        }
        qint32 costs = 0;
        if (costList.size() > 0)
        {
            costs = costList[i];
        }

        oxygine::spBox9Sprite pItemBox = new oxygine::Box9Sprite();
        pAnim = pGameManager->getResAnim("menu+middle");
        pItemBox->setResAnim(pAnim);
        pItemBox->setSize(pAnim->getSize());
        pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pItemBox->addChild(icons[i]);
        icons[i]->setPosition(3, 0);
        pItemBox->setHeight(GameMap::getImageSize());

        pItemBox->setY(y);
        pItemBox->setX(x);
        pItemBox->setWidth(width);

        // text for the item
        oxygine::spTextField textField = new oxygine::TextField();

        // set color font based
        if (!enabled)
        {
            style.color = QColor(255, 0, 0, 255);
        }
        else
        {
            style.color = FontManager::getFontColor();
        }
        textField->setStyle(style);
        textField->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize));
        textField->setHtmlText(texts[i]);
        textField->attachTo(pItemBox);
        textField->setPosition(3 + GameMap::getImageSize(), 0);
        textField->setSize(width - textField->getX(), GameMap::getImageSize() - 4);

        this->addChild(pItemBox);
        pItemBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *pEvent)->void
        {
            Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
            pEvent->stopPropagation();
            m_Cursor->setY(y + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
            m_Cursor->setX(x + width);
            currentAction = i;
        });
        QString action = actionIDs[i];
        if (enabled)
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                pTouchEvent->stopPropagation();
                if (pTouchEvent != nullptr)
                {
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("okay.wav");
                        emit sigItemSelected(action, costs);
                    }
                    else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                    {                        emit sigCanceled(0, 0);
                    }
                }

            });
        }
        else
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                pTouchEvent->stopPropagation();
                if (pTouchEvent != nullptr)
                {
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                    {
                        emit sigCanceled(0, 0);
                    }
                }
            });
        }
        y += static_cast<qint32>(pItemBox->getHeight());
        if (y > maxY)
        {
            maxY = y;
        }
        itemHeigth = static_cast<qint32>(pItemBox->getHeight());
    }
    if (xCount > 0)
    {
        qint32 emptyItems = ((xCount + 1) * Settings::getMenuItemCount()) - xCount * Settings::getMenuItemCount() - actionIDs.size() % Settings::getMenuItemCount();
        if (emptyItems < Settings::getMenuItemCount())
        {
            for (qint32 i = 0; i < emptyItems; i++)
            {
                oxygine::spBox9Sprite pItemBox = new oxygine::Box9Sprite();
                pAnim = pGameManager->getResAnim("menu+middle");
                pItemBox->setResAnim(pAnim);
                pItemBox->setSize(pAnim->getSize());
                pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                pItemBox->setHeight(GameMap::getImageSize());
                pItemBox->setY(y);
                pItemBox->setX(x);
                pItemBox->setWidth(width);
                pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
                {
                    oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                    pTouchEvent->stopPropagation();
                    if (pTouchEvent != nullptr)
                    {
                        if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                        {
                            emit sigCanceled(0, 0);
                        }
                    }
                });
                this->addChild(pItemBox);
                y += static_cast<qint32>(pItemBox->getHeight());
                itemHeigth = static_cast<qint32>(pItemBox->getHeight());
            }
        }
    }

    qint32 bottomHeigth = createBottomSprite(x, y, width);
    this->addChild(m_Cursor);
    this->setPriority(static_cast<qint16>(Mainapp::ZOrder::FocusedObjects));
    this->setHeight(maxY + bottomHeigth);
    this->setWidth(width * Mainapp::roundUp((actionIDs.size() / static_cast<float>(Settings::getMenuItemCount()))));
}

qint32 HumanPlayerInputMenu::createBottomSprite(qint32 x, qint32 y, qint32 width)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    oxygine::spBox9Sprite pBottomBox = new oxygine::Box9Sprite();
    pAnim = pGameManager->getResAnim("menu+bottom");
    pBottomBox->setResAnim(pAnim);
    pBottomBox->setSize(pAnim->getSize());
    pBottomBox->setWidth(width);
    pBottomBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setY(y);
    pBottomBox->setX(x);
    this->addChild(pBottomBox);
    return static_cast<qint32>(pBottomBox->getHeight());
}

qint32 HumanPlayerInputMenu::createTopSprite(qint32 x, qint32 width)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    pTopBox->setResAnim(pAnim);
    pTopBox->setSize(pAnim->getSize());
    pTopBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setWidth(width);
    pTopBox->setX(x);
    this->addChild(pTopBox);
    return static_cast<qint32>(pTopBox->getHeight());
}

void HumanPlayerInputMenu::leftClick(qint32, qint32)
{
    emit sigItemSelected(m_ActionIDs[currentAction], m_CostList[currentAction]);
}

void HumanPlayerInputMenu::setMenuPosition(qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();

    if (x + getWidth() + GameMap::getImageSize() / 2 > pMap->getMapWidth() * GameMap::getImageSize())
    {
        x = pMap->getMapWidth() * GameMap::getImageSize() - getWidth() - GameMap::getImageSize() / 2;
        if (x < 0)
        {
            x = 0;
        }
    }
    if (y + getHeight() + GameMap::getImageSize() / 2 > pMap->getMapHeight() * GameMap::getImageSize())
    {
        y = pMap->getMapHeight() * GameMap::getImageSize() - getHeight() - GameMap::getImageSize() / 2;
        if (y < 0)
        {
            y = 0;
        }
    }
    this->setPosition(x, y);
    moveMouseToItem(0, 0);
}

void HumanPlayerInputMenu::keyInput(oxygine::KeyEvent event)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        Chat* pChat = pMenu->getChat();
        if (m_Focused && (pChat == nullptr || pChat->getVisible() == false))
        {
            // for debugging
            Qt::Key cur = event.getKey();
            if (cur == Settings::getKey_up())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (currentAction > 0)
                {
                    currentAction--;
                }
                else
                {
                    currentAction = m_ActionIDs.size() - 1;
                }
            }
            else if (cur == Settings::getKey_down())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (currentAction < m_ActionIDs.size() - 1)
                {
                    currentAction++;
                }
                else
                {
                    currentAction = 0;
                }
            }
            if (cur == Settings::getKey_left())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (currentAction - Settings::getMenuItemCount() >= 0)
                {
                    currentAction -= Settings::getMenuItemCount();
                }
                else
                {
                    currentAction = m_ActionIDs.size() - 1;
                }
            }
            else if (cur == Settings::getKey_right())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (currentAction + Settings::getMenuItemCount() < m_ActionIDs.size())
                {

                    currentAction += Settings::getMenuItemCount();
                }
                else
                {
                    currentAction = 0;
                }
            }
            else if (cur == Settings::getKey_confirm())
            {
                if (m_ActionIDs.size() > 0)
                {
                    if ((m_EnabledList.size() > 0 && m_EnabledList[currentAction]) ||
                        (m_EnabledList.size() == 0))
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
                        if (m_CostList.size() == m_ActionIDs.size())
                        {
                            emit sigItemSelected(m_ActionIDs[currentAction], m_CostList[currentAction]);
                        }
                        else
                        {
                            emit sigItemSelected(m_ActionIDs[currentAction], 0);
                        }
                    }
                }
            }
            else if (cur == Settings::getKey_cancel())
            {
                emit sigCanceled(0, 0);
            }
            else if (cur == Settings::getKey_information())
            {
                Mainapp* pApp = Mainapp::getInstance();
                pApp->suspendThread();
                QString id = m_ActionIDs[currentAction];
                UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
                if (pUnitSpriteManager->exists(id))
                {
                    spUnit pDummy = new Unit(id, GameMap::getInstance()->getCurrentPlayer(), false);
                    spFieldInfo fieldinfo = new FieldInfo(nullptr, pDummy.get());
                    pMenu->addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, [=]
                    {
                        m_Focused = true;
                    });
                    m_Focused = false;
                }
                else
                {
                    WikiDatabase* pDatabase = WikiDatabase::getInstance();
                    WikiDatabase::pageData data = pDatabase->getEntry(id);
                    if (std::get<1>(data) != "")
                    {
                        spWikipage page = pDatabase->getPage(data);
                        pMenu->addChild(page);
                        connect(page.get(), &FieldInfo::sigFinished, [=]
                        {
                            m_Focused = true;
                        });
                        m_Focused = false;
                    }
                }
                pApp->continueThread();
            }

            qint32 x = 0;
            qint32 y = currentAction;
            while (y >= Settings::getMenuItemCount())
            {
                y -= Settings::getMenuItemCount();
                x++;
            }
            m_Cursor->setY(startY + y * itemHeigth + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
            m_Cursor->setX(itemWidth * (x + 1));
        }
    }
}

void HumanPlayerInputMenu::moveMouseToItem(qint32 x, qint32 y)
{
    oxygine::Vector2 pos = local2stage();
    Mainapp* pApp = Mainapp::getInstance();
    QPoint curPos = pApp->mapToGlobal(QPoint(pos.x + itemWidth / 2 + itemWidth * x, pos.y + startY + itemHeigth / 2 + itemHeigth * y));
    pApp->cursor().setPos(curPos);
}
