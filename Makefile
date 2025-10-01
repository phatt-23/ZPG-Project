all: build
	pushd $<
	cmake..
	cmake --build . --target=run -j10 
	popd

build:
	mkdir build
