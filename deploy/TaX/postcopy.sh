#!/bin/bash -x

USERNAME="tax"

pushd '/home/tax'

curl -sL https://deb.nodesource.com/setup_4.x | bash -
apt-get install -y nodejs
apt-get install -y npm

npm install

chown -R root:root .
chown -R tax:tax ./data
chown tax *.db

popd
