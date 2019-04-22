#ifndef DAYTODAYRECORD_H
#define DAYTODAYRECORD_H

#include <QObject>

#include <QVector>

#include "coreengine/fileserializable.h"

#include "oxygine-framework.h"

#include "game/gamerecording/playerrecord.h"

#include "game/gamerecording/specialevent.h"

class DayToDayRecord;
typedef oxygine::intrusive_ptr<DayToDayRecord> spDayToDayRecord;

class DayToDayRecord : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit DayToDayRecord();
    explicit DayToDayRecord(qint32 playerCount);

    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }
    /**
     * @brief addSpecialEvent
     * @param player
     * @param day
     * @param event
     */
    void addSpecialEvent(qint32 player, qint32 day, GameEnums::GameRecord_SpecialEvents event);
    /**
     * @brief addPlayerRecord
     * @param player
     */
    void addPlayerRecord(qint32 player, qint32 day);
private:
    QVector<spSpecialEvent> m_SpecialEvents;
    QVector<spPlayerRecord> m_PlayerRecords;
};

#endif // DAYTODAYRECORD_H