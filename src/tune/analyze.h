// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef ANALYZE_H
#define ANALYZE_H

#include <array>
#include <vector>

#include <fftw3.h>

class Analyze
{
public:
	explicit Analyze(int sampleCnt);
	~Analyze();
	int sampleCnt() const;
	float mpm(float *in, int loBin, int hiBin);
private:
	int sampleCnt_;
	float *fftIn_ = nullptr;
	fftwf_complex *fftOut_ = nullptr;	
	fftwf_plan plan_ = nullptr;
	float *fftOut2_ = nullptr;
	fftwf_plan plan2_ = nullptr;
};

#endif // ANALYZE_H
