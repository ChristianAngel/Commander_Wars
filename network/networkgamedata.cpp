#include "networkgamedata.h"

#include "coreengine/filesupport.h"

NetworkGameData::NetworkGameData()
    : QObject()
{

}

void NetworkGameData::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_players;
    pStream << m_maxPlayers;
    Filesupport::writeVectorList(pStream, m_Mods);
    pStream << m_description;
    pStream << m_mapName;
    pStream << m_slaveName;
}

void NetworkGameData::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    pStream >> m_players;
    pStream >> m_maxPlayers;
    m_Mods = Filesupport::readVectorList<QString, QList>(pStream);
    pStream >> m_description;
    pStream >> m_mapName;
    pStream >> m_slaveName;
}

QString NetworkGameData::getMapName() const
{
    return m_mapName;
}

void NetworkGameData::setMapName(const QString &mapName)
{
    m_mapName = mapName;
}

QString NetworkGameData::getDescription() const
{
    return m_description;
}

void NetworkGameData::setDescription(const QString &value)
{
    m_description = value;
}

QStringList NetworkGameData::getMods() const
{
    return m_Mods;
}

void NetworkGameData::setMods(const QStringList &Mods)
{
    m_Mods = Mods;
}

qint32 NetworkGameData::getMaxPlayers() const
{
    return m_maxPlayers;
}

void NetworkGameData::setMaxPlayers(const qint32 &maxPlayers)
{
    m_maxPlayers = maxPlayers;
}

qint32 NetworkGameData::getPlayers() const
{
    return m_players;
}

void NetworkGameData::setPlayers(const qint32 &players)
{
    m_players = players;
}

QString NetworkGameData::getSlaveName() const
{
    return m_slaveName;
}

void NetworkGameData::setSlaveName(const QString &slaveName)
{
    m_slaveName = slaveName;
}

bool NetworkGameData::getLaunched() const
{
    return m_launched;
}

void NetworkGameData::setLaunched(bool launched)
{
    m_launched = launched;
}
