#!/bin/bash

if [ -z "$1" ]; then
    echo "Missing distribution name (i.e. debian/testing)"
    exit 1
fi

dist=$(echo $1 | cut -d '/' -f 1)
name=$(echo $1 | cut -d '/' -f 2)

BASETGZ="/var/cache/pbuilder/base-${dist}-${name}.tgz"
MIRROR="ftp://mirror.switch.ch/mirror/${dist}/"
OTHERMIRROR="deb [trusted=yes] http://grid-deployment.web.cern.ch/grid-deployment/dms/dmc/repos/apt/${distribution}/ dmc/"
if [ "${dist}" = "ubuntu" ]; then
    OTHERMIRROR="$OTHERMIRROR|deb ftp://mirror.switch.ch/mirror/${dist} ${name} universe"
fi

TEMPDIR=`mktemp -d`
sudo make deb \
    PBUILDER_FLAGS="--buildresult . --distribution '${name}' --basetgz '${BASETGZ}' --mirror '${MIRROR}' --othermirror '${OTHERMIRROR}'" \
    RELEASE=r`date +%y%m%d%H%M`\
    PBUILDER_TMP="${TEMPDIR}"
ls *.deb
sudo rm -rf "${TEMPDIR}"

