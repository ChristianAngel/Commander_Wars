#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"
#include "objects/tableview.h"

#include "network/tcpclient.h"
#include "network/networkgamedata.h"

class LobbyMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit LobbyMenu();
    virtual ~LobbyMenu();

signals:    
    void sigExitMenue();
    void sigHostServer();
    void sigHostLocal();
    void sigJoinGame();
    void sigJoinAdress();
    void sigUpdateGamesView();
public slots:
    void exitMenue();    
    void hostLocal();
    void hostServer();
    void joinGame();
    void join(QString adress);
    void joinAdress();
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    void updateGamesView();
    void selectGame();
private:
    spPanel m_pGamesPanel;
    spNetworkInterface m_pTCPClient{nullptr};
    QVector<spNetworkGameData> m_games;
    spNetworkGameData m_currentGame;
    spTableView m_Gamesview;
};

#endif // LOBBYMENU_H
