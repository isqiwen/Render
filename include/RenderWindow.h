#ifndef RENDER_RENDERWINDOW_H
#define RENDER_RENDERWINDOW_H

#include <iostream>
#include <cstdint>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

/**
 * Define a generic interface of a window.
 * Any engine rendering element is displayed inside a window.
 */
class RenderWindow {
    private:
        std::string window_name_;
        uint32_t window_width_;
        uint32_t window_height_;
        uint32_t x_;
        uint32_t y_;

    public:
        RenderWindow();
        
        ~RenderWindow();

        const std::string GetWindowName();
        void SetWindowName(const std::string &window_name); 
        uint32_t GetWidth();
        uint32_t GetHeight();
        std::tuple<uint32_t, uint32_t> GetPosition();
        void ShowImg(const cv::Mat &img);

        /**
         * Initialize all data for this window.
         * Window is ready to be displayed.
         *
         * \param window_name      Title of the window.
         * \param width     Window's width.
         * \param height    Window's height.
         * \param x         The x position of the window.
         * \param y         The y position of the window.
         */
        void InitRenderWindow(const std::string &window_name, uint32_t width, uint32_t height, uint32_t x, uint32_t y);
};

#endif