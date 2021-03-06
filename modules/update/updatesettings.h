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

#ifndef UPDATECTROLMIRRORS_H
#define UPDATECTROLMIRRORS_H

#include "contentwidget.h"
#include "switchwidget.h"
#include "nextpagewidget.h"

using namespace dcc::widgets;

namespace dcc{
namespace update{

class UpdateModel;

class UpdateSettings : public ContentWidget
{
    Q_OBJECT

public:
    explicit UpdateSettings(UpdateModel* model, QWidget *parent = 0);

    void setModel(UpdateModel *model);

signals:
    void requestShowMirrorsView();
    void requestSetAutoUpdate(const bool &autoUpdate);
    void requestSetAutoCleanCache(const bool autoClean);

private:
    UpdateModel* m_model;
    SwitchWidget *m_autoCleanCache;
    SwitchWidget* m_autoDownloadSwitch;
#ifndef DISABLE_SYS_UPDATE_MIRRORS
    NextPageWidget* m_updateMirrors;
#endif
};

}
}
#endif // UPDATECTROLMIRRORS_H
