#include "tooltip.h"

#include "coreengine/mainapp.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include <qguiapplication.h>

Tooltip::Tooltip()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    m_TooltipTimer.setSingleShot(true);
    addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event*)
    {
        emit sigStartTooltip();
    });
    addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        emit sigStartTooltip();
    });
    addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        emit sigStopTooltip();
        emit sigHideTooltip();
    });
    addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigStopTooltip();
    });

    connect(this, &Tooltip::sigHideTooltip, this, &Tooltip::hideTooltip, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStopTooltip, this, &Tooltip::stopTooltiptimer, Qt::QueuedConnection);
    connect(this, &Tooltip::sigStartTooltip, this, &Tooltip::restartTooltiptimer, Qt::QueuedConnection);
    connect(&m_TooltipTimer, &QTimer::timeout, this, &Tooltip::showTooltip, Qt::QueuedConnection);
}

Tooltip::~Tooltip()
{
    m_TooltipTimer.stop();
    if (m_Tooltip.get() != nullptr)
    {
        m_Tooltip->detach();
        m_Tooltip = nullptr;
    }
}

void Tooltip::restartTooltiptimer()
{
    if (!m_disabled)
    {
        m_TooltipTimer.start(std::chrono::milliseconds(1000));
    }
    emit sigHideTooltip();
}

void Tooltip::stopTooltiptimer()
{
    m_TooltipTimer.stop();
}

QString Tooltip::getTooltipText() const
{
    return m_tooltipText;
}

void Tooltip::setTooltipText(const QString &tooltipText)
{
    m_tooltipText = tooltipText;
}

void Tooltip::showTooltip()
{
    if (!m_disabled)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        hideTooltip();

        if (oxygine::getStage()->isDescendant(this) && m_enabled)
        {
            if (QGuiApplication::focusWindow() == pApp && !m_tooltipText.isEmpty())
            {
                QPoint curPos = pApp->mapFromGlobal(pApp->cursor().pos());

                m_Tooltip = new oxygine::Actor();
                m_Tooltip->setPriority(static_cast<short>(Mainapp::ZOrder::Tooltip));
                oxygine::getStage()->addChild(m_Tooltip);

                oxygine::TextStyle style = FontManager::getMainFont24();
                style.color = FontManager::getFontColor();
                style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
                style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
                style.multiline = true;

                ObjectManager* pObjectManager = ObjectManager::getInstance();
                oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
                pSpriteBox->setResAnim(pAnim);

                pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
                pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
                m_Tooltip->addChild(pSpriteBox);
                pSpriteBox->setPosition(0, 0);
                pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
                oxygine::spTextField pText = new oxygine::TextField();
                pText->setHtmlText(m_tooltipText);
                pText->setWidth(Settings::getWidth() / 4);
                pText->setStyle(style);
                pText->setPosition(10, 10);
                pSpriteBox->addChild(pText);
                pSpriteBox->setSize(pText->getTextRect().getSize() + oxygine::Point(30, 30));

                if (curPos.x() + 10 + pSpriteBox->getWidth() + 5 > Settings::getWidth())
                {
                    m_Tooltip->setX(curPos.x() - 10 - pSpriteBox->getWidth());
                }
                else
                {
                    m_Tooltip->setX(curPos.x() + 10);
                }
                if (curPos.y() + 10 + pSpriteBox->getHeight() + 5 > Settings::getHeight())
                {
                    m_Tooltip->setY(curPos.y() - 10 - pSpriteBox->getHeight());
                }
                else
                {
                    m_Tooltip->setY(curPos.y() + 10);
                }
            }
        }
        pApp->continueThread();
    }
}

void Tooltip::enableTooltip()
{
    m_disabled = false;
}

void Tooltip::disableTooltip()
{
    m_disabled = true;
    stopTooltiptimer();
    emit sigHideTooltip();
}

void Tooltip::hideTooltip()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_Tooltip.get() != nullptr)
    {
        m_Tooltip->detach();
        m_Tooltip = nullptr;
        stopTooltiptimer();
    }
    pApp->continueThread();
}
