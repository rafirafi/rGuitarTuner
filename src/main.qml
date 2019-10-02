// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4 // Button , quit
import QtQuick.Extras 1.4 // StatusIndicator

import app.tuner.guitar 1.0

Window {
    visible: true
    minimumWidth: 240
    minimumHeight: 400
    width: 480
    height: 480
    title: Qt.application.name
    ColumnLayout {
        id: colLayout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5
        CaptureSrcComboBox {
            id : comboBox
            Layout.alignment: Qt.AlignLeft
        }
        StatusIndicator {
            Layout.alignment: Qt.AlignLeft
            active: tune.recActive
            color: "green"
        }
        CustomCircularGauge {
            id: circularGauge
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.rightMargin: 15
            Layout.leftMargin: 5
            Text {
                id: noteMapText
                anchors.top: circularGauge.verticalCenter
                anchors.topMargin: 30
                width : circularGauge.width
                wrapMode: Text.WrapAnywhere
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                color: "#656565"
                text: qsTr("E:40 |A:45 |D:50 |G:55 |B:59 |E:64")
            }
            Rectangle {
                id: aboutText
                visible: false
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: childrenRect.height
                border.width: 1
                color: "Gainsboro"
                Text {
                    width : parent.width
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignCenter
                    padding: 5
                    text:
                    "<h4\> rGuitarTuner </h4>"
                    + "<br\>" + "<i>"+ qsTr("Author")+ ":</i>"
                    + " Copyright 2019 Rafael Gandolfi &lt;rafirafi.at@gmail.fr&gt;"
                    + "<br\>" + "<i>"+ qsTr("Source")+ ":</i>" + " github.com/rafirafi/rGuitarTuner"
                    + "<br\>" + "<i>"+ qsTr("License")+ ":</i>" + " GPLv2"
                    + "<br\>" + "<i>"+ qsTr("Libraries") + ":</i>"
                    + "<ul>"
                    + "<li>" + "openal LGPLv2" + "</li>"
                    + "<li>" + "fftw GPLv2" + "</li>"
                    + "<li>" + "Qt LGPLv3" + "</li>"
                    + "</ul>"
                    + "<i>" + qsTr("Pitch detection method adapted from") + ":</i>"
                    + "<ul>"
                    + "<li> 'A smarter way to find pitch' (2005) by Philip Mcleod, Geoff Wyvill </li>"
                    + "</ul>"
                }
            }
        }
        RowLayout {
            Button {
                id: quitButton
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 0
                text: qsTr("Quit")
                onClicked: Qt.quit()
            }
            Item {
                Layout.fillWidth: true
            }
            Button {
                id: aboutButton
                Layout.alignment: Qt.AlignRight
                Layout.topMargin: 0
                text: qsTr("About")
                onClicked: {
                    aboutText.visible = !aboutText.visible
                }
            }
        }
    }
}
