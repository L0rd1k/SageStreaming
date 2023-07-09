#!/bin/bash

PACKAGE_DIR="boost"
CURRENT_DIR=$PWD
BUILD_DIR="$PWD/build_${PACKAGE_DIR}"
BOOST_VERSION="1.74.0"

rm -rf $BUILD_DIR
mkdir -p packs
mkdir -p $BUILD_DIR/usr
mkdir -p $BUILD_DIR/DEBIAN

cd ../$PACKAGE_DIR/
git checkout $PACKAGE_DIR-$BOOST_VERSION


./bootstrap.sh --prefix=/usr/local/$PACKAGE_DIR-$BOOST_VERSION
./b2

DEB_PACK_ARCHITECTURE=`dpkg --print-architecture`
DEB_PACK_SIZE=`du -sh -b ${BUILD_DIR} | cut -f1`
cat >$BUILD_DIR/DEBIAN/control <<EOL
Package: Boost-dev
Version: $BOOST_VERSION
Section: multimedia
Priority: optional
Architecture: ${DEB_PACK_ARCHITECTURE}
Homepage: "https://github.com/boostorg/boost"
Installed-Size: ${DEB_PACK_SIZE}
Maintainer: "Boost"
Description: "Boost library"
EOL

sudo ./b2 install

cd $CURRENT_DIR
fakeroot dpkg-deb -b $BUILD_DIR ./packs
echo "Boost pack created!"





