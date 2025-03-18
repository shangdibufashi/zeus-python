cmake_minimum_required(VERSION 3.31)
project(zeus_ext LANGUAGES CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0148 NEW)
set(CMAKE_STRIP "echo")

# set(CMAKE_VERBOSE_MAKEFILE ON)

string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)

add_definitions(-DWIN64)
add_definitions(-DWIN32)

if (UPPER_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message(STATUS " -- DEBUG -- ")
    add_definitions(-DEBUG)
    add_definitions(-D_DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wno-all") # C++ 编译器设置
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wno-all") # C 编译器设置
else ()
    message(STATUS " -- RELEASE -- ")
    add_definitions(-DNDEBUG)
    add_definitions(-D_DNDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wno-all")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wno-all")
endif ()

# 
# 如何在mingw64中的clang上面编译出兼容MSVC的动态库?
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC")
# option(WITH_BENCHMARK "Build with benchmark code" OFF) #for RT
# add_compile_options(-Wno-vla-cxx-extension)
add_compile_options(-Wno-constant-logical-operand)
add_compile_options(-Wno-packed-non-pod)
add_compile_options(-Wno-misleading-indentation)
add_compile_options(-Wno-deprecated-declarations)

# We might want to build using the old C++ ABI, even when using a new GCC
# version:
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
# By default we don't use a specific processor target, so PROC_TARGET_NUMBER is
# set to 0. Specify other values to optimize for specific processor architecture
# as listed in ProcessorTargets.cmake:
set(PROC_TARGET_NUMBER
    0 CACHE STRING
    "Selected target processor from the list above (taken from ProcessorTargets.cmake)"
)

# suppress warning https://github.com/Beep6581/RawTherapee/issues/6105
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
add_definitions(-DMS_WIN64)
add_definitions(-DWIN64)
set(EXTRA_LIB "Ws2_32")

message(STATUS "current platform: ${CMAKE_SYSTEM_NAME} ")

find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

# 查找依赖库
# find_package(PkgConfig)


# 查找Python依赖
message(STATUS "Python root: ${Python_EXECUTABLE}")
find_package(Python REQUIRED COMPONENTS Interpreter Development)

message(STATUS "Python include dir: ${PYTHON_INCLUDE_DIR}")
message(STATUS "Python libraries: ${PYTHON_LIBRARY}")
message(STATUS "Python lib file: ${PYTHON_LIBRARY_FILE}")
message(STATUS "Python root: ${Python_ROOT_DIR}")
include_directories("$PYTHON_INCLUDE_DIR" "${PYTHON_INCLUDE_DIR}" "zeus")
link_directories("PYTHON_LIBRARY" "${PYTHON_LIBRARY}" "zeus")

list(APPEND CMAKE_PREFIX_PATH "${Python_ROOT_DIR}/Lib/site-packages/pybind11/share/cmake/pybind11/")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# find_package(pybind11 REQUIRED CONFIG COMPONENTS numpy OPTIONAL_COMPONENTS stl)
find_package(pybind11 REQUIRED) 
message(STATUS "pybind11_INCLUDE_DIRS: ${pybind11_INCLUDE_DIRS}")

# 创建扩展模块
pybind11_add_module(zeus zeus/zeus.cpp)

# 目标属性配置
set_target_properties(zeus PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    SUFFIX ".pyd"
    PREFIX ""
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# 包含目录
target_include_directories(zeus PRIVATE
    #${Python_INCLUDE_DIRS}
    #${pybind11_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接库配置
target_link_libraries(zeus PRIVATE
    #${PYTHON_LIBRARY_FILE}
    #${Python_LIBRARIES}
    ${OpenCV_LIBS}
    ${CMAKE_SOURCE_DIR}/zeus/libairaw.dll.a
)
