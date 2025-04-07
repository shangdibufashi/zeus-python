#!/bin/bash
set -e -x


# export PYTHON_ARCH=arm64
# export MACOS_MIN_VERSION=12.0  


# Used by CMake, clang, and Python's distutils
export MACOSX_DEPLOYMENT_TARGET=$MACOS_MIN_VERSION

# The Python variant to install, see exception below.
export PYTHON_INSTALLER_MACOS_VERSION=$MACOS_MIN_VERSION

# By default, wheels are tagged with the architecture of the Python
# installation, which would produce universal2 even if only building
# for x86_64. The following line overrides that behavior.
export _PYTHON_HOST_PLATFORM="macosx-${MACOS_MIN_VERSION}-${PYTHON_ARCH}"

export CC=clang
export CXX=clang++
export CFLAGS="-arch ${PYTHON_ARCH}"
export CXXFLAGS=$CFLAGS
export LDFLAGS=$CFLAGS
export ARCHFLAGS=$CFLAGS

# Build wheel
which python3.10
python3.10 --version

echo "------------> start building wheel"
sleep 1
rm -rf build dist
python3.10 setup_macos14.py bdist_wheel
echo "------------> end building wheel"

delocate-listdeps --all --depending dist/*.whl > /dev/null # lists library dependencies
delocate-wheel --verbose --require-archs=${PYTHON_ARCH} dist/*.whl > /dev/null # copies library dependencies into wheel
delocate-listdeps --all --depending dist/*.whl # verify

python3.10 -m pip install dist/*.whl

# 获取目录中唯一的 .whl 文件
whl_file=$(ls dist/*.whl | head -n 1)
numpy_version=${NUMPY_VERSION//\*/}
new_whl_file="${whl_file%.whl}__np${numpy_version}.whl"
mv "$whl_file" "$new_whl_file"
echo "Renamed $whl_file to $new_whl_file"

pwd
cd ..
python3.10 zeus_test.py