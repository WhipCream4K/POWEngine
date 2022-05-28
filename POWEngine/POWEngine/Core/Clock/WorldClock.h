#pragma once

#include <chrono>

namespace powe
{
	class WorldClock final
	{
	public:

		friend class Core;

		WorldClock();

		void Start();
		void End();

		[[nodiscard]] float GetTotalElapsedTime() const { return m_TotalElapsedTime; }
		[[nodiscard]] float GetDeltaTime() const { return m_DeltaTime; }

	private:

		void ResetTime();

		std::chrono::high_resolution_clock::time_point m_CurrentFrame;
		std::chrono::high_resolution_clock::time_point m_EndFrame;

		float m_DeltaTime;
		float m_TotalElapsedTime;
	};
}


