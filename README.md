# Klauncher
A Qt/Kirigami proton launcher for linux

### Disclaimer
Currently, code in an insanely bad state, many things don't make any sense nor work properly, so I wouldn't recommend using it for now. However, in theory, it should work, and be compatible with future versions(Not sure yet)

### Installation

#### Requirements

- Dependencies

```
base-devel extra-cmake-modules cmake kirigami kirigami-addons ki18n kcoreaddons breeze kiconthemes qt6-base qt6-declarative qqc2-desktop-style
```

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

