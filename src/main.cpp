#include <vector>
#include <glm/glm.hpp>
#include <imgpp/imgpp.hpp>
#include <imgpp/opencvbinding.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace {

void ClearFramebuffer(imgpp::Img &img) {
  img.Zeros();
}

// get perspective project matrix
glm::mat4 PerspectiveProj(double left, double right, double bottom, double top,
  double near, double far) {
  double inv_rsubl = right - left;
  double inv_tsubb = top - bottom;
  double inv_fsubn = far - near;
  glm::mat4 proj_mat;
  proj_mat[0] = glm::vec4(2 * near * inv_rsubl, 0.f, 0.f, 0.f);
  proj_mat[1] = glm::vec4(0, 2 * near * inv_tsubb, 0.f, 0.f);
  proj_mat[2] = glm::vec4((right + left) * inv_rsubl, (top + bottom) * inv_tsubb, \
    -(far + near) * inv_fsubn, -1);
  proj_mat[3] = glm::vec4(0.f, 0.f, -2 * far * near * inv_fsubn, 0.f);

  return proj_mat;   
}



}  // namespace


int main(int argc, char const *argv[]) {

  // create framebuffer
  enum {kFrameBufferSize = 512};
  imgpp::Img framebuffer;
  framebuffer.SetSize(kFrameBufferSize, kFrameBufferSize, 1, 3, 8);
  static const char kWindowName[] = "Render";

  // create window
  cv::namedWindow(kWindowName, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
  cv::resizeWindow(kWindowName, kFrameBufferSize, kFrameBufferSize);
  cv::setWindowTitle(kWindowName, kWindowName);

  // bind framebuffer to cv mat
  auto framebuffer_cv_mat = imgpp::RefCVMat(framebuffer.ROI());
  std::vector<glm::vec4> points;
  points.push_back(glm::vec4(100, 100, -100, 1));
  
  // render loop
  while (true) {

    // handle key events
    int key = cv::waitKey(1);

    if (key == 'q') {
      break;
    } else if (key == 'w') {
      std::cout << "w pressed!" << std::endl;
    }

    // actual rendering should happen here!
    // TODO: render
    

    // show img on screen
    cv::imshow(kWindowName, framebuffer_cv_mat);
  }

  return 0;
}