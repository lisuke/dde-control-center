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

#include "updatesettings.h"

#include <QVBoxLayout>

#include "settingsgroup.h"
#include "updatemodel.h"
#include "translucentframe.h"
#include "labels/smalllabel.h"

namespace dcc{
namespace update{

UpdateSettings::UpdateSettings(UpdateModel *model, QWidget *parent)
    :ContentWidget(parent),
      m_model(nullptr)
{
    setTitle(tr("Update Settings"));

    TranslucentFrame* widget = new TranslucentFrame;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addSpacing(10);

    SettingsGroup* ug = new SettingsGroup;
    SettingsGroup* mg = new SettingsGroup;

    m_autoCleanCache = new SwitchWidget;
    m_autoCleanCache->setTitle(tr("Auto clear package cache"));

    m_autoDownloadSwitch = new SwitchWidget;
    m_autoDownloadSwitch->setTitle(tr("Auto-download Updates"));

    TipsLabel* label = new TipsLabel(tr("Updates will be auto-downloaded in wireless or wired network"));
    label->setWordWrap(true);
    label->setContentsMargins(20, 0, 20, 0);

#ifndef DISABLE_SYS_UPDATE_MIRRORS
    m_updateMirrors = new NextPageWidget;
    m_updateMirrors->setTitle(tr("Switch Mirror"));
#endif

    ug->appendItem(m_autoCleanCache);
    ug->appendItem(m_autoDownloadSwitch);

#ifndef DISABLE_SYS_UPDATE_MIRRORS
    mg->appendItem(m_updateMirrors);
#endif

    layout->addWidget(ug);
    layout->addSpacing(8);
    layout->addWidget(label);
    layout->addSpacing(15);
    layout->addWidget(mg);
    layout->addStretch();

    widget->setLayout(layout);

    setContent(widget);

#ifndef DISABLE_SYS_UPDATE_MIRRORS
    connect(m_updateMirrors, &NextPageWidget::clicked, this, &UpdateSettings::requestShowMirrorsView);
#endif

    connect(m_autoCleanCache, &SwitchWidget::checkedChanged, this, &UpdateSettings::requestSetAutoCleanCache);
    connect(m_autoDownloadSwitch, &SwitchWidget::checkedChanged, this, &UpdateSettings::requestSetAutoUpdate);

    setModel(model);
}

void UpdateSettings::setModel(UpdateModel *model)
{
    if (model) {
        m_model = model;

        auto setAutoDownload = [this] (const bool &autoDownload) {
            m_autoDownloadSwitch->setChecked(autoDownload);
        };

#ifndef DISABLE_SYS_UPDATE_MIRRORS
        auto setDefaultMirror = [this] (const MirrorInfo &mirror) {
            m_updateMirrors->setValue(mirror.m_name);
        };
#endif

        setAutoDownload(model->autoDownloadUpdates());

#ifndef DISABLE_SYS_UPDATE_MIRRORS
        setDefaultMirror(model->defaultMirror());
#endif

        connect(model, &UpdateModel::autoDownloadUpdatesChanged, this, setAutoDownload);
        connect(model, &UpdateModel::autoCleanCacheChanged, m_autoCleanCache, &SwitchWidget::setChecked);

        m_autoCleanCache->setChecked(m_model->autoCleanCache());

#ifndef DISABLE_SYS_UPDATE_MIRRORS
        connect(model, &UpdateModel::defaultMirrorChanged, this, setDefaultMirror);
#endif
    }
}

}
}
