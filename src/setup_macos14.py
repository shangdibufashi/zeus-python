import os
from setuptools import setup, Extension, find_packages
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
import numpy
from Cython.Build import cythonize
pwd = os.path.abspath(os.path.dirname(__file__))
arch = os.environ.get('PYTHON_ARCH', 'x86_64')

class get_pybind_include(object):
    def __init__(self, user=False):
        self.user = user  # 控制是否优先用户安装路径

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)  # 调用pybind11的路径获取方法

class bdist_wheel(_bdist_wheel):
    def finalize_options(self):
        super().finalize_options()
        self.root_is_pure = False
        self.plat_name = "macosx_14_0_arm64" if arch=='arm64' else "macosx_11_0_x86_64" 
        
extensions = cythonize([
    Extension("zeus.zeus",
        sources=['zeus/zeus.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            numpy.get_include()
        ],
        libraries=['airaw'],
        library_dirs=[os.path.abspath('zeus/')],
        extra_compile_args=[
            '-std=c++17',
            '-stdlib=libc++',
            '-arch', arch  # 关键：仅编译arm64架构[4](@ref)
        ],
        extra_link_args=[
            '-Wl,-rpath,@loader_path',  # 相对路径优先[1](@ref)
        ]
    )
])

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
        'pybind11>=2.6'  # 显式声明依赖[6](@ref)
    ],
    zip_safe=False,
    classifiers=[
        'Operating System :: MacOS :: MacOS X',
    ]
)