# Loton
Experimental Audio Workstation  

# Building

### Dependencies:

#### Qwt:
I don't know of a build of this not relying on QtCreator.  
Download qwt-6.1.6 from here: https://qwt.sourceforge.io/  
Unzip it  
Open qwt.pro in QtCreator  
Hit build  

#### LuaJit:
```
git clone https://luajit.org/git/luajit.git
cd luajit
make
```
Make sure the luajit folder is in the system PATH.

#### Flan:
See https://github.com/loganmcbroom/flan

#### My NodeEditor fork:
See https://github.com/loganmcbroom/nodeeditor

### Loton:
```
git clone https://github.com/loganmcbroom/Loton
cd Loton
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
Finally, make sure the qwt and lua shared libraries are visible.

# Usage

Guide coming soon, for now drag and drop audio and functions into the graph and connect them.