#include "creditsmenue.h"

#include "menue/mainwindow.h"
#include "coreengine/console.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include <qfile.h>
#include <qtextstream.h>

CreditsMenue::CreditsMenue()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Credits Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(10,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &CreditsMenue::sigExitMenue, this, &CreditsMenue::exitMenue, Qt::QueuedConnection);

    QFile file("resources/credits/credits.cred");
    file.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString line = stream.readLine().trimmed();
        if (line.startsWith("//"))
        {
            continue;
        }
        if (line.toLower().startsWith("headline:"))
        {
            QString headline = line.remove(0, line.indexOf(":") + 1);
            m_Headlines.append(headline);
            m_Authors.append(QVector<QString>());
            while (!stream.atEnd())
            {
                line = stream.readLine().trimmed();
                if (line.toLower().startsWith("end"))
                {
                    break;
                }
                else if (line.startsWith("//"))
                {
                    continue;
                }
                else
                {
                    m_Authors[m_Headlines.size() - 1].append(line);
                }
            }
        }
    }
    creditsActor = new oxygine::Actor();
    addChild(creditsActor);
    creditsActor->setY(Settings::getHeight());
    qint32 y = 0;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headstyle = FontManager::getMainFont48();
    headstyle.color = FontManager::getFontColor();
    headstyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headstyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headstyle.multiline = false;

    qint32 x = Settings::getWidth() / 2;
    oxygine::spTextField pTextfield;
    for (qint32 i = 0; i < m_Headlines.size(); i++)
    {
        pTextfield = new oxygine::TextField();
        pTextfield->setStyle(headstyle);
        pTextfield->setHtmlText(m_Headlines[i]);
        pTextfield->setPosition(x - pTextfield->getTextRect().getWidth() / 2, y);
        creditsActor->addChild(pTextfield);
        y += pTextfield->getTextRect().getHeight() + 10;
        for (qint32 i2 = 0; i2 < m_Authors[i].size(); i2++)
        {
            pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(m_Authors[i][i2]);
            pTextfield->setPosition(x - pTextfield->getTextRect().getWidth() / 2.0f, y);
            creditsActor->addChild(pTextfield);
            y += pTextfield->getTextRect().getHeight() + 5;
        }
    }
    m_creditsHeigth = y;
    speedTimer.start();
}

void CreditsMenue::doUpdate(const oxygine::UpdateState&)
{
    if (speedTimer.elapsed() > 40)
    {
        creditsActor->setY(creditsActor->getY() - 2);
        speedTimer.start();
    }
    if (creditsActor->getY() - Settings::getHeight() / 2.0f + m_creditsHeigth < 0)
    {
        emit sigExitMenue();
    }
}

void CreditsMenue::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Credits Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}
