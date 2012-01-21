%{!?python_sitearch: %define python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print get_python_lib(1)")}

Name:				gfal2-bindings
Version:			1.0.0
Release:			5beta1
Summary:			Python bindings for gfal 2.0
Group:				Applications/Internet
License:			ASL 2.0
URL:				https://svnweb.cern.ch/trac/lcgutil/wiki/gfal2
# svn export http://svn.cern.ch/guest/lcgutil/wlcggridfs/trunk gfalfs
Source0:			http://grid-deployment.web.cern.ch/grid-deployment/dms/lcgutil/tar/%{name}/%{name}-%{version}.tar.gz 
BuildRoot:			%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:		cmake
BuildRequires:		glib2-devel
BuildRequires:		gfal2-devel
BuildRequires:		boost-devel
BuildRequires:		python-devel

%description
Aggregation of the bindings for GFAL 2.0

%package -n gfal2-python
Summary:			python bindings for gfal 2.0
Group:				Applications/Internet
Requires:			glib2%{?_isa}
Requires:			gfal2-core%{?_isa}
Requires:			python%{?_isa}
Requires:			boost%{?_isa}

%description -n gfal2-python
python bindings for gfal 2.0

%post 

%clean
rm -rf "$RPM_BUILD_ROOT";
make clean

%prep
%setup -q

%build
%cmake -DDOC_INSTALL_DIR=%{_docdir}/%{name}-%{version} .
make %{?_smp_mflags}

%postun


%install
rm -rf "$RPM_BUILD_ROOT"; 
make %{?_smp_mflags} DESTDIR=$RPM_BUILD_ROOT install

%files -n gfal2-python
%defattr (-,root,root)
%{python_sitearch}/gfal2.so

 

%changelog
* Mon Nov 14 2011 adevress at cern.ch 
 - Initial gfal2-python bindings 1.0 preview release
