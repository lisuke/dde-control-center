/*
 * Copyright (C) 2016 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QMap>

#include "user.h"

namespace dcc {
namespace accounts {

class UserModel : public QObject
{
    Q_OBJECT
public:
    explicit UserModel(QObject *parent = 0);
    ~UserModel();

    User *getUser(const QString &id);
    QList<User *> userList() const;
    void addUser(const QString &id, User *user);
    void removeUser(const QString &id);
    bool contains(const QString &id);

signals:
    void userAdded(User *user);
    void userRemoved(User *user);

private:
    QMap<QString, User *> m_userList;
};

} // namespace accounts
} // namespace dcc

#endif // USERMODEL_H
