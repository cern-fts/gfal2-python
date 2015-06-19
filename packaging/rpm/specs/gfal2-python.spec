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

Name:			gfal2-python
Version:		1.8.2
Release:		1%{?dist}
Summary:		Python bindings for gfal 2
Group:			Applications/Internet
License:		ASL 2.0
URL:			http://dmc.web.cern.ch/
# git clone https://gitlab.cern.ch/dmc/gfal2-bindings.git gfal2-python-1.8.1
# pushd gfal2-python-1.8.1
# git checkout v1.8.1
# git submodule init && git submodule update
# popd
# tar czf gfal2-python-1.8.1.tar.gz gfal2-python-1.8.1
Source0:		%{name}-%{version}.tar.gz
BuildRoot:		%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:		cmake
BuildRequires:		gfal2-devel >= 2.9.1
%if 0%{?el5}
BuildRequires:		boost141-devel
%else
BuildRequires:		boost-devel
%endif
BuildRequires:		python2-devel
BuildRequires:		epydoc

Requires:		gfal2-core >= 2.9.1

%description
Python bindings for gfal2.
GFAL2 offers an a single, simple and portable API
for the file operations in grids and cloud environments.

%package doc
Summary:			Documentation for %{name}
Group:				Applications/Internet
%if 0%{?fedora} > 10 || 0%{?rhel}>5
BuildArch:			noarch
%endif

%description doc
Documentation files for %{name} .

%clean
rm -rf %{buildroot};
make clean

%prep
%setup -q

%build
# Make sure the version in the spec file and the version used
# for building matches
gfal2_python_cmake_ver=`sed -n 's/^set *(VERSION_\(MAJOR\|MINOR\|PATCH\) *\([0-9]\+\).*/\2/p' CMakeLists.txt | paste -sd '.'`
gfal2_python_spec_ver=`expr "%{version}" : '\([0-9]*\\.[0-9]*\\.[0-9]*\)'`
if [ "$gfal2_python_cmake_ver=" != "$gfal2_python_spec_ver=" ]; then
    echo "The version in the spec file does not match the CMakeLists.txt version!"
    echo "$gfal2_python_cmake_ver!= %{version}"
    exit 1
fi

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
%{_pkgdocdir}/readme.html
%dir %{_pkgdocdir}/html
%dir %{_pkgdocdir}/examples
%{_pkgdocdir}/html/*
%{_pkgdocdir}/examples/*

%changelog
* Fri Apr 17 2015 Alejandro Alvarez <aalvarez at cern.ch> - 1.8.1-1
- Update for release 1.8.1

* Thu Apr 02 2015 Alejandro Alvarez <aalvarez at cern.ch> - 1.7.1-1
- Update for release 1.7.1

* Fri Feb 06 2015 Alejandro Alvarez <aalvarez at cern.ch> - 1.7.0-1
- Update for release 1.7.0

* Tue Jan 27 2015 Petr Machata <pmachata@redhat.com> - 1.6.0-2
- Rebuild for boost 1.57.0

* Fri Nov 07 2014 Alejandro Alvarez <aalvarez at cern.ch> - 1.6.0-1
- Update for release 1.6.0

* Sat Aug 16 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.5.0-1
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_22_Mass_Rebuild

* Tue Jul 01 2014 Alejandro Alvarez <aalvarez at cern.ch> - 1.5.0-1
- Update for release 1.5.0

* Sat Jun 07 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.4.1-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_Mass_Rebuild

* Fri May 23 2014 Petr Machata <pmachata@redhat.com> - 1.4.1-2
- Rebuild for boost 1.55.0

* Fri Feb 28 2014 Adrien Devresse <adevress at cern.ch> - 1.4.1-1
 - Release 1.4.1 of gfal2 python bindings, see RELEASE-NOTES for details

* Sat Nov 16 2013 Ville Skyttä <ville.skytta@iki.fi> - 1.3.0-2
- Install docs to %%{_pkgdocdir} where available (#993774).
- Own doc dirs.

* Fri Nov 01 2013 Adrien Devresse <adevress at cern.ch> - 1.3.0-1  
 - Release 1.3.0 of gfal2 python bindings, see RELEASE-NOTES for details

* Wed May 08 2013 Adrien Devresse <adevress at cern.ch> - 1.2.1-1 
 - First release compatible for EPEL



