#!/usr/bin/env bash

if [ -z ${VERSION} ]
then
    echo VERSION variable is not specified
    VERSION=$(date +"%Y%m%d")
    echo VERSION defatuls to ${VERSION}
fi

DOCKER_IMAGE=quay.io/pypa/manylinux2014_x86_64
# NOTE(jigu): need to wrap in an additional variable `COMMAND` to use $@
COMMAND="cd /workspace && bash ./dev/build_wheels.sh $@"

docker run -v `pwd`:/workspace --rm -it \
    -u $(id -u ${USER}):$(id -g ${USER}) \
    -e PYCOACD_VERSION=${VERSION} \
    ${DOCKER_IMAGE} bash -c "${COMMAND}"