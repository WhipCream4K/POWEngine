#include "pch.h"

#include <execution>

#include "Game.h"
#include "Scene.h"
#include "SceneSystem.h"

#include "Core/Thread/SimpleThreadPool.h"

using namespace powe;

Scene::Scene(Game &game) : m_Game(game)
{
    const AllocatorContext context{};
    auto* upStream{context.GetResource()};
    
    m_ECSManager = AllocateUnique<ECSManager>(upStream);

    m_ThreadPoolModule = Application::GetModule<SimpleThreadPool>();
    m_SequenceSystems = Vector<SharedPtr<SceneSystem>>(upStream);
    m_UnsequenceSystems = Vector<SharedPtr<SceneSystem>>(upStream);
}

void Scene::Start()
{
    for (auto &system : m_SequenceSystems)
    {
        system->OnStart(*this);
    }
}

void Scene::Exit()
{
    for (auto &system : m_SequenceSystems)
    {
        system->OnExit(*this);
    }
}

void Scene::Update(float deltaTime)
{
    if (!m_UnsequenceSystems.empty())
    {
        auto parUnseqSystem{[this, deltaTime]() {
            std::for_each(
                std::execution::par_unseq, m_UnsequenceSystems.begin(), m_UnsequenceSystems.end(),
                [this, deltaTime](const SharedPtr<SceneSystem> &system) { system->OnUpdate(*this, deltaTime); });
        }};

        if (auto threadPool{m_ThreadPoolModule.lock()}; threadPool)
        {
            m_SysytemFuture = threadPool->EnqueueReturn(parUnseqSystem);
        }
    }

    for (auto &system : m_SequenceSystems)
    {
        system->OnUpdate(*this, deltaTime);
    }
}

void Scene::SceduleSystem(const SharedPtr<SceneSystem> &system, SchedulePolicy policy) noexcept
{
	if (policy == SchedulePolicy::Sequence)
	{
		m_SequenceSystems.emplace_back(system);
	}
	else
	{
		m_UnsequenceSystems.emplace_back(system);
	}
}

SharedPtr<PMRResource> Scene::GetResource() const noexcept
{
    return m_Game->GetResource();
}

std::string_view Scene::GetName() const noexcept
{
    return m_Game->GetSceneName(this);
}
