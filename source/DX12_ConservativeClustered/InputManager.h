#pragma once
#include <SDL.h>
#include <vector>
#include "Types.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

#define GAMEPAD_DEADZONE 8000

enum class KeyState : uint32
{
	UP,
	DOWN,
	DOWN_EDGE,
	UP_EDGE,
};

enum class MouseButton : uint32
{
	LEFT = 490,
	MIDDLE,
	RIGHT,
	X1,
	X2,
};

enum class GamepadButton : uint32
{
	A = 495,
	B,
	X,
	Y,
	BACK,
	GUIDE,
	START,
	LSTICK,
	RSTICK,
	LSHOULDER,
	RSHOULDER,
	DUP,
	DDOWN,
	DLEFT,
	DRIGHT,
};

struct ThumbSticks
{
	float leftX;
	float leftY;
	float rightX;
	float rightY;
};

struct Triggers
{
	float left;
	float right;
};

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update(SDL_Event& event);
	void Reset();

	KeyState GetKeyState(SDL_Scancode key);
	KeyState GetKeyState(MouseButton button);
	KeyState GetKeyState(GamepadButton button);

	Vector2 GetGlobalMousePos();
	Vector2 GetDeltaMousPos();

	ThumbSticks GetThumbSticks();
	Triggers GetTriggers();

private:
	std::vector<KeyState> m_keyState;
	std::vector<KeyState> m_prevKeyState;

	Vector2 m_deltaMousePos;
	Vector2 m_globalMousePos;

	ThumbSticks m_thumbSticks;
	Triggers m_triggers;

	int32 m_mouseScroll;

	SDL_GameController* m_controller;
};
