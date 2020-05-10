#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <imgpp/imgpp.hpp>
#include "Camera.h"
#include "RenderWindow.h"
#include "Mesh.h"

// TODO Probably to change later
// Default window value
#define WINDOW_DEFAULT_SIZE_W 960
#define WINDOW_DEFAULT_SIZE_H 780
#define WINDOW_DEFAULT_LEFT 0
#define WINDOW_DEFAULT_TOP 0

/**
 * The core engine that runs the rendering.
 *
 */
class Engine {
    private:
        Engine();

    public:
        ~Engine();
        static Engine& GetInstance();

    private:
        bool is_runing_;
        RenderWindow render_window_;
        Camera camera_;
        Mesh mesh_;
        imgpp::Img frame_buffer_;
        imgpp::Img depth_buffer_;

    public:
        bool Init();
        bool StartRendering();
        bool StopRendering();
        void Destroy();

    private:
        bool RenderOneFrame();
        void RenderAll();
        void HandleEvent(int key);
};

#endif