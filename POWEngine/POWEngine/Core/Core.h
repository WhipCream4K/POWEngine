#pragma once

namespace powe
{
	class ServiceLocator;
	class Window;
	class WorldEntity;
	class WorldClock;
	//class CoreImpl;
	class Core final
	{
	public:

		Core();

		bool TranslateWindowInputs(const SharedPtr<Window>& window);

		bool TranslateWindowInputs(const Window& window);

		/**
		 * \brief Call this function before entering the core loop to prevent a big deltatime in the first frame
		 */
		void StartWorldClock();


		void Step(const SharedPtr<WorldEntity>& worldEntt);

		void Render(const SharedPtr<Window>& window,const SharedPtr<WorldEntity>& worldEntt);

		~Core();

		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		//Core(Core&&) = delete;
		//Core& operator=(Core&&) = delete;

	private:

		/**
		 * \brief Implementation of the core engine to expand the scalability of the core engine
		 * i.e Inputs handle by sdl and rendering handles by OpenGL or DX12
		 */
		//OwnedPtr<CoreImpl> m_CoreImpl;

		SharedPtr<WorldClock> m_WorldClock;
	};
}


