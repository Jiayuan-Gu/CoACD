#!/usr/bin/env bash

# conda install anaconda-client conda-build

VERSION=$(date +"%Y%m%d")
WHEEL_FILES=$(find ./wheelhouse -name pycoacd-${VERSION}-*.whl -print0 | xargs -r -0 ls -1 -t)
for WHEEL_FILE in ${WHEEL_FILES};
do
    echo upload ${WHEEL_FILE}
    anaconda upload ${WHEEL_FILE}
done