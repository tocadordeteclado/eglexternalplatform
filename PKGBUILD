#
# Contribuidor: {{ nome_do_autor(); }}
#

pkgname=eglexternalplatform
pkgver=1.1
pkgrel=2
pkgdesc="Interface de plataforma externa EGL"
url=http://localhost/eglexternalplatform
arch=(any)
license=(cIO)
depends=(libegl)
makedepends=(git)
source=("eglexternalplatform.tar.gz")
sha256sums=('SKIP')

pkgver()
{
    cd $pkgname
    git describe --tags | sed 's/[^-]*-g/r&/;s/-/+/g'
}

prepare()
{
    cd $pkgname
}

package()
{
    cd $pkgname
    install -Dt "$pkgdir/usr/include/EGL" -m644 interface/*
    install -Dt "$pkgdir/usr/share/pkgconfig" -m644 *.pc
    install -Dt "$pkgdir/usr/share/licenses/$pkgname" -m644 COPYING
}
