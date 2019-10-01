// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include <QtGlobal>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QTranslator>

#include "qtune.h"
#if defined (Q_OS_ANDROID)
#include "android/askpermission.h"
#endif

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	QTranslator appTranslator;
	appTranslator.load(":/ts/ts.qm");
	app.installTranslator(&appTranslator);
	
	QTune tune;
	QQmlContext *ctxt = engine.rootContext();
	qmlRegisterType<QTune>("app.tuner.guitar", 1, 0, "QTune");
	ctxt->setContextProperty("tune", &tune);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty()) {
		return -1;
	}

	app.setWindowIcon(QIcon(":/img/win_icon.png"));

#if defined (Q_OS_ANDROID)
	requestRecordPermission();
#endif	

	return app.exec();
}
