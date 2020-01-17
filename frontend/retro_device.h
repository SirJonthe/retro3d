#ifndef RETRO_DEVICE_H
#define RETRO_DEVICE_H

namespace retro3d
{

class Engine;

class Device
{
	friend class retro3d::Engine;

private:
	retro3d::Engine *m_engine;

public:
	retro3d::Engine       *GetEngine( void )       { return m_engine; }
	const retro3d::Engine *GetEngine( void ) const { return m_engine; }

	// Initialize
	// Queue job
	// Flush queue
};

}

#endif // RETRO_DEVICE_H
