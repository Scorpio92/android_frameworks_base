LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	fillrate.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
    libEGL \
    libGLESv1_CM \
    libui

ifeq ($(strip $(BOARD_USES_MTK_HARDWARE)),true)
LOCAL_SHARED_LIBRARIES += libsurfaceflinger \
					      libandroid \
						  libgui
endif

LOCAL_MODULE:= test-opengl-fillrate

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
