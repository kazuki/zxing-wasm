#!/bin/bash
W=$(dirname $0)/build
NCPU=$(grep processor /proc/cpuinfo | wc -l)

rm -rf $W && mkdir $W && cd $W && \
    emcmake cmake -DBUILD_WRITERS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_BLACKBOX_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_UNIT_TESTS=OFF -DCMAKE_CXX_FLAGS="-O3 -flto" ../zxing-cpp && \
    emmake make -j$NCPU && \
    em++ --no-entry -O3 -flto -o ../zxing_reader.wasm \
         -I../zxing-cpp/core/src \
         -s STRICT=1 \
         -s ALLOW_MEMORY_GROWTH=0 \
         -s EXPORTED_FUNCTIONS="['_ZXing_ReadBarcode','_malloc','_free']" \
         -s ENVIRONMENT=web -s FILESYSTEM=0 \
         ../wrapper.cpp core/libZXing.a
