# consumer_data_privacy_gca



# consumer-dna-privacy

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


# Hack emp-ag2pc


In the dependencies/emp-ag2pc/CMakeLists.txt file,  add the following line...

```
add_test (2pc_scal)
```


Go to `test/2pc_scal.cpp` and make the following change...


```
+++ b/test/2pc_scal.cpp
@@ -1,5 +1,5 @@
 #include <emp-tool/emp-tool.h>
-#include "2pc.h"
+#include "../emp-ag2pc/2pc.h"
 using namespace std;
 using namespace emp;
 ```
 
 
 Then while in the `dependencies/emp-ag2pc` directory...
 
 ```
 cmake . && make && ./bin/2pc_scal 1 2 3 4 5
 ```
 
