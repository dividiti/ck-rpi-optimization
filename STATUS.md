
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

1. Found a possible issue in GCC 4.9.2: it fails to compile [aubio][] with the following message/flags. GCC6 works fine.

        [57/67] Compiling examples/aubiotrack.c
        16:22:40 runner ['gcc-4.9', '-DXOPENME', '-I/home/pi/CK-TOOLS/lib-rtl-xopenme-0.3-gcc-4.9.2-linux-32/include', '-O3', '-fbranch-probabilities', '-ffloat-store', '-fif-conversion2', '-findirect-inlining', '-fno-ipa-pure-const', '-floop-strip-mine', '-floop-nest-optimize', '-fmodulo-sched', '-fno-branch-count-reg', '-fsched-interblock', '-fno-sched-spec-load-dangerous', '-fselective-scheduling', '-ftracer', '-ftree-copy-prop', '-fno-tree-loop-if-convert-stores', '-fno-tree-phiprop', '-ftree-pre', '-fno-tree-sra', '-fno-tree-vrp', '-fno-unsafe-math-optimizations', '-fno-unswitch-loops', '-fno-vect-cost-model', '-fuse-linker-plugin', '-ffp-contract=fast', '-fPIC', '-I/home/pi/CK/ck-zlib/program/aubio/tmp-ck-B25KxH/aubio-0.4.3/build/src', '-I/home/pi/CK/ck-zlib/program/aubio/tmp-ck-B25KxH/aubio-0.4.3/src', '-DAUBIO_PREFIX="/usr/local"', '-DPACKAGE="aubio"', '../examples/aubiotrack.c', '-c', '-o', '/home/pi/CK/ck-zlib/program/aubio/tmp-ck-B25KxH/aubio-0.4.3/build/examples/aubiotrack.c.7.o']
        [58/67] Compiling aubio.pc.in
        *** Error in `/usr/lib/gcc/arm-linux-gnueabihf/4.9/cc1': corrupted double-linked list: 0x0170f5ce ***

[zlib]: http://www.zlib.net/
[gzip]: http://www.gzip.org/
[7z]: http://www.7-zip.org/
[aubio]: https://aubio.org/
[SonicPi]: http://sonic-pi.net/
[ccrypt]: http://ccrypt.sourceforge.net/
[sha512sum]: https://www.gnu.org/software/coreutils/coreutils.html
[rhash]: https://packages.debian.org/jessie/rhash
