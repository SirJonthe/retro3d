#ifndef RETRO_INPUT_DEVICE_H
#define RETRO_INPUT_DEVICE_H

#include <cstdint>
#include "../common/MiniLib/MTL/mtlArray.h"
#include "../common/MiniLib/MTL/mtlStringMap.h"
#include "../api/tiny3d/tiny_system.h"

namespace retro3d
{

class InputMap
{
private:
	mtlStringMap<uint64_t> m_binds;

public:
	void Bind(const mtlChars &action, uint64_t input_code);
	void Unbind(const mtlChars &action);
	void UnbindAll( void );
	const uint64_t *GetInput(const mtlChars &action) const;
};

class InputDevice
{
public:
	struct Input
	{
		float  activation; // usually 0.0 - 1.0, although an axis would probably give -1.0 - 1.0
		float  delta;      // the difference in activation since last update
		double time;       // the amount of time the state has been non-zero or zero (changes reset timer)
		bool   awoken;     // 1 on the frame when inactive input becomes active, 0 all other times
	};

private:
	const InputMap *m_binds;

protected:
	mtlArray<Input> m_state; // fixed length, number of inputs on the device
	mtlArray<Input> m_delta; // all changes in input, in order (replaces m_state?)

protected:
	void CreateStateTable(uint64_t size);

public:
	InputDevice( void );
	virtual ~InputDevice( void );

	virtual bool     Init( void ) = 0;

	virtual bool     UserQuit( void ) const = 0;
	virtual uint64_t GetRealTimeMS( void ) const = 0;
	virtual uint64_t GetProgramTimeMS( void ) const = 0;
	virtual void     Sleep(uint64_t time_ms) const = 0;
	virtual void     ToggleMouseCursorLock( void ) = 0;
	virtual bool     IsMouseCursorLocked( void ) const = 0;
	virtual void     ToggleMouseCursorVisibility( void ) = 0;
	virtual bool     IsMouseCursorVisible( void ) const = 0;
	virtual void     Update( void ) = 0;

	void SetInputMap(const retro3d::InputMap *binds);
	Input GetState(const mtlChars &action) const;
};



struct Keyboard
{
	static constexpr uint64_t FIRST = 0;
	enum Input
	{
		A = Keyboard::FIRST,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
		KP0,KP1,KP2,KP3,KP4,KP5,KP6,KP7,KP8,KP9,
		NP0,NP1,NP2,NP3,NP4,NP5,NP6,NP7,NP8,NP9,NPPlus,NPMinus,NPMul,NPDiv,NPEnter,
		F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,
		Space,Enter,Escape,Backspace,LShift,RShift,LControl,RControl,LAlt,RAlt,
		Up,Down,Left,Right,
		Tab,
		LAST
	};
	static constexpr uint64_t COUNT = LAST - FIRST + 1;
};

struct Mouse
{
	static constexpr uint64_t FIRST = Keyboard::LAST;
	enum Input
	{
		Left = Mouse::FIRST, Right, Mid,
		WheelUp, WheelDown,
		MoveX, MoveY,
		LAST
	};
	static constexpr uint64_t COUNT = LAST - FIRST + 1;
};

struct Controller
{
	static constexpr uint64_t FIRST = Mouse::LAST;
	enum Input
	{
		A = Controller::FIRST, B, X, Y,
		LTrigger, RTrigger,
		LBumper, RBumper,
		LStickX, LStickY,
		RStickX, RStickY,
		Up, Down, Left, Right,
		GyroX, GyroY, GyroZ,
		Start, Select,
		LAST
	};
	static constexpr uint64_t COUNT = LAST - FIRST + 1;
};

static constexpr uint64_t INPUT_COUNT = retro3d::Keyboard::COUNT + retro3d::Mouse::COUNT + retro3d::Controller::COUNT;

}

#endif // RETRO_INPUT_DEVICE_H
