Name:          ekstertera
Version:       0.0.11
Release:       1
Summary:       Yandex.Disk GUI client
Group:         Applications/Internet
License:       BSD-2-clause
URL:           https://github.com/abbat/ekstertera
Requires:      libqt4 >= 4.6
BuildRequires: libqt4-devel >= 4.6

%define qmake qmake
%define lrelease lrelease

%if 0%{?fedora} || 0%{?centos}
%define qmake qmake-qt4
%define lrelease lrelease-qt4
%endif

Source0:       https://build.opensuse.org/source/home:antonbatenev:%{name}/%{name}/%{name}_%{version}.tar.bz2
BuildRoot:     %{_tmppath}/%{name}-%{version}-build


%description
GUI tool to upload, retrieve and manage data in Yandex.Disk service


%prep
%setup -q -n %{name}


%build
export QT_SELECT=4
%{qmake} -project -recursive -Wall -nopwd -o %{name}.pro \
    "CODEC = UTF-8" \
    "CODECFORTR = UTF-8" \
    "CONFIG += release" \
    "QT += network" \
    "INCLUDEPATH += src" \
    "TRANSLATIONS += src/translations/%{name}_en.ts" \
    src 3dparty/json 3dparty/qt5
%{lrelease} -compress -removeidentical %{name}.pro
%{qmake} %{name}.pro
make


%install
install -d %{buildroot}%{_bindir}
install -d %{buildroot}%{_datadir}/pixmaps
install -d %{buildroot}%{_datadir}/applications

install -m755 %{name}               %{buildroot}%{_bindir}/%{name}
install -m644 src/icons/%{name}.xpm %{buildroot}%{_datadir}/pixmaps/%{name}.xpm
install -m644 %{name}.desktop       %{buildroot}%{_datadir}/applications/%{name}.desktop


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc README.md
%{_bindir}/%{name}
%{_datadir}/pixmaps/%{name}.xpm
%{_datadir}/applications/%{name}.desktop


%changelog
* Sun May 24 2015 Anton Batenev <antonbatenev@yandex.ru> 0.0.11-1
- Initial RPM release
