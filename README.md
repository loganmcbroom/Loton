# Loton
Experimental Audio Workstation  

# Usage
Guide coming soon, for now drag and drop audio and functions into the graph and connect them.

# Building
Loton relies on the following:

	libsndfile
		git clone https://github.com/libsndfile/libsndfile.git
		cd libsndfile
		mkdir CMakeBuild
		cd CMakeBuild
		cmake ..
		Build with whatever tool cmake generates to
		There is also a windows installer available here: http://www.mega-nerd.com/libsndfile/#Download

	FFTWF 	
		https://www.fftw.org/
		I wish you luck.

	OpenCL: 
		This should exist on most computers already.
		Contact me if you need help with it.

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
		git clone https://github.com/loganmcbroom/flan
		cd flan
		mkdir build
		cd build
		cmake .. 
		Build with whatever tool cmake generates to

	My NodeEditor fork
		git clone https://github.com/loganmcbroom/nodeeditor
		cd nodeeditor
		mkdir build
		cd build
		cmake .. 
		Build with whatever tool cmake generates to

IMPORTANT: The flan and nodeeditor .libs are currently placed in the wrong folder, they should be "flan/lib/flan.lib" and "nodeeditor/lib/nodes.lib". On my todo.