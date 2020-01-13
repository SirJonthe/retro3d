# retro3d
Copyright (c) 2020 Jonathan Karlsson

## About

Retro3d is a small 3D game engine written in C++ designed to be cross-platform, extensible, and easy to use. 

The engine started development as a response to game developers using Unity or Unreal Engine to develop games that aim to look and feel retro. The illusion often falls short when using a modern engines as a result of their modern capabilities. The goal of the engine is to easily create games that feel more retro than what can be easily achieved in a modern game engine - Games made with retro3d automatically look and feel retro without impacting playability.

## Features

### Platform agnostic

Retro3d tries to remain platform agnostic by making external interfaces and libraries implementation details. Abstract interfaces are used to more easily switch between external interfaces and libraries. Calls to external interfaces and libraries are localized to platform-dependent .cpp files in order for the rest of the engine to function independently of the external interfaces and libraries.

### ECS model

Retro3d supports an entity-component-system (ECS) model that Unity users will feel at home with.

```cpp
retro_component(Foo)
{
private:
	int32_t m_ticker;
protected:
	void OnSpawn( void )   override { m_ticker = 0; }
	void OnUpdate( void )  override { ++m_ticker; }
	void OnDestroy( void ) override { m_ticker = -1; }
public:
	Foo( void ) : mtlInherit(this), m_ticker(0) {}
};

/* ... */

retro3d::Entity *e = Engine.SpawnEntity();
Foo             *c = e->AddComponent<Foo>();
```

The ECS model is mainly a model of composition over inheritance where a game object, called entity (which is essentially a completely blank data slot) consists of a number of custom components that define its behavior. This eliminates the need to create a completely new type of entity when the behavior needs to be modified regardless of if its behavior should differ only slightly or even greatly from an already existing base object. Systems, in turn, work on classes of components, and can be readily compared to a rulebook of a game. Systems, however, are not just a way to modify the game rules, but also the behavior of retro3d itself.

A benefit with this kind of model is two-fold; The processor can work on multiple components in tandem enabling parallel processing, and the programmer can alter the behavior of each game object on the fly without having to resort to contrived gameplay code or inheritance on a game object basis when only a slight change of behavior is sought.

### Graphics

Retro3d comes with a software renderer by default. The software renderer is meant to emulate Playstation 1 and/or Quake era games. This means low resolution, low color depth, low polygon count, no antialiasing, and primitive lighting.

The renderer uses [tiny3d](https://github.com/SirJonthe/tiny3d.git) as its render API. Tiny3d offers a fast highly parallel rasterizer, and optimized texture access (Morton order, compression).

![alt text](https://i.imgur.com/yJDghZr.png "Color cell compression")

Tiny3d renders graphics, but does not interface with graphics hardware. As such tiny3d is combined with SDL in order to get access to the frame buffer and display graphics on the screen.

The graphics component of the engine is implemented as a platform independent device. Users of retro3d can implement their own renderers that adhere to the render device interface.

### Collisions and Physics

Retro3d comes with a custom collision and physics engine that detects collisions (using GJK and the expanding polytope algorithm) and applies proper responses to collisions (separating translations for movable colliders and linear and angular velocities for rigid bodies).

[![Video](https://i.imgur.com/kppXVx9.png)](https://i.imgur.com/yfbXti1.mp4 "Physics")
_Click image for video_

Warning - While collisions for colliders work as expected the physics engine is currently a work in progress and does not react as expected in all situations.

### Sound

Retro3d comes with a sound device based around SDL_mixer.

The sound component of the engine is implemented as a platform independent device. Users of retro3d can implement their own mixers that adhere to the sound device interface.

### Input

Retro3d comes with an input device based around SDL.

The input device makes interfacing with input devices abstract. Instead of querying if a button is pressed the used queries whether an action has been performed (instead of checking if SPACE is pressed, the used checks if "jump" is active).

The input component of the engine is implemented as a platform independent device. Users of retro3d can implement their own input devices that adhere to the input device interface.

## Optimization

### General

In general, retro3d uses only a few acceleration structures - mainly with regards to graphics rendering.

### Binary space partitioning (BSP)

Non-convex graphical objects are recursively split in half until the resulting geometry consists only of convex pieces. These pieces, together with their respective splitting plane are stored in a binary tree that, in combination with a view frustum, can be used to discard geometry outside of the hull and determine what order to draw the visible pieces so that they do not overlap.

[![Video](https://i.imgur.com/veW5Lzd.png)](https://i.imgur.com/b8MPBS7.mp4 "Binary space partitioning")
_Click image for video_

While BSP trees are common practice the difficulty lies in creating a well balanced tree that does not go needlessly deep.

### Bounding volume hierarchy (BVH)

A BVH is a binary tree that recursively groups two finite sub-spaces together to form a greater space containing both sub-spaces. A BVH has great general-purpose use within retro3d and is used to everything from collision detection, to the lighting model and the potentially visible set (PVS).

![alt text](https://i.imgur.com/m1VEvHG.png "Bounding volume hierarchy")

### Parallelism

The engine tries to embody a principle of data parallelism that enables for efficient, and easy-to-follow parallelism. Rather than letting threads do separate jobs, all threads do the same job on a proportional sub-set of the work load. Each thread gets to work undisturbed and in complete isolation from the other threads. This principle also enables efficient use of vector instructions (SIMD).

As an example, for the software renderer, the complete view frustum is split into a number of separate sub-frustums that each determine the potentially visible set (PVS) separately by a number of threads. The PVS:s are then traversed in-order and rendered by a number of threads to separate portions of the screen where pixels are processed in groups using SIMD (size of the group depends on what vector instructions the engine is compiled with support for).

![alt text](https://i.imgur.com/lMz7emQ.png "Renderer parallelism")

Unfortunately, such parallelism is not possible for all jobs, but is utilized wherever it can.

## Future

### General

The feature set of the engine is meant to be small and portable. This will make maintaining the engine easy, and make porting the engine to other platforms simpler. This also prevents users of the engine to be tempted to do things that the engine was never designed to do in the first place, thus keeping games running on the engine simple. More advanced game ideas or graphically demanding games should be implemented in engines with a wide and modern feature set.

There are still features that should ideally be implemented. Most of the features listed below are already implemented in code, but lack a proper interface that make operating the features an automatic process rather than a manual one. Manually operating the features are error prone, which 

### Depth buffers

Depth buffers will be removed from the software renderer. This increases performance, but may cause some scenes not to be rendered properly.

This feature is already implemented, but requires manual control for the users to take advantage of.

### Color depth

For the software renderer, 16 bit colors will be reduced to 8 bit indices + 256 entry 16 bit color palette. This allows for better performance and visual consistency, but may result in scenes that do not look the way the artists intended.

### Portals

Portals can be thought of as windows are another way of achieving better performance for indoor spaces that can be used in combination with many other acceleration techniques. Before a level model is divided into a BSP tree artist-defined portals split the geometry into sectors. The portals can then be used in conjunction with the view frustum to determine what sectors are visible which enables the engine to completely discard entire sectors (and their content) if the portal into that sector is not in view.

![alt text](https://i.imgur.com/NRKbuc8.png "Portals 1")

![alt text](https://i.imgur.com/cLhMPoj.png "Portals 2")

[![Video](https://i.imgur.com/y1Bmsfc.png)](https://i.imgur.com/DHPbLcN.mp4 "Potentially visible set")
_Click image for video_

This feature is already implemented, but requires manual control for the users to take advantage of.

### Hardware acceleration

There are plans to implement an OpenGL or Vulkan backend for the graphics engine component. This is, however, a low priority at the moment.

### Lightmap generation

While retro3d can render a lightmap it still needs the user to input the texture coordinates for the map. This can easily be done in modelling programs such as Blender, but exchanging that data through the supported model formats is very difficult. Therefore the engine needs to be able to generate lightmap texture coordinates on the fly that does not result in texture distortion.

This feature is partially implemented; Lightmaps are supported, but lightmap texture coordinates have to be provided by the artist.

### Custom sound engine

Retro3d currently uses the stock SDL mixer (SDL_mixer) for sound mixing. The SDL mixer is, however, very limited in its capabilities. There are plans to implement a software-based mixer that enables the engine to proceurally manipulate sounds as well as greater freedom for users to manipulate sounds.

#### Proper animation

Currently, the engine enables animations by loading a series of models, each representing a frame in an animation loop. This is far from ideal from a creative perspective which can make development harder than it has to be, difficulties in achieving procedural animation, and an excessive memory footprint.

In its feature-complete form, the engine will support vertex blended skeletal animation.

### Bounding volume hierarchy (BVH) culling

A BVH can, in combination with view and portal frustums, quickly discard major portions of out-of-view geometry which can greatly increase performance in both indoor and outdoor scenes.

This feature is already implemented, but requires manual control for the users to take advantage of.

### Bounding volume hierarchy (BVH) lighting

Lights in a spatial hierarchy allows the engine to quickly determine what surfaces recieve light. It also allows for automatic lighting LOD for distant surfaces or surfaces that recieve many lights by combining lights together in a simplified model.

This feature is partially implemented; A BVH can quickly be used to determine lighting, but the interface of the BVH was never meant to store any data in nodes, only in leaves.

### Automatic level of detail (LOD)

Distant objects can be simplified significantly by reducing their vertex and face count and increasing performance.

### Graphical user interface (GUI)

There is little implementation independent interface to rendering a GUI (except for text). Ideally there needs to be proper support for widgets (text input fields, buttons, scroll bars, images etc.)

This feature is already implemented, but requires manual control for the users to take advantage of.

### Tools

While an editor is crucial for game development, there are currently no plans to develop one for retro3d. Users of retro3d are better off creating their own editor using the engine as a base to suit their own needs rather than including a poor one-size-fits-all solution.

Some form of inspector will be made available that hooks into the retro3d engine via the traditional ECS model that will allow the user to pause the game and visually inspect and do crude manipulation of active entities, systems, and the game engine in order to allow for faster visual debugging.

Some form of console will be made available that supports viewing game output and giving the engine commands.
