#!/usr/bin/env bash
set -e

cd ${BUILDDIR}
DESTDIR=${ROOTDIR} make install V=1

cd ${BUILDDIR_DEBUG}
DESTDIR=${ROOTDIR_DEBUG} make install V=1

cd ${ROOTDIR}
cp -f ${ROOTDIR_DEBUG}/usr/lib/x86_64-linux-gnu/libCQLDriver.so \
	${ROOTDIR}/usr/lib/x86_64-linux-gnu/libCQLDriver_debug.so
cp -f ${ROOTDIR_DEBUG}/usr/lib/x86_64-linux-gnu/pkgconfig/cqldriver.pc \
	${ROOTDIR}/usr/lib/x86_64-linux-gnu/pkgconfig/cqldriver-debug.pc
strip ${ROOTDIR}/usr/lib/x86_64-linux-gnu/*.so

sed -i "s#-lCQLDriver#-lCQLDriver_debug#g" \
  ${ROOTDIR}/usr/lib/x86_64-linux-gnu/pkgconfig/cqldriver-debug.pc

patchelf --set-soname libCQLDriver_debug.so ${ROOTDIR}/usr/lib/x86_64-linux-gnu/libCQLDriver_debug.so


