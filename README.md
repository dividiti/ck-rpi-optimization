[![compatibility](https://github.com/ctuning/ck-guide-images/blob/master/ck-compatible.svg)](https://github.com/ctuning/ck)
[![automation](https://github.com/ctuning/ck-guide-images/blob/master/ck-artifact-automated-and-reusable.svg)](http://cTuning.org/ae)
[![workflow](https://github.com/ctuning/ck-guide-images/blob/master/ck-workflow.svg)](http://cKnowledge.org)
[![License: CC BY 4.0](https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg)](http://creativecommons.org/licenses/by/4.0/)

Introduction
============

Demonstration of customizable Collective Knowledge autotuning workflows 
with a portable package manager to teach students how to compile, run, 
benchmark and optimize various workloads across diverse platforms
in a unified way!

License
=======
* BB CY 4.0

Copyright
=========
2016-2018 (c) dividiti

Status
======
Stable

Prerequisites
=============
* Collective Knowledge framework ([@GitHub](http://github.com/ctuning/ck))
* Python 2.7 or 3.3+
* Python PIP
* Git client

Minimal CK installation
=======================

The minimal installation requires:

* Python 2.7 or 3.3+ (limitation is mainly due to unitests)
* Git command line client.

You can install CK in your local user space as follows:

```
$ git clone http://github.com/ctuning/ck
$ export PATH=$PWD/ck/bin:$PATH
$ export PYTHONPATH=$PWD/ck:$PYTHONPATH
```

You can also install CK via PIP with sudo to avoid setting up environment variables yourself:

```
$ sudo pip install ck
```

CK workflow installation
========================


Install this CK repository:

```
 $ ck pull repo --url=https://github.com/dividiti/ck-rpi-optimization
```

Update all CK repositories at any time
```
 $ ck pull all
```

List available programs and data sets

```
 $ ck ls ck-rpi-optimization:program:*
 $ ck ls ck-rpi-optimization:dataset:*
```

Compile a given program on your platform via unified CK autotuning workflow with portable package manager:
```
 $ ck compile program:rhash --speed
```

Run a given program via unified CK autotuning workflow:

```
 $ ck run program:rhash
```

More details
============

Please check out a related report with shared aritfacts in another CK repository:
* https://github.com/ctuning/ck-rpi-optimization-results
