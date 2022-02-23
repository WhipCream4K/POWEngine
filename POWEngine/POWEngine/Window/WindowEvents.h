#pragma once

// NOTE: Change this if new input system is implemented
enum class WindowEvents : uint8_t
{
	Closed						,								//!< The window requested to be closed (no data)
	Resized						,							//!< The window was resized (data in event.size)
	LostFocus					,							//!< The window lost the focus (no data)
	GainedFocus					,						//!< The window gained the focus (no data)
	TextEntered					,						//!< A character was entered (data in event.text)
	KeyPressed					,			//!< A key was pressed (data in event.key)
	KeyReleased					,			//!< A key was released (data in event.key)
	MouseWheelScrolled			,			//!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
	MouseButtonPressed			,			//!< A mouse button was pressed (data in event.mouseButton)
	MouseButtonReleased			,			//!< A mouse button was released (data in event.mouseButton)
	MouseMoved					,			//!< The mouse cursor moved (data in event.mouseMove)

	Count									//!< Keep last -- the total number of event types
};
