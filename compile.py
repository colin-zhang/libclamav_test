#!/usr/bin/env python3
import os
import argparse

pwd = os.getcwd()
#clamav_rel_dir = os.popen('find . -maxdepth 1 -type d -name "*clamav*"').read()

clamav_dir = os.path.join(pwd, "clamav-devel-clamav-0.100.0")
install_dir = pwd + "/opt"
vendors_dir = pwd + "/vendors"

if not os.path.exists(install_dir):
    os.mkdir(install_dir)
if not os.path.exists(vendors_dir):
    os.mkdir(vendors_dir)

def confiure_clamav():
    os.putenv('CPPFLAGS', '-g')
    os.putenv('CFLAGS', '-g')
    os.chdir(clamav_dir)
    os.system('make distclean')

    cmd = './configure --prefix=' + install_dir + \
          ' --exec-prefix=' + install_dir + \
          ' --enable-static  --disable-rpath ' + \
          ' --enable-fast-install ' + \
          ' CFLAGS="-DHAVE_REMATCHALGORITHM=1 -I%s/include"'%(install_dir) + \
          ' LDFLAGS="-L%s/lib -L%s/lib64 -l:libhs.a -fPIC  "'%(install_dir,install_dir)
    # " --with-zlib=" + install_dir \
    # --with-zlib=
    # --with-system-llvm=xxxxx/bin/llvm-config
    # " --with-libjson no "
    print(cmd)
    os.system(cmd)
    os.chdir(pwd)

def make_clamav():
    os.chdir(clamav_dir)
    cmd = 'make -j4 && make install'
    os.system(cmd)
    os.chdir(pwd)


def build_hyperscan():
    boost_dir="boost_1_67_0"
    boost_tar="boost_1_67_0.tar.gz"
    boost_url="https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz"

    boost_dir = os.path.join(vendors_dir, boost_dir)
    boost_tar = os.path.join(vendors_dir, boost_tar)

    if not os.path.exists(boost_tar):
        os.system("wget  %s -O %s"%(boost_url,boost_tar))
        os.system("tar -xvf %s -C %s"%(boost_tar,vendors_dir))

    hyperscan_dir=os.path.join(pwd, "vendors/hyperscan")
    if not os.path.exists(hyperscan_dir):
        os.mkdir(hyperscan_dir)
        os.chdir(vendors_dir)
        os.system("git clone https://github.com/intel/hyperscan")

    os.chdir(hyperscan_dir)
    os.system("cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_C_FLAGS=-fPIC -DBOOST_ROOT=%s -DCMAKE_INSTALL_PREFIX=%s  ."%(boost_dir,install_dir))
    os.system("make -j2 install")
    os.chdir(pwd)


def run():
    parser = argparse.ArgumentParser()
    parser.add_argument('-hyperscan', action='store_true', default=False)
    parser.add_argument('-configure', action='store_true', default=False)
    args = parser.parse_args()

    if args.hyperscan:
        build_hyperscan()
        os.exit(0)
    else:
        try:
            if args.configure:
                confiure_clamav()
            else:
                make_clamav()
        finally:
            pass
        
if __name__=='__main__':
    run()
