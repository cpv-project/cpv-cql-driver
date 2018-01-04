#!/usr/bin/env bash
# break after command error
set -e

# setup docker
cd ../3rd-party/cpv-manage-scripts/scylla
sh docker_setup.sh
sh ssl_generate_cadb.sh || true

# create 1 scylla server without password and ssl
docker rm -f db-simple-1 || true
docker run \
  --name "db-simple-1" \
  --hostname "db-simple-1" \
  --net "database" \
  --ip "172.88.0.201" \
  --restart always \
  -p 9043:9042 \
  -d scylladb/scylla

# create 2 scylla server with password and ssl
sh ssl_generate_db.sh ssl-1 || true
sh ssl_generate_db.sh ssl-2 || true
sh docker_new_db.sh ssl-1 "-p 9044:9042"
sh docker_new_db.sh ssl-2 "-p 9045:9042" "--seeds $(docker inspect --format='{{ .NetworkSettings.Networks.database.IPAddress }}' db-ssl-1)"

