#include "pch.h"
#include "SFML2DRendererThreaded.h"

//#if USE_SFML_RENDERER


#include <deque>

#include "POWEngine/Window/Window.h"
#include "POWEngine/Window/SFML/SFMLWindow.h"

#include <SFML/Graphics.hpp>
#include <future>

#include "POWEngine/LockFree/LFQueue.h"
#include "POWEngine/Logger/LoggerUtils.h"


namespace powe
{
    enum class BufferTask
    {
        Default,
        Render,
        WindowResize,
        ClearBackBuffer,
        RenderEnd,
    };

    struct RenderEntity
    {
        const sf::Drawable* sfDraw{};
        sf::RenderStates sfRenderStates{};
        BufferTask syncTask{BufferTask::Render};
    };

    class RenderBuffer // uint representation
    {
    public:
        enum
        {
            FrontBuffer,
            BackBuffer,

            Count
        };
    };

    static int g_TempID{};

    namespace impl
    {
        class Buffer
        {
        public:
            void Create(uint32_t width, uint32_t height);
            void Create(const Window& window);
            void SetActive(bool value);
            void Render(const RenderEntity& renderEntt);
            void Wait();
            void Display(const Window& window);
            void DisplaySavedWindow();
            // void Display2(sf::RenderWindow* renderWindow);
            void Standby();
            void Clear(const sf::Color& color);
            void Resize(uint32_t width, uint32_t height);
            void EndBuffer();

            void SignalDisplay();

            int m_Id{};

        private:
            std::future<void> m_DisplayReadyFuture{};
            std::promise<void> m_DisplayReadyPromise{};

            // std::atomic_flag m_DisplayWait{};
            // std::shared_future<void> m_FinishedDisplayFuture{};
            // std::future<void> m_DisplayThread{};

            sf::RenderTexture m_RenderTexture{};
            sf::Sprite m_RenderSprite{};

            sf::RenderWindow* m_CacheRenderWindow{};
            const Window* m_MainWindow{};
        };
    }
}

class powe::SFML2DRendererThreaded::SFML2DRendererImpl
{
public:
    SFML2DRendererImpl();
    ~SFML2DRendererImpl();

    void ClearBackBuffer();
    void ClearBackBufferImpl(int targetBuffer);

    void Resize(uint32_t width, uint32_t height);
    void ResizeImpl();

    // main thread called once
    void BufferCreate(const Window& window);

    // void RunRenderThread();
    void RunRenderThread(const Window& window);


    void SubmitRenderEntity(const sf::Drawable& drawEntity, const sf::RenderStates& renderStates);

    void DisplayInActiveBuffer(const Window&);

    void MarkEndOfDrawCommand();

private:
    impl::Buffer m_Buffer[RenderBuffer::Count]{};

    int m_ActiveBufferIdx{};
    sf::Color m_ClearColor{};

    std::atomic_bool m_IsRenderThreadExit{};
    std::future<void> m_RenderThread{};
    std::condition_variable m_RenderThreadWait{};

    std::mutex m_RenderTasksMutex{};
    std::deque<RenderEntity> m_RenderTasks{};

    uint32_t m_ResizeWidth{};
    uint32_t m_ResizeHeight{};
};


namespace powe
{
    static inline void RunDisplayThread(sf::RenderWindow* window,
                                        std::atomic_flag& displayFlag,
                                        std::promise<void>& displayAvailable,
                                        const int& bufferIdx,
                                        impl::Buffer* targetBuffer)
    {
        window->setActive(true);

        // This Thread suppose to start in render thread to get the active buffer
        while (true)
        {
            displayFlag.wait(true);

            targetBuffer[bufferIdx].DisplaySavedWindow();

            displayFlag.test_and_set();
            displayAvailable.set_value();
        }
    }
}


void powe::impl::Buffer::Create(uint32_t width, uint32_t height)
{
    m_RenderTexture.create(width, height);
    m_RenderSprite.setTexture(m_RenderTexture.getTexture());
}

void powe::impl::Buffer::Create(const Window& window)
{
    sf::RenderWindow* sfRenderWindow{&window.GetWindowInstance<SFMLWindow>()->GetRenderWindow()};
    m_CacheRenderWindow = sfRenderWindow;
    m_MainWindow = &window;

    m_RenderTexture.create(window.GetWidth(), window.GetHeight());
    m_RenderSprite.setTexture(m_RenderTexture.getTexture());
}

void powe::impl::Buffer::SetActive(bool value)
{
    m_RenderTexture.setActive(value);
}

void powe::impl::Buffer::Render(const RenderEntity& renderEntt)
{
    m_RenderTexture.draw(*renderEntt.sfDraw, renderEntt.sfRenderStates);
}

void powe::impl::Buffer::Wait()
{
    if (m_DisplayReadyFuture.valid())
        m_DisplayReadyFuture.get();

    // if(m_FinishedDisplayFuture.valid())
    //     m_FinishedDisplayFuture.get();
}

void powe::impl::Buffer::Display(const Window& window)
{
    SetActive(true);

    SFMLWindow* sfmlWindow{window.GetWindowInstance<SFMLWindow>()};
    sf::RenderWindow& renderWindow{sfmlWindow->GetRenderWindow()};

    // renderWindow.clear();
    renderWindow.draw(m_RenderSprite, sf::BlendNone);

    window.Display();

    SetActive(false);
}

void powe::impl::Buffer::DisplaySavedWindow()
{
    m_RenderTexture.display();

    m_CacheRenderWindow->draw(m_RenderSprite,sf::BlendNone);
    m_CacheRenderWindow->display();

    m_DisplayReadyPromise.set_value();
}

void powe::impl::Buffer::Standby()
{
    m_RenderTexture.display();

    SetActive(false);

    m_DisplayReadyPromise.set_value();
}

void powe::impl::Buffer::Clear(const sf::Color& color)
{
    // SetActive(true);
    m_RenderTexture.clear(color);
}

void powe::impl::Buffer::Resize(uint32_t width, uint32_t height)
{
    SetActive(true);

    m_RenderTexture.display();
    const sf::Sprite tempRender{m_RenderTexture.getTexture()};

    m_RenderTexture.create(width, height);
    m_RenderTexture.draw(tempRender);

    SetActive(false);
}

void powe::impl::Buffer::EndBuffer()
{
    m_DisplayReadyPromise = std::promise<void>{};
    m_DisplayReadyFuture = m_DisplayReadyPromise.get_future();
    // m_FinishedDisplayFuture = m_DisplayReadyPromise.get_future();
}

void powe::impl::Buffer::SignalDisplay()
{
    // m_DisplayWait.clear();
    // m_DisplayWait.notify_one();
}

powe::SFML2DRendererThreaded::SFML2DRendererImpl::SFML2DRendererImpl()
{
    // m_RenderThread = std::async(std::launch::async, &SFML2DRendererImpl::RunRenderThread, this);
    m_ClearColor = sf::Color::Transparent;
}

powe::SFML2DRendererThreaded::SFML2DRendererImpl::~SFML2DRendererImpl()
{
    m_IsRenderThreadExit = true;

    {
        std::scoped_lock lock{m_RenderTasksMutex};
        m_RenderTasks.emplace_back(nullptr, sf::RenderStates{}, BufferTask::Default);
    }

    m_RenderThread.get();
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::ClearBackBuffer()
{
    {
        std::scoped_lock lock{m_RenderTasksMutex};
        m_RenderTasks.emplace_back(nullptr, sf::RenderStates{}, BufferTask::ClearBackBuffer);
    }

    m_RenderThreadWait.notify_one();
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::ClearBackBufferImpl(int targetBuffer)
{
    m_Buffer[targetBuffer].Clear(m_ClearColor);
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::Resize(uint32_t width, uint32_t height)
{
    m_ResizeWidth = width;
    m_ResizeHeight = height;

    {
        std::scoped_lock lock{m_RenderTasksMutex};
        m_RenderTasks.emplace_back(nullptr, sf::RenderStates{}, BufferTask::WindowResize);
    }

    m_RenderThreadWait.notify_one();
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::ResizeImpl()
{
    const uint32_t width{m_ResizeWidth};
    const uint32_t height{m_ResizeHeight};

    m_Buffer[RenderBuffer::FrontBuffer].Resize(width, height);
    m_Buffer[RenderBuffer::BackBuffer].Resize(width, height);
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::BufferCreate(const Window& window)
{
    // const uint32_t width{window.GetWidth()};
    // const uint32_t height{window.GetHeight()};

    sf::RenderWindow& sfRenderWindow{window.GetWindowInstance<SFMLWindow>()->GetRenderWindow()};
    sfRenderWindow.setActive(false);

    m_RenderThread = std::async(std::launch::async, &SFML2DRendererImpl::RunRenderThread, this, std::ref(window));
}

// void powe::SFML2DRendererThreaded::SFML2DRendererImpl::RunRenderThread()
// {
//     // a local value to check against total draw commands sent by the main thread
//     int activeBuffer{};
//     
//     m_Buffer[RenderBuffer::FrontBuffer].Create(1280,720);
//     m_Buffer[RenderBuffer::BackBuffer].Create(1280,720);
//
//     try
//     {
//         while (!m_RenderThreadExit.load())
//         {
//             {
//                 std::unique_lock lock{m_RenderTasksMutex};
//
//                 m_RenderThreadWait.wait(lock, [this]()
//                 {
//                     return !m_RenderTasks.empty();
//                 });
//             }
//
//             // if there's a render task then we keep looping the render tasks until empty
//             // if we are lucky enough that by the time the loop comes back again
//             // and the render tasks aren't empty then we don't have to do lock and wait again
//             while (true)
//             {
//                 RenderEntity renderEntt{};
//
//                 {
//                     std::scoped_lock readLock{m_RenderTasksMutex};
//
//                     if (m_RenderTasks.empty())
//                         break;
//
//                     renderEntt = m_RenderTasks.front();
//                     m_RenderTasks.pop_front();
//                 }
//
//                 switch (renderEntt.syncTask)
//                 {
//                 case BufferTask::Render:
//                     m_Buffer[activeBuffer].Render(renderEntt);
//                     break;
//                 case BufferTask::WindowResize:
//                     ResizeImpl();
//                     break;
//                 case BufferTask::ClearBackBuffer:
//                     m_Buffer[activeBuffer].Clear(m_ClearColor);
//                     break;
//                 case BufferTask::RenderEnd:
//
//                     // m_Buffer[activeBuffer].Standby();
//
//                     m_Buffer[!activeBuffer].Wait();
//                     m_Buffer[activeBuffer].SignalDisplay();
//                     
//                     activeBuffer = !activeBuffer;
//
//                     break;
//                 case BufferTask::Default: break;
//                 default: ;
//                 }
//             }
//         }
//     }
//     catch (const std::exception& e)
//     {
//         const std::string errMsg{e.what()};
//         POWLOGERROR(errMsg);
//     }
// }

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::RunRenderThread(const Window& window)
{
    // a local value to check against total draw commands sent by the main thread
    int activeBuffer{};

    m_Buffer[RenderBuffer::FrontBuffer].Create(window);
    m_Buffer[RenderBuffer::BackBuffer].Create(window);

    m_Buffer[RenderBuffer::FrontBuffer].m_Id = g_TempID++;
    m_Buffer[RenderBuffer::BackBuffer].m_Id = g_TempID;

    // launch display thread
    sf::RenderWindow* renderWindow{&window.GetWindowInstance<SFMLWindow>()->GetRenderWindow()};

    std::atomic_flag displayThreadWait{};
    displayThreadWait.test_and_set();

    std::promise<void> finishedDisplayPromise{};
    std::future<void> finishedDisplayFuture{};

    int displayBufferIdx{};

    std::future<void> displayThread{
        std::async(
            std::launch::async,
            &RunDisplayThread,
            renderWindow,
            std::ref(displayThreadWait),
            std::ref(finishedDisplayPromise),
            std::ref(displayBufferIdx),
            &m_Buffer[0])
    };
    // -------------------------

    try
    {
        while (!m_IsRenderThreadExit.load())
        {
            {
                std::unique_lock lock{m_RenderTasksMutex};

                m_RenderThreadWait.wait(lock, [this]()
                {
                    return !m_RenderTasks.empty();
                });
            }

            // if there's a render task then we keep looping the render tasks until empty
            // if we are lucky enough that by the time the loop comes back again
            // and the render tasks aren't empty then we don't have to do lock and wait again
            while (true)
            {
                RenderEntity renderEntt{};

                {
                    std::scoped_lock readLock{m_RenderTasksMutex};

                    if (m_RenderTasks.empty())
                        break;

                    renderEntt = m_RenderTasks.front();
                    m_RenderTasks.pop_front();
                }

                switch (renderEntt.syncTask)
                {
                case BufferTask::Render:
                    m_Buffer[activeBuffer].Render(renderEntt);
                    break;
                case BufferTask::WindowResize:
                    ResizeImpl();
                    break;
                case BufferTask::ClearBackBuffer:
                    m_Buffer[activeBuffer].Clear(m_ClearColor);
                    break;
                case BufferTask::RenderEnd:

                    // m_Buffer[!activeBuffer].StandBy();

                    if (finishedDisplayFuture.valid())
                        finishedDisplayFuture.get();
                    
                    m_Buffer[activeBuffer].SetActive(false);

                    finishedDisplayPromise = std::promise<void>();
                    finishedDisplayFuture = finishedDisplayPromise.get_future();

                    displayBufferIdx = activeBuffer;
                    activeBuffer = !activeBuffer;

                    displayThreadWait.clear();
                    displayThreadWait.notify_one();


                    break;
                case BufferTask::Default: break;
                default: ;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        const std::string errMsg{e.what()};
        POWLOGERROR(errMsg);
    }
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::SubmitRenderEntity(const sf::Drawable& drawEntity,
                                                                          const sf::RenderStates& renderStates)
{
    // TODO: Measure performance between my lock free queue and vector with mutex
    // main thread
    // m_RenderTasks.Push({&drawEntity, renderStates, 0});

    {
        std::scoped_lock lock{m_RenderTasksMutex};
        m_RenderTasks.emplace_back(&drawEntity, renderStates, BufferTask::Render);
    }

    m_RenderThreadWait.notify_one();
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::DisplayInActiveBuffer(const Window&)
{
    const int standbyBufferIdx{!m_ActiveBufferIdx};
    impl::Buffer& buffer{m_Buffer[standbyBufferIdx]};
    buffer.Wait();
    // buffer.Display(window);
}

void powe::SFML2DRendererThreaded::SFML2DRendererImpl::MarkEndOfDrawCommand()
{
    m_Buffer[m_ActiveBufferIdx].EndBuffer();

    {
        std::scoped_lock lock{m_RenderTasksMutex};
        m_RenderTasks.emplace_back(nullptr, sf::RenderStates{}, BufferTask::RenderEnd);
    }

    m_ActiveBufferIdx = !m_ActiveBufferIdx;

    m_RenderThreadWait.notify_one();
}

powe::SFML2DRendererThreaded::SFML2DRendererThreaded()
    : RenderAPI()
      , m_RenderImpl(std::make_unique<SFML2DRendererImpl>())
{
}

powe::SFML2DRendererThreaded::~SFML2DRendererThreaded() = default;

void powe::SFML2DRendererThreaded::SubmitDrawSprite(sf::Drawable*, const sf::RenderStates&,
                                                    int) const
{
}

void powe::SFML2DRendererThreaded::SubmitRenderEntity(const sf::Drawable& drawable,
                                                      const sf::RenderStates& renderStates) const
{
    m_RenderImpl->SubmitRenderEntity(drawable, renderStates);
}

void powe::SFML2DRendererThreaded::OnChangeWindow(const Window& window)
{
    m_RenderImpl->Resize(window.GetWidth(), window.GetHeight());
}

void powe::SFML2DRendererThreaded::OnWindowResize(const Window& window)
{
    const uint32_t width{window.GetWidth()};
    const uint32_t height{window.GetHeight()};

    m_RenderImpl->Resize(width, height);
}

void powe::SFML2DRendererThreaded::OnWindowCreate(const Window& window)
{
    m_RenderImpl->BufferCreate(window);
}

void powe::SFML2DRendererThreaded::DisplayBuffer(const Window& window)
{
    m_RenderImpl->DisplayInActiveBuffer(window);
}

void powe::SFML2DRendererThreaded::DeferredDrawOnWindow(const Window&)
{
    m_RenderImpl->MarkEndOfDrawCommand();
}

void powe::SFML2DRendererThreaded::ClearBackBuffer()
{
    m_RenderImpl->ClearBackBuffer();
}


//#endif
