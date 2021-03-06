#include "perkselection.h"

#include "resource_management/coperkmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "objects/label.h"

PerkSelection::PerkSelection(CO* pCO, qint32 width, qint32 maxPerks, bool banning)
    : m_pCO(pCO),
      m_maxPerks(maxPerks),
      m_banning(banning)
{
    setWidth(width);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    connect(this, &PerkSelection::sigUpdatePerkCount, this, &PerkSelection::updatePerkCount, Qt::QueuedConnection);
    updatePerksView(pCO);
}

void PerkSelection::updatePerksView(CO* pCO)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pCO = pCO;
    removeChildren();
    if (m_banning)
    {
        m_perks = GameMap::getInstance()->getGameRules()->getAllowedPerks();
    }
    else
    {
        m_perks =  pCO->getPerkList();
    }

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    qint32 count = pCOPerkManager->getCount();
    qint32 y = 0;
    qint32 x = 0;
    spGameMap pMap = GameMap::getInstance();
    const qint32 width = 370;
    for (qint32 i = 0; i < count; i++)
    {
        QString id = pCOPerkManager->getID(i);
        if (pCOPerkManager->isSelectable(i) &&
            (m_banning || pMap->getGameRules()->getAllowedPerks().contains(id)))
        {
            QString name = pCOPerkManager->getName(i);
            QString icon = pCOPerkManager->getIcon(i);

            oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon, oxygine::error_policy::ep_ignore_error);
            QString description = pCOPerkManager->getDescription(i);

            spCheckbox pCheckbox = new Checkbox();
            pCheckbox->setPosition(x, y + 5);
            pCheckbox->setTooltipText(description);
            pCheckbox->setChecked(m_perks.contains(id));
            connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool value)
            {
                if (m_banning)
                {
                    if (value)
                    {
                        m_perks.append(id);
                    }
                    else
                    {
                        m_perks.removeAll(id);
                    }
                }
                else
                {
                    if (value)
                    {
                        m_pCO->addPerk(id);
                    }
                    else
                    {
                        m_pCO->removePerk(id);
                    }
                    emit sigUpdatePerkCount();
                }
            });
            m_Checkboxes.append(pCheckbox);
            addChild(pCheckbox);

            oxygine::spSprite pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale((GameMap::getImageSize() * 2) / pAnim->getWidth());
            }
            pSprite->setPosition(x + 45, y);
            addChild(pSprite);
            spLabel pLabel = new Label(250);
            pLabel->setStyle(style);
            pLabel->setText(name);
            pLabel->setPosition(x + GameMap::getImageSize() * 2 + 50, y + 10);
            addChild(pLabel);


            x += width;
            if (x + width > getWidth())
            {
                x = 0;
                y += GameMap::getImageSize() * 2 + 10;
            }
        }
    }
    y += GameMap::getImageSize() * 2 + 10;
    setHeight(y);
    updatePerkCount();
    pApp->continueThread();
}

void PerkSelection::updatePerkCount()
{
    if (!m_banning)
    {
        bool enable = (m_pCO->getPerkList().size() < m_maxPerks);
        for (auto & checkbox : m_Checkboxes)
        {
            if (enable || checkbox->getChecked())
            {
                checkbox->setEnabled(true);
            }
            else
            {
                checkbox->setEnabled(false);
            }
        }
    }
}

void PerkSelection::toggleAll(bool toggle)
{
    for (auto & checkbox : m_Checkboxes)
    {
        checkbox->setChecked(toggle);
    }
    if (toggle)
    {
        m_perks = GameMap::getInstance()->getGameRules()->getAllowedPerks();
    }
    else
    {
       m_perks.clear();
    }
}

QStringList PerkSelection::getPerks() const
{
    return m_perks;
}

void PerkSelection::setPerks(const QStringList &perks)
{
    m_perks = perks;
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    qint32 count = pCOPerkManager->getCount();
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < count; i++)
    {
        QString id = pCOPerkManager->getID(i);
        if (pCOPerkManager->isSelectable(i) &&
            (m_banning || pMap->getGameRules()->getAllowedPerks().contains(id)))
        {
            if (m_perks.contains(id))
            {
                m_Checkboxes[i]->setChecked(true);
            }
            else
            {
                m_Checkboxes[i]->setChecked(false);
            }
        }
    }
}
