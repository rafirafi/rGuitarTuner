// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef SCREENSLEEPINHIBIT_H
#define SCREENSLEEPINHIBIT_H

#include <QObject>
#include <QTimer>

class ScreenSleepHelper : public QObject
{
	Q_OBJECT

public:
	explicit ScreenSleepHelper(QObject *parent = nullptr);
	void inhibitSleep(int seconds);
private:
	QTimer *timer_ = nullptr;
};

#endif // SCREENSLEEPINHIBIT_H
