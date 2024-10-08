#ifndef RETRO3D_H
#define RETRO3D_H

#include "lib/jobs/jobs.h"
#include "device/api/retro_input_device.h"
#include "device/api/retro_render_device.h"
#include "device/api/retro_sound_device.h"

namespace retro3d
{

/// @brief The engine core of retro3d. Provides a graph of various nodes such as devices, systems, 
class core
{
public:
	class node : private cc0::job
	{
	private:
		core *m_core;
	};

private:
	class device_manager : public node
	{};
	
	class system_manager : public node
	{};

	class component_manager : public node
	{};

	class root_node : public node
	{
	private:
		device_manager    m_devices;
		system_manager    m_systems;
		component_manager m_components;
	};

private:
	root_node m_root;

public:
	template < typename device_t >
	device_t *create_render_device( void );

	template < typename device_t >
	device_t *create_sound_device( void );

	template < typename device_t >
	device_t *create_input_device( void );

	template < typename system_t >
	system_t *new_system( void );

	template < typename object_t >
	object_t *new_object( void );

	void run( void );
};

}

#endif
