import os
from setuptools import setup, Extension, find_packages
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
import numpy
import glob
from Cython.Build import cythonize
pwd = os.path.abspath(os.path.dirname(__file__))
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
        self.plat_name = "linux_x86_64"  # 匹配目标系统

# 扩展模块配置
extensions = cythonize([
    Extension("zeus.zeus",
        sources=['zeus/zeus.cpp'],
        include_dirs=[
            get_pybind_include(),        # 系统安装路径
            get_pybind_include(user=True),  # 用户级安装路径
            numpy.get_include()
        ],
        libraries=['airaw'],
        library_dirs=['./zeus/'],
        extra_link_args=[
        '-Wl,-rpath,$ORIGIN/libs',          # 相对路径修正
        '-Wl,--disable-new-dtags'           # 确保RPATH生效[7](@ref)
        ],
        runtime_library_dirs=['$ORIGIN']
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
            'libairaw.so', 
            'camconst.json',
            'libs/*.so',        # 通配符匹配所有so文件
            'libs/*.so.*'       # 包含带版本号的库[3](@ref)
        ]
    },
    include_package_data=True,  # 必须启用[3,6](@ref)


    install_requires=[
        'numpy>=1.18',
        'Cython>=0.29'
    ],
    zip_safe=False,
    classifiers=[
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python :: 3.8'
    ]
)