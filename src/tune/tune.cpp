// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "tune.h"

constexpr float Tune::minFreq_;
constexpr float Tune::maxFreq_;
constexpr int Tune::captureFreq_;

Tune::Tune()
{
	alInit_ = al_.init();
	buf_.reserve(5.5e5);
}

Tune::~Tune()
{
	freeCapture();
	freeAnalyze();
	al_.free();
}

std::vector<std::string> Tune::getCaptureDeviceNames()
{
	if (!alInit_) {
		return std::vector<std::string>{};
	}
	return Capture::getCaptureDeviceNames();
}

bool Tune::initCapture(const char *captureDeviceName)
{
	if (!alInit_) {
		return false;
	}

	if (capt_) {
		stopCapture();
		freeCapture();
	}

	capt_ = new Capture;

	capt_->setFreq(captureFreq_);

	if (!capt_->init(captureDeviceName)) {
		freeCapture();
		return false;
	}

	return true;
}

void Tune::freeCapture()
{
	if (capt_) {
		capt_->free();
		delete capt_;
		capt_ = nullptr;
	}
}

void Tune::startCapture()
{
	if (capt_) {
		capt_->start();
	}
}

void Tune::stopCapture()
{
	if (capt_) {
		capt_->stop();
		buf_.resize(0);
	}
}

bool Tune::initAnalyze(int msWinLen, int msSlideLen)
{
	if (!capt_) {
		return false;
	}

	msWinLen_ = msWinLen;
	msSlideLen_ = msSlideLen;

	float fac = (float)msWinLen_ / 1000.f;
	sampleWinLen_ = capt_->freq() * fac;

	fac = (float)msSlideLen_ / 1000.f;
	sampleSlideLen_ = capt_->freq() * fac;

	res_ = (float)capt_->freq() / (float)sampleWinLen_;

	loBin_ = std::floor(minFreq_ / res_);
	hiBin_ = std::ceil(maxFreq_ / res_);

	if (ana_ && ana_->sampleCnt() != sampleWinLen_) {
		freeAnalyze();
	}
	if (!ana_) {
		ana_ = new Analyze(sampleWinLen_);
	}

	return true;
}

void Tune::freeAnalyze()
{
	if (ana_) {
		delete ana_;
		ana_ = nullptr;
	}
}

bool Tune::sampleReady()
{
	if (!capt_) {
		return false;
	}
	int wanted = buf_.empty() ? sampleWinLen_ : sampleSlideLen_;
	return capt_->availableSampleCnt() >= wanted;
}

float Tune::sampleOne()
{
	int wanted = buf_.empty() ? sampleWinLen_  : sampleSlideLen_;
	if (!capt_->getSample(wanted, buf_, buf_.size())) {
		fprintf(stderr, "%s : capture failed\n", __PRETTY_FUNCTION__);
		return NAN;
	}

	if (buf_.size() > 5e5) {
		std::copy(buf_.end() - ana_->sampleCnt(),
			  buf_.end(), buf_.begin());
		buf_.resize(ana_->sampleCnt());
	}

	int offset = buf_.size() - ana_->sampleCnt();
	float bin = ana_->mpm(buf_.data() + offset, loBin_, hiBin_);
	if (std::isnan(bin)) {
		return NAN;
	}

	// Hz to midi notes :
	// from tartini's comment : http://www.borg.com/~jglatt/tutr/notenum.htm
	float freq = bin * res_;
	float note = -36.3763165622959152488 + 12.0 * std::log2(freq);
	return note;
}
