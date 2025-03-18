import os
from setuptools import setup, Extension, find_packages
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
import numpy
import subprocess

class get_pybind_include(object):
    def __init__(self, user=False):
        self.user = user
    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)

class bdist_wheel(_bdist_wheel):
    def finalize_options(self):
        super().finalize_options()
        self.root_is_pure = False
        self.plat_name = "linux_x86_64"

def get_opencv_flags():
    try:
        cflags = subprocess.check_output(['pkg-config', '--cflags', 'opencv4']).decode().strip().split()
        libs = subprocess.check_output(['pkg-config', '--libs', 'opencv4']).decode().strip().split()
        include_dirs = [flag[2:] for flag in cflags if flag.startswith('-I')]
        libraries = [flag[2:] for flag in libs if flag.startswith('-l')]
        library_dirs = [flag[2:] for flag in libs if flag.startswith('-L')]
        return include_dirs, libraries, library_dirs
    except subprocess.CalledProcessError:
        print("Error: pkg-config for opencv4 not found. Ensure OpenCV is installed and pkg-config is available.")
        exit(1)

opencv_include_dirs, opencv_libraries, opencv_library_dirs = get_opencv_flags()

extensions = [
    Extension("zeus.zeus",
        sources=['zeus/zeus.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            numpy.get_include()
        ] + opencv_include_dirs,
        libraries=['airaw'] + opencv_libraries,
        library_dirs=['./zeus/'] + opencv_library_dirs,
        extra_link_args=[
            '-Wl,-rpath,$ORIGIN/libs',
            '-Wl,--disable-new-dtags'
        ],
        runtime_library_dirs=['$ORIGIN']
    )
]

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
            'libs/*.so',
            'libs/*.so.*'
        ]
    },
    include_package_data=True,
    install_requires=[
        'numpy>=1.18',
        'pybind11>=2.6.0'
    ],
    zip_safe=False,
    classifiers=[
        'Operating System :: POSIX :: Linux',
    ]
)