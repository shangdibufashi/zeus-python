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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC")
# option(WITH_BENCHMARK "Build with benchmark code" OFF) #for RT
# add_compile_options(-Wno-vla-cxx-extension)
add_compile_options(-Wno-constant-logical-operand)
add_compile_options(-Wno-packed-non-pod)
add_compile_options(-Wno-misleading-indentation)
add_compile_options(-Wno-deprecated-declarations)

# Set required C and C++ standards and check GCC version:
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# message(STATUS "WARNING: gcc ${CMAKE_CXX_COMPILER_VERSION} is known to miscompile RawTherapee when using --ffp-contract=fast, forcing the option to be off")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffp-contract=off")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffp-contract=off")
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
# Set special compilation flags for rtengine which get added to CMAKE_CXX_FLAGS:
# Some Linux distros build with -O2 instead of -O3. We explicitly enable auto
# vectorization by using -ftree-vectorize
set(RTENGINE_CXX_FLAGS
    "-ftree-vectorize"
    CACHE STRING "Special compilation flags for RTEngine")
# Stop compilation on typos such as std:swap (missing colon will be detected as
# unused label):
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=unused-label")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=delete-incomplete")

# Do net set math errno, as we never check its value.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-math-errno")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-math-errno")

# suppress warning https://github.com/Beep6581/RawTherapee/issues/6105
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
add_definitions(-DMS_WIN64)
add_definitions(-DWIN64)
set(EXTRA_LIB "Ws2_32")

message(STATUS "current platform: ${CMAKE_SYSTEM_NAME} ")

# find_package(OpenMP REQUIRED)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

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
    # ${Python_INCLUDE_DIRS}
    # ${pybind11_INCLUDE_DIRS}
)

# 链接库配置
target_link_libraries(zeus PRIVATE
    ${PYTHON_LIBRARY_FILE}
    ${Python_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/zeus/libairaw.dll.a
)

# MinGW特殊配置
# target_link_libraries(zeus PRIVATE gcc mingwex msvcrt)
# add_compile_options(-Wa,-mbig-obj)
# add_link_options(-static-libgcc -static-libstdc++)

