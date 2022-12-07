#include "pch.h"
#include "SFML2DRenderer.h"

//#if USE_SFML_RENDERER


#include "POWEngine/Window/Window.h"
#include "POWEngine/Window/SFML/SFMLWindow.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include <SFML/Graphics.hpp>

#include <future>

#include "POWEngine/LockFree/LFQueue.h"

class RenderBuffer
{
public:
    enum
    {
        FrontBuffer,
        BackBuffer,

        Count
    };
};

class powe::SFML2DRenderer::SFML2DRendererImpl
{
    struct RenderEntity
    {
        const sf::Drawable* sfDraw{};
        sf::RenderStates sfRenderStates{};
        int drawOrder{};
    };

public:
    SFML2DRendererImpl();

    void ClearBackBuffer();

    void Resize(uint32_t width, uint32_t height);

    void OnWindowCreate(const Window& window);

    void RunRenderThread();

    void SubmitRenderEntity(const sf::Drawable& drawEntity, const sf::RenderStates& renderStates);
    void Render(const RenderEntity& renderEntity);

    // this function display backbuffer to the window and swap backbuffer to frontbuffer
    // renderer should still be able to access draw command when it's still being display
    void DrawBufferOnWindow(const Window& window)
    {
        m_RenderBuffer[m_BackBufferIdx].display();
        m_RenderSprite.setTexture(m_RenderBuffer[m_BackBufferIdx].getTexture());

        SFMLWindow* sfmlWindow{window.GetWindowInstance<SFMLWindow>()};
        sf::RenderWindow& renderWindow{sfmlWindow->GetRenderWindow()};

        renderWindow.draw(m_RenderSprite);

        m_BackBufferIdx = !m_BackBufferIdx; // swap between 0 and 1

        // sort the render order
        // std::ranges::sort(m_SF2DDrawEntities, [](const DrawEntity& left, const DrawEntity& right)
        // 	{
        // 		return left.drawOrder < right.drawOrder;
        // 	});

        // for (const auto& drawEntt : m_SF2DDrawEntities)
        // {
        // 	//POWLOGERROR(std::to_string(m_SF2DDrawEntities.size()));
        // 	renderWindow.draw(*drawEntt.sfDraw, drawEntt.sfRenderStates);
        // }

        // m_SF2DDrawEntities.clear();
    }

    // void SubmitDrawSprite(sf::Drawable* drawObject, const sf::RenderStates& renderStates, int drawOrder)
    // {
    //     if (drawObject)
    //         m_SF2DDrawEntities.emplace_back(drawObject, renderStates, drawOrder);
    // }

private:
    sf::RenderTexture m_RenderBuffer[RenderBuffer::Count]{};
    int m_BackBufferIdx{};
    sf::Sprite m_RenderSprite{};

    sf::Color m_ClearColor{};

    std::future<void> m_RenderThread{};
    std::condition_variable m_RenderThreadWait{};
    std::mutex m_RenderThreadMutex{};

    LFQueue<RenderEntity> m_RenderTasks{};
};

powe::SFML2DRenderer::SFML2DRendererImpl::SFML2DRendererImpl()
    : m_BackBufferIdx(RenderBuffer::BackBuffer)
{
    m_RenderThread = std::async(std::launch::async, &SFML2DRendererImpl::RunRenderThread, this);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::ClearBackBuffer()
{
    m_RenderBuffer[m_BackBufferIdx].clear(m_ClearColor);

    m_BackBufferIdx = !m_BackBufferIdx;
}

void powe::SFML2DRenderer::SFML2DRendererImpl::Resize(uint32_t width, uint32_t height)
{
}

void powe::SFML2DRenderer::SFML2DRendererImpl::OnWindowCreate(const Window& window)
{
    const uint32_t width{window.GetWidth()};
    const uint32_t height{window.GetHeight()};

    m_RenderBuffer[RenderBuffer::BackBuffer].create(width, height);
    m_RenderBuffer[RenderBuffer::FrontBuffer].create(width, height);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::RunRenderThread()
{
    // One producer, one consumer
    while (true)
    {
        RenderEntity temp{};

        {
            std::unique_lock lock{m_RenderThreadMutex};

            m_RenderThreadWait.wait(lock, [this]()
            {
                return !m_RenderTasks.Empty();
            });
        }

        temp = m_RenderTasks.Front();
        m_RenderTasks.Pop();

        Render(temp);
    }
}

void powe::SFML2DRenderer::SFML2DRendererImpl::SubmitRenderEntity(const sf::Drawable& drawEntity,
                                                                  const sf::RenderStates& renderStates)
{
    m_RenderTasks.Push({&drawEntity, renderStates, 0});
    m_RenderThreadWait.notify_one();
}

void powe::SFML2DRenderer::SFML2DRendererImpl::Render(const RenderEntity& renderEntity)
{
    m_RenderBuffer[m_BackBufferIdx].draw(*renderEntity.sfDraw, renderEntity.sfRenderStates);
}

powe::SFML2DRenderer::SFML2DRenderer()
    : RenderAPI()
      , m_RenderImpl(std::make_unique<SFML2DRendererImpl>())
{
}

powe::SFML2DRenderer::~SFML2DRenderer() = default;

void powe::SFML2DRenderer::SubmitDrawSprite(sf::Drawable*, const sf::RenderStates&,
                                            int) const
{
}

void powe::SFML2DRenderer::SubmitRenderEntity(const sf::Drawable& drawable, const sf::RenderStates& renderStates) const
{
    m_RenderImpl->SubmitRenderEntity(drawable, renderStates);
}

void powe::SFML2DRenderer::OnWindowResize(const Window& window)
{
    const uint32_t width{window.GetWidth()};
    const uint32_t height{window.GetHeight()};

    m_RenderImpl->Resize(width, height);
}

void powe::SFML2DRenderer::OnWindowCreate(const Window& window)
{
    m_RenderImpl->OnWindowCreate(window);
}

void powe::SFML2DRenderer::DrawBufferOnWindow(const Window& window) const
{
    m_RenderImpl->DrawBufferOnWindow(window);
}

void powe::SFML2DRenderer::ClearBackBuffer()
{
    m_RenderImpl->ClearBackBuffer();
}


//#endif
