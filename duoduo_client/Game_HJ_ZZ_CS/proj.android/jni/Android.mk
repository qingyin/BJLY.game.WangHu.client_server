LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../GameBase/GameLib)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := hellocpp/main.cpp  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/ClientHN_SC/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/ClientHN_SC/Main/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/ClientHN_SC/Scene/*.cpp)   
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../GameBase/GameLib/JniCross/*.cpp)  
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes/ClientHN_SC/Main
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../GameBase/GameLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../GameBase/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += android_support
LOCAL_STATIC_LIBRARIES += GameLib_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,../../../GameBase/GameLib)

$(call import-module,android/support)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
