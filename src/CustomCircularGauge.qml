// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

import app.tuner.guitar 1.0

CircularGauge {
    minimumValue: -10
    maximumValue: 10
    stepSize: 0.05
    value: tune.noteOffset
    style: CircularGaugeStyle {
        minimumValueAngle : -60
        maximumValueAngle : 60
        minorTickmarkCount : 9
        labelInset : -20
        tickmarkLabel: Text {
            font.pixelSize: Math.max(6, outerRadius * 0.12)
            text: tune.note +  styleData.index - 1
            color: "blue"
            antialiasing: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
