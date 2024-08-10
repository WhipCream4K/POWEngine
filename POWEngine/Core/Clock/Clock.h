#pragma once

#include <chrono>

namespace powe
{
	class Clock final
	{
	public:

		Clock();

		void Start();
		void End();

		[[nodiscard]] float GetTotalElapsedTime() const { return m_TotalElapsedTime; }
		[[nodiscard]] float GetDeltaTime() const { return m_DeltaTime; }
		void ResetTime();

	private:


		std::chrono::high_resolution_clock::time_point m_CurrentFrame;
		std::chrono::high_resolution_clock::time_point m_EndFrame;

		float m_DeltaTime;
		float m_TotalElapsedTime;
		float m_FrameTimeStamp;
		int m_FrameCounter;
	};
}


