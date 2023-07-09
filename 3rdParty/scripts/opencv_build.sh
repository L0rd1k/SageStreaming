#!/bin/bash

PACKAGE_DIR="opencv"
CURRENT_DIR=$PWD
BUILD_DIR="$PWD/build_${PACKAGE_DIR}"


rm -rf $BUILD_DIR
mkdir -p packs
mkdir -p $BUILD_DIR/usr
mkdir -p $BUILD_DIR/DEBIAN

cd ../$PACKAGE_DIR/
mkdir build
cd build

rm ../CMakeCache.txt

cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
-DCMAKE_INSTALL_PREFIX=/usr/local/opencv_4.7.0 \
-DOPENCV_GENERATE_PKGCONFIG=ON \
-DBUILD_EXAMPLES=OFF \
-DINSTALL_PYTHON_EXAMPLES=OFF \
-DINSTALL_C_EXAMPLES=OFF \
-DWITH_GSTREAMER=ON \
-DWITH_CUDA=ON \
-DENABLE_PRECOMPILED_HEADERS=OFF ..

make -j$(nproc)

DEB_PACK_ARCHITECTURE=`dpkg --print-architecture`
DEB_PACK_SIZE=`du -sh -b ${BUILD_DIR} | cut -f1`
cat >$BUILD_DIR/DEBIAN/control <<EOL
Package: Opencv-dev
Version: 4.7.0
Section: multimedia
Priority: optional
Architecture: ${DEB_PACK_ARCHITECTURE}
Homepage: "https://github.com/opencv/opencv"
Installed-Size: ${DEB_PACK_SIZE}
Maintainer: "L0rd1k"
Description: "OpenCV library"
EOL

make install DESTDIR=$BUILD_DIR
ldconfig

cd $CURRENT_DIR
fakeroot dpkg-deb -b $BUILD_DIR ./packs
echo "Opencv pack created!"





