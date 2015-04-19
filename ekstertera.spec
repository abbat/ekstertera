Name:          ekstertera
Version:       0.0.1
Release:       1
Summary:       Yandex.Disk GUI client
Group:         Applications/Internet
License:       BSD-2-clause
URL:           https://github.com/abbat/ekstertera
Requires:      libqt4 >= 4.4
BuildRequires: libqt4-devel >= 4.4

%define qmake qmake
%define lrelease lrelease

%if 0%{?fedora}
%define qmake qmake-qt4
%define lrelease lrelease-qt4
%endif

Source0:       https://build.opensuse.org/source/home:antonbatenev:ekstertera/ekstertera/ekstertera_%{version}.tar.bz2
BuildRoot:     %{_tmppath}/%{name}-%{version}-build


%description
GUI tool to upload, retrieve and manage data in Yandex.Disk service


%prep
%setup -q -n ekstertera


%build
%{qmake} -project -recursive -Wall -nopwd -o ekstertera.pro \
    "CODEC = UTF-8" \
    "CODECFORTR = UTF-8" \
    "CONFIG += debug_and_release" \
    "QT += network" \
    "TRANSLATIONS += src/translations/ekstertera_en.ts" \
    src
%{lrelease} -compress -removeidentical ekstertera.pro
%{qmake} ekstertera.pro
make


%install
install -d %{buildroot}%{_bindir}
install -d %{buildroot}%{_datadir}/pixmaps
install -d %{buildroot}%{_datadir}/applications

install -m755 ekstertera %{buildroot}%{_bindir}/ekstertera
install -m644 src/icons/ekstertera.xpm %{buildroot}%{_datadir}/pixmaps/ekstertera.xpm
install -m644 ekstertera.desktop %{buildroot}%{_datadir}/applications/ekstertera.desktop


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc README.md
%{_bindir}/ekstertera
%{_datadir}/pixmaps/ekstertera.xpm
%{_datadir}/applications/ekstertera.desktop


%changelog
* Sun Apr 19 2015 Anton Batenev <antonbatenev@yandex.ru> 0.0.1-1
- Initial RPM release
