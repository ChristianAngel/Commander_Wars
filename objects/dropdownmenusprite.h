#ifndef DROPDOWNMENUSPRITE_H
#define DROPDOWNMENUSPRITE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <oxygine-framework.h>

#include "objects/panel.h"
#include "objects/dropdownmenubase.h"

class DropDownmenuSprite;
typedef oxygine::intrusive_ptr<DropDownmenuSprite> spDropDownmenuSprite;

class DropDownmenuSprite : public DropDownmenuBase
{
    Q_OBJECT
public:
    explicit DropDownmenuSprite(qint32 width, QVector<QString>& items, std::function<oxygine::spActor(QString item)> creator, qint32 dropDownWidth = -1);
    /**
     * @brief getCurrentItemText text of the current item
     * @return
     */
    QString getCurrentItemText();
    /**
     * @brief setCurrentItem
     * @param item
     */
    void setCurrentItem(QString item);
    /**
     * @brief setCurrentItem
     * @param index
     */
    virtual void setCurrentItem(qint32 index) override;
signals:
    void sigItemChanged(qint32 item);
public slots:
    virtual void itemChanged(qint32 item) override;
protected:
    void addDropDownText(QString spriteID, qint32 id, qint32 dropDownWidth);
private:
    QVector<QString> m_ItemTexts;
    std::function<oxygine::spActor(QString item)> m_Creator;
    QVector<oxygine::spBox9Sprite> m_Items;
    QString m_currentText;


};

#endif // DROPDOWNMENUSPRITE_H
