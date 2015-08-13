
LOCAL_PATH	:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= egis_diagnostic

# Add your source files here (relative paths)
LOCAL_SRC_FILES	+= ../diagnostic_tool/main.cpp \
				   ../diagnostic_tool/driver_io.cpp \
				   ../diagnostic_tool/image_manager.cpp \
				   ../diagnostic_tool/sensor_control.cpp

include $(BUILD_EXECUTABLE)
