#!/bin/sh

set -e


git clone git@github.com:Cisco-Talos/clamav-devel.git

mkdir -p opt
mkdir -p pre/database

wget http://database.clamav.net/main.cvd  -o pre/database/main.cvd
wget http://database.clamav.net/daily.cvd  -o pre/database/daily.cvd
wget http://database.clamav.net/bytecode.cvd  -o pre/database/bytecode.cvd

