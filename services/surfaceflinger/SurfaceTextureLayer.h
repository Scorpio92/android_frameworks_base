/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_SURFACE_TEXTURE_LAYER_H
#define ANDROID_SURFACE_TEXTURE_LAYER_H

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include <utils/Errors.h>
#include <gui/SurfaceTexture.h>

#ifdef MTK_HARDWARE
#include "DisplayHardware/HWComposer.h"
#endif//MTK_HARDWARE
namespace android {
// ---------------------------------------------------------------------------

class Layer;

class SurfaceTextureLayer : public SurfaceTexture
{
    wp<Layer> mLayer;
    uint32_t mDefaultFormat;

public:
    SurfaceTextureLayer(GLuint tex, const sp<Layer>& layer);
    ~SurfaceTextureLayer();

    status_t setDefaultBufferSize(uint32_t w, uint32_t h);
    status_t setDefaultBufferFormat(uint32_t format);

public:
    virtual status_t setBufferCount(int bufferCount);

protected:
#ifdef QCOM_HARDWARE
    virtual int query(int what, int* value);
#endif

    virtual status_t queueBuffer(int buf, int64_t timestamp,
            uint32_t* outWidth, uint32_t* outHeight, uint32_t* outTransform);

    virtual status_t dequeueBuffer(int *buf, uint32_t w, uint32_t h,
            uint32_t format, uint32_t usage);

    virtual status_t connect(int api,
            uint32_t* outWidth, uint32_t* outHeight, uint32_t* outTransform);

#ifdef MTK_HARDWARE
public:
    virtual status_t convertToAuxSlot(bool isForce) {
        return (true == isForce) ? SurfaceTexture::convertToAuxSlot(true) : -1;
    }

private:
    bool mLogBuffer;        // to switch debug log

private:
    virtual status_t bypassGL();

    HWComposer *mHWC;

public:
    void setHWC(HWComposer *hwc) { mHWC = hwc; }
#endif//MTK_HARDWARE
};

// ---------------------------------------------------------------------------
}; // namespace android

#endif // ANDROID_SURFACE_TEXTURE_LAYER_H
