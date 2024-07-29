# Gfal2-Python

`gfal2-python` is a C++ library offering a thin layer of Python wrappers via `PyBoost`.
We recommend installing it via RPMs, but it is also available on [PyPI/gfal2-python][1].

## Using the Gfal2 Python Bindings

The Gfal2 Python bindings are intended to be easy to use. 

#### Pre-requisites

Before jumping into usage, ensure you have a proxy certificate 
available in the default location (`/tmp/x509up_u$(id -u)`).

Optionally, you may want to install relevant Gfal2 protocol plugins. 
Our example will use HTTP protocol, so we must ensure we have
the Gfal2 HTTP plugin installed:
```bash
$ dnf install -y gfal2-plugin-http
```

#### Examples

Example of stating a file location: 
```python
import gfal2

ctx = gfal2.creat_context()
ctx.stat("https://eospublic.cern.ch/eos/opstest/dteam/file.test")
```

Example of copying a file from local to remote:
```python
import gfal2

ctx = gfal2.creat_context()
ctx.filecopy("file:///tmp/file.test", "https://eospublic.cern.ch/eos/opstest/dteam/file.test")
ctx.stat("https://eospublic.cern.ch/eos/opstest/dteam/file.test")
```

Example of copying the file from remote to local (with custom transfer parameters):
```python
import gfal2

ctx = gfal2.creat_context()
params = ctx.transfer_parameters()
params.set_checksum = ("both", "adler32", None)
params.overwrite = True

ctx.filecopy("https://eospublic.cern.ch/eos/opstest/dteam/file.test", "file:///tmp/file.test")
ctx.checksum("file:///tmp/file.test", "adler32")
```

Further code examples can be found in the [example/python/][2] source directory.

## Installing via Pip

Installing the package via `pip install gfal2-python` will reproduce the compilation
process, hence why you need to make sure you have all the right dependencies.
This guide details the steps needed to get all the dependencies, starting
from a *blank* machine.

This guide assumes you have the EPEL repositories installed (`epel-release` package).
In this example, the build platform is `Alma9` and we will use `v1.12.2`,
but the same process applies for other versions or build platforms.

```bash
# Clone the gfal2-python repository with the desired version (example for v1.12.2)
$ git clone --branch v1.12.2 https://github.com/cern-fts/gfal2-python.git
$ cd gfal2-python/

# Install standard set of build packages
$ ./ci/fedora-packages.sh

# Build the source RPM and install dependencies
$ cd packaging/
$ make srpm
$ dnf builddep -y gfal2-python-*.src.rpm

# Ready to install now from PyPI
$ pip install gfal2-python
```

## Useful links

- DMC documentation: https://dmc-docs.web.cern.ch/dmc-docs/
- Apache License, version 2.0: https://www.apache.org/licenses/LICENSE-2.0
- Issue tracking: https://github.com/cern-fts/gfal2-python/issues
- Support channels: dmc-support@cern.ch
- Development team contact: dmc-devel@cern.ch

Enjoy using the Gfal2 Python bindings!


[1]: https://pypi.org/project/gfal2-python/
[2]: https://github.com/cern-fts/gfal2-python/tree/v1.12.2/example/python
