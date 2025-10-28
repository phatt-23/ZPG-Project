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

# Known Problems

## The VRAM is not cleared between scene switches

- This means that the resources are not being freed on the GPU.
- If the VRAM's capacity is exceeded, then the geometries and textures will get corrupted.
- One solution is to clear the VRAM before switching scenes. 
- But this will cause long loading times between scene switches.