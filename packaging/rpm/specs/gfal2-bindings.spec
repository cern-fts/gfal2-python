Name:				gfal2-bindings
Version:			1.0.0
Release:			1.13_preview
Summary:			Python bindings for gfal 2.0
Group:				Applications/Internet
License:			ASL 2.0
URL:				https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2
## source URL
# svn export http://svn.cern.ch/guest/lcgutil/wlcggridfs/trunk gfalfs
#
Source:				%{name}-%{version}.src.tar.gz
BuildRoot:			%{_tmppath}/%{name}-%{version}-%{release}

BuildRequires:		scons
BuildRequires:		glib2-devel%{?_isa}
BuildRequires:		gfal2-devel%{?_isa}
BuildRequires:		boost141-devel%{?_isa}
BuildRequires:		python-devel%{?_isa}
BuildRequires:		python26-devel%{?_isa}

%package -n gfal2-python
Summary:			python bindings for gfal 2.0
Group:				Applications/Internet
Requires:			glib2%{?_isa}
Requires:			gfal2-core%{?_isa}
Requires:			python%{?_isa}
Requires:			python26%{?_isa}
Requires:			boost141%{?_isa}

%description -n gfal2-python
python bindings for gfal 2.0

%post 

%clean
rm -rf "$RPM_BUILD_ROOT";
scons python_core=yes production=yes -c build

%prep
%setup -q

%build
scons python_core=yes production=yes build

%postun


%install
rm -rf "$RPM_BUILD_ROOT"; 
scons python_core=yes production=yes \
--install-sandbox="$RPM_BUILD_ROOT" install 

%files -n gfal2-python
%defattr (-,root,root)
%{_libdir}/python2.6/site-packages/gfal2.so
%{_libdir}/python2.6/site-packages/libgfal2_python.so 
%{_libdir}/python2.4/site-packages/gfal2.so
%{_libdir}/python2.4/site-packages/libgfal2_python.so	
 

%changelog
* Mon Nov 14 2011 adevress at cern.ch 
 - Initial gfal2-python bindings 1.0 preview release
