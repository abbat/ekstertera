Name:           ekstertera
Version:        0.1.9
Release:        1
Summary:        Yandex.Disk GUI client
Group:          Applications/Internet
License:        BSD-2-clause
URL:            https://github.com/abbat/ekstertera
BuildRequires:  gtk2-devel, glib2-devel
Source0:        https://build.opensuse.org/source/home:antonbatenev:%{name}/%{name}/%{name}_%{version}.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%if 0%{?suse_version}
BuildRequires:  libqt5-qtbase-devel, libqt5-qttools-devel, libqt5-qtsvg-devel, gdk-pixbuf-devel
%else
BuildRequires:  qt5-qtbase-devel, qt5-qttools-devel, qt5-qtsvg-devel, gdk-pixbuf2-devel
%endif

%description
GUI tool to upload, retrieve and manage data in Yandex.Disk service


%prep
%setup -q -n %{name}


%build
export builddir=$(pwd)

qmake-qt5 -project -recursive -Wall -nopwd -o %{name}.pro \
    "CODEC         = UTF-8"                               \
    "CODECFORTR    = UTF-8"                               \
    "QT           += network core widgets"                \
    "CONFIG       += release link_pkgconfig"              \
    "PKGCONFIG    += glib-2.0 gtk+-2.0 gdk-pixbuf-2.0"    \
    "DEFINES      += ETERA_CUSTOM_TRAY_ICON_GTK"          \
    "INCLUDEPATH  += src"                                 \
    "TRANSLATIONS +=                                      \
        ${builddir}/src/translations/ekstertera_en.ts     \
        ${builddir}/src/translations/ekstertera_fr.ts"    \
    "${builddir}/src" "${builddir}/3dparty/json"

%if 0%{?suse_version} == 1320
export QMAKE=/usr/bin/qmake-qt5
%endif

lrelease-qt5 -compress -removeidentical %{name}.pro
qmake-qt5 %{name}.pro
make %{?_smp_mflags}


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
* Thu Jun 23 2016 Anton Batenev <antonbatenev@yandex.ru> 0.1.9-1
- Initial RPM release
