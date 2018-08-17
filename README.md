# Duality, yet another WIP 3D Game Engine
#### An amature venture with the goals of learning and having fun
> Duality Engine is written in **C++**
> - Renders using **OpenGL** 3.3+ , hoping to move to **Vulkan** soon!
> - Uses **SDL 2** for windowing/context creation and user input
> - Uses **Open Asset Importer** (assimp) for 3D model loading

----------
### Duality is based on an **Entity-Component-System** structure, designed for easy modability and expansion:

All game data is kept in components, which are kept in unsorted associative collections (hash maps).

Components are just bags of data. They are implemented as structs with no methods.

Components are accessible with an ID number, and two components that share an ID are considered by the engine to be parts of the same entity.
	
> Examples of components include
> - a position component to supply an entity with a location in 3D space
> - a model component to give an entity visual geometry
> - a camera component to allow the viewport to "see" from an entity's point of view.

> **Note** 
> There really are no entities, per se. An entity is an abstraction, described best as several components that share an ID. As such, pointers to components are not required.

A system is a set of code that runs on a loop, driven by an engine.

Systems are where all the game logic (the functions) lie.

Systems have access to the collection of components, and operate on the appropriate ones	every "tick".

> Examples of systems include
> - Rendering system - draws any existing model components each tick
> - Ballistics system - if a position component exists at the same ID as a velocity component, moves the position component accordingly each tick

An engine is essentially just a managed thread that can loop the "tick" of one or more	systems that have been assigned to it.

> Examples of engines include
> - Graphics engine - calls tick() on various rendering systems and maybe an animation system in a loop
> - Physics engine - calls tick() in a loop on a Ballistics system, a Collision system, a user input system, and more.

It's all configurable, lending a flexibility to multithreading.
#### all this enables a **Data-centric** design ideal, keeping game data entirely separate from the systems that operate on that data.
This is ideal for online games that draw game data froma server.
It also makes saving and loading game states naturally very simple.

Duality Engine also aims to investigate the worlds of **Physically Based Rendering** and procedurally generated destructible terrains and objects, possibly via **non-cubic voxels.** And what would an amature WIP game engine be without taking a swing at **machine learning** and **neural net AI** for good measure?
----------
### If we think of any other schmoozy buzzwords to stick in, this section may be expanded...
#### [shmooze] LOCK FREE
#### [shmooze] um... ORGANIC?

_____________________
## **Setup Guide using CMake**
Make sure the following are installed: SDL2, OPENGL version 3.3+, GLEW, Open Asset Importer, FreeType2, CMake version 3.3+
On windows, you'll need to add the above as environment variables. eg: name:SDL2 value: "C:\Program Files (x86)\SDL2-2.0.3"

* run cmake on the DualityEngine root directory from the directory in which you'd like Duality to be built.

* run make from that same build directory

_____________________
## **Setup Guide for CLion**
Make sure the following are installed: SDL2, OPENGL version 3.3+, GLEW, Open Asset Importer, FreeType2
### On Debian linux run:

* sudo apt-get install libsdl2-2.0-0

* sudo apt-get install libsdl2-dev

* sudo apt-get install libglew-dev

* sudo apt-get install libfreetype6-dev

* sudo apt-get install libglm-dev

#### Open the project in CLion
#### Build the project, set the target to DualityEngine, and then click run.
_____________________
### **Controls**
#### use  ~  (tilde) to access the in-game console, into which you may type "help" to see a listing of available commands.
#### Given an entity possessing camera, position, orientation, linear velocity, angular velocity, and control components (currently limited to spaceship-style controls), movement inputs are set up in the standard WASD-mouse configuration (or arrow keys to rotate view also work for now).
#### TL;DR : once running ("no cam" should be visible in black window), hit ~, type "run scene0", fiddle.
