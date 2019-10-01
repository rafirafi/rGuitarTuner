### rGuitarTuner

A tuner which covers the tone range of guitar strings.

<img src="https://raw.githubusercontent.com/rafirafi/rGuitarTuner/master/android_demo.gif" width="236" height="420">
	
---

* simple 
	+ autodetect the pitch so it needs a relatively quiet environment
* accurate	
	+ 1 second window for better pitch precision and update every 0.1s
* portable	
	+ android and linux
* localized	
	+ english and french
---

- for ubuntu 18.04 : 
	+ to build :
```
	sudo apt install git
	git clone https://github.com/rafirafi/rGuitarTuner
	sudo apt install libfftw3-dev pkg-config libopenal-dev qt5-default qtbase5-dev qtdeclarative5-dev
	cd src
	qmake rGuitarTuner.pro
	make
```
-	+ to run :
```
	sudo apt install libfftw3-single3 libopenal1 libqt5core5a libqt5gui5 libqt5qml5 qml-module-qtquick-controls qml-module-qtquick-layouts qml-module-qtquick-window2 qml-module-qtquick-extras 
	./rGuitarTuner
```

- for android :
	+ tested with android-ndk-r17c and Qt 5.12
	+ prebuilt libraries and headers are available in the prebuilt directory
	+ a debug armv7 apk is available for testing

---

Libraries used :
-	openal 	https://kcat.strangesoft.net/openal.html
-	fftw3  	http://www.fftw.org/
-	qt5	https://www.qt.io/

---

Pitch detection method adapted from
 "A smarter way to find pitch" (2005) by Philip Mcleod, Geoff Wyvill

---

License GPLv2

Copyright 2019 Rafael Gandolfi
