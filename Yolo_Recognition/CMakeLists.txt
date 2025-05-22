cmake_minimum_required(VERSION 3.16)

project(Yolo_Recognition VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# -------------- 支持 C++17 以使用文件系统 ------------------#
set(CMAKE_CXX_STANDARD 17)  # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定的C++标准
set(CMAKE_CXX_EXTENSIONS ON)  # 启用C++扩展




find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)



# -------------- OpenCV ------------------#
include_directories("C:/Users/Ld/Desktop/Cjiajia/souko/opencv/build/include")
set(OPENCV_LIB_DIR "C:/Users/Ld/Desktop/Cjiajia/souko/opencv/build/x64/vc16/lib")
file(GLOB OPENCV_LIBS "${OPENCV_LIB_DIR}/*.lib")


# -------------- 如果需要，编译CUDA以进行FP16推理 ------------------#
option(USE_CUDA "Enable CUDA support" ON)  # 设置选项以启用CUDA支持
if (NOT APPLE AND USE_CUDA)  # 如果不是苹果系统且启用CUDA
    include_directories("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8/include")
    set(CUDA_LIBRARIE "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8/lib")
    file(GLOB_RECURSE CUDA_LIBS "${CUDA_LIBRARIE}/*.lib")

    add_definitions(-DUSE_CUDA)  # 当编译器看到这个宏定义后，会在编译过程中将 USE_CUDA 视为已定义的宏
else ()
    set(USE_CUDA OFF)  # 禁用CUDA
endif ()

# 遍历所有文件并打印以.lib结尾的文件
# foreach(FILE ${CUDA_LIBS})
#     if(FILE MATCHES "\\.lib$")
#         message(STATUS "Found lib file: ${FILE}")
#     endif()
# endforeach()


# -------------- ONNX Runtime ------------------#
if (WIN32)  # 如果是Windows系统
    if (USE_CUDA)  # 如果启用CUDA
        include_directories("C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime-win-x64-gpu-1.17.1/include")
        set(ONNXRUNTIME_LIB_DIR "C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime-win-x64-gpu-1.17.1/lib")  # 设置ONNX Runtime根目录
        file(GLOB ONNXRUNTIME_LIBS "${ONNXRUNTIME_LIB_DIR}/*.lib")


    else ()
        include_directories("C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime-win-x64-1.17.1/include")
        set(ONNXRUNTIME_LIB_DIR "C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime-win-x64-1.17.1/lib")  # 设置ONNX Runtime根目录
        file(GLOB ONNXRUNTIME_LIBS "${ONNXRUNTIME_LIB_DIR}/*.lib")

    endif ()
endif ()

# 遍历所有文件并打印以.lib结尾的文件
foreach(FILE ${ONNXRUNTIME_LIBS})
    if(FILE MATCHES "\\.lib$")
        message(STATUS "Found lib file: ${FILE}")
    endif()
endforeach()




# --------------------------------#
set(PROJECT_SOURCES
        main.cpp
        yolo_recognition.cpp
        yolo_recognition.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Yolo_Recognition
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        recognition_draw.h recognition_draw.cpp
        inference.cpp inference.h
        execute_inference.h execute_inference.cpp
        thread_control.h thread_control.cpp



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Yolo_Recognition APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Yolo_Recognition SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Yolo_Recognition
            ${PROJECT_SOURCES}
        )
    endif()
endif()


# -------------- 链接 ------------------#
if (WIN32)  # 如果是Windows系统
    target_link_libraries(Yolo_Recognition PRIVATE  ${ONNXRUNTIME_LIBS}) #ONNX Runtime库
    if (USE_CUDA)  # 如果启用CUDA
        target_link_libraries(Yolo_Recognition PRIVATE ${CUDA_LIBS})  # 链接CUDA库
    endif ()
endif ()

target_link_libraries(Yolo_Recognition PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Yolo_Recognition PRIVATE  ${OPENCV_LIBS})

target_link_libraries(Yolo_Recognition PRIVATE Qt6::OpenGLWidgets)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Yolo_Recognition)
endif()
set_target_properties(Yolo_Recognition PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Yolo_Recognition
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Yolo_Recognition)
endif()
