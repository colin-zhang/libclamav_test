#!/usr/bin/env python3
import os

pwd = os.getcwd()
#clamav_rel_dir = os.popen('find . -maxdepth 1 -type d -name "*clamav*"').read()

clamav_dir = os.path.join(pwd, "clamav-devel-clamav-0.100.0")
install_dir = pwd + "/opt"

def confiure_clamav():
    os.putenv('CPPFLAGS', '-g')
    os.putenv('CFLAGS', '-g')
    os.chdir(clamav_dir)
    os.system('make distclean')

    cmd = './configure --prefix=' + install_dir + \
          ' --exec-prefix=' + install_dir + \
          ' --enable-static  --disable-rpath ' + \
          ' --enable-fast-install ' + \
          ' CFLAGS="-I%s/include"'%(install_dir) + \
          ' LDFLAGS="-I%s/lib -l:libhs.a "'%(install_dir)
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

def run():
    try:
        confiure_clamav()
        make_clamav()
    finally:
        print("error")

if __name__=='main':
    run()
else:
    run()
