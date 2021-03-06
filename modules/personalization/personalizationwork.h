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

#ifndef PERSONALIZATIONWORK_H
#define PERSONALIZATIONWORK_H

#include "personalizationmodel.h"
#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <com_deepin_daemon_apperance.h>
#include <com_deepin_wm_switcher.h>

using com::deepin::daemon::Appearance;
using com::deepin::wm_switcher;

namespace dcc
{
namespace personalization
{
class ThemeModel;
class PersonalizationWork : public QObject
{
    Q_OBJECT
public:
    PersonalizationWork(PersonalizationModel *model, QObject *parent = 0);
    void active();
    void deactive();
    void onGetList();

public slots:
    void setDefault(const QJsonObject &value);
    void setFontSize(const int value);
    void switchWM();

private slots:
    void FontSizeChanged(const double value) const;
    void onStandardFontFinished(QDBusPendingCallWatcher *w);
    void onMonoFontFinished(QDBusPendingCallWatcher *w);
    void onGetThemeFinished(QDBusPendingCallWatcher *w);
    void onGetPicFinished(QDBusPendingCallWatcher *w);
    void onRefreshedChanged(const QString &type);
    void onToggleWM();
    void onGetCurrentWMFinished(QDBusPendingCallWatcher *w);

private:
    int sizeToSliderValue(const double value) const;
    float sliderValueToSize(const int value) const;
    QList<QJsonObject> converToList(const QString &type, QJsonArray &array);
    void addList(ThemeModel *model, const QString &type, QJsonArray &array);

private:
    PersonalizationModel *m_model;
    Appearance           *m_dbus;
    wm_switcher *m_wmSwitcher;
};
}
}


#endif // PERSONALIZATIONWORK_H
