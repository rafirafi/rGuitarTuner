QT += quick

CONFIG += c++11

TARGET = rGuitarTuner

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += qml.qrc \
    resources.qrc

unix:!android {
    #DESTDIR = ../
    QMAKE_LFLAGS += -no-pie
    CONFIG += link_pkgconfig
    PKGCONFIG += openal
    LIBS += -lfftw3f
}

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

android {
    # QAndroidJniObject
    QT += androidextras

    DEFINES += USE_MONO16_FALLBACK

    SOURCES += \
	android/screensleepinhibit.cpp \
	android/askpermission.cpp

    HEADERS += \
	android/screensleepinhibit.h \
	android/askpermission.h

    DISTFILES += \
	android/AndroidManifest.xml

    ANDROID_PACKAGE_SOURCE_DIR = \
       $$PWD/android

    ANDROID_PREBUILT_DIR = \
       $$PWD/../prebuilt/android

    ANDROID_EXTRA_LIBS += $$ANDROID_PREBUILT_DIR/openal/$$ANDROID_TARGET_ARCH/libopenal.so
    INCLUDEPATH += $$ANDROID_PREBUILT_DIR/openal/

    ANDROID_EXTRA_LIBS += $$ANDROID_PREBUILT_DIR/fftw3/$$ANDROID_TARGET_ARCH/lib/libfftw3f.so
    INCLUDEPATH += $$ANDROID_PREBUILT_DIR/fftw3/$$ANDROID_TARGET_ARCH/include/

    LIBS += $$ANDROID_PREBUILT_DIR/openal/$$ANDROID_TARGET_ARCH/libopenal.so \
       $$ANDROID_PREBUILT_DIR/fftw3/$$ANDROID_TARGET_ARCH/lib/libfftw3f.so
}

TRANSLATIONS = languages/fr_FR.ts

SOURCES += \
    main.cpp \
    qtune.cpp \
    tune/alutil.cpp \
    tune/analyze.cpp \
    tune/capture.cpp \
    tune/tune.cpp

HEADERS += \
    tune/alutil.h \
    tune/analyze.h \
    tune/capture.h \
    tune/alutil.h \
    tune/analyze.h \
    tune/capture.h \
    tune/tune.h \
    qtune.h

DISTFILES += \
    GPL-2.0
