%{!?_pkgdocdir: %global _pkgdocdir %{_docdir}/%{name}-%{version}}


#include boost > 141 for EL5
%if 0%{?el5}
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include/boost141 -DBOOST_LIBRARYDIR=%{_libdir}/boost141
%else
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include
%endif

# python path discovery
%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

# python modules filtering
%if 0%{?el6} || 0%{?el5}
%{?filter_setup:
%filter_provides_in %{python_sitearch}/.*\.so$ 
%filter_setup
}
%else
%global __provides_exclude_from ^(%{python_sitearch}/.*\\.so)$
%endif

Name:				gfal2-python
Version:			1.5.0
Release:			1%{?dist}
Summary:			Python bindings for gfal 2.0
Group:				Applications/Internet
License:			ASL 2.0
URL:				https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2-python
# svn export http://svn.cern.ch/guest/lcgutil/gfal2-bindings/trunk gfal2-bindings
Source0:			http://grid-deployment.web.cern.ch/grid-deployment/dms/lcgutil/tar/%{name}/%{name}-%{version}.tar.gz
BuildRoot:			%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:		cmake
BuildRequires:		gfal2-devel >= 2.7.0
%if 0%{?el5}
BuildRequires:		boost141-devel
%else
BuildRequires:		boost-devel
%endif
BuildRequires:		python2-devel
BuildRequires:		epydoc

%description
Python bindings for gfal 2.0.
GFAL 2.0 offers an a single, simple and portable API
for the file operations in grids and cloud environments.

%package doc
Summary:			Documentation for %{name}
Group:				Applications/Internet
%if 0%{?fedora} > 10 || 0%{?rhel}>5
BuildArch:			noarch
%endif

%description doc
documentation files  of %{name} .

%clean
rm -rf %{buildroot};
make clean

%prep
%setup -q

%build
%cmake \
-DDOC_INSTALL_DIR=%{_pkgdocdir} \
 %{boost_cmake_flags} \
 -DUNIT_TESTS=TRUE . 

make %{?_smp_mflags}
make doc

%check
ctest -V -T Test .

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install

%files
%defattr (-,root,root)
%{python_sitearch}/gfal2.so
%{_pkgdocdir}/LICENSE
%{_pkgdocdir}/RELEASE-NOTES
%{_pkgdocdir}/README


%files doc
%defattr (-,root,root)
%dir %{_pkgdocdir}/html
%{_pkgdocdir}/html/*

%changelog
* Fri Feb 28 2014 Adrien Devresse <adevress at cern.ch> - 1.4.1-1
 - Release 1.4.1 of gfal2 python bindings, see RELEASE-NOTES for details

* Sat Nov 16 2013 Ville Skyttä <ville.skytta@iki.fi> - 1.3.0-2
- Install docs to %%{_pkgdocdir} where available (#993774).
- Own doc dirs.

* Fri Nov 01 2013 Adrien Devresse <adevress at cern.ch> - 1.3.0-1  
 - Release 1.3.0 of gfal2 python bindings, see RELEASE-NOTES for details

* Wed May 08 2013 Adrien Devresse <adevress at cern.ch> - 1.2.1-1 
 - First release compatible for EPEL



