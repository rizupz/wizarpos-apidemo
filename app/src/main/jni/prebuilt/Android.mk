LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := DecodeWlt
LOCAL_SRC_FILES := libDecodeWlt.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include 
include $(PREBUILT_SHARED_LIBRARY)