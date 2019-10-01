// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#ifndef ALUTIL_H
#define ALUTIL_H

#include <cstdio>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

const char *getAlErrorStr(ALenum error);

#define printAlError_(pream, msg, err) \
	do { \
	fprintf(stderr, "%s : %s : %s\n", pream, msg, getAlErrorStr(err)); \
	} while(0)

#define printAlError(msg, err) printAlError_(__PRETTY_FUNCTION__, msg, err)

class AlLib
{
public:
	bool init();
	void free();
private:
	ALCcontext *audioContext_ = nullptr;
	ALCdevice *audioDevice_ = nullptr;
};

#endif // ALUTIL_H
