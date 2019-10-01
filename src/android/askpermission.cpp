// SPDX-License-Identifier: GPL-2.0-only
/*
 * This file is part of rGuitartuner.
 * Copyright (c) 2019 Rafael Gandolfi  <rafirafi.at@gmail.fr>
 */

#include "askpermission.h"

#include <QtAndroid>

bool requestRecordPermission()
{
	const QStringList perm({"android.permission.RECORD_AUDIO",});
	auto result = QtAndroid::checkPermission(perm[0]);
	if (result != QtAndroid::PermissionResult::Denied) {
		return true;
	}
	auto resultHash = QtAndroid::requestPermissionsSync(perm);
	return resultHash[perm[0]] != QtAndroid::PermissionResult::Denied;
}
