#pragma once

#include <cstdint>
#include <string>

namespace powe
{
	enum class InputDevice
	{
		D_Keyboard,
		D_Mouse,
		D_Gamepad
	};

	using KeyType = uint16_t;

	struct Key
	{

		Key() = default;

		Key(InputDevice device, KeyType code)
			: inputDevice(device)
			, keyCode(code)
		{
		}

		bool operator!=(const Key& other) const
		{
			return inputDevice != other.inputDevice &&
				keyCode != other.keyCode;
		}

		bool operator==(const Key& other) const
		{
			return inputDevice == other.inputDevice &&
				keyCode == other.keyCode;
		}

		bool operator<(const Key& other) const
		{
			return inputDevice < other.inputDevice ||
				keyCode < other.keyCode;
		}

		InputDevice inputDevice;
		KeyType keyCode;
	};

	struct KeyHasher
	{
		size_t operator()(const Key& key) const
		{
			return std::hash<decltype(key.keyCode)>{}(key.keyCode) ^
				(std::hash<decltype(key.inputDevice)>{}(key.inputDevice) << 1);
		}

		size_t operator()(uint16_t exterKey, InputDevice device) const
		{
			return std::hash<uint16_t>{}(exterKey) ^
				(std::hash<InputDevice>{}(device) << 1);
		}
	};

	struct ActionKey
	{
		ActionKey(const Key& defKey)
			:key(defKey)
		{
		}

		bool operator!=(const ActionKey& other) const
		{
			return key.keyCode != other.key.keyCode;
		}

		bool operator==(const ActionKey& other) const
		{
			return key.keyCode == other.key.keyCode;
		}

		Key key;
	};

	struct AxisKey
	{
		AxisKey() = default;

		AxisKey(const Key& defKey, float scaleValue)
			:key(defKey)
			, scale(scaleValue)
		{
		}

		Key key;
		float scale;
	};
}
