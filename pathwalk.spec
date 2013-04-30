Name:  
Version:
Release:

%define debug_package %{nil}

Summary: Pathwalk - simulate path search
License: GPL
Group: Applications/System
Url: http://code.google.com/p/powerman/
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%package data
Summary: Data for pathwalk benchmark
Group: Applications/System
AutoReq: no


%description
Pathwalk simulates the file system load created by path search.

%description data
Data for pathwalk benchmark


%prep
%setup

%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/opt/pathwalk/bin
cp pathwalk $RPM_BUILD_ROOT/opt/pathwalk/bin/
./pathwalk -c $RPM_BUILD_ROOT/opt/pathwalk/data/
cd $RPM_BUILD_ROOT/opt/pathwalk && tar cf data.tar data
rm -rf $RPM_BUILD_ROOT/opt/pathwalk/data

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0755)
/opt/pathwalk/bin

%files data
%defattr(-,root,root,0644)
/opt/pathwalk/data.tar

%post data
umask 022
cd /opt/pathwalk && tar --no-same-owner -xf data.tar

%preun data
rm -rf /opt/pathwalk/data
