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

#ifndef ANDROID_SF_WATCHDOG
#define ANDROID_SF_WATCHDOG

#include <utils/threads.h>
#include "SurfaceFlinger.h"

namespace android {

class SFWatchDog : public Thread
{
public:
                SFWatchDog(const sp<SurfaceFlinger>& flinger);
    virtual     ~SFWatchDog();

    bool        isSFThreadHang(nsecs_t& ct);
    void        markStartTransactionTime();
    void        unmarkStartTransactionTime();
    void        screenReleased(DisplayID dpy);
    void        screenAcquired(DisplayID dpy);
            
private:
    virtual bool        threadLoop();
    virtual status_t    readyToRun();
    virtual void        onFirstRef();
    
    nsecs_t             mStartTransactionTime;

    sp<SurfaceFlinger>  mFlinger;
    mutable Mutex       mLock;
    mutable Mutex       mScreenLock;

    void                getProperty();
    uint32_t            mThreshold;
    uint32_t            mTimer;
    bool                mShowLog;
};

}; // namespace android

#endif //ANDROID_SF_WATCHDOG
