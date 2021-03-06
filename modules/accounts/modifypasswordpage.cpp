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

#include "modifypasswordpage.h"
#include "settingsgroup.h"
#include "translucentframe.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

using namespace dcc::widgets;
using namespace dcc::accounts;

ModifyPasswordPage::ModifyPasswordPage(User *user, QWidget *parent)
    : ContentWidget(parent),

      m_userInter(user),

      m_pwdEdit(new LineEditWidget),
      m_pwdEditRepeat(new LineEditWidget),

      m_buttonTuple(new ButtonTuple)
{
    m_pwdEdit->textEdit()->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setTitle(tr("New Password"));
    m_pwdEditRepeat->textEdit()->setEchoMode(QLineEdit::Password);
    m_pwdEditRepeat->setTitle(tr("Repeat Password"));

    m_pwdEdit->setPlaceholderText(tr("Required"));
    m_pwdEditRepeat->setPlaceholderText(tr("Required"));

    QPushButton *cancel = m_buttonTuple->leftButton();
    QPushButton *accept = m_buttonTuple->rightButton();
    cancel->setText(tr("Cancel"));
    cancel->setAccessibleName("Modify_Cancel");
    accept->setText(tr("Accept"));
    accept->setAccessibleName("Modify_Accept");

    SettingsGroup *pwdGroup = new SettingsGroup;
    pwdGroup->appendItem(m_pwdEdit);
    pwdGroup->appendItem(m_pwdEditRepeat);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addWidget(pwdGroup);
    mainLayout->addWidget(m_buttonTuple);
    mainLayout->setSpacing(10);
    mainLayout->setMargin(0);

    TranslucentFrame *mainWidget = new TranslucentFrame;
    mainWidget->setLayout(mainLayout);

    setContent(mainWidget);
    setTitle(tr("Password") + " - " + user->name());

    connect(accept, &QPushButton::clicked, this, &ModifyPasswordPage::passwordSubmit);
    connect(cancel, &QPushButton::clicked, this, &ModifyPasswordPage::back);
    connect(m_pwdEdit->textEdit(), &QLineEdit::editingFinished, this, &ModifyPasswordPage::checkPwd);
    connect(m_pwdEditRepeat->textEdit(), &QLineEdit::editingFinished, this, &ModifyPasswordPage::checkPwd);
}

void ModifyPasswordPage::passwordSubmit()
{
    const QString pwd0 = m_pwdEdit->textEdit()->text();
    const QString pwd1 = m_pwdEditRepeat->textEdit()->text();

    checkPwd();

    if (m_pwdEdit->text().isEmpty() || m_pwdEditRepeat->text().isEmpty())
        return;

    if (pwd0 != pwd1)
        return;

    emit requestChangePassword(m_userInter, pwd0);
    emit back();
}

void ModifyPasswordPage::checkPwd()
{
    m_pwdEdit->setIsErr(m_pwdEdit->text().isEmpty());
    m_pwdEditRepeat->setIsErr(m_pwdEditRepeat->text() != m_pwdEdit->text());
}
