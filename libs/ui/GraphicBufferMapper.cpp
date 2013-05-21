/*
 * Copyright (C) 2007 The Android Open Source Project
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

#define LOG_TAG "GraphicBufferMapper"

#include <stdint.h>
#include <errno.h>

#include <utils/Errors.h>
#include <utils/Log.h>

#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>

#include <hardware/gralloc.h>

#ifdef MTK_HARDWARE
#include <utils/String8.h>
#include <cutils/properties.h>
#endif//MTK_HARDWARE

namespace android {
// ---------------------------------------------------------------------------

ANDROID_SINGLETON_STATIC_INSTANCE( GraphicBufferMapper )

#ifdef MTK_HARDWARE
Mutex GraphicBufferMapper::sLock;
KeyedVector<buffer_handle_t,
    GraphicBufferMapper::reg_rec_t> GraphicBufferMapper::sRegList;
#endif//MTK_HARDWARE
GraphicBufferMapper::GraphicBufferMapper()
    : mAllocMod(0)
{
    hw_module_t const* module;
    int err = hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module);
    LOGE_IF(err, "FATAL: can't find the %s module", GRALLOC_HARDWARE_MODULE_ID);
    if (err == 0) {
        mAllocMod = (gralloc_module_t const *)module;
    }
}

status_t GraphicBufferMapper::registerBuffer(buffer_handle_t handle)
{
    status_t err;

    err = mAllocMod->registerBuffer(mAllocMod, handle);

    LOGW_IF(err, "registerBuffer(%p) failed %d (%s)",
            handle, err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::unregisterBuffer(buffer_handle_t handle)
{
    status_t err;

    err = mAllocMod->unregisterBuffer(mAllocMod, handle);

    LOGW_IF(err, "unregisterBuffer(%p) failed %d (%s)",
            handle, err, strerror(-err));
#ifdef MTK_HARDWARE
    if (err == NO_ERROR) {
        Mutex::Autolock _l(sLock);
        KeyedVector<buffer_handle_t, reg_rec_t>& list(sRegList);
        list.removeItem(handle);
    }
#endif//MTK_HARDWARE
    return err;
}

status_t GraphicBufferMapper::lock(buffer_handle_t handle, 
        int usage, const Rect& bounds, void** vaddr)
{
    status_t err;
#ifdef MISSING_GRALLOC_BUFFERS
    int tries=5;
#endif

    err = mAllocMod->lock(mAllocMod, handle, usage,
            bounds.left, bounds.top, bounds.width(), bounds.height(),
            vaddr);
#ifdef MISSING_GRALLOC_BUFFERS
    while (err && tries) {
	usleep(1000);
        err = mAllocMod->unlock(mAllocMod, handle);
        err = mAllocMod->lock(mAllocMod, handle, usage,
            bounds.left, bounds.top, bounds.width(), bounds.height(),
            vaddr);
	tries--;
    }
#endif

    LOGW_IF(err, "lock(...) failed %d (%s)", err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::unlock(buffer_handle_t handle)
{
    status_t err;

    err = mAllocMod->unlock(mAllocMod, handle);

    LOGW_IF(err, "unlock(...) failed %d (%s)", err, strerror(-err));
    return err;
}
#ifdef MTK_HARDWARE
status_t GraphicBufferMapper::registerBuffer(buffer_handle_t handle,
    int w, int h, int stride, int format, int usage)
{
    Mutex::Autolock _l(sLock);
    
    KeyedVector<buffer_handle_t, reg_rec_t>& list(sRegList);
    int bpp = bytesPerPixel(format);
    if (bpp < 0) {
        // probably a HAL custom format. in any case, we don't know
        // what its pixel size is.
        bpp = 0;
    }
    reg_rec_t rec;
    rec.w = w;
    rec.h = h;
    rec.s = stride;
    rec.format = format;
    rec.usage = usage;
    rec.size = h * stride * bpp;
    list.add(handle, rec);   
    return NO_ERROR;
}
#endif//MTK_HARDWARE

#ifdef EXYNOS4210_ENHANCEMENTS
status_t GraphicBufferMapper::getphys(buffer_handle_t handle, void** paddr)
{
    status_t err;

    err = mAllocMod->getphys(mAllocMod, handle, paddr);

    LOGW_IF(err, "getphys(%p) fail %d(%s)",
            handle, err, strerror(-err));
    return err;
}
#endif

// ---------------------------------------------------------------------------
}; // namespace android
