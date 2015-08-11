
LOCAL_PATH	:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= egis_diagnostic

# Add your source files here (relative paths)
LOCAL_SRC_FILES	+= ../diagnostec_tool/main.cpp \
				   ../diagnostec_tool/driver_io.cpp \
				   ../diagnostec_tool/image_manager.cpp \
				   ../diagnostec_tool/sensor_control.cpp

include $(BUILD_EXECUTABLE)
