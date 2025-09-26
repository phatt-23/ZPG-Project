# ZPG Project

To build and run the project:

```sh
mkdir build
cd build
cmake ..
cmake --build . --target=run -j10
```

To run from the root directory:

```sh
mkdir build
pushd build; cmake --build . --target=run -j10; popd
```