// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef TUNE_H
#define TUNE_H

#include <unistd.h>

#include <cfenv>
#include <cmath>
#include <cstdio>

#include "analyze.h"
#include "capture.h"

class Tune
{
public:
	Tune();
	~Tune();
	std::vector<std::string> getCaptureDeviceNames();
	bool initCapture(const char *captureDeviceName);
	void freeCapture();
	void startCapture();
	void stopCapture();
	bool initAnalyze(int msWinLen, int msSlideLen);
	void freeAnalyze();
	float sampleOne();
	bool sampleReady();
private:
	AlLib al_;
	bool alInit_;
	Capture* capt_ = nullptr;
	Analyze *ana_ = nullptr;
	std::vector<float> buf_;
	// TODO: check if we keep this here
	int msWinLen_;
	int msSlideLen_;
	int sampleWinLen_;
	int sampleSlideLen_;
	float res_;
	static constexpr float minFreq_ = 65.f;
	static constexpr float maxFreq_ = 1500.f;
	static constexpr int captureFreq_ = 44100;
	int loBin_;
	int hiBin_;
};


#endif // TUNE_H
