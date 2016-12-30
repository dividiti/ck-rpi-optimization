
# Search status

A number of experiments have been made on RPi 3 Model B (rev. a02082) and RPi B+ (rev. 0013). The detailed results
(experiment directories with point descriptions and all required information) are available archived in files 
[experiment-rpi-rev-a02082.zip](./experiment-rpi-rev-a02082.zip) and 
[experiment-rpi-rev-0013.zip](./experiment-rpi-rev-0013.zip).

Some recap is below:

1. Benchmark programs have been made for the following software: [zlib][], [gzip][], [7z][], [aubio][] (a lib used by [SonicPi][]), [ccrypt][], [sha512sum][], [rhash][]

1. 19 raw, [deflated][zlib] and [gzipped][gzip] datasets have been prepared for benchmarking purposes.

1. Found speed increase by 11% for calculating SHA3-512 hashes with the [rhash][] program with compilation flags (before pruning):

        -O3 -fassociative-math -fcx-limited-range -fno-delete-null-pointer-checks -fno-inline-small-functions -fipa-pta -fisolate-erroneous-paths-attribute -flive-range-shrinkage -fomit-frame-pointer -frename-registers -fno-sched-spec-load -fsched-group-heuristic -fsched-spec-insn-heuristic -fno-sched-rank-heuristic -fno-sched-dep-count-heuristic -fno-schedule-insns -fno-split-ivs-in-unroller -fno-tree-reassoc -fno-unroll-loops -fno-whole-program -fno-wpa -fexcess-precision=fast

1. Found speed increase by 19% for decompressing some files with the [gzip][] program with compilation flags (after pruning): `-O3 -funroll-loops`

1. Found speed increase by 19% for compressing some files with [zlib][] with compilation flags (after pruning): 

        -O3 -fgcse-sm -fno-move-loop-invariants -fno-tree-forwprop -funroll-all-loops -fno-web 

[zlib]: http://www.zlib.net/
[gzip]: http://www.gzip.org/
[7z]: http://www.7-zip.org/
[aubio]: https://aubio.org/
[SonicPi]: http://sonic-pi.net/
[ccrypt]: http://ccrypt.sourceforge.net/
[sha512sum]: https://www.gnu.org/software/coreutils/coreutils.html
[rhash]: https://packages.debian.org/jessie/rhash
