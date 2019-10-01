// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "screensleepinhibit.h"

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

static const int kFLAG_KEEP_SCREEN_ON = 128;

static void screenSleep(bool allow)
{
	QtAndroid::runOnAndroidThread([allow]{
		QAndroidJniObject activity = QtAndroid::androidActivity();
		if (activity.isValid()) {
			QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
			if (window.isValid()) {
				if (allow) {
					window.callMethod<void>("clearFlags", "(I)V", kFLAG_KEEP_SCREEN_ON);
				} else {
					window.callMethod<void>("addFlags", "(I)V", kFLAG_KEEP_SCREEN_ON);
				}
			}
		}

		QAndroidJniEnvironment env;
		if (env->ExceptionCheck()) {
			env->ExceptionClear();
		}
	});
}

static void screenSleepAllow()
{
	screenSleep(true);
}

ScreenSleepHelper::ScreenSleepHelper(QObject *parent) : QObject(parent)
{
	timer_ = new QTimer(this);
	timer_->setSingleShot(true);
	QObject::connect(timer_, &QTimer::timeout, screenSleepAllow);
}

void ScreenSleepHelper::inhibitSleep(int seconds)
{
	screenSleep(false);
	timer_->start(seconds * 1000);
}
