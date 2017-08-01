LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../cocos2d/cocos)

LOCAL_MODULE := GameLib_static

LOCAL_MODULE_FILENAME := libGameLib

FILE_LIST := $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/kernel/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/kernel/game/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/kernel/server/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/kernel/user/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/network/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/network/client_net/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Kernel/socket/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFDefine/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFDefine/data/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFDefine/df/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFDefine/msg/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFKernel/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/LoadScene/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/LoginScene/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/ModeScene/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/ServerListScene/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Platform/PFView/ServerScene/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/pthread/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Tools/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Tools/core/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Tools/Dialog/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Tools/Manager/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Tools/tools/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/FV/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/HNMJ/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/NIUNIU/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/NCMJ/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/XZDD/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Game/SYMJ/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Script/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Widget/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Sound/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Sound/amr_nb/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/Game/Sound/amr_nb/*.c)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/GameNet/cocosHttp/*.cpp)  
FILE_LIST += $(wildcard $(LOCAL_PATH)/../GameLib/GameNet/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../Classes/ClientHN_THJ/Main
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../GameLib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += android_support

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)

$(call import-module,android/support)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
