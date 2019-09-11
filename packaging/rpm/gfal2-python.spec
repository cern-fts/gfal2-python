# Use static linking against boost
%bcond_with static_boost_python

# Python 3
%if %{?fedora}%{!?fedora:0} >= 23 || %{?rhel}%{!?rhel:0} >= 7
%global with_python3 1
%endif

# Doc directory
%{!?_pkgdocdir: %global _pkgdocdir %{_docdir}/%{name}-%{version}}

#include boost > 141 for EL5
%if 0%{?el5}
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include/boost141 -DBOOST_LIBRARYDIR=%{_libdir}/boost141
%else
%global boost_cmake_flags -DBOOST_INCLUDEDIR=/usr/include
%endif


# python path discovery
%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

%if 0%{?with_python3}
%{!?python3_sitearch: %define python3_sitearch %(%{__python3} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif

# python modules filtering
%if 0%{?el6} || 0%{?el5}
%{?filter_setup:
%filter_provides_in %{python_sitearch}/.*\.so$ 
%filter_setup
}
%else
%global __provides_exclude_from ^((%{python_sitearch})|(%{python3_sitearch})/.*\\.so)$
%endif

Name:			gfal2-python
Version:		1.9.5
Release:		1%{?dist}
Summary:		Python bindings for gfal 2
Group:			Applications/Internet
License:		ASL 2.0
URL:			http://dmc.web.cern.ch/
# git clone --branch master https://gitlab.cern.ch/dmc/gfal2-bindings.git gfal2-python-1.9.4
# pushd gfal2-python-1.9.4
# git checkout v1.9.4
# popd
# tar czf gfal2-python-1.9.4.tar.gz --exclude-vcs gfal2-python-1.9.4
Source0:		%{name}-%{version}.tar.gz
BuildRoot:		%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

%if 0%{?el5}
BuildRequires:      cmake28
%else
BuildRequires:      cmake
%endif
BuildRequires:		gfal2-devel >= 2.13.0
%if 0%{?el5}
BuildRequires:		boost141-devel
%else
BuildRequires:		boost-devel
%endif
%if 0%{?with_static_boost_python}
%if 0%{?el5}
BuildRequires:      boost141-static
%else
BuildRequires:      boost-static
%endif
%endif
BuildRequires:		python2-devel
BuildRequires:		epydoc

Requires:		gfal2-core >= 2.13.0

# Python 3
%if 0%{?with_python3} 
BuildRequires:      python36-devel
BuildRequires:      boost-python36-devel
%endif

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
Documentation files for %{name}.

%if 0%{?with_python3}
%package -n gfal2-python3
Summary:            gfal2 python birngins for Python 3
Group:              Applications/Internet

%description -n gfal2-python3
Python 3 bindings for gfal2.
GFAL2 offers an a single, simple and portable API
for the file operations in grids and cloud environments.
%endif

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

%if 0%{?el5}
    %cmake28 \
     -DDOC_INSTALL_DIR=%{_pkgdocdir} \
     %{boost_cmake_flags} \
%if 0%{?with_static_boost_python}
     -DBoost_USE_STATIC_LIBS=ON \
%endif
     -DUNIT_TESTS=TRUE .
%else
    %cmake \
     -DDOC_INSTALL_DIR=%{_pkgdocdir} \
     %{boost_cmake_flags} \
%if 0%{?with_static_boost_python}
     -DBoost_USE_STATIC_LIBS=ON \
%endif
     -DUNIT_TESTS=TRUE .
%endif

make %{?_smp_mflags}
make doc

%check
%if 0%{?el5}
    ctest28 -V -T Test .
%else
    ctest -V -T Test .
%endif

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

%if %{with python3}
%files -n gfal2-python3
%defattr (-,root,root)
%{python3_sitearch}/gfal2.so
#/usr/lib64/python3.6/site-packages/gfal2.so
%endif

%changelog
* Mon Feb 20 2017 Alejandro Alvarez <aalvarez@cern.ch> - 1.9.1-1
- Update for release 1.9.0

* Fri Feb 10 2017 Fedora Release Engineering <releng@fedoraproject.org> - 1.8.5-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_26_Mass_Rebuild

* Fri Jan 27 2017 Jonathan Wakely <jwakely@redhat.com> - 1.8.5-2
- Rebuilt for Boost 1.63

* Wed Jan 18 2017 Alejandro Alvarez <aalvarez@cern.ch> - 1.8.5-1
- Update to upstream release

* Tue Jul 19 2016 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.8.4-5
- https://fedoraproject.org/wiki/Changes/Automatic_Provides_for_Python_RPM_Packages

* Tue Mar 08 2016 Alejandro Alvarez <aalvarez@cern.ch> - 1.8.4-4
- Add patch to work with newer versions of Boost

* Wed Feb 03 2016 Fedora Release Engineering <releng@fedoraproject.org> - 1.8.4-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_24_Mass_Rebuild

* Fri Jan 15 2016 Jonathan Wakely <jwakely@redhat.com> - 1.8.4-2
- Rebuilt for Boost 1.60

* Mon Nov 09 2015 Alejandro Alvarez <aalvarez@cern.ch> - 1.8.4-1
- Update for release 1.8.4

* Thu Aug 27 2015 Jonathan Wakely <jwakely@redhat.com> - 1.8.3-4
- Rebuilt for Boost 1.59

* Wed Jul 29 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.8.3-3
- Rebuilt for https://fedoraproject.org/wiki/Changes/F23Boost159

* Wed Jul 22 2015 David Tardon <dtardon@redhat.com> - 1.8.3-2
- rebuild for Boost 1.58

* Fri Jul 03 2015 Alejandro Alvarez <aalvarez at cern.ch> - 1.8.3-1
- Update for release 1.8.3

* Wed Jun 17 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.8.1-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_23_Mass_Rebuild

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



