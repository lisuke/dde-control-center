/****************************************************************************
**
**  Copyright (C) 2011~2014 Deepin, Inc.
**                2011~2014 Kaisheng Ye
**
**  Author:     Kaisheng Ye <kaisheng.ye@gmail.com>
**  Maintainer: Kaisheng Ye <kaisheng.ye@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import QtQuick 2.0
import Deepin.Widgets 1.0

Item {
    id: weekTitleItem
    width: itemWidth
    height: weekTitleHeight

    property color borderColor: Qt.rgba(1, 1, 1, 0.2)

    Item {
        id: content
        width: itemWidth - 1
        height: weekTitleHeight - 1

        Rectangle {
            anchors.fill: parent
            color: Qt.rgba(57/255, 64/255, 32/255, 0.5)
            visible: weekTitleItem.GridView.view.currentIndex == index
        }

        DssH2 {
            id: weekTitleLabel
            anchors.centerIn: parent
            text: dayText
        }
    }

    Rectangle {
        id: rightBorder
        anchors.left: content.right
        width: 1
        height: parent.height
        color: borderColor
        visible: (index + 1) % 7 != 0
    }

    Rectangle {
        id: bottomBorder
        anchors.top: content.bottom
        width: parent.width
        height: 1
        color: borderColor
    }
}
