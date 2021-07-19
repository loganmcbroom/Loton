# Loton
Experimental Audio Workstation  

# Usage
Guide coming soon, for now drag and drop audio and functions into the graph and connect them.

# Building
Dependencies:
	Qwt
		I don't know of a build of this not relying on QtCreator.
		Download qwt-6.1.6 from here: https://qwt.sourceforge.io/
		Unzip it
		Open qwt.pro in QtCreator
		Hit build

	LuaJit
		git clone https://luajit.org/git/luajit.git
		cd luajit
		make

	Flan
		See https://github.com/loganmcbroom/flan

	My NodeEditor fork
		See https://github.com/loganmcbroom/nodeeditor

Loton:
	git clone https://github.com/loganmcbroom/Loton
	cd Loton
	cmake -S . -B build
	cmake --build build --config Release