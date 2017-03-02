/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include "contentwidget.h"
#include "settingsgroup.h"
#include "lineeditwidget.h"
#include "shortcutitem.h"
#include "shortcutmodel.h"
#include "keyboardwork.h"
#include <QObject>

using namespace dcc;
using namespace dcc::widgets;

namespace dcc {
namespace keyboard{
struct ShortcutInfo;

class CustomEdit : public ContentWidget
{
    Q_OBJECT
public:
    explicit CustomEdit(KeyboardWork* work, QWidget *parent = 0);
    void setShortcut(ShortcutInfo* info);

public slots:
    void onClick();

private slots:
    void onOpenFile();

private:
    KeyboardWork   *m_work;
    SettingsGroup  *m_commandGroup;
    LineEditWidget *m_name;
    LineEditWidget *m_command;
    ShortcutItem   *m_short;
    ShortcutInfo   *m_info;
};
}
}

#endif // CUSTOMEDIT_H
