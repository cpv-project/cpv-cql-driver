#!/usr/bin/env bash
# run tests in docker on travis ci
# reason:
# - build all things on ubuntu 14.04 is hard
# - even if the build is successful, the asan on 14.04 is buggy

# break after command error
set -e

# prepare docker
docker pull ubuntu:17.10

# copy ca certificate
cp -f ~/scylla-keys/cadb.pem .

# create container and run
# sed -i 's/archive.ubuntu.com/cn.archive.ubuntu.com/g' /etc/apt/sources.list
Command=$(cat <<"EOF"
  sed -i 's/archive.ubuntu.com/cn.archive.ubuntu.com/g' /etc/apt/sources.list && \
  apt-get update && \
  apt-get install sudo -y && \
  cd /project && \
  sh install-dependencies.sh && \
  cd tests && \
  mv -f cadb.pem /usr/local/share/ca-certificates/cadb.crt && \
  update-ca-certificates && \
  sh run_tests.sh
EOF
)
docker run \
  -it \
  --rm \
  --privileged \
  --net "host" \
  --volume "$(realpath ../):/project" \
  ubuntu:17.10 \
  bash -c "$Command"

