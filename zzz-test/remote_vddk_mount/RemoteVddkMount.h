#pragma once

#include "MountParams.h"
#include "VolumeOperationCallback.h"

HRESULT RemoteVddkMount(CONST MountParams & params, IVolumeOperationCallback * pVolCallback);