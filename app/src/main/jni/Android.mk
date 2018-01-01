LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE := jni_cloudpos_printer
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_SRC_FILES += printer/jni_register.cpp 
LOCAL_SRC_FILES += printer/printer_jni_interface.cpp 
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jni_cloudpos_cashdrawer
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_SRC_FILES += cashdrawer/moneybox_jni_register.cpp
LOCAL_SRC_FILES += cashdrawer/moneybox_jni_interface.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE := jni_cloudpos_secondarydisplay
LOCAL_SRC_FILES += hal_sys_log.c 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/secondarydisplay
LOCAL_SRC_FILES += secondarydisplay/customer_display_jni_interface.cpp
LOCAL_SRC_FILES += secondarydisplay/customer_display_jni_register.cpp
LOCAL_LDLIBS    := -llog

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE    := jni_cloudpos_pinpad
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/pinpad
LOCAL_SRC_FILES += pinpad/jni_register.cpp 
LOCAL_SRC_FILES += pinpad/pinpad_jni_interface.cpp 
LOCAL_SRC_FILES += pinpad/prove/DES.cpp
LOCAL_SRC_FILES += pinpad/prove/pinpad_utility.cpp
LOCAL_SRC_FILES += pinpad/prove/pinpad_test.cpp
LOCAL_LDLIBS := -lm -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE    := jni_cloudpos_smartcard
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/smartcard
LOCAL_SRC_FILES += smartcard/smart_card_jni_interface.cpp 
LOCAL_SRC_FILES += smartcard/smart_card_jni_register.cpp
LOCAL_SRC_FILES += smartcard/smart_card_event.cpp 
LOCAL_LDLIBS := -lm -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE    := jni_cloudpos_rfcard
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/contactlesscard
LOCAL_SRC_FILES += contactlesscard/contactless_card_jni_register.cpp 
LOCAL_SRC_FILES += contactlesscard/contactless_card_jni_interface.cpp 
LOCAL_SRC_FILES += contactlesscard/contactless_card_event.cpp
LOCAL_LDLIBS := -lm -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE := jni_cloudpos_msr
LOCAL_SRC_FILES += hal_sys_log.c 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/msr
LOCAL_SRC_FILES += msr/msr_jni_interface.cpp
LOCAL_SRC_FILES += msr/msr_jni_register.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CFLAGS = -Wno-psabi
LOCAL_MODULE := jni_cloudpos_serial
LOCAL_SRC_FILES += hal_sys_log.c 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/serial
LOCAL_SRC_FILES += serial/serial_port_jni_interface.cpp
LOCAL_SRC_FILES += serial/serial_port_jni_register.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jni_cloudpos_idcard
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_SRC_FILES += identitycard/identity_card_jni_register.cpp
LOCAL_SRC_FILES += identitycard/identity_card_jni_interface.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jni_cloudpos_led
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_SRC_FILES += led/led_jni_register.cpp
LOCAL_SRC_FILES += led/led_jni_interface.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jni_cloudpos_fingerprint
LOCAL_SRC_FILES += hal_sys_log.c
LOCAL_SRC_FILES += fingerprint/fingerprint_jni_register.cpp
LOCAL_SRC_FILES += fingerprint/fingerprint_jni_interface.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


ifeq ($(TARGET_ARCH_ABI),armeabi)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/prebuilt/Android.mk
endif 

Only compile-time support for the following framework (remove armeabi, mips64, mips)
APP_ABI := arm64-v8a armeabi-v7a x86 x86_64