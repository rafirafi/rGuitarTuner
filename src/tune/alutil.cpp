// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "alutil.h"

const char *getAlErrorStr(ALenum error)
{
	switch (error) {
	case AL_NO_ERROR:
		return "NO_ERROR";
	case AL_INVALID_NAME:
		return "INVALID_NAME";
	case AL_INVALID_ENUM:
		return "INVALID_ENUM";
	case AL_INVALID_VALUE:
		return "INVALID_VALUE";
	case AL_INVALID_OPERATION:
		return "INVALID_OPERATION";
	case AL_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
	}

	return "UNKNOWN";
}

bool AlLib::init()
{
	audioDevice_ = alcOpenDevice(nullptr);
	if (!audioDevice_) {
		printAlError("alcOpenDevice", alGetError());
		return false;
	}

	audioContext_ = alcCreateContext(audioDevice_, nullptr);
	if (!audioContext_) {
		printAlError("alcCreateContext", alGetError());
		return false;
	}

	if (alcMakeContextCurrent(audioContext_) != ALC_TRUE) {
		printAlError("alcMakeContextCurrent", alGetError());
		return false;
	}

	if (alcIsExtensionPresent(nullptr, "ALC_EXT_capture") != ALC_TRUE) {
		printAlError("alcIsExtensionPresent ALC_EXT_capture",
			     alGetError());
		return false;
	}

	if (alIsExtensionPresent("AL_EXT_FLOAT32") != AL_TRUE) {
		printAlError("alIsExtensionPresent AL_EXT_float32",
			     alGetError());
		return false;
	}

	return true;
}

void AlLib::free()
{
	if (audioContext_) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(audioContext_);
		audioContext_ = nullptr;
	}

	if (audioDevice_) {
		alcCloseDevice(audioDevice_);
		audioDevice_ = nullptr;
	}
}
