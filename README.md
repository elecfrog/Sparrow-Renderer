# Sparrow OpenGL Renderer

A Tiny OpenGL Renderer Playground in C++.

## Build and Run

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Roadmap

This project is currently in a chaos, most of the code made me shamed. The only reason I make it currently now is that I need to refer it in my own coursework.

## FeatureList

- Basis
  - Forward Rendering Pipeline
  - Skybox
  - Model Loading(GLTF / FBX / OBJ)
  - Multiple Scenes
  - IMGUI UI Controls
  - Input and Window System
  - Camera System
  - Automatic Multiple Texture
- Rendering
  - OpenGL RHI( still in processing strictly...)
  - Simple Triangle (Scene01_Triangle.hpp)
  - Forward Albedo PBR (Scene_LoadModel.hpp)
  - Forward Bloom (Scene_LoadModel.hpp)
  - Forward SSAO (🐞 in repairing)
  - Forward IBL (🐞 in repairing)
- Animation
  - BVH Viewer(Scene_BVHView.cpp)
- Simulation
  - Cloth Simulation(Scene_ClothSimulationSS1.hpp & Scene_ClothSimulationSS2.hpp)

## `TODO` WORKS

- Rendering Features
  - [ ] SSAO
  - [ ] IBL
  - [ ] Deferred Shading
- Animation Features
  - [ ] Inverse Kinematics (BVH/FBX/GLTF)
- Simulation Features
  - [ ] Rigidbody
  - [ ] SPH Fluids
- Application
  - [ ] C++20 Coroutine Introduction

## Acknowledge

- Most of the examples followed by [LearnOpenGL](https://learnopengl.com/)
- The original code framework followed by [The Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)
- [Youtube OpenGL](https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-) is a good project to learn how to implement OpenGL features in OOP mode
- [Piccolo Engine](https://github.com/BoomingTech/Piccolo) is a good project with engine structure. Files like WindowSystem, LogSystem are copy from this project.
