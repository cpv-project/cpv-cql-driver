#!/usr/bin/env bash
# run tests in docker on travis ci
# reason:
# - build all things on ubuntu 14.04 is hard
# - even if build successfully, address sanitizer won't work on 14.04

# break after command error
set -e

# get absolute paths
TESTS_DIR=$(realpath .)
PROJECT_DIR=$(realpath ../)

# start scylla containers (2 ssl and 1 simple)
git clone https://github.com/cpv-project/cpv-manage-scripts
cd ./cpv-manage-scripts/scylla
sh setup_node.sh
sh generate_ca_key.sh
sh generate_db_key.sh ssl-1
sh generate_db_key.sh ssl-2
sh create_ssl_instance.sh ssl-1 "-p 9044:9042 -p 9164:9160"
sh create_ssl_instance.sh ssl-2 "-p 9045:9042 -p 9165:9160" "--seeds $(docker inspect --format='{{ .NetworkSettings.Networks.database.IPAddress }}' db-ssl-1)"
docker run \
  --name "db-simple" \
  --hostname "db-simple" \
  --restart always \
  -p 9043:9042 \
  -p 9163:9160 \
  -d scylladb/scylla
cd "${TESTS_DIR}"

# copy ca certificate
cp -f ~/.scylla/keys/ca.pem .

# start test container
Command=$(cat <<"EOF"
  apt-get update && \
  apt-get install -y ca-certificates software-properties-common && \
  add-apt-repository -y ppa:compiv/cpv-project
  apt-get update
  apt-get install -y seastar
  cd /project/tests && \
  cp -f ca.pem /usr/local/share/ca-certificates/ca.crt && \
  update-ca-certificates && \
  sh run_tests.sh
EOF
)
docker run \
  -ti \
  --rm \
  --privileged \
  --net host \
  --volume "${PROJECT_DIR}:/project" \
  ubuntu:18.04 \
  bash -c "$Command"

# cleanup
docker rm -f $(docker ps -a -q)

