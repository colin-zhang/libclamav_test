#!/bin/sh
set -e

#if [[ -d  clamav-devel ]]; then
#    echo "exitst git pull"
#    cd clamav-devel && git pull
#    cd $OLDPWD
#else
#    git clone git@github.com:Cisco-Talos/clamav-devel.git
#fi

if [[ ! -d  clamav-devel-clamav-0.100.0 ]]; then
    wget https://github.com/Cisco-Talos/clamav-devel/archive/clamav-0.100.0.tar.gz
    tar -xf clamav-0.100.0.tar.gz
fi

if [[ ! -d  clamav-devel-clamav-0.99.4 ]]; then
    wget https://github.com/Cisco-Talos/clamav-devel/archive/clamav-0.99.4.tar.gz
    tar -xf clamav-0.99.4.tar.gz
fi

mkdir -p opt
mkdir -p pre/database

wget http://database.clamav.net/main.cvd -O   pre/database/main.cvd 
wget http://database.clamav.net/daily.cvd  -O   pre/database/daily.cvd
wget http://database.clamav.net/bytecode.cvd  -O   pre/database/bytecode.cvd

