#!/usr/bin/env bash
set -e

function print_info {
  printf "======================\n"
  printf "Distribution:\t%s\n" "${DIST}"
  printf "Dist name:\t%s\n" "${DISTNAME}"
  printf "Build type:\t%s\n" "${BUILD}"
  printf "Branch:\t\t%s\n" "${BRANCH}"
  printf "Release:\t%s\n" "${RELEASE}"
  printf "DMC Repository:\t%s\n" "${REPO_FILE}"
  printf "RPM build flags:\t%s\n" "${RPMBUILD_FLAGS}"
  printf "======================\n"
}

TIMESTAMP=`date +%y%m%d%H%M`
GITREF=`git rev-parse --short HEAD`
RELEASE=r${TIMESTAMP}git${GITREF}
BUILD="devel"

if [[ -z ${BRANCH} ]]; then
  BRANCH=`git name-rev $GITREF --name-only`
else
  printf "Using environment set variable BRANCH=%s\n" "${BRANCH}"
fi

if [[ $BRANCH =~ ^(tags/)?(v)[.0-9]+(-[0-9]+)?$ ]]; then
  RELEASE=
  BUILD="rc"
fi

DIST=$(rpm --eval "%{dist}" | cut -d. -f2)
DISTNAME=${DIST}

# Special handling of FC rawhide
[[ "${DISTNAME}" == "fc35" ]] && DISTNAME="fc-rawhide"
[[ "${DISTNAME}" == "fc36" ]] && DISTNAME="fc-rawhide"

# Write repository files to /etc/yum.repos.d/ based on the branch name
./ci/write-repo-file.sh

REPO_FILE="${BUILD}/dmc-${BUILD}-${DISTNAME}.repo"
print_info

RPMBUILD=${PWD}/build
SRPMS=${RPMBUILD}/SRPMS

cd packaging/
make srpm RELEASE=${RELEASE} RPMBUILD=${RPMBUILD} SRPMS=${SRPMS} RPMBUILD_SRC_EXTRA_FLAGS="${RPMBUILD_FLAGS}"

if [[ -f /usr/bin/dnf ]]; then
  dnf install -y epel-release || true
  dnf builddep -y ${SRPMS}/*
else
  yum-builddep -y ${SRPMS}/*
fi

rpmbuild --rebuild --define="_topdir ${RPMBUILD}" ${RPMBUILD_FLAGS} ${SRPMS}/*
