#!/usr/bin/env bash
set -e

GITREF=`git rev-parse --short HEAD`
BUILD="devel"

if [[ -z ${BRANCH} ]]; then
  BRANCH=`git name-rev $GITREF --name-only`
else
  printf "Using environment set variable BRANCH=%s\n" "${BRANCH}"
fi

if [[ $BRANCH =~ ^(tags/)?(v)[.0-9]+(-[0-9]+)?$ ]]; then
  BUILD="rc"
fi

DIST=$(rpm --eval "%{dist}" | cut -d. -f2)
DISTNAME=${DIST}

# Special handling of FC rawhide
[[ "${DISTNAME}" == "fc35" ]] && DISTNAME="fc-rawhide"
[[ "${DISTNAME}" == "fc36" ]] && DISTNAME="fc-rawhide"

if [[ ${BUILD} == "rc" ]]; then
	REPO_PATH="${BUILD}/${DISTNAME}/\$basearch"
elif [[ ${BRANCH} == "develop" ]]; then
	REPO_PATH="testing/${DISTNAME}/\$basearch"
else
	REPO_PATH="testing/${BRANCH}/${DISTNAME}/\$basearch"
fi

printf "Installing dmc-%s-%s in /etc/yum.repo.d/\n" ${BRANCH} ${DISTNAME}
cat <<- EOF > /etc/yum.repos.d/dmc-${BRANCH}-${DISTNAME}.repo
	[dmc-${BRANCH}-${DISTNAME}]
	name=DMC Development Repository
	baseurl=http://dmc-repo.web.cern.ch/dmc-repo/${REPO_PATH}
	gpgcheck=0
	enabled=1
	protect=0
	priority=3
	EOF
