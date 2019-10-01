// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef CAPTURE_H
#define CAPTURE_H

#include <string>
#include <vector>

#include "alutil.h"

class Capture
{
public:
	ALsizei freq() const;
	bool getSample(int sampleCnt, std::vector<float> &out, int offset = 0);
	void free();
	void start();
	void stop();
	static std::vector<std::string> getCaptureDeviceNames();
	static std::string getDefaultCaptureDeviceName();
	std::string getCurrentCaptureDeviceName();
	void setFreq(const ALsizei &freq);
	bool init(const std::string &deviceName);
	int availableSampleCnt() const;
private:
	ALCdevice* inputDevice_ = nullptr;
	ALsizei freq_ = 0;
	int bufSecLen_ = 3;
	ALCenum format_ = AL_FORMAT_MONO_FLOAT32;
#if defined (USE_MONO16_FALLBACK)
	std::vector<int16_t> tmpBuf_;
#endif
};

#endif // CAPTURE_H
