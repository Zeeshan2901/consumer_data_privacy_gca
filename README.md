# consumer_data_privacy_gca


# The Garbled Circuit Approach



Related overleaf document:
https://www.overleaf.com/project/5c2ff8405c17106274d0b564


This project uses submodules. You can pull them with this command:

```
git submodule update --init --recursive
```

## Core dependencies

Install dependencies, following this script...
https://github.com/emp-toolkit/emp-readme/blob/master/scripts/install_packages.sh



# Build relic

Go into relic...

```
cd dependencies
cd relic
cmake -DALIGN=16 -DARCH=X64 -DARITH=curve2251-sse -DCHECK=off -DFB_POLYN=251 -DFB_METHD="INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK" -DFB_PRECO=on -DFB_SQRTF=off -DEB_METHD="PROJC;LODAH;COMBD;INTER" -DEC_METHD="CHAR2" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -march=native -msse4.2 -mpclmul" -DTIMER=CYCLE -DWITH="MD;DV;BN;FB;EB;EC" -DWSIZE=64 .
make
sudo make install
```


# Build emp-tool

```
cd dependencies
cd emp-tool
cmake .
make
sudo make install
```


# Build emp-ot

```
cd dependencies
cd emp-ot && cmake . && sudo make install
```

# Build emp-ag2pc

```
cd dependencies
cd emp-ag2pc && cmake . && make
```


Owen: I had some troubles with emp-ot and emp-ag2pc.  Cmake had generated
makefiles that left out -lrelic

I just went into the affected XYZ.dir directories (maybe 7)
and manually hacked in -lrelic  ; of course this is not the right way
to proceed, but it let me build the tools.

Had to do the same thing in project's toplevel and edit
CMakefiles/*.dir/link.txt

 
