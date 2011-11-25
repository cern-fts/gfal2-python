%define projectname gfal2-bindings
%define version 1.0
%define release 1.12_preview


%define debug_package %{nil}

Name: %{projectname}
License: Apache-2.0
Summary: File system for lcg storage system
Version: %{version}
Release: %{release}
Requires: gfal2-python
Group: Grid/lcg
BuildRoot: %{_tmppath}/%{projectname}-%{version}-%{release}
Source: %{projectname}-%{version}-%{release}.src.tar.gz

%description
meta package for gfal2 bindings install


%package -n gfal2-python
Summary: python bindings for gfal 2.0
Group: grid/lcg
BuildRequires: gfal2-devel, glib2-devel, fuse-devel
Requires: gfal2, glib2, fuse
%description -n gfal2-python
python bindings for gfal 2.0






%post 

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT";
scons  python_core=yes production=yes -c build

%prep
%setup -q

%build
# 2 parallel build max, else compilation time bug with gcc
scons -j 2 python_core=yes production=yes build

%postun


%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf "$RPM_BUILD_ROOT"; 
scons  -j 2 python_core=yes production=yes --install-sandbox="$RPM_BUILD_ROOT" install 

%files -n gfal2-python
/usr/%{_lib}/python2.6/site-packages/gfal2.so
/usr/%{_lib}/python2.6/site-packages/libgfal2_python.so 
/usr/%{_lib}/python2.4/site-packages/gfal2.so
/usr/%{_lib}/python2.4/site-packages/libgfal2_python.so	
 
%files

%changelog
* Mon Nov 14 2011 adevress at cern.ch 
 - Initial gfal2-python bindings 1.0 preview release
