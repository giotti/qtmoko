#!/bin/bash

# Settaggio specifico per l'applicazione
APP="qbriscola"
REV=$(svn info -r HEAD | grep Revisione: | awk '{print $2}')
VERSION=1.1~svn$REV
GROUP="games"
BUILD=build
mkdir -p $BUILD
SPEC=$BUILD/description-pak
echo "Gioco della briscola" > $SPEC
REQUIRES="libc6 (>= 2.1.3), libqtcore4 (>= 4.4.3), libqtgui4 (>= 4.4.3), libstdc++6 (>= 4.1.1), libqt4-dbus (>= 4.4.3), libqt4-network (>= 4.4.3), libqt4-sql (>= 4.4.3), libqt4-sql-sqlite (>= 4.4.3)"

# Settaggi comuni
svn up
MANTEINER=francesco.betti.sorbelli@gmail.com
LICENSE=GPL
ARCH=i386
PKGDEST=/home/fra/debian/binary


mkdir -p $BUILD
cd $BUILD
cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix`
make -j2

echo "####################################################"
echo
echo $REQUIRES
echo
echo "####################################################"

rm -f $PKGDEST/$APP*.deb

checkinstall -D \
    --arch=$ARCH \
    --pkglicense=$LICENSE \
    --pkgname=$APP \
    --pkggroup=$GROUP \
    --pkgversion=$VERSION \
    --pakdir=$PKGDEST \
    --maintainer=$MANTEINER \
    --nodoc \
    --fstrans \
    --install=yes \
    --deldoc=yes \
    --deldesc=yes \
    --delspec=yes \
    --backup=no

rm -f *.tgz
rm -f *.deb
rm -f $SPEC

cd $PKGDEST/../
dpkg-scanpackages binary /dev/null | gzip -9c > binary/Packages.gz

# dpkg -i $PKGDEST/$APP*.deb
# rm -f $PKGDEST/$APP*.deb
