pkgname="klauncher-git"
pkgver=0.0.1
pkgrel=1
pkgdesc="A Qt/Kirigami game launcher for Linux"
url="https://github.com/KamiNuke/Klauncher"
arch=("x86_64")
license=("GPL-3.0-or-later")
sha512sums=("SKIP")

source=("git+https://github.com/KamiNuke/Klauncher")

depends=(
    "kirigami"
    "kirigami-addons"
    "kio"
    "qqc2-desktop-style"
    "umu-launcher"
    "icoutils"
)

optdepends=(
     "proton-ge-custom-bin"   
)

makedepends=(
    "cmake"
    "extra-cmake-modules"
    "git"
    "ninja"
)

provides=("$pkgname")
conflicts=("$pkgname")

build() {
    cmake \
        -S "$srcdir/Klauncher" \
        -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr

    cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build
}
