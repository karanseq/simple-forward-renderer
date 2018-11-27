#include "Application\SDLApplication.h"

// Library includes
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// External includes
#include "GL\glew.h"

// Engine includes
#include "Common\Engine.h"
#include "Logger\Logger.h"

namespace engine {
namespace application {

    // Static member initialization
    ApplicationInterface* ApplicationInterface::g_application = nullptr;

    SDLApplication::SDLApplication(const char* i_window_title) :
        window_title_(i_window_title)
    {}

    bool SDLApplication::Init()
    {
        g_application = this;

        // SDL Init
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LOG_ERROR("Failed to init SDL! Error:%s", SDL_GetError());
            return false;
        }

        // Window and context creation
        {
            window_ = SDL_CreateWindow(window_title_,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_width_,
                window_height_,
                SDL_WINDOW_OPENGL);

            if (window_ == nullptr)
            {
                LOG_ERROR("Failed to create a window! Error:%s", SDL_GetError());
                return false;
            }
        }

        if (InitGLAttributes() == false)
        {
            return false;
        }

        engine::Init();
        return true;
    }

    void SDLApplication::Run()
    {
        bool running = true;
        while (running)
        {
            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                HandleSDLEvent(event);
            }

            Update();
            Render();
        }
    }

    void SDLApplication::Shutdown()
    {
        engine::Shutdown();

#if defined(_DEBUG)
        _CrtDumpMemoryLeaks();
#endif // _DEBUG
    }

    void SDLApplication::Update()
    {
        engine::Update();
    }

    void SDLApplication::Render()
    {
        engine::Render();
        SDL_GL_SwapWindow(window_);
    }

    bool SDLApplication::InitGLAttributes()
    {
        gl_context_ = SDL_GL_CreateContext(window_);
        if (gl_context_ == nullptr)
        {
            LOG_ERROR("Failed to create an OpenGL context! Error:%s", SDL_GetError());
            return false;
        }

        // OpenGL core profile - deprecated functions are disabled
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
        {
            LOG_ERROR("Failed to set GL attribute! Error:%s", SDL_GetError());
            return false;
        }

        // Set GL version
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) < 0)
        {
            LOG_ERROR("Failed to set GL attribute! Error:%s", SDL_GetError());
            return false;
        }

        // Enable double buffering
        if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
        {
            LOG_ERROR("Failed to set GL attribute! Error:%s", SDL_GetError());
            return false;
        }

        // Synchronize buffer swap with monitor's vertical retrace
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
            LOG_ERROR("Failed to set swap interval! Error:%s", SDL_GetError());
            return false;
        }

        glEnable(GL_DEPTH_TEST);

        // Init GLEW
        GLenum glew_init_success = glewInit();
        if (glew_init_success != GLEW_OK)
        {
            LOG_ERROR("Failed to init GLEW! Error:%s", glewGetErrorString(glew_init_success));
            return false;
        }

        return true;
    }

    void SDLApplication::HandleSDLEvent(const SDL_Event& i_event)
    {

    }

} // namespace application
} // namespace engine
