/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "networkworker.h"

using namespace dcc::network;

NetworkWorker::NetworkWorker(NetworkModel *model, QObject *parent)
    : QObject(parent),
      m_networkInter("com.deepin.daemon.Network", "/com/deepin/daemon/Network", QDBusConnection::sessionBus(), this),
      m_chainsInter(new ProxyChains("com.deepin.daemon.Network", "/com/deepin/daemon/Network/ProxyChains", QDBusConnection::sessionBus(), this)),
      m_networkModel(model)
{
    connect(&m_networkInter, &NetworkInter::ActiveConnectionsChanged, this, &NetworkWorker::queryActiveConnInfo);
    connect(&m_networkInter, &NetworkInter::DevicesChanged, m_networkModel, &NetworkModel::onDeviceListChanged);
    connect(&m_networkInter, &NetworkInter::ActiveConnectionsChanged, m_networkModel, &NetworkModel::onActiveConnectionsChanged);
    connect(&m_networkInter, &NetworkInter::ConnectionsChanged, m_networkModel, &NetworkModel::onConnectionListChanged);
    connect(&m_networkInter, &NetworkInter::DeviceEnabled, m_networkModel, &NetworkModel::onDeviceEnableChanged);
    connect(&m_networkInter, &NetworkInter::AccessPointAdded, m_networkModel, &NetworkModel::onDeviceAPInfoChanged);
    connect(&m_networkInter, &NetworkInter::AccessPointPropertiesChanged, m_networkModel, &NetworkModel::onDeviceAPInfoChanged);
    connect(&m_networkInter, &NetworkInter::AccessPointRemoved, m_networkModel, &NetworkModel::onDeviceAPRemoved);
    connect(&m_networkInter, &NetworkInter::VpnEnabledChanged, m_networkModel, &NetworkModel::onVPNEnabledChanged);
    connect(m_networkModel, &NetworkModel::requestDeviceStatus, this, &NetworkWorker::queryDeviceStatus, Qt::QueuedConnection);
    connect(m_networkModel, &NetworkModel::connectionListChanged, this, &NetworkWorker::queryDevicesConnections, Qt::QueuedConnection);

    connect(m_chainsInter, &ProxyChains::IPChanged, model, &NetworkModel::onChainsAddrChanged);
    connect(m_chainsInter, &ProxyChains::PasswordChanged, model, &NetworkModel::onChainsPasswdChanged);
    connect(m_chainsInter, &ProxyChains::TypeChanged, model, &NetworkModel::onChainsTypeChanged);
    connect(m_chainsInter, &ProxyChains::UserChanged, model, &NetworkModel::onChainsUserChanged);
    connect(m_chainsInter, &ProxyChains::PortChanged, this, [=] (uint port) {
        model->onChainsPortChanged(QString::number(port));
    });

    m_networkModel->onDeviceListChanged(m_networkInter.devices());
    m_networkModel->onConnectionListChanged(m_networkInter.connections());
    m_networkModel->onVPNEnabledChanged(m_networkInter.vpnEnabled());
    m_networkModel->onActiveConnectionsChanged(m_networkInter.activeConnections());

    m_networkInter.setSync(false);
    m_chainsInter->setSync(false);

    queryActiveConnInfo();
}

void NetworkWorker::setVpnEnable(const bool enable)
{
    m_networkInter.setVpnEnabled(enable);
}

void NetworkWorker::setDeviceEnable(const QString &devPath, const bool enable)
{
    m_networkInter.EnableDevice(QDBusObjectPath(devPath), enable);
}

void NetworkWorker::setProxyMethod(const QString &proxyMethod)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.SetProxyMethod(proxyMethod), this);

    // requery result
    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryProxyMethod);
    connect(w, &QDBusPendingCallWatcher::finished, w, &QDBusPendingCallWatcher::deleteLater);
}

void NetworkWorker::setProxyIgnoreHosts(const QString &hosts)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.SetProxyIgnoreHosts(hosts), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryProxyIgnoreHosts);
    connect(w, &QDBusPendingCallWatcher::finished, w, &QDBusPendingCallWatcher::deleteLater);
}

void NetworkWorker::setAutoProxy(const QString &proxy)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.SetAutoProxy(proxy), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryAutoProxy);
    connect(w, &QDBusPendingCallWatcher::finished, w, &QDBusPendingCallWatcher::deleteLater);
}

void NetworkWorker::setProxy(const QString &type, const QString &addr, const QString &port)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.SetProxy(type, addr, port), this);

    connect(w, &QDBusPendingCallWatcher::finished, [=] { queryProxy(type); });
    connect(w, &QDBusPendingCallWatcher::finished, w, &QDBusPendingCallWatcher::deleteLater);
}

void NetworkWorker::setChainsProxy(const ProxyConfig &config)
{
    m_chainsInter->Set(config.type, config.url, config.port.toUInt(), config.username, config.password);
}

void NetworkWorker::initWirelessHotspot(const QString &devPath)
{
    m_networkInter.EnableWirelessHotspotMode(QDBusObjectPath(devPath));
}

void NetworkWorker::queryProxy(const QString &type)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.asyncCall(QStringLiteral("GetProxy"), type), this);

    w->setProperty("proxyType", type);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryProxyCB);
}

void NetworkWorker::queryChains()
{
    m_networkModel->onChainsTypeChanged(m_chainsInter->type());
    m_networkModel->onChainsAddrChanged(m_chainsInter->iP());
    m_networkModel->onChainsPortChanged(QString::number(m_chainsInter->port()));
    m_networkModel->onChainsUserChanged(m_chainsInter->user());
    m_networkModel->onChainsPasswdChanged(m_chainsInter->password());
}

void NetworkWorker::queryAutoProxy()
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.GetAutoProxy(), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryAutoProxyCB);
}

void NetworkWorker::queryProxyData()
{
    queryProxy("http");
    queryProxy("https");
    queryProxy("ftp");
    queryProxy("socks");
    queryAutoProxy();
    queryProxyMethod();
    queryProxyIgnoreHosts();
}

void NetworkWorker::queryProxyMethod()
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.GetProxyMethod(), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryProxyMethodCB);
}

void NetworkWorker::queryProxyIgnoreHosts()
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.GetProxyIgnoreHosts(), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryProxyIgnoreHostsCB);
}

void NetworkWorker::queryActiveConnInfo()
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.GetActiveConnectionInfo(), this);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryActiveConnInfoCB);
}

void NetworkWorker::queryDevicesConnections()
{
    for (auto *dev : m_networkModel->devices())
        if (dev->type() == NetworkDevice::Wired)
            queryDeviceConnections(dev->path());
}

void NetworkWorker::queryAccessPoints(const QString &devPath)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.GetAccessPoints(QDBusObjectPath(devPath)));

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryAccessPointsCB);
}

void NetworkWorker::queryConnectionSession(const QString &devPath, const QString &uuid)
{
    Q_ASSERT_X(!uuid.isEmpty(), Q_FUNC_INFO, "uuid is empty");

    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.EditConnection(uuid, QDBusObjectPath(devPath)), this);

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryConnectionSessionCB);
}

void NetworkWorker::queryDeviceStatus(const QString &devPath)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.IsDeviceEnabled(QDBusObjectPath(devPath)), this);

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryDeviceStatusCB);
}

void NetworkWorker::queryDeviceConnections(const QString &devPath)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.ListDeviceConnections(QDBusObjectPath(devPath)), this);

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryDeviceConnectionsCB);
}

void NetworkWorker::deleteConnection(const QString &uuid)
{
    m_networkInter.DeleteConnection(uuid);
}

void NetworkWorker::deactiveConnection(const QString &uuid)
{
    m_networkInter.DeactivateConnection(uuid);
}

void NetworkWorker::createApConfig(const QString &devPath, const QString &apPath)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.CreateConnectionForAccessPoint(QDBusObjectPath(apPath), QDBusObjectPath(devPath)));

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryConnectionSessionCB);
}

void NetworkWorker::createConnection(const QString &type, const QString &devPath)
{
    QDBusPendingCallWatcher *w = new QDBusPendingCallWatcher(m_networkInter.CreateConnection(type, QDBusObjectPath(devPath)));

    w->setProperty("devPath", devPath);

    connect(w, &QDBusPendingCallWatcher::finished, this, &NetworkWorker::queryConnectionSessionCB);
}

void NetworkWorker::activateConnection(const QString &devPath, const QString &uuid)
{
    m_networkInter.ActivateConnection(uuid, QDBusObjectPath(devPath));
}

void NetworkWorker::activateAccessPoint(const QString &devPath, const QString &apPath, const QString &uuid)
{
    m_networkInter.ActivateAccessPoint(uuid, QDBusObjectPath(apPath), QDBusObjectPath(devPath));
}

void NetworkWorker::queryAutoProxyCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QString> reply = *w;

    m_networkModel->onAutoProxyChanged(reply.value());

    w->deleteLater();
}

void NetworkWorker::queryProxyCB(QDBusPendingCallWatcher *w)
{
    QDBusMessage reply = w->reply();

    const QString type = w->property("proxyType").toString();
    const QString addr = reply.arguments()[0].toString();
    const QString port = reply.arguments()[1].toString();

    m_networkModel->onProxiesChanged(type, addr, port);

    w->deleteLater();
}

void NetworkWorker::queryProxyMethodCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QString> reply = *w;

    m_networkModel->onProxyMethodChanged(reply.value());

    w->deleteLater();
}

void NetworkWorker::queryProxyIgnoreHostsCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QString> reply = *w;

    m_networkModel->onProxyIgnoreHostsChanged(reply.value());

    w->deleteLater();
}

void NetworkWorker::queryAccessPointsCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QString> reply = *w;

    m_networkModel->onDeviceAPListChanged(w->property("devPath").toString(), reply.value());

    w->deleteLater();
}

void NetworkWorker::queryConnectionSessionCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QDBusObjectPath> reply = *w;

    m_networkModel->onConnectionSessionCreated(w->property("devPath").toString(), reply.value().path());

    w->deleteLater();
}

void NetworkWorker::queryDeviceStatusCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<bool> reply = *w;

    m_networkModel->onDeviceEnableChanged(w->property("devPath").toString(), reply.value());

    w->deleteLater();
}

void NetworkWorker::queryDeviceConnectionsCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QList<QDBusObjectPath>> reply = *w;

    m_networkModel->onDeviceConnectionsChanged(w->property("devPath").toString(), reply.value());

    w->deleteLater();
}

void NetworkWorker::queryActiveConnInfoCB(QDBusPendingCallWatcher *w)
{
    QDBusPendingReply<QString> reply = *w;

    m_networkModel->onActiveConnInfoChanged(reply.value());

    w->deleteLater();
}
