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

#ifndef GRUBBACKGROUNDITEM_H
#define GRUBBACKGROUNDITEM_H

#include "settingsitem.h"
#include <com_deepin_daemon_grub2_theme.h>
using namespace dcc::widgets;

using GrubThemeDbus=com::deepin::daemon::grub2::Theme;

namespace dcc{
namespace systeminfo{

class GrubBackgroundItem : public SettingsItem
{
    Q_OBJECT

public:
    explicit GrubBackgroundItem(QFrame* parent = 0);

signals:
    void requestEnableTheme(const bool state);

public slots:
    void setThemeEnable(const bool state);

protected:
    void paintEvent(QPaintEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dropEvent(QDropEvent *e);

private:
    QPixmap m_background;
    GrubThemeDbus *m_themeDbus;
    bool m_isDrop;
    bool m_themeEnable;
    Q_SLOT bool updateBackground(const QString &filename);
    Q_SLOT void onProperty(const QString& property, const QVariant&  variant);
};

}
}
#endif // GRUBBACKGROUNDITEM_H
