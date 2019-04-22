#include "daytodayrecord.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

DayToDayRecord::DayToDayRecord()
    : QObject()
{
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

DayToDayRecord::DayToDayRecord(qint32 playerCount)
{
    moveToThread(Mainapp::getInstance()->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_PlayerRecords.append(new PlayerRecord());
    }
}

void DayToDayRecord::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_SpecialEvents.size());
    for (qint32 i = 0; i < m_SpecialEvents.size(); i++)
    {
        m_SpecialEvents[i]->serializeObject(pStream);
    }
    pStream << static_cast<qint32>(m_PlayerRecords.size());
    for (qint32 i = 0; i < m_PlayerRecords.size(); i++)
    {
        m_PlayerRecords[i]->serializeObject(pStream);
    }
}

void DayToDayRecord::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_SpecialEvents.append(new SpecialEvent());
        m_SpecialEvents[i]->deserializeObject(pStream);
    }
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        m_PlayerRecords.append(new PlayerRecord());
        m_PlayerRecords[i]->deserializeObject(pStream);
    }
}

void DayToDayRecord::addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event)
{
    m_SpecialEvents.append(new SpecialEvent(player, day, event));
}

void DayToDayRecord::addPlayerRecord(qint32 player, qint32 day)
{
    GameMap* pMap = GameMap::getInstance();
    Player* pPlayer = pMap->getPlayer(player);
    if (!pPlayer->getIsDefeated())
    {
        m_PlayerRecords[player] = new PlayerRecord(day, player, pPlayer->getFonds(), pPlayer->calcIncome(),
                                                   pPlayer->getBuildingCount(), pPlayer->getUnitCount(), pPlayer->calculatePlayerStrength());
    }
}