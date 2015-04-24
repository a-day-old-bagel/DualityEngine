# WIP 3D Game Engine
	An amature attempt, with the goals of learning and having fun
# C++ formatted for NetBeans
	Currently uses SDL 2 and OpenGL 3.3
# Entity-Component-System structure, designed for easy modability and expansion.
	Every component has an ID.
	Components are only composed of data. They are implemented as structs.
	All game data is kept in components, which are kept in unsorted collections, accessible as hash maps.
	Examples of components include a motion component, allowing a game entity to be mobile, a model component, giving an entity a physical (visual) form, and a camera component, allowing the viewport to be switched to an entity's point of view
	There really are no entities, per se. An entity is an abstraction, described best as several components that share an ID.
	A system is a set of code that runs on a loop, driven by an engine.
	Systems are where all the logic (the functions) lie.
	Systems have access to the collection of components, and operate on the appropriate ones every "tick".
	An example of a system is the render system, which draws all model components (or other visual components) every tick.
# Data driven design ideals, keeping game data entirely separate from the systems that operate on that data.
	Having all of the data in components, and all of the functions in systems, allows for the data to be kept anywhere. This is ideal for online games that draw game data from a server.
	It also makes saving and loading game states naturally very simple.
