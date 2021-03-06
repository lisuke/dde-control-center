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

#ifndef BRIGHTNESSITEM_H
#define BRIGHTNESSITEM_H

#include "settingsitem.h"

#include <QSlider>

#include <dimagebutton.h>

namespace dcc {

namespace display {

class Monitor;
class BrightnessItem : public widgets::SettingsItem
{
    Q_OBJECT

public:
    explicit BrightnessItem(QWidget *parent = 0);

signals:
    void requestSetMonitorBrightness(Monitor *mon, const double brightness) const;

public slots:
    void setValue(const double value);
    void setMonitor(Monitor * const mon);

private:
    Dtk::Widget::DImageButton *m_leftIcon;
    Dtk::Widget::DImageButton *m_rightIcon;
    QSlider *m_brightnessSlider;

    Monitor *m_monitor;
};

}

}

#endif // BRIGHTNESSITEM_H
