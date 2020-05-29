# Usage
0. change **isSS** in **CFIHello.cpp**(line 23) and **X86ShadowStackCFIPass.cpp**(line 49) to **false**
1. change **targetfileName** and **preloadStr** in **victim.c**(line 8-11)
2. make your llvm
3. make
4. `./victim 1` (or `./victim 2`, `./victim 3`)