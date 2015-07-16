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
import sys
import distutils.command.build as _build
import setuptools.command.install as _install
import distutils.spawn
import distutils.dir_util

from setuptools import setup

# Change this when there are changes in the setup.py or MANIFEST.in
RELEASE=3

def get_version():
    ver_components = dict(VERSION_RELEASE=RELEASE)
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
    return "%(VERSION_MAJOR)s.%(VERSION_MINOR)s.%(VERSION_PATCH)s.post%(VERSION_RELEASE)s" % ver_components


def validate():
    if distutils.spawn.find_executable('cmake') is None:
        print 'Missing CMake executable'
        sys.exit(-1)


def _run_make():
    validate()
    distutils.dir_util.mkpath('build')
    os.chdir('build')
    try:
        distutils.spawn.spawn(['cmake', '-DSKIP_DOC=TRUE', '-DSKIP_TESTS=TRUE', '..'])
        distutils.spawn.spawn(['make'])
    finally:
        os.chdir('..')


class build(_build.build):
    def run(self):
        _run_make()


class install(_install.install):
    def run(self):
        if not os.path.exists('build/src/gfal2.so'):
            _run_make()
        os.chdir('build')
        try:
            distutils.spawn.spawn(['make', 'install'])
        finally:
            os.chdir('..')


setup(
    name = 'gfal2-python',
    version = get_version(),
    description = 'Python bindings for gfal2',
    requires = [],
    install_requires = [],
    url = 'https://dmc.web.cern.ch/',
    download_url = 'https://gitlab.cern.ch/dmc/gfal2-bindings',
    author = 'DMC Devel',
    author_email = 'dmc-devel@cern.ch',
    maintainer_email = 'dmc-devel@cern.ch',
    license = 'Apache 2',
    long_description = 'Python bindings for gfal2',
    keywords = 'gfal2, grid, dmc, data management clients',
    platforms = ['GNU/Linux'],
    classifiers = [
		"Intended Audience :: Developers",
		"Topic :: Software Development :: Libraries :: Python Modules",
		"License :: OSI Approved :: Apache Software License",
		"Development Status :: 5 - Production/Stable",
		"Operating System :: Unix",
		"Programming Language :: C"
    ],
    cmdclass = {'install': install, 'build': build},
    zip_safe = False,
    packages = []
)

