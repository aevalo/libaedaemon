#!/bin/bash

PWD=`pwd`
PKGNAME=`egrep '^pkgname' $PWD/PKGBUILD | awk -F= '{print $2}'`
PKGVER=`egrep '^pkgver' $PWD/PKGBUILD | awk -F= '{print $2}'`

echo "Creating temporary folder structure to /tmp/$PKGNAME-$PKGVER..."
mkdir -p /tmp/$PKGNAME-$PKGVER
mkdir -p /tmp/$PKGNAME-$PKGVER/cmake
mkdir -p /tmp/$PKGNAME-$PKGVER/scripts
mkdir -p /tmp/$PKGNAME-$PKGVER/src

echo "Copying files..."
cp    ../README /tmp/$PKGNAME-$PKGVER/
cp    ../CMakeLists.txt /tmp/$PKGNAME-$PKGVER/
cp -r ../cmake/* /tmp/$PKGNAME-$PKGVER/cmake
cp -r ../scripts/* /tmp/$PKGNAME-$PKGVER/scripts
cp -r ../src/* /tmp/$PKGNAME-$PKGVER/src

echo "Creating package $PKGNAME-$PKGVER.tar.gz"
tar --directory /tmp -czf $PKGNAME-$PKGVER.tar.gz $PKGNAME-$PKGVER

echo "Removing temporary folder..."
rm -rf /tmp/$PKGNAME-$PKGVER

CHECKSUM=`sha256sum libaedaemon-0.0.1.tar.gz | cut -d ' ' -f1`
echo "Checksum for the file is $CHECKSUM"

echo "Done."
