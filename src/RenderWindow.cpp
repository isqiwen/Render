#include "../include/RenderWindow.h"


RenderWindow::RenderWindow()
    : window_name_(""), window_width_(0), window_height_(0), x_(0), y_(0) {
}

RenderWindow::~RenderWindow() {
    cv::destroyWindow(window_name_);
}

const std::string RenderWindow::GetWindowName() {
    return window_name_;
}

void RenderWindow::SetWindowName(const std::string &window_name) {
    window_name_ = window_name;
}

uint32_t RenderWindow::GetWidth() {
    return window_width_;
}

uint32_t RenderWindow::GetHeight() {
    return window_height_;
}

std::tuple<uint32_t, uint32_t> RenderWindow::GetPosition() {
    return std::make_tuple(x_, y_);
}

void RenderWindow::InitRenderWindow(const std::string &window_name, uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
    window_name_ = window_name;
    window_width_ = width;
    window_height_ = height;
    x_ = x;
    y_ = y;
    cv::namedWindow(window_name_, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
    cv::resizeWindow(window_name_, window_width_, window_height_);
    cv::setWindowTitle(window_name_, window_name_);
}

void RenderWindow::ShowImg(const cv::Mat &img) {
	cv::imshow(window_name_, img);
}
