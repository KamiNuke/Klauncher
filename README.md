# Klauncher
A Qt/Kirigami proton launcher for linux

### Screenshots
![](./screenshots/mainMenu.png)
![](./screenshots/addDialog.png)
![](./screenshots/appSettings.png)
![](./screenshots/options.png)
![](./screenshots/settingsPage.png)

### Installation

#### Build Dependencies

Arch Linux
```
sudo pacman -S --needed base-devel extra-cmake-modules cmake kirigami kirigami-addons ki18n kcoreaddons breeze kiconthemes qt6-base qt6-declarative qqc2-desktop-style
```

Fedora Linux
```
sudo dnf install @development-tools @development-libs cmake extra-cmake-modules kf6-kirigami2-devel kf6-ki18n-devel kf6-kcoreaddons-devel kf6-kiconthemes-devel qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtquickcontrols2-devel kf6-qqc2-desktop-style cmake ninja kf6-kirigami-addons-devel
```

#### Runtime Dependencies

`` umu-launcher icoutils ``

#### Any distro
if you want to install it to ~/.local change /usr to ~/.local

```bash
git clone https://github.com/KamiNuke/Klauncher
cd klauncher
cmake -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release --install-prefix /usr
cmake --build build --parallel
sudo cmake --install build
```

#### Arch Linux
```bash
mkdir klauncher && cd klauncher
wget https://raw.githubusercontent.com/KamiNuke/Klauncher/refs/heads/main/PKGBUILD
makepkg -si
```

