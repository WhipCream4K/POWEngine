#pragma once

namespace powe
{
	class ServiceLocator;
	class Window;
	class WorldEntity;
	//class CoreImpl;
	class Core final
	{
	public:

		Core();

		bool TranslateWindowInputs(const SharedPtr<Window>& window);
		bool TranslateWindowInputs(const Window& window);
		void Step(const SharedPtr<WorldEntity>& worldEntt);
		void Render(const SharedPtr<Window>& window);

		~Core();

	private:

		/**
		 * \brief Implementation of the core engine to expand the scalability of the core engine
		 * i.e Inputs handle by sdl and rendering handles by OpenGL or DX12
		 */
		//OwnedPtr<CoreImpl> m_CoreImpl;
	};
}


