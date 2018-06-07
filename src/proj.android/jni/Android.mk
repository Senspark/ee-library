LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Warning flags.
# LOCAL_CFLAGS += -Wdouble-promotion -Wignored-qualifiers -Wundef
LOCAL_CFLAGS += -Wchar-subscripts -Wcomment -Wformat=2 -Wnonnull -Winit-self
LOCAL_CFLAGS += -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point
LOCAL_CFLAGS += -Wreturn-type -Waddress -Wmissing-declarations -Wmissing-field-initializers
LOCAL_CFLAGS += -Wunused-function -Wunused-label -Wuninitialized

LOCAL_CPPFLAGS += -std=c++1z

LOCAL_MODULE := ee_static

LOCAL_MODULE_FILENAME := libee

LOCAL_SRC_FILES := ${shell find $(LOCAL_PATH)/../../Classes -name "*.cpp" -print}
                   
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../cocos2d \
$(LOCAL_PATH)/../../cocos2d/cocos \
$(LOCAL_PATH)/../../cocos2d/cocos/editor-support \
$(LOCAL_PATH)/../../cocos2d/cocos/platform/android/jni \
$(LOCAL_PATH)/../../cocos2d/cocos/ui \
$(LOCAL_PATH)/../../cocos2d/extensions \
$(LOCAL_PATH)/../../cocos2d/external

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Classes

include $(BUILD_STATIC_LIBRARY)
