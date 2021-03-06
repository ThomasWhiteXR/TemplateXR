// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.


#include "PXR_Shaders.h"

#if ENGINE_MINOR_VERSION > 24
IMPLEMENT_SHADER_TYPE(, FMRCPSMipLevel, TEXT("/Plugin/PicoXR/Private/PicoShaders.usf"), TEXT("MainMipLevel"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMRCPSsRGBSourceMipLevel, TEXT("/Plugin/PicoXR/Private/PicoShaders.usf"), TEXT("MainsRGBSourceMipLevel"), SF_Pixel);
#else
IMPLEMENT_SHADER_TYPE(, FMRCPS, TEXT("/Plugin/PicoXR/Private/PicoShaders.usf"), TEXT("Main"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMRCPSsRGBSource, TEXT("/Plugin/PicoXR/Private/PicoShaders.usf"), TEXT("MainsRGBSource"), SF_Pixel);
#endif
