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

# Deferred Shading notes

- u_gPosMap seems to be working correctly
- writing to u_gNormalMap values in range [-1,1] will map them into the range [0,1]

```
decode v :: 2.0 * v - 1.0
encode v :: 0.5 * v - 0.5

encode 1 == 0.5 * 1 - 0.5 == 0
encode 0 = 0.5 * 0 - .5 == -0.5
```




# Known Problems

## The VRAM is not cleared between scene switches

- This means that the resources are not being freed on the GPU.
- If the VRAM's capacity is exceeded, then the geometries and textures will get corrupted.
- One solution is to clear the VRAM before switching scenes. 
- But this will cause long loading times between scene switches.
