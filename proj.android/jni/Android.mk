LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Warning flags.
# LOCAL_CFLAGS += -Wdouble-promotion -Wignored-qualifiers -Wundef
LOCAL_CFLAGS += -Wchar-subscripts -Wcomment -Wformat=2 -Wnonnull -Winit-self
LOCAL_CFLAGS += -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point
LOCAL_CFLAGS += -Wreturn-type -Waddress -Wmissing-declarations -Wmissing-field-initializers
LOCAL_CFLAGS += -Wunused-function -Wunused-label -Wuninitialized

LOCAL_MODULE := ee_static

LOCAL_MODULE_FILENAME := libee

# FILE_LIST_BEGIN

INCREMENT_BUILD := \
../../Classes/EEBackButtonListener.cpp \
../../Classes/EEBoxBlur1D.cpp \
../../Classes/EEBoxBlur2D.cpp \
../../Classes/EECheckBox.cpp \
../../Classes/EECocosUtils.cpp \
../../Classes/EEControl.cpp \
../../Classes/EECrc.cpp \
../../Classes/EEData.cpp \
../../Classes/EEDialog.cpp \
../../Classes/EEDialogManager.cpp \
../../Classes/EEDynamicValue.cpp \
../../Classes/EEEvent.cpp \
../../Classes/EEGesture.cpp \
../../Classes/EEGroupNode.cpp \
../../Classes/EEImage.cpp \
../../Classes/EESha1.cpp \
../../Classes/EETentBlur1D.cpp \
../../Classes/EETouchListener.cpp \
../../Classes/EETouchManager.cpp

# FILE_LIST_END

UNIVERSAL_BUILD := ee-everything.cpp

LOCAL_SRC_FILES := $(INCREMENT_BUILD)
                   
LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../../cocos2d \
$(LOCAL_PATH)/../../../cocos2d/cocos \
$(LOCAL_PATH)/../../../cocos2d/cocos/2d \
$(LOCAL_PATH)/../../../cocos2d/cocos/editor-support \
$(LOCAL_PATH)/../../../cocos2d/cocos/ui

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../Classes

include $(BUILD_STATIC_LIBRARY)
