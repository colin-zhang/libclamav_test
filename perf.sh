#!/bin/sh
set -e
set -u

#https://gperftools.github.io/gperftools/cpuprofile.html
#env CPUPROFILE_FREQUENCY=500

#pprof --svg ./av_test scan.perf > scan.svg && chromium-browser scan.svg

perf_file_list=`find -name "scan_*.perf" `

for perf_file in $perf_file_list; do
    pprof --callgrind ./av_test $perf_file > $perf_file.callgrind
    pprof --svg ./av_test $perf_file > $perf_file.svg 
    chromium-browser $perf_file.svg
done

#kcachegrind scan_01.perf.callgrind

##
#valgrind --tool=callgrind ./av_test -d pre/database/temp/daily
