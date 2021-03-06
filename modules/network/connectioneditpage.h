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

#ifndef CONNECTIONEDITPAGE_H
#define CONNECTIONEDITPAGE_H

#include "contentwidget.h"

#include <QMap>
#include <QPointer>

#include <types/networkerrors.h>

class QVBoxLayout;
class QPushButton;

namespace dcc {

namespace widgets {

class SettingsGroup;
class SettingsItem;
class ButtonTuple;

}

namespace network {

class NetworkModel;
class ConnectionSessionModel;
class ConnectionEditPage : public ContentWidget
{
    Q_OBJECT

public:
    explicit ConnectionEditPage(QWidget *parent = 0);
    ~ConnectionEditPage();

    void setModel(NetworkModel *networkModel, ConnectionSessionModel *sessionModel);

signals:
    void accept() const;
    void requestCancelSession() const;
    void requestRemove(const QString &uuid) const;
    void requestDisconnect(const QString &uuid) const;
    void requestChangeSettings(const QString &section, const QString &vKey, const QString &data) const;
    void requestNextPage(ContentWidget * const page) const;
    void requestFrameKeepAutoHide(const bool autoHide) const;

public slots:
    void onDeviceRemoved();

private slots:
    void recreateUI();
    void refershUI();

    void exportConnConfig();
    void saveFinished(const bool ret);
    void initPlaceholderText(const NetworkErrors &errors);

    void onActiveStateChanged();
    void onErrorsChanged(const NetworkErrors &errors);
    void onTypeChanged(const QString &type);

private:
    widgets::SettingsItem *optionWidgets(const QString &section, const QJsonObject &keyObject);
    widgets::SettingsItem *createSwitchWidget(const QJsonObject &keyObject, const QJsonObject &infoObject);
    widgets::SettingsItem *createEditWidget(const QJsonObject &keyObject, const QJsonObject &infoObject, const bool password);
    widgets::SettingsItem *createComboWidget(const QJsonObject &keyObject, const QJsonObject &infoObject, const bool editable);
    widgets::SettingsItem *createFileChooserWidget(const QJsonObject &keyObject, const QJsonObject &infoObject);
    widgets::SettingsItem *createSpinBoxWidget(const QJsonObject &keyObject, const QJsonObject &infoObject);
    void updateSpinBoxWidget(widgets::SettingsItem *item, const QJsonObject &infoObject);
    void setItemRequired(widgets::SettingsItem *item);

private:
    NetworkModel *m_networkModel;
    ConnectionSessionModel *m_sessionModel;

    QPushButton *m_disconnectBtn;
    QPushButton *m_removeBtn;
    QPushButton *m_exportBtn;
    dcc::widgets::ButtonTuple *m_buttonTuple;
    QVBoxLayout *m_sectionsLayout;

    QMap<QString, widgets::SettingsGroup *> m_sectionWidgets;
    QMap<QString, QMap<QString, widgets::SettingsItem*>> m_optionWidgets;

    QPointer<ContentWidget> m_nextPage;
    QTimer *m_recreateUITimer;
};

}

}

#endif // CONNECTIONEDITPAGE_H
