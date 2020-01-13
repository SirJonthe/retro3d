#include "retro_input_device.h"

void retro3d::InputMap::Bind(const mtlChars &action, uint64_t input_code)
{
	*m_binds.CreateEntry(action) = input_code;
}

void retro3d::InputMap::Unbind(const mtlChars &action)
{
	m_binds.RemoveEntry(action);
}

void retro3d::InputMap::UnbindAll( void )
{
	m_binds.RemoveAll();
}

const uint64_t *retro3d::InputMap::GetInput(const mtlChars &action) const
{
	return m_binds.GetEntry(action);
}

void retro3d::InputDevice::CreateStateTable(uint64_t size)
{
	m_state.Create(int(size));
	for (int i = 0; i < m_state.GetSize(); ++i) {
		m_state[i] = Input{ 0.0f, 0.0f, 0.0, false };
	}
}

retro3d::InputDevice::InputDevice( void ) : m_binds(nullptr)
{}

retro3d::InputDevice::~InputDevice( void )
{
	m_state.Free();
}

void retro3d::InputDevice::SetInputMap(const InputMap *binds)
{
	m_binds = binds;
}

retro3d::InputDevice::Input retro3d::InputDevice::GetState(const mtlChars &action) const
{
	const uint64_t *lookup = m_binds != nullptr ? m_binds->GetInput(action) : nullptr;
	return lookup != nullptr ? m_state[*lookup] : Input{ 0.0f, 0.0f, 0.0, false };
}
