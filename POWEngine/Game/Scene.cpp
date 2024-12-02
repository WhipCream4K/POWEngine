#include "pch.h"

#include <execution>

#include "Game.h"
#include "Scene.h"
#include "SceneSystem.h"

#include "Core/Thread/SimpleThreadPool.h"

using namespace powe;

Scene::Scene(Game &game) : m_Game(game)
{
    m_ThreadPoolModule = Application::GetModule<SimpleThreadPool>();
}

void Scene::Start()
{
}

void Scene::Exit()
{
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

SharedPtr<PMRResource> Scene::GetResource() const noexcept
{
    return m_Game->GetResource();
}

std::string_view Scene::GetName() const noexcept
{
    return m_Game->GetSceneName(this);
}
