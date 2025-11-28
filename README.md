# ZPG Project

To build and run the project:

```sh
mkdir build
cd build
cmake ..

# only build (compile with 10 threads)
cmake --build . -j10

# build + run
cmake --build . --target=run -j10
```

The program must be ran from the directory containing this README.md.

Project Structure:
```txt
ZPGine/     # Engine code
Client/     # Client code
README.md   
```

