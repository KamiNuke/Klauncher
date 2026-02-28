Name:		klauncher
Version:	0.0.1
Release:    %autorelease
Summary:	A Qt/Kirigami gui for running Windows apps using Proton

License:	LICENSE
URL:		https://github.com/KamiNuke/Klauncher
Source0:	%{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  extra-cmake-modules
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtdeclarative-devel
BuildRequires:  qt6-qtquickcontrols2-devel
BuildRequires:  kf6-kirigami-devel
BuildRequires:  kf6-kcoreaddons-devel
BuildRequires:  kf6-ki18n-devel
BuildRequires:  kf6-kiconthemes-devel
#BuildRequires:  kf6-qqc2-desktop-style-devel
BuildRequires:  kf6-kirigami-addons-devel
Requires:       qt6-qtbase
Requires:       kf6-kirigami
Requires:       kf6-kirigami-addons
#Requires:       umu-launcher
Requires:       icoutils


%description
Klauncher is a GUI launcher for running Windows apps using Proton.

%prep
%autosetup


%build
%cmake
%cmake_build


%install
%cmake_install

%files
%license LICENSE
%{_bindir}/klauncher
%{_datadir}/applications/klauncher.desktop
%{_datadir}/icons/hicolor/scalable/apps/klauncher.svg

%changelog
* %(date "+%a %b %d %Y") KamiNuke vaityplasma@proton.me - 0.0.1-1
- Initial release
