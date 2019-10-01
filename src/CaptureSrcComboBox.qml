// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import app.tuner.guitar 1.0

RowLayout {
    Layout.maximumWidth: parent.width // workaround android width pb
    Rectangle {
        color: "#f0f0f0"
        Label {
            id: label
            text: qsTr("Capture source") + " :"
        }
        width: childrenRect.width
        height: childrenRect.height
    }
    ComboBox {
        id : comboBox
        editable:false
        currentIndex: 0
        model: tune.captDevNames
        onActivated: {
            var text = comboBox.textAt(index)
            tune.currentCaptureDeviceChanged(text)
        }
    }
    Item {
        Layout.fillWidth: true
    }
}

