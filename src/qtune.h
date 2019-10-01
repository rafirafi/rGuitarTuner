// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef QTUNE_H
#define QTUNE_H

#include <QObject>
#include <QTimer>
#include <QtGlobal>

#include "tune/tune.h"

#if defined (Q_OS_ANDROID)
#include "android/screensleepinhibit.h"
#endif

class QTune : public QObject
{
	Q_OBJECT

	// openal doesn't update device list after startup : CONSTANT
	Q_PROPERTY(QStringList captDevNames READ captureDeviceNames CONSTANT)
	Q_PROPERTY(int note MEMBER note_ NOTIFY noteUpdated)
	Q_PROPERTY(float noteOffset MEMBER noteOffset_ NOTIFY noteUpdated)
	Q_PROPERTY(bool recActive MEMBER recActive_ NOTIFY recActivated)
public:
	explicit QTune(QObject *parent = nullptr);
	~QTune();
	QStringList captureDeviceNames();
	void stopTuner();
	void startTuner(const QString &devName);
	void setRecStatus(bool activated);
	static const QString noneStr();
signals:
	void noteUpdated();
	void recActivated();
public slots:
	void currentCaptureDeviceChanged(const QString &devName);
	void updateNote();
	void setRecStatusInactive();
private:
	Tune *tune_ = nullptr;
	int captDevDefaultIdx = 0;
	QTimer *tunerTimer_ = nullptr;
	int note_ = 0;
	float noteOffset_ = 0.f;
	QTimer *recStatusTimer_ = nullptr;
	bool recActive_ = false;

#if defined (Q_OS_ANDROID)
	ScreenSleepHelper *screenSleepHelper_ = nullptr;
#endif
};

#endif // QTUNE_H
