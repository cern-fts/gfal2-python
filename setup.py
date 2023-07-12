#!/usr/bin/env python
#
# Copyright @ CERN, 2015
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
# limitations under the License.
#

import os
import re
import shutil
import sys
import distutils.command.build_ext as _build_ext
import distutils.spawn
import distutils.dir_util
from glob import glob
from subprocess import check_call
from setuptools import Extension, setup

# Change this for a post release i.e when there are changes in the setup.py or MANIFEST.in but no version change
POST_RELEASE = None


def get_version():
    ver_components = dict()
    if POST_RELEASE:
        ver_components['VERSION_RELEASE'] = POST_RELEASE

    with open('CMakeLists.txt') as cmake:
        for line in cmake:
            line = line.strip()
            if line.startswith('set') and line.endswith(')'):
                before, var, after = re.split(r'\(|\)', line)
                varname, varval = var.split()
                if varname.startswith('VERSION_'):
                    ver_components[varname] = varval
    if len(ver_components) == 0:
        raise ValueError('Could not find the version')

    version = "%(VERSION_MAJOR)s.%(VERSION_MINOR)s.%(VERSION_PATCH)s"
    if POST_RELEASE:
        version += ".post%(VERSION_RELEASE)s"

    return version % ver_components


def validate():
    if distutils.spawn.find_executable('cmake') is None:
        print('Missing CMake executable')
        sys.exit(-1)


def _run_make(build_dir, lib_path):
    validate()

    # prepare paths
    full_lib_path = os.path.dirname(os.path.abspath(lib_path))
    source_dir = os.path.abspath(os.path.dirname(__file__))

    # ensure involved directories exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    if not os.path.exists(full_lib_path):
        os.makedirs(full_lib_path)

    # build the cmake command
    major = sys.version_info[0]
    cmake_cmd = ['cmake', '-DSKIP_TESTS=TRUE']
    if major == 3:
        cmake_cmd += ['-DPYTHON_EXECUTABLE_3=%s' % sys.executable]
    cmake_cmd += [source_dir]

    # run the cmake command and make
    check_call(cmake_cmd, cwd=build_dir)
    check_call(['make'], cwd=build_dir)

    # install
    shutil.copy(os.path.join(build_dir, 'src/python%d/gfal2.so' % major), full_lib_path)


class build_ext(_build_ext.build_ext):
    def run(self):
        _run_make(self.build_temp, self.get_ext_fullpath('gfal2'))


setup(
    name='gfal2-python',
    version=get_version(),
    description='Python bindings for gfal2',
    long_description_content_type='text/markdown',
    requires=[],
    install_requires=[],
    url='https://dmc.web.cern.ch/',
    download_url='https://gitlab.cern.ch/dmc/gfal2-bindings',
    author='DMC Devel',
    author_email='dmc-devel@cern.ch',
    maintainer_email='dmc-devel@cern.ch',
    license='Apache 2',
    long_description='Python bindings for gfal2',
    keywords='gfal2, grid, dmc, data management clients',
    platforms=['GNU/Linux'],
    classifiers=[
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "License :: OSI Approved :: Apache Software License",
        "Development Status :: 5 - Production/Stable",
        "Operating System :: Unix",
        "Programming Language :: C"
    ],
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
    packages=[],
    ext_modules=[
        Extension('gfal2', sources=glob("src/*.cpp"))
    ]
)
