Name:               QStudioMetrics
Version:            1.0.6
Release:            0.testing%{?dist}
Summary:            QStudio Metrics: Make Easy Multivariate Models
License:            BSD
Group:              Applications/Engineering
Source:             %{name}-%{version}.tar.bz2
URL:                http://www.gmrandazzo.org
BuildRequires:      cmake
BuildRequires:      vtk-devel
BuildRequires:      qt-devel
BuildRequires:      libscientific-devel


%define _applicationsdir /usr/share/applications/
%define _iconsdir /usr/share/icons/


%description
 The purpose of this project is to develop a free (open-source), platform independent
 to enable scientists on a simple mode to analyze data and make easy multivariate models.
 %{name} is being actively improved, all your suggestions to our "wish to" list and 
 all your contributions are most welcome!

%prep
%setup -q -n %{name}-%{version}

%build

mkdir build
pushd build
%cmake -DCMAKE_BUILD_TYPE=Release ..
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}
pushd build
make install DESTDIR=%{buildroot}

mkdir -p %{buildroot}%{_applicationsdir}
install -cpm 0644 %{_builddir}/%{name}-%{version}/%{name}.desktop \
        %{buildroot}%{_applicationsdir}/QStudioMetrics.desktop

mkdir -p %{buildroot}%{_iconsdir}
install -cpm 0644 %{_builddir}/%{name}-%{version}/qstudiometrics.png \
        %{buildroot}%{_iconsdir}/qstudiometrics.png


%post
update-mime-database %{_datadir}/mime &> /dev/null || :
update-desktop-database &> /dev/null ||:
touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
update-mime-database %{_datadir}/mime &> /dev/null || :
update-desktop-database &> /dev/null ||:
if [ $1 -eq 0 ] ; then
    touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_applicationsdir}/QStudioMetrics.desktop
%{_iconsdir}/qstudiometrics.png

%changelog
* Fri Oct  25 2012 Giuseppe Marco Randazzo <gmrandazzo@gmail.com> - 1.0
- New upstream release

