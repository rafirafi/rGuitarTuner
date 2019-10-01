// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "capture.h"

#include <iostream>
#include <sstream>

#include "alutil.h"

std::vector<std::string> Capture::getCaptureDeviceNames()
{
	std::vector<std::string> res;
	const char *name = alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER );
	while (name[0]) {
		res.push_back(name);
		name += res.back().size() + 1;
	}
	return res;
}

std::string Capture::getDefaultCaptureDeviceName()
{
	return alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
}

std::string Capture::getCurrentCaptureDeviceName()
{
	std::string res;
	if (inputDevice_) {
		res = alcGetString(inputDevice_, ALC_CAPTURE_DEVICE_SPECIFIER );
	}
	return res;
}

ALsizei Capture::freq() const
{
	return freq_;
}

void Capture::setFreq(const ALsizei &freq)
{
	if (inputDevice_) {
		fprintf(stderr, "%s : failed : device started\n", __func__);
	}

	freq_ = freq;
}

bool Capture::init(const std::string &deviceName)
{	
	if (freq_ == 0) {
		fprintf(stderr, "%s : failed : freq not set\n", __func__);
		return false;
	}

	if (inputDevice_) {
		const char *name;
		name = alcGetString(inputDevice_, ALC_CAPTURE_DEVICE_SPECIFIER);
		if (name != deviceName) {
			free();
		}
	}

	// set a 3 seconds internal buffer
	if (!inputDevice_) {
		inputDevice_ = alcCaptureOpenDevice(deviceName.data(),
						    freq_,
						    format_,
						    bufSecLen_ * freq_);
	}

#if defined (USE_MONO16_FALLBACK)
	// fallback for android version < 5
	if (!inputDevice_ && format_ == AL_FORMAT_MONO_FLOAT32) {
		format_ = AL_FORMAT_MONO16;
		inputDevice_ = alcCaptureOpenDevice(deviceName.data(),
						    freq_,
						    format_,
						    bufSecLen_ * freq_);
		if (!inputDevice_) {
			format_ = AL_FORMAT_MONO_FLOAT32;
		}
	}
#endif

	if (!inputDevice_) {
		if (!deviceName.empty()) {
			fprintf(stderr, "Failed to open device : \"%s\"\n",
				deviceName.data());
		}
		printAlError("alcCaptureOpenDevice", alGetError());
		return false;
	}

	fprintf(stderr, "Capturing from : \"%s\"\n",
		alcGetString(inputDevice_, ALC_CAPTURE_DEVICE_SPECIFIER));

	return true;
}

void Capture::free()
{
	if (inputDevice_) {
		alcCaptureStop(inputDevice_);
		alcCaptureCloseDevice(inputDevice_);
		inputDevice_ = nullptr;
	}
}

void Capture::start()
{
	if (inputDevice_) {
		alcCaptureStart(inputDevice_);
	}
}

void Capture::stop()
{
	if (inputDevice_) {
		alcCaptureStop(inputDevice_);
	}
}

int Capture::availableSampleCnt() const
{
	if (!inputDevice_) {
		return 0;
	}
	ALCint samplesIn = 0;
	alcGetIntegerv(inputDevice_, ALC_CAPTURE_SAMPLES, 1, &samplesIn);
	return samplesIn;
}

bool Capture::getSample(int sampleCnt, std::vector<float> &out, int offset)
{
	if (!inputDevice_) {
		return false;
	}

	double dur = (double)sampleCnt / (double)freq_;
	// TODO : check if it's still necessary to oversize the buffer
	if (dur > .9 * (double)bufSecLen_) {
		fprintf(stderr, "%s internal buffer too small\n", __func__);
		return false;
	}

	if (availableSampleCnt() < sampleCnt) {
		return false;
	}

	if (out.size() < offset + sampleCnt) {
		out.resize(offset + sampleCnt);
	}

#if defined (USE_MONO16_FALLBACK)
	if (format_ == AL_FORMAT_MONO_FLOAT32) {
		alcCaptureSamples(inputDevice_, out.data() + offset, sampleCnt);
	} else { // AL_FORMAT_MONO16
		// necessary for android < 5
		// Cf https://github.com/kcat/openal-soft/issues/293
		tmpBuf_.resize(sampleCnt);
		alcCaptureSamples(inputDevice_, tmpBuf_.data(), sampleCnt);
		for (int i = 0; i < sampleCnt; i++) {
			out[offset + i] = float(tmpBuf_[i]) * (1.f / 32768.f);
		}
	}
#else
	alcCaptureSamples(inputDevice_, out.data() + offset, sampleCnt);
#endif

	auto error = alGetError();
	if (error != AL_NO_ERROR) {
		printAlError("alBufferData", error);
		return false;
	}

	return true;
}
