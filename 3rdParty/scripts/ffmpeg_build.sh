#!/bin/bash

PACKAGE_DIR="FFmpeg"
CURRENT_DIR=$PWD
BUILD_DIR="$PWD/build_${PACKAGE_DIR}"


rm -rf $BUILD_DIR
mkdir -p packs
mkdir -p $BUILD_DIR/usr
mkdir -p $BUILD_DIR/DEBIAN

cd ../$PACKAGE_DIR/
git checkout release/4.0


make clean > /dev/null | true
./configure	\
	--enable-debug		\
	--enable-gpl		\
	--enable-shared		\
	--enable-libx264	\
	--enable-libv4l2 	\
	--enable-rpath		\
	--disable-optimizations	\
	--disable-mmx		\
	--disable-stripping	\
	--disable-vaapi		\
	--disable-vdpau		\
	--disable-static	\
	--disable-doc		\
	--prefix=/usr/local/ffmpeg_dev

make -j$(nproc)

DEB_PACK_ARCHITECTURE=`dpkg --print-architecture`
DEB_PACK_SIZE=`du -sh -b ${BUILD_DIR} | cut -f1`
cat >$BUILD_DIR/DEBIAN/control <<EOL
Package: FFmpeg-dev
Version: 5.0
Section: multimedia
Priority: optional
Architecture: ${DEB_PACK_ARCHITECTURE}
Homepage: "https://github.com/L0rd1k/FFmpeg"
Installed-Size: ${DEB_PACK_SIZE}
Maintainer: "L0rd1k"
Description: "FFmpeg library"
EOL

make install DESTDIR=$BUILD_DIR

cd $CURRENT_DIR
fakeroot dpkg-deb -b $BUILD_DIR ./packs
echo "FFmpeg pack created!"





