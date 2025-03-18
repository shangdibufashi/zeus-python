import os
from setuptools import setup, Extension, find_packages
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
import numpy
from Cython.Build import cythonize

# 获取当前目录
pwd = os.path.abspath(os.path.dirname(__file__))

# 检测架构，默认为 x86_64
arch = os.environ.get('PYTHON_ARCH', 'x86_64')

# 根据架构设置 OpenCV 路径
if arch == 'arm64':
    opencv_include = '/opt/homebrew/include/opencv4'
    opencv_lib = '/opt/homebrew/lib'
    platform_name = 'macosx_14_0_arm64'
elif arch == 'x86_64':
    opencv_include = '/usr/local/include/opencv4'
    opencv_lib = '/usr/local/lib'
    platform_name = 'macosx_11_0_x86_64'
else:
    raise ValueError(f"不支持的架构: {arch}")

assert os.path.exists(opencv_include), f"OpenCV 头文件路径不存在: {opencv_include}"
assert os.path.exists(opencv_lib), f"OpenCV 库文件路径不存在: {opencv_lib}"

# 获取 pybind11 的 include 路径
class get_pybind_include(object):
    def __init__(self, user=False):
        self.user = user
    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)

# 自定义 bdist_wheel，确保生成正确的平台标签
class bdist_wheel(_bdist_wheel):
    def finalize_options(self):
        super().finalize_options()
        self.root_is_pure = False
        self.plat_name = platform_name

# 定义扩展模块
extensions = cythonize([
    Extension(
        "zeus.zeus",
        sources=['zeus/zeus.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            numpy.get_include(),
            opencv_include  # 动态设置 OpenCV 头文件路径
        ],
        libraries=['airaw', 'opencv_core', 'opencv_imgproc'],
        library_dirs=[os.path.abspath('zeus/'), opencv_lib],  # 动态设置 OpenCV 库路径
        extra_compile_args=[
            '-std=c++17',
            '-stdlib=libc++',
            '-arch', arch
        ],
        extra_link_args=[
            '-Wl,-rpath,@loader_path',
        ]
    )
])

# 设置 setup 参数
setup(
    name='zeus.zeus',
    version="1.0.1",
    cmdclass={'bdist_wheel': bdist_wheel},
    ext_modules=extensions,
    packages=find_packages(),
    package_dir={'zeus': 'zeus'},
    package_data={
        'zeus': [
            'libairaw.dylib',
        ]
    },
    include_package_data=True,
    install_requires=[
        'numpy>=1.18',
        'Cython>=0.29',
        'pybind11>=2.6'
    ],
    zip_safe=False,
    classifiers=[
        'Operating System :: MacOS :: MacOS X',
    ]
)