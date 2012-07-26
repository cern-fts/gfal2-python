
%if 0%{?el5}
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include/boost141 -DBOOST_LIBRARYDIR=%{_libdir}/boost141
%else
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include
%endif

%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

Name:				gfal2-python
Version:			1.0.1
Release:			1%{?dist}
Summary:			Python bindings for gfal 2.0
Group:				Applications/Internet
License:			ASL 2.0
URL:				https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2
# svn export http://svn.cern.ch/guest/lcgutil/gfal2-bindings/trunk gfal2-bindings
Source0:			http://grid-deployment.web.cern.ch/grid-deployment/dms/lcgutil/tar/%{name}/%{name}-%{version}.tar.gz 
BuildRoot:			%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

%description
Python bindings for gfal 2.0
Officialy supported.

BuildRequires:		cmake
BuildRequires:		glib2-devel
BuildRequires:		gfal2-devel
%if 0%{?el5}
BuildRequires:		boost141-devel
%else
BuildRequires:		boost-devel
%endif
BuildRequires:		python-devel

Requires:			python%{?_isa}
Requires:			boost%{?_isa}

%prep
%setup -q

%build
%cmake \
-DDOC_INSTALL_DIR=%{_docdir}/%{name}-%{version} \
-DUNIT_TESTS=TRUE \
%{boost_cmake_flags} \
.

make %{?_smp_mflags}

%post

%postun


%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install

%clean
rm -rf %{buildroot};
make clean

%check
ctest -V

%files
%defattr (-,root,root)
%{python_sitearch}/gfal2.so
%doc RELEASE-NOTES VERSION


%changelog
* Fri Jul 20 2012 Adrien Devresse <adevress at cern.ch> - 1.0.0-1
 - initial 1.0 release for gfal 2.0 python bindings
 - minor refactory of the python API

* Mon Nov 14 2011 Adrien Devresse <adevress at cern.ch> - 1.0.1-0.1.20120503010snap
 - Initial gfal 2.0 bindigns preview with posix scope
