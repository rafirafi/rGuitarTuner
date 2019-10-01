// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "qtune.h"

#include <QCoreApplication>
#include <QTimer>

#include <QtDebug>

const QString QTune::noneStr()
{
	return QObject::tr("None");
}

QTune::QTune(QObject *parent) : QObject(parent)
{
	tune_ = new Tune();
	tunerTimer_ = new QTimer(this);
	connect(tunerTimer_, SIGNAL(timeout()), this, SLOT(updateNote()));
	recStatusTimer_ = new QTimer(this);
	recStatusTimer_->setSingleShot(true);
	connect(recStatusTimer_, SIGNAL(timeout()),
		this, SLOT(setRecStatusInactive()));
#if defined (Q_OS_ANDROID)
	screenSleepHelper_ = new ScreenSleepHelper(this);
#endif
}

QTune::~QTune()
{
	delete tune_;
}

QStringList QTune::captureDeviceNames()
{
	// noneStr must be index 0, in sync with qml
	QStringList names{noneStr()};
	if (tune_) {
		for (auto &name : tune_->getCaptureDeviceNames()) {
			names.append(name.c_str());
		}
	}
	return names;
}

void QTune::stopTuner()
{
	qDebug() << __func__;
	tune_->stopCapture();
	tunerTimer_->stop();
	setRecStatus(false);
}

void QTune::startTuner(const QString &devName)
{
	qDebug() << __func__ << devName;
	// TODO: set the device to none in qml; display err msg ?
	if (!tune_->initCapture(devName.toLocal8Bit().constData())) {
		qDebug() << __func__ << "initCapture" << "failed";
		return;
	}
	if (!tune_->initAnalyze(1000, 100)) {
		qDebug() << __func__ << "initAnalyze" << "failed";
		return;
	}
	tune_->startCapture();
	tunerTimer_->start(50);
}

void QTune::setRecStatus(bool activated)
{
	if (activated != recActive_) {
		recActive_ = activated;
		recActivated();
	}
	if (activated) {
		recStatusTimer_->start(300);
	}
}

// TODO: use index instead of using noneStr as identifier
void QTune::currentCaptureDeviceChanged(const QString &devName)
{
	if (devName == noneStr()) {
		return stopTuner();
	}
	startTuner(devName);
}

void QTune::updateNote()
{
	if (tune_ && tune_->sampleReady()) {
		float val  = tune_->sampleOne();
		if (std::isnan(val)) {
			return;
		}
		float intpart = std::nearbyint(val);
		note_ = intpart;
		noteOffset_ = val - intpart;
		noteOffset_ *= 10.f;
		qDebug() << __func__ << note_ << noteOffset_;
		noteUpdated();
		setRecStatus(true);
#if defined (Q_OS_ANDROID)
		screenSleepHelper_->inhibitSleep(10);
#endif
	}
}

void QTune::setRecStatusInactive()
{
	setRecStatus(false);
}
