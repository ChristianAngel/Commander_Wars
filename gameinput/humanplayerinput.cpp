#include "humanplayerinput.h"

#include <QSound>

#include "game/gamemap.h"

#include "game/gameaction.h"

#include "game/terrain.h"

#include "game/unitpathfindingsystem.h"

#include "game/building.h"

#include "game/player.h"

#include "game/gameanimationfactory.h"

#include "resource_management/gamemanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"

#include "coreengine/mainapp.h"

#include "coreengine/interpreter.h"

HumanPlayerInput::HumanPlayerInput(GameMenue* pMenue)
{
    connect(pMenue, SIGNAL(sigRightClick(qint32,qint32)), this, SLOT(rightClick(qint32,qint32)), Qt::QueuedConnection);
    connect(pMenue, SIGNAL(sigLeftClick(qint32,qint32)), this, SLOT(leftClick(qint32,qint32)), Qt::QueuedConnection);
    connect(pMenue->getCursor(), SIGNAL(sigCursorMoved(qint32,qint32)), this, SLOT(cursorMoved(qint32,qint32)), Qt::QueuedConnection);
    connect(this, SIGNAL(performAction(GameAction*)), pMenue, SLOT(performAction(GameAction*)), Qt::QueuedConnection);
}

HumanPlayerInput::~HumanPlayerInput()
{
    delete m_pGameAction;
    m_pGameAction = nullptr;
    delete m_pUnitPathFindingSystem;
    m_pUnitPathFindingSystem = nullptr;
}


void HumanPlayerInput::rightClick(qint32 x, qint32 y)
{
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    else if (m_pGameAction != nullptr)
    {
        if (m_CurrentMenu.get() == nullptr)
        {
            cleanUpInput();
        }
        else
        {
            Unit* pUnit = m_pGameAction->getTargetUnit();
            if (pUnit != nullptr)
            {
                // go one step back :)
                m_CurrentMenu->getParent()->removeChild(m_CurrentMenu);
                m_CurrentMenu = nullptr;
            }
            else
            {
                cleanUpInput();
            }
        }
    }
}

void HumanPlayerInput::cleanUpInput()
{
    if (m_CurrentMenu.get() != nullptr)
    {
        m_CurrentMenu->getParent()->removeChild(m_CurrentMenu);
        m_CurrentMenu = nullptr;
    }
    // delete action
    delete m_pGameAction;
    m_pGameAction = nullptr;
    delete m_pUnitPathFindingSystem;
    m_pUnitPathFindingSystem = nullptr;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_Fields.size(); i++)
    {
        pMap->getTerrain(m_FieldPoints[i].x(), m_FieldPoints[i].y())->removeChild(m_Fields[i]);
    }
    m_FieldPoints.clear();
    m_Fields.clear();
    deleteArrow();
}

void HumanPlayerInput::leftClick(qint32 x, qint32 y)
{
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        // do nothing
    }
    // no action selected
    else if (m_pGameAction == nullptr)
    {
        // prepare action
        m_pGameAction = new GameAction();
        m_pGameAction->setTarget(QPoint(x, y));
        GameMap* pMap = GameMap::getInstance();
        Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
        if (pUnit != nullptr)
        {
            selectUnit(x, y);
        }
        else
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            QStringList actions;
            QStringList possibleActions;
            if (pBuilding != nullptr)
            {
                actions = pBuilding->getActionList();
                for (qint32 i = 0; i < actions.size(); i++)
                {
                    if (m_pGameAction->canBePerformed(actions[i]))
                    {
                        possibleActions.append(actions[i]);
                    }
                }
            }
            if (possibleActions.size() > 0)
            {
                if ((possibleActions.size() == 1) &&
                    m_pGameAction->isFinalStep(actions[0]))
                {
                    // skip show select action menu
                    m_pGameAction->setActionID(actions[0]);
                    getNextStepData();
                }
                else
                {
                    if (possibleActions.size() > 0)
                    {
                        createActionMenu(possibleActions, x, y);
                    }
                }
            }
            else
            {
                actions = getEmptyActionList();
                possibleActions.clear();
                for (qint32 i = 0; i < actions.size(); i++)
                {
                    if (m_pGameAction->canBePerformed(actions[i]))
                    {
                        possibleActions.append(actions[i]);
                    }
                }
                if (possibleActions.size() > 0)
                {
                    createActionMenu(possibleActions, x, y);
                }
                else
                {
                    cleanUpInput();
                }
            }
        }

    }
    // we want to select an action
    else if (m_pGameAction->getActionID() == "")
    {
        if ((m_pUnitPathFindingSystem != nullptr) &&
            (m_pUnitPathFindingSystem->isReachable(x, y)) &&
            (m_CurrentMenu.get() == nullptr))
        {
            Unit* pUnit = m_pGameAction->getTargetUnit();
            m_pGameAction->setMovepath(m_ArrowPoints);
            if (pUnit != nullptr)
            {
                // we want to do something with this unit :)
                QStringList actions = pUnit->getActionList();
                QStringList possibleActions;
                for (qint32 i = 0; i < actions.size(); i++)
                {
                    if (m_pGameAction->canBePerformed(actions[i]))
                    {
                        possibleActions.append(actions[i]);
                    }
                }
                if (possibleActions.size() > 0)
                {
                    createActionMenu(possibleActions, x, y);
                }
            }
        }
        else
        {
            cleanUpInput();
        }
    }
    else
    {
        cleanUpInput();
    }
}

void HumanPlayerInput::menuItemSelected(QString itemID, qint32 cost)
{
    // we're currently selecting the action for this action
    if (m_pGameAction->getActionID() == "")
    {
        // set the id
        m_pGameAction->setActionID(itemID);
        m_pGameAction->setCosts(cost);
        // check if the action needs further information
    }
    // we want to append some data to the action
    else
    {
        m_pGameAction->writeDataString(itemID);
        // increase costs and input step
        m_pGameAction->setCosts(m_pGameAction->getCosts() + cost);
        m_pGameAction->setInputStep(m_pGameAction->getInputStep() + 1);
    }
    if (m_pGameAction->isFinalStep())
    {
        // if not perform action
        finishAction();
    }
    else
    {
        // else introduce next step
        getNextStepData();
    }
}

void HumanPlayerInput::getNextStepData()
{
    QString stepType = m_pGameAction->getStepInputType();
    if (stepType.toUpper() == "MENU")
    {
        GameMap* pMap = GameMap::getInstance();
        MenuData* pData = m_pGameAction->getMenuStepData();
        QVector<oxygine::spActor> sprites;
        QStringList actionIDs = pData->getActionIDs();
        GameManager* pGameManager = GameManager::getInstance();
        for (qint32 i = 0; i < actionIDs.size(); i++)
        {
            oxygine::ResAnim* pAnim = pGameManager->getResAnim(GameAction::getActionIcon(actionIDs[i]).toStdString().c_str());
            if (pAnim != nullptr)
            {
                sprites.append(getIconSprite(actionIDs[i]));
            }
            else
            {
                UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
                if (pUnitSpriteManager->existsUnit(actionIDs[i]))
                {
                    sprites.append(new Unit(actionIDs[i], pMap->getSpCurrentPlayer()));
                }
                else
                {
                    // check buildings?
                }
            }
        }
        m_CurrentMenu = new HumanPlayerInputMenu(pData->getTexts(), pData->getActionIDs(), sprites, pData->getCostList(), pData->getEnabledList());
        m_CurrentMenu->setMenuPosition(m_pGameAction->getTarget().x() * GameMap::Imagesize, m_pGameAction->getTarget().y() * GameMap::Imagesize);
        pMap->addChild(m_CurrentMenu);
        connect(m_CurrentMenu.get(), SIGNAL(sigItemSelected(QString, qint32)), this, SLOT(menuItemSelected(QString, qint32)), Qt::QueuedConnection);
        delete pData;
    }
}

void HumanPlayerInput::finishAction()
{
    emit performAction(m_pGameAction);
    m_pGameAction = nullptr;
    cleanUpInput();
}

oxygine::spSprite HumanPlayerInput::getIconSprite(QString itemID)
{
    oxygine::spSprite pSprite = new oxygine::Sprite();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim(GameAction::getActionIcon(itemID).toStdString().c_str());
    if (pAnim != nullptr)
    {
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
    }
    pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
    return pSprite;
}

void HumanPlayerInput::createActionMenu(QStringList actionIDs, qint32 x, qint32 y)
{
    QStringList actionTexts;
    QVector<oxygine::spActor> sprites;

    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        actionTexts.append(GameAction::getActionText(actionIDs[i]));
        sprites.append(getIconSprite(actionIDs[i]));
    }
    m_CurrentMenu = new HumanPlayerInputMenu(actionTexts, actionIDs, sprites);
    m_CurrentMenu->setMenuPosition(x * GameMap::Imagesize, y * GameMap::Imagesize);

    pMap->addChild(m_CurrentMenu);
    connect(m_CurrentMenu.get(), SIGNAL(sigItemSelected(QString, qint32)), this, SLOT(menuItemSelected(QString, qint32)), Qt::QueuedConnection);
}

void HumanPlayerInput::selectUnit(qint32 x, qint32 y)
{
    QSound::play("resources/sounds/selectunit.wav");

    GameMap* pMap = GameMap::getInstance();
    m_pUnitPathFindingSystem = new UnitPathFindingSystem(pMap->getTerrain(x, y)->getSpUnit());
    m_pUnitPathFindingSystem->explore();
    GameManager* pGameManager = GameManager::getInstance();
    QVector<QPoint> points = m_pUnitPathFindingSystem->getAllNodePoints();
    for (qint32 i = 0; i < points.size(); i++)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("marked+field");
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }

        oxygine::Sprite::TweenColor tweenColor(oxygine::Color(50, 230, 200, 255));
        oxygine::spTween tween2 = oxygine::createTween(tweenColor, 1);
        pSprite->addTween(tween2);
        pSprite->setPriority(static_cast<qint8>(Terrain::DrawPriority::MarkedFieldLow));
        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        pMap->getSpTerrain(points[i].x(), points[i].y())->addChild(pSprite);
        m_Fields.append(pSprite);
        m_FieldPoints.append(points[i]);
    }
}

void HumanPlayerInput::cursorMoved(qint32 x, qint32 y)
{
    if (m_pUnitPathFindingSystem != nullptr)
    {
        if ((m_CurrentMenu.get() == nullptr) && m_pGameAction->getActionID() == "")
        {
            createCursorPath(x, y);
        }
    }
}

 void HumanPlayerInput::createCursorPath(qint32 x, qint32 y)
 {
     // create unit moving arrow :)
     deleteArrow();
     QVector<QPoint> points = m_pUnitPathFindingSystem->getPath(x, y);
     GameMap* pMap = GameMap::getInstance();
     GameManager* pGameManager = GameManager::getInstance();
     for (qint32 i = 0; i < points.size() - 1; i++)
     {
         oxygine::spSprite pSprite = new oxygine::Sprite();
         oxygine::ResAnim* pAnim = pGameManager->getResAnim("arrow+unit");
         pSprite->setResAnim(pAnim);
         pSprite->setPriority(static_cast<qint8>(Terrain::DrawPriority::Arrow));
         pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
         pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
         pMap->getSpTerrain(points[i].x(), points[i].y())->addChild(pSprite);
         m_Arrows.append(pSprite);
         m_ArrowPoints.append(points[i]);
         if (i > 0)
         {
             // select arrow
             if (((points[i].x() < points[i + 1].x()) && (points[i].x() > points[i - 1].x())) ||
                 ((points[i].x() < points[i - 1].x()) && (points[i].x() > points[i + 1].x())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::LeftRight));
             }
             else if (((points[i].y() < points[i + 1].y()) && (points[i].y() > points[i - 1].y())) ||
                      ((points[i].y() < points[i - 1].y()) && (points[i].y() > points[i + 1].y())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::UpDown));
             }
             else if (((points[i].y() < points[i + 1].y()) && (points[i].x() < points[i - 1].x())) ||
                      ((points[i].y() < points[i - 1].y()) && (points[i].x() < points[i + 1].x())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::DownRight));
             }
             else if (((points[i].y() < points[i + 1].y()) && (points[i].x() > points[i - 1].x())) ||
                      ((points[i].y() < points[i - 1].y()) && (points[i].x() > points[i + 1].x())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::DownLeft));
             }
             else if (((points[i].y() > points[i + 1].y()) && (points[i].x() < points[i - 1].x())) ||
                      ((points[i].y() > points[i - 1].y()) && (points[i].x() < points[i + 1].x())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::UpRight));
             }
             else if (((points[i].y() > points[i + 1].y()) && (points[i].x() > points[i - 1].x())) ||
                      ((points[i].y() > points[i - 1].y()) && (points[i].x() > points[i + 1].x())))
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::UpLeft));
             }
         }
         else
         {
             // final arrow
             if (points[i].x() < points[i + 1].x())
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::Right));
             }
             else if (points[i].x() > points[i + 1].x())
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::Left));
             }
             else if (points[i].y() < points[i + 1].y())
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::Down));
             }
             else if (points[i].y() > points[i + 1].y())
             {
                 pSprite->setColumn(static_cast<qint32>(Arrows::Up));
             }
         }
     }
 }

 QStringList HumanPlayerInput::getEmptyActionList()
 {
     Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
     QJSValue value = pInterpreter->doFunction("ACTION", "getEmptyFieldActions");
     if (value.isString())
     {
         return value.toString().split(",");
     }
     else
     {
        return QStringList();
     }
 }

void HumanPlayerInput::deleteArrow()
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_Arrows.size(); i++)
    {
        pMap->getTerrain(m_ArrowPoints[i].x(), m_ArrowPoints[i].y())->removeChild(m_Arrows[i]);
    }
    m_ArrowPoints.clear();
    m_Arrows.clear();
}
