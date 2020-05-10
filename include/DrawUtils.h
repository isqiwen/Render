#ifndef RENDER_DRAW_UTILS_H
#define RENDER_DRAW_UTILS_H

#include <cmath>
#include <glm/glm.hpp>
#include <imgpp/imgpp.hpp>

/**
 * Data for a vertices.
 */
typedef struct {
    glm::vec3 screen_pos; // Position projected on the 2D screen
    glm::vec3 normal; // Normal position in transformed coordinates
    glm::vec3 trans_pos; // Position in transformed coordinates
} VertexData;

class DrawUtils {

    public:
        // static void DrawLine(imgpp::Img &frame_buffer, const glm::vec3 &p1, const glm::vec3 &p2, int w, int h);

        // /**
        //  * Draw a line using Digital Differential Analyzer algorithm (DDA).
        //  *
        //  * \note
        //  * This isn't the most efficient algorithm since it still use one
        //  * floating point operation per pixel. But still better than naive
        //  * algorithm that uses 2 floating-point operations per pixel.
        //  */
        // static void DrawLineDDA(imgpp::Img &frame_buffer, int x1, int y1, int x2, int y2, int w, int h);

        // static void DrawScanLineTriangle(imgpp::Img &frame_buffer,
        //                                  imgpp::Img &depth_buffer,
        //                                  VertexData & v1,
        //                                  VertexData & v2,
        //                                  VertexData & v3,
        //                                  int w, int h,
        //                                  glm::ivec4 &color);

        static void DrawGouraudTriangle(imgpp::Img &frame_buffer,
                                        imgpp::Img &depth_buffer,
                                        VertexData &v1,
                                        VertexData &v2,
                                        VertexData &v3,
                                        int w, int h,
                                        const glm::ivec4 &color);

        // static void DrawScanLineTriangle(imgpp::Img &frame_buffer,
        //                                  int p1_x, int p1_y,
        //                                  int p2_x, int p2_y,
        //                                  int p3_x, int p3_y,
        //                                  int w, int h);

        // static void DrawClippedPoint(imgpp::Img &frame_buffer, int x, int y, int w, int h);
};

#endif