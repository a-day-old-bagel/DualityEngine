# Duality, a WIP 3D Game Engine
####An amature venture with the goals of learning and having fun

> Duality Engine is written in **C++**

> - Renders using **OpenGL** 3.3+ , hoping to move to **Vulkan** soon!

> - Uses **SDL 2** for windowing/context creation and user input

> - Uses **Open Asset Importer** (assimp) for 3D model loading

> - For the moment, Duality is being developed using **NetBeans**

----------
###Duality is based on an **Entity-Component-System** structure, designed for easy modability and expansion:

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

> - Graphics engine - calls tick() on a a rendering system and maybe an animation system in a loop

> - Physics engine - calls tick() in a loop on a Ballistics system, a Collision system, a user input system, and more.

It's all configurable, lending a flexibility to multithreading.
#### all this enables a **Data-centric** design ideal, keeping game data entirely separate from the systems that operate on that data.
This is ideal for online games that draw game data froma server.
It also makes saving and loading game states naturally very simple.

----------
###Duality Engine also aims to investigate the world of **Physically Based Rendering**, and maybe go one step further, investigating the possibility of a fully **unified physics engine** that can handle the typically ray-based lighting calculations of modern PBR and the calculation of kinematic interactions of game objects using a single model.
