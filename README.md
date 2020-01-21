# retro3d
Copyright (c) 2020 Jonathan Karlsson

## About

Retro3d is a small 3D game engine written in C++ designed to be cross-platform, extensible, and easy to use. 

The engine started development as a response to game developers using Unity or Unreal Engine to develop games that aim to look and feel retro. The illusion often falls short when using modern engines as a result of their modern capabilities. The goal of the engine is to easily create games that feel more retro than what can be easily achieved in a modern game engine - Games made with retro3d automatically look and feel retro without impacting playability.

[View picture and video album.](https://imgur.com/a/8kzXn62)

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

The ECS model is mainly a model of composition over inheritance where a game object, called entity, (which is essentially a completely blank data slot) consists of a number of custom components that define its behavior. This eliminates the need to create a completely new type of entity when the behavior needs to be modified regardless of if its behavior should differ only slightly or even greatly from an already existing base object. Systems, in turn, work on classes of components, and can be readily compared to a rulebook of a game. Systems, however, are not just a way to modify the game rules, but also the behavior of retro3d itself.

A benefit with this kind of model is two-fold; The processor can work on multiple components in tandem enabling parallel processing, and the programmer can alter the behavior of each game object on the fly without having to resort to contrived gameplay code or inheritance on a game object basis when only a slight change of behavior is sought.

### Graphics

Retro3d comes with a software renderer by default. The software renderer is meant to emulate Playstation 1 and/or Quake era games. This means low resolution, low color depth, low polygon count, no antialiasing, and primitive lighting.

The renderer uses [tiny3d](https://github.com/SirJonthe/tiny3d.git) as its render API. Tiny3d offers a fast highly parallel rasterizer, and optimized texture access (Morton order, compression).

![alt text](https://i.imgur.com/yJDghZr.png "Color cell compression")

Tiny3d renders graphics, but does not interface with graphics hardware. As such tiny3d is combined with SDL in order to get access to the frame buffer and display graphics on the screen.

The graphics component of the engine is implemented as a platform independent device. Users of retro3d can implement their own renderers that adhere to the render device interface.

### Collisions and Physics

Retro3d comes with a custom collision and physics engine that detects collisions (using GJK and the expanding polytope algorithm) and applies proper responses to collisions (separating translations for movable colliders and linear and angular velocities for rigid bodies).

[![Video](https://i.imgur.com/kppXVx9.png)](https://i.imgur.com/yfbXti1.mp4 "Physics (VIDEO)")
_Click image for video_

**WARNING!!** While collisions for convex colliders work as expected the physics engine is currently a work in progress and does not react as expected in all situations.

### Sound

Retro3d comes with a sound device based around SDL_mixer.

The sound component of the engine is implemented as a platform independent device. Users of retro3d can implement their own mixers that adhere to the sound device interface.

### Input

Retro3d comes with an input device based around SDL.

The input device makes interfacing with input devices abstract. Instead of querying if a button is pressed the user queries whether an action has been performed (instead of checking if SPACE is pressed, the user checks if "jump" is active).

The input component of the engine is implemented as a platform independent device. Users of retro3d can implement their own input devices that adhere to the input device interface.

## Optimization

### General

In general, retro3d uses only a few acceleration structures - mainly with regards to graphics rendering.

### Binary space partitioning (BSP)

Non-convex graphical objects are recursively split in half until the resulting geometry consists only of convex pieces. These pieces, together with their respective splitting plane are stored in a binary tree that, in combination with a view frustum, can be used to discard geometry outside of the view and determine what order to draw the visible pieces so that they do not overlap.

[![Video](https://i.imgur.com/veW5Lzd.png)](https://i.imgur.com/b8MPBS7.mp4 "Binary space partitioning (VIDEO)")
_Click image for video_

The software renderer traverses the geometry in accordance with the painter's algorithm (not in accordance with the reverse painter's algorithm) and uses a mask lock pixels from being drawn more than once.

The BSP tree can be used to accelerate ray tracing against geometry by testing for intersections as they sequentially occur along the ray's travel path.

While BSP trees are common practice the difficulty lies in creating a well balanced tree that does not go needlessly deep.

### Bounding volume hierarchy (BVH)

A BVH is a binary tree that recursively groups two finite sub-spaces together to form a greater space containing both sub-spaces. A BVH has great general-purpose use within retro3d and is used to everything from collision detection, to the lighting model and the potentially visible set (PVS).

![alt text](https://i.imgur.com/m1VEvHG.png "Bounding volume hierarchy")

### Parallelism

The engine tries to embody a principle of data parallelism that enables efficient and easy-to-follow parallelism. Rather than letting threads do separate jobs, all threads do the same job on a proportional sub-set of the work load. Each thread gets to work undisturbed and in complete isolation from the other threads. This principle also enables efficient use of vector instructions (SIMD).

As an example, for the software renderer, the complete view frustum is split into a number of separate sub-frustums that each determine the potentially visible set (PVS) separately by a number of threads. The PVS:s are then traversed in-order and rendered by a number of threads to separate portions of the screen where pixels are processed in groups using SIMD (size of the group depends on what vector instructions the engine is compiled with support for).

![alt text](https://i.imgur.com/lMz7emQ.png "Renderer parallelism")

Unfortunately, such parallelism is not possible for all jobs, but is utilized wherever it can.

## Future

### General

The feature set of the engine is meant to be small and portable. This will make maintaining the engine easy, and make porting the engine to other platforms simpler. This also prevents users of the engine to be tempted to do things that the engine was never designed to do in the first place, thus keeping games running on the engine simple. More advanced game ideas or graphically demanding games should be implemented in engines with a wide and modern feature set.

There are still features that should ideally be implemented. Most of the features listed below are already implemented in code, but lack a proper interface that make operating the features an automatic process rather than a manual one. Manually operating the features are error prone and not recommended, but fully possible if done with care.

### Depth buffers

Support for depth buffers will be removed from the software renderer. This increases performance, but may cause some scenes not to be rendered properly.

This feature is already implemented, but requires manual control for the users to take advantage of.

### Color depth

For the software renderer, 16 bit colors will be reduced to 8 bit indices + 256 entry 16 bit color palette. This allows for better performance and visual consistency, but may result in scenes that do not look the way the artists intended.

### Portals

Portals can be thought of as windows and are another way of achieving better performance for indoor spaces that can be used in combination with many other acceleration techniques. Before a level model is divided into a BSP tree artist-defined portals split the geometry into sectors. The portals can then be used in conjunction with the view frustum to determine what sectors are visible which enables the engine to completely discard entire sectors (and their content) if the portal into that sector is not in view.

![alt text](https://i.imgur.com/NRKbuc8.png "Portals 1")

![alt text](https://i.imgur.com/cLhMPoj.png "Portals 2")

[![Video](https://i.imgur.com/y1Bmsfc.png)](https://i.imgur.com/DHPbLcN.mp4 "Potentially visible set (VIDEO)")
_Click image for video_

This feature is already implemented, but requires manual control for the users to take advantage of.

### Hardware acceleration

There are plans to implement an OpenGL or Vulkan backend for the graphics engine component. This is, however, a low priority at the moment.

### Lightmap generation

While retro3d can render a lightmap it still needs the user to input the texture coordinates for the map. This can easily be done in modelling programs such as Blender, but exchanging that data through the supported model formats is very difficult. Therefore the engine needs to be able to generate lightmap texture coordinates on the fly that does not result in texture distortion.

This feature is partially implemented; Lightmaps are supported, but lightmap texture coordinates have to be provided by the artist.

### Custom sound mixer

Retro3d currently uses the stock SDL\_mixer for sound mixing. The SDL mixer is, however, very limited in its capabilities. There are plans to implement a software-based mixer that enables the engine to proceurally manipulate sounds as well as allows for greater freedom for artists to manipulate sounds.

### Proper animation

Currently, the engine enables animations by loading a series of models, each representing a frame in an animation loop. This is far from ideal from a creative perspective which can make development harder than it has to be, difficulties in achieving procedural animation, and an excessive memory footprint.

In its feature-complete form, the engine will support vertex blended skeletal animation.

### Bounding volume hierarchy (BVH) culling

A BVH can, in combination with view and portal frustums, quickly discard major portions of out-of-view geometry which can greatly increase performance in both indoor and outdoor scenes.

This feature is already implemented, but requires manual control for the users to take advantage of.

### Bounding volume hierarchy (BVH) lighting

Lights in a spatial hierarchy allows the engine to quickly determine what surfaces recieve light. It also allows for automatic lighting LOD for distant surfaces or surfaces that recieve many lights by combining lights together in a simplified model.

This feature is partially implemented; A BVH can quickly be used to determine lighting, but the interface of the BVH was never meant to store any data in nodes, only in leaves.

### Automatic level of detail (LOD)

Distant objects can be simplified significantly by reducing their vertex and face count thereby increasing performance. While it is possible to implement this using artist defined LOD:s combined with game code that checks for distance efforts will be made to make this an automatic process.

### Graphical user interface (GUI)

The interface to rendering a GUI is limited. Ideally there needs to be proper support for widgets (text input fields, buttons, scroll bars, images etc.).

This feature is already implemented in the form of solid block and text rendering, but requires manual implementation of widgets for the users to take advantage of.

### Tools

While an editor is crucial for game development, there are currently no plans to develop one for retro3d. Users of retro3d are better off creating their own editor using the engine as a base to suit their own needs rather than including a poor one-size-fits-all solution.

Some form of inspector will be made available that hooks into the retro3d engine via the traditional ECS model that will allow the user to pause the game and visually inspect and do crude manipulation of active entities, systems, and the game engine in order to allow for faster visual debugging.

Some form of console will be made available that supports viewing game output and giving the engine commands.

## Building

The core of retro3d should be able to be built using most C++ compilers on essentially any platform since the core itself uses nothing but standard libraries and platform independent code. However, in order to display graphics, output audio, and send inputs to the application the engine needs non-standard modules and/or platform dependent code. By default the engine ships with support for SDL1 or SDL2 video, audio, and input devices, and a tiny3d software render device (with plans for an OpenGL or a Vulkan hardware render device).

Compiling with support for SDL requires additional compilation steps. First the user needs to decide on an SDL version (SDL2 is recommended, especially on newer versions of macOS as SDL1 has poor support and require several additional build steps), by defining either the macro ```RETRO3D\_USE\_SDL1``` or ```RETRO3D\_USE\_SDL2``` at the build stage. A successful compilation of the SDL devices require that the user install SDL (1 and/or 2, see information about installation instructions via official SDL resources as instructions may vary depending on system). The SDL headers must be included in the compiler's standard search directories (if you install SDL on Linux via official package managers, this step should be done for you). For SDL1 you need to link against ```SDL```, ```SDLmain```, and ```SDL\_mixer``` and for SDL2 you need to link against ```SDL2```, ```SDL2main```, and ```SDL2\_mixer``` in the linker stage. macOS may also require the user to link against the ```Cocoa``` framework.

Compiling with support for the tiny3d render device requires the user to compile with support for C++11 threading (STL). Some systems (Linux) requires the user to link against ```X11``` and ```pthread``` for threading to work.

## Credits

The images and videos above include content from the following creators:

Spiderbot and gun model (c) 2012 killyoverdrive (https://opengameart.org/users/killyoverdrive)
Plant models (c) 2018 pistachio (https://opengameart.org/users/pistachio)
Canister model (c) 2014 Ron Kapaun (https://opengameart.org/users/hreikin)

