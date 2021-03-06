#include "objects/h_scrollbar.h"
#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"

#include "coreengine/console.h"

#include "coreengine/mainapp.h"

H_Scrollbar::H_Scrollbar(qint32 heigth, qint32 contentHeigth)
    : m_Heigth(heigth),
      m_ContentHeigth(contentHeigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    m_ScrollTimer.start();
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setSize(33, heigth);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("scrollbar");
    m_pBox = new oxygine::Box9Sprite();
    m_pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBox->setResAnim(pAnim);
    m_pBox->setSize(33, heigth);
    this->addChild(m_pBox);

    m_pArrowDown = new oxygine::Button();
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    m_pArrowDown->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        m_pArrowDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_scroll = 1;
        m_Scrollspeed = 1.0f;
        speedCounter = 0;
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_scroll = 0;
    });
    m_pBox->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(9, heigth - m_pArrowDown->getHeight() - 8);

    oxygine::spButton pArrowUp = new oxygine::Button();
    // pButton->setPosition(200, 200);
    pArrowUp->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+down"));
    pArrowUp->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pArrowUp->setFlippedY(true);
    pArrowUp->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        pArrowUp->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        m_scroll = -1;
        m_Scrollspeed = 1.0f;
        speedCounter = 0;
    });
    pArrowUp->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        m_scroll = 0;
    });
    m_pBox->addChild(pArrowUp);
    pArrowUp->setPosition(9, 8);

    m_slider = new oxygine::Box9Sprite();
    pAnim = pObjectManager->getResAnim("h_scrollbar");
    m_slider->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_slider->setResAnim(pAnim);

    qint32 sliderHeight = 50;
    sliderHeight = ((heigth - m_slider->getHeight() - 20 - 20) * heigth) / contentHeigth;
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (heigth - m_slider->getHeight() - 20 - 20))
    {
        sliderHeight = (heigth - m_slider->getHeight() - 20 - 20);
    }

    m_slider->setSize(18, sliderHeight);
    m_slider->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    m_slider->setPosition(9, 20);
    m_pBox->addChild(m_slider);
    m_slider->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_active");
        m_slider->setResAnim(pAnimState);
        m_slider->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        setSliding(false);
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [ = ](oxygine::Event*)
    {
        setSliding(true);
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_UP, [ = ](oxygine::Event*)
    {
        setSliding(false);
    });
    m_pBox->addEventListener(oxygine::TouchEvent::MOVE, [ = ](oxygine::Event* pEvent)
    {
        scroll(pEvent);
    });
    connect(this, &H_Scrollbar::sigChangeScrollValue, this, &H_Scrollbar::changeScrollValue, Qt::QueuedConnection);
}

void H_Scrollbar::scroll(oxygine::Event* pEvent)
{
    if (m_sliding)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            qint32 y = pTouchEvent->localPosition.y - m_slider->getHeight() / 2;
            if (y < 20)
            {
                y = 20;
            }
            else if (y > this->getHeight() - m_slider->getHeight() - 20)
            {
                y = this->getHeight() - m_slider->getHeight() - 20;
            }
            m_slider->setY(y);
            // calc new scroll value :)
            if (static_cast<float>(this->getHeight() - m_slider->getHeight() - 20 - 20) > 0)
            {
                m_Scrollvalue = static_cast<float>(y - 20) / static_cast<float>(this->getHeight() - m_slider->getHeight() - 20 - 20);
            }
            else
            {
                m_Scrollvalue = 0;
            }
            emit sigScrollValueChanged(m_Scrollvalue);
        }
        pApp->continueThread();
    }
}

bool H_Scrollbar::getSliding() const
{
    return m_sliding;
}

void H_Scrollbar::setSliding(bool sliding)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_sliding = sliding;
    if (sliding)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar_pressed");
        m_slider->setResAnim(pAnimState);
    }
    else
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("h_scrollbar");
        m_slider->setResAnim(pAnimState);
    }
    pApp->continueThread();
}

void H_Scrollbar::setContentHeigth(qint32 heigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_ContentHeigth = heigth;
    qint32 sliderHeight = 50;
    sliderHeight = ((this->getHeight() - 20 - 20) * this->getHeight()) / m_ContentHeigth;
    if (sliderHeight < 11)
    {
        sliderHeight = 11;
    }
    else if (sliderHeight > (this->getHeight() - 20 - 20))
    {
        sliderHeight = (this->getHeight() - 20 - 20);
    }
    m_Scrollvalue = 0;
    m_slider->setSize(18, sliderHeight);
    changeScrollValue(m_Scrollvalue);
    pApp->continueThread();
}

void H_Scrollbar::update(const oxygine::UpdateState&)
{
    // no need to calculate more than we need if we're invisible
    if(m_scroll != 0)
    {
        if (m_ScrollTimer.elapsed() > 250)
        {
            if (m_ContentHeigth > m_Heigth)
            {
                speedCounter++;
                if (speedCounter % 8 == 0)
                {
                    m_Scrollspeed *= 2;
                }
                emit sigChangeScrollValue(m_scroll * m_Scrollspeed * 10.0f / static_cast<float>(m_ContentHeigth));
                m_ScrollTimer.start();
            }
        }
    }
}

void H_Scrollbar::changeScrollValue(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Scrollvalue += value;
    if (m_Scrollvalue < 0)
    {
        m_Scrollvalue = 0;
    }
    else if (m_Scrollvalue > 1.0f)
    {
        m_Scrollvalue = 1.0f;
    }
    else
    {
        // all fine do nothing
    }
    m_slider->setY(20 + m_Scrollvalue * (m_Heigth - m_slider->getHeight() - 20 - 20));
    emit sigScrollValueChanged(m_Scrollvalue);
    pApp->continueThread();
}

void H_Scrollbar::setScrollvalue(float Scrollvalue)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Scrollvalue = Scrollvalue;
    if (m_Scrollvalue < 0)
    {
        m_Scrollvalue = 0;
    }
    else if (m_Scrollvalue > 1.0f)
    {
        m_Scrollvalue = 1.0f;
    }
    else
    {
        // all fine do nothing
    }
    m_slider->setY(20 + m_Scrollvalue * (m_Heigth - m_slider->getHeight() - 20 - 20));
    pApp->continueThread();
}

void H_Scrollbar::setHeight(float h)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::Actor::setHeight(h);
    m_Heigth = h;
    m_pBox->setHeight(h);
    m_pArrowDown->setPosition(9, m_Heigth - m_pArrowDown->getHeight() - 8);
    setContentHeigth(m_ContentHeigth);
    pApp->continueThread();
}
