# Maintainer: HanSolo1000Falcon <hansolo1000falcon.business@gmail.com>
pkgname=curx-git
pkgver=r3.e448873
pkgrel=1
pkgdesc="Simple CLI currency exchange program."
arch=('x86_64')
url="https://github.com/HanSolo1000Falcon/curx"
license=('MIT')
depends=('curl' 'cjson')
makedepends=('cmake' 'clang' 'git')
provides=('curx')
conflicts=('curx')
source=("${pkgname}::git+https://github.com/HanSolo1000Falcon/curx.git")
sha256sums=('SKIP')

pkgver() {
  cd "$pkgname"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd "$pkgname"
  cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build build
}

package() {
  cd "$pkgname"
  install -Dm755 build/curx "$pkgdir/usr/bin/curx"
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
