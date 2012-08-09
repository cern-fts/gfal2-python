%global checkout_tag 20120907svn

%if 0%{?el5}
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include/boost141 -DBOOST_LIBRARYDIR=%{_libdir}/boost141
%else
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include
%endif

%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

Name:				gfal2-python
Version:			1.0.1
Release:			1.%{checkout_tag}%{?dist}
Summary:			Python bindings for gfal 2.0
Group:				Applications/Internet
License:			ASL 2.0
URL:				https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2-python
# svn export http://svn.cern.ch/guest/lcgutil/gfal2-bindings/trunk gfal2-bindings
Source0:			http://grid-deployment.web.cern.ch/grid-deployment/dms/lcgutil/tar/%{name}/%{name}-%{version}-%{checkout_tag}.tar.gz
BuildRoot:			%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:		cmake
BuildRequires:		glib2-devel
BuildRequires:		gfal2-devel
%if 0%{?el5}
BuildRequires:		boost141-devel
%else
BuildRequires:		boost-devel
%endif
BuildRequires:		python-devel
BuildRequires:          epydoc

Requires:			python%{?_isa}
Requires:			boost%{?_isa}

%description
Python bindings for gfal 2.0.
GFAL 2.0 offers an a single, simple and portable API
for the file operations in grids and cloud environments.

%package doc
Summary:			Documentation for %{name}
Group:				Applications/Internet
Requires:			%{name}%{?_isa} = %{version}-%{release}

%description doc
documentation files  of %{name} .

%clean
rm -rf %{buildroot};
make clean

%prep
%setup -q

%build
%cmake -DDOC_INSTALL_DIR=%{_docdir}/%{name}-%{version} \
 %{boost_cmake_flags} \
 -DUNIT_TESTS=TRUE . 


make %{?_smp_mflags}
make doc

%check
ctest -V .

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install

%files
%defattr (-,root,root)
%{python_sitearch}/gfal2.so

%files doc
%defattr (-,root,root)
%dir %{_docdir}/%{name}-%{version}/html
%{_docdir}/%{name}-%{version}/html/*


%changelog
* Fri Jul 20 2012 Adrien Devresse <adevress at cern.ch> - 1.0.0-1
 - initial 1.0 release for gfal 2.0 python bindings
 - minor refactory of the python API

* Mon Nov 14 2011 Adrien Devresse <adevress at cern.ch> - 1.0.1-0.1.20120503010snap
 - Initial gfal 2.0 bindigns preview with posix scope
