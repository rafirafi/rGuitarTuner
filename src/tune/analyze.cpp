// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "analyze.h"

#include <cassert>
#include <cmath>

Analyze::Analyze(int sampleCnt)
	: sampleCnt_(sampleCnt)
{
	fftIn_ = (float *)fftwf_malloc(sizeof(float) * sampleCnt_);

	int outSize = sampleCnt_ / 2 + 1;
	fftOut_ = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex)
						* outSize);

	plan_ = fftwf_plan_dft_r2c_1d(sampleCnt_, fftIn_,
				      fftOut_, FFTW_ESTIMATE);

	fftOut2_ = (float *)fftwf_malloc(sizeof(float) * sampleCnt_);

	plan2_ = fftwf_plan_dft_c2r_1d(sampleCnt_, fftOut_,
				       fftOut2_, FFTW_ESTIMATE);
}

Analyze::~Analyze()
{
	if (plan2_) {
		fftwf_destroy_plan(plan2_);
		fftwf_cleanup();
	}

	if (fftOut2_) {
		fftwf_free(fftOut2_);
		fftOut2_ = nullptr;
	}

	if (plan_) {
		fftwf_destroy_plan(plan_);
		fftwf_cleanup();
	}

	if (fftOut_) {
		fftwf_free(fftOut_);
		fftOut_ = nullptr;
	}

	if (fftIn_) {
		fftwf_free(fftIn_);
		fftIn_ = nullptr;
	}
}

int Analyze::sampleCnt() const
{
	return sampleCnt_;
}

static inline float estimateInterBin(int bin, const float *data)
{
	float interBin = (data[bin + 1] - data[bin - 1]) * .5;
	interBin /= 2. * data[bin] - (data[bin + 1] + data[bin - 1]);
	return interBin + bin;
}

// Cf "A smarter way to find pitch" (2005) by Philip Mcleod , Geoff Wyvill
// mpm : Mcleod pitch method
float Analyze::mpm(float *in, int loBin, int hiBin)
{
	assert(loBin > 0 && hiBin > 0);

	int lo = std::floor((float)sampleCnt_ / (float)hiBin);
	int hi = std::ceil((float)sampleCnt_ / (float)loBin);

	std::vector<float> out(sampleCnt_);

	float mean = 0.f;
	for (int i = 0; i < sampleCnt_; i++) {
		mean += in[i];
	}
	mean /= float(sampleCnt_);

#if 0
	for (int i = 0; i < sampleCnt_; i++) {
		in[i] -= mean;
	}

	for (int i = 0; i < hi /*sampleCnt_*/; i++) {
		float r = 0.f, m = 0.f;
		for (int j = 0; j < sampleCnt_ - i; j++) {
			float a = in[j];
			float b = in[j + i];
			r += a * b;
			m += a * a + b * b;
		}
		out[i] = 2.f * r / m;
	}
#else
	for (int i = 0; i < sampleCnt_; i++) {
		fftIn_[i] = in[i] - mean;
	}

	fftwf_execute(plan_);
	for (int i = 0; i < sampleCnt_ / 2; i++) {
		fftOut_[i][0] = fftOut_[i][0] * fftOut_[i][0]
				+ fftOut_[i][1] * fftOut_[i][1];
		fftOut_[i][1] = 0.f;
	}
	fftwf_execute(plan2_);

	float m = 2.f * fftOut2_[0];
	for (int i = 0; i < hi; i++) {
		float r = fftOut2_[i];
		m -= in[i] * in[i];
		m -= in[sampleCnt_ - 1 - i] * in[sampleCnt_ - 1 - i];
		out[i] = 2.f * r / m;
	}
#endif

	// Peak picking
	// skip to first peak
	int loIdx  = 0;
	for (; loIdx < hi && out[loIdx] > 0.f; loIdx++) {
		;
	}
	// then get biggest peak
	int maxPeakIdx = loIdx;
	for (int i = loIdx + 1; i < hi; i++) {
		if (out[i] > out[maxPeakIdx]) {
			maxPeakIdx = i;
		}
	}
	// then get the fundamental peak in the range we're interested in
	const float k = 0.85f;
	float maxPeakVal = k * out[maxPeakIdx];
	maxPeakIdx = -1;
	for (int i = std::max(lo, loIdx), lPeak = -1; i < hi; i++) {
		if (lPeak != -1 && out[i] < 0.f) {
			maxPeakIdx = lPeak;
			break;
		}
		if (out[i] > maxPeakVal) {
			maxPeakVal = out[i];
			lPeak = i;
		}
	}

	float interBin = NAN;
	if (maxPeakIdx > 0) {
		// estimate real peak on the curve
		float mpmInterbin = estimateInterBin(maxPeakIdx, out.data());
		interBin = float(sampleCnt_) / mpmInterbin;
		// filter low amplitude result
		float peakVal = out[maxPeakIdx];
		float logRms = 20 * log10(fftOut2_[0] / float(sampleCnt_));
		fprintf(stderr, "peakCorr %f logRms %f\n", peakVal, logRms);
		float score = peakVal * logRms;
		if (score < 12.f) {
			interBin = NAN;
		} else {
			fprintf(stderr, "nsdf % 12.1f (%5d) score % 12.0f\n",
				interBin, maxPeakIdx, score);
		}
	}

	return interBin;
}
