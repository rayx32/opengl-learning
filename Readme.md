## Itch Engine

A small SDL based game engine with support for HTML5 deployments

## Building

### Prerequisites
- vcpkg (Windows only)
- SDL2 (Handled automatically via vcpkg in Windows)
- Emscripten

#### Building for Web  
``mkdir build-web``  
``cd build-web``  
``cmake -G Ninja -DUSE_EMSCRIPTEN=ON -DEMSCRIPTEN_ROOT_PATH=D:/workspace/emsdk/upstream/emscripten ..``

#### Building for Desktop Windows  
``mkdir build``  
``cd build``  
``cmake -G "Ninja" ..``
