# 🧱 Voxel Engine

A custom Minecraft-like voxel engine built from scratch using **C++**, **SDL2**, **OpenGL**, and **CMake**.

This project was a deep dive into game engine architecture, voxel rendering, performance optimization, and data structures.

<img width="1879" height="1007" alt="Image" src="https://github.com/user-attachments/assets/2479e043-fce5-43bc-bca1-93fe837f54ca" />

---

## 🚀 Features

- ✅ Chunk-based world rendering  
- ✅ **Greedy meshing** for performance-optimized voxel rendering  
- ✅ **Face culling** to eliminate hidden surfaces  
- ✅ **Texture atlas** for efficient texture usage  
- ✅ **Chunk serialization** using binary files  
- ✅ Custom input handling with SDL2  
- ✅ Basic camera controls

---

## 🧠 What I Learned

- Advanced **OpenGL** rendering (VBOs, VAOs, shaders)  
- Efficient memory management and dynamic data handling  
- Use of **3D arrays**, **unordered maps**, **bitsets**, and **linked lists**  
- Greedy meshing algorithms  
- Binary file I/O  
- Basics of **multithreaded chunk generation**  
- Using **CMake** to organize and build cross-platform projects

---

## 🔧 Building the Project

### 🛠 Requirements

- C++17 compiler  
- CMake >= 3.10  
- SDL2  
- OpenGL  
- GLEW or GLAD (depending on your setup)

### 🧪 Build Instructions

```bash
# Clone the repo
git clone https://github.com/LakhyarajRajkhowa/VoxelEngine
cd VoxelEngine

# Create build directory and run CMake
mkdir build && cd build
cmake ..

# Build the project
cmake --build .

- Copy all the binaries from /bin folder to the /build folder 
- Run the VoxelEngine.exe program
