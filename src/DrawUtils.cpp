#include "../include/DrawUtils.h"
#include <iostream>

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

// TODO To move in math lib
static float Interpolate(const float min, const float max, const float gradient) {
    return min + (max - min) * gradient;
}

static glm::vec3 Normalize(const glm::vec3 &v) {
        float k = glm::dot(v, v);
        k = sqrt(k);
        if (k < 0.0000000000001) {
                k = 1;
                std::cout << "Attention! Vector is zero!" << std::endl;
        }

        return glm::vec3(v.x / k, v.y / k, v.z / k);
}

// Swap v1 with v2
static void SwapVerticeData(VertexData &v1, VertexData &v2) {
    VertexData tmp = {v1.screen_pos, v1.normal, v1.trans_pos};
    // v1 = v2
    v1.screen_pos = v2.screen_pos;
    v1.normal = v2.normal;
    v1.trans_pos = v2.trans_pos;
    // v2 = tmp
    v2.screen_pos = tmp.screen_pos;
    v2.normal = tmp.normal;
    v2.trans_pos = tmp.trans_pos;
}

static float CalCosNormLight(const glm::vec3 &vertex_pos, const glm::vec3 &normal, const glm::vec3 &light_source) {
    glm::vec3 light_dir = light_source - vertex_pos;
    light_dir = Normalize(light_dir);
    glm::vec3 normal_norm = Normalize(normal);
    return std::max(glm::dot(normal_norm, light_dir), 0.0f);
}

/*

// -----------------------------------------------------------------------------
// Draw Line
// -----------------------------------------------------------------------------

void DrawUtils::DrawLine(imgpp::Img &frame_buffer, VectF3 const& p1, VectF3 const& p2, int w, int h) {
    DrawSDLUtils::DrawLineDDA(renderer, p1.x, p1.y, p2.x, p2.y, w, h);
}

void DrawSDLUtils::DrawLineDDA(imgpp::Img &frame_buffer, int x1, int y1, int x2, int y2, int w, int h) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float m = dy / dx;
    if (m <= 1.0f && m >= -1.0f) {
        int x_start = x1;
        int x_end   = x2;
        float y     = y1;
        // This is used to start drawing from the point with lower x value.
        if (dx < 0.0f) {
            x_start = x2;
            x_end   = x1;
            y       = y2;
        }
        for (int x = x_start; x <= x_end; x++) {
            y += m;
            DrawClippedPoint(renderer, x, (int)y, w, h);
        }
    } else {
        m = dx / dy;
        int y_start = y1;
        int y_end   = y2;
        float x     = x1;
        // This is used to start drawing from the point with lower y value.
        if (dy < 0.0f) {
            y_start = y2;
            y_end   = y1;
            x       = x2;
        }
        for (int y = y_start; y <= y_end; y++) {
            x += m;
            DrawClippedPoint(renderer, x, (int)y, w, h);
        }
    }
}


// -----------------------------------------------------------------------------
// Draw Triangle with Z-Buffering and Flag-shading
// -----------------------------------------------------------------------------

// Draw line between 2 points, at y position, from left to right.
// Points are start point (On line p1p2) and end point (On line p3p4);
// p1p2 is at the left, p3p4 at the right
static void DrawOneScanLineTriangle(imgpp::Img &frame_buffer,
                                   imgpp::Img &depth_buffer,
                                    int y,
                                    int p1_x, int p1_y, float p1_z,
                                    int p2_x, int p2_y, float p2_z,
                                    int p3_x, int p3_y, float p3_z,
                                    int p4_x, int p4_y, float p4_z,
                                    int w, int h,
                                    glm::ivec4 &color) {
    float gradientP1P2 = ((p2_y - p1_y) != 0) ? (y - p1_y) / (float)(p2_y - p1_y) : 1;
    float gradientP3P4 = ((p4_y - p3_y) != 0) ? (y - p3_y) / (float)(p4_y - p3_y) : 1;

    float sx = interpolate(p1_x, p2_x, gradientP1P2);
    float ex = interpolate(p3_x, p4_x, gradientP3P4);

    float z1 = interpolate(p1_z, p2_z, gradientP1P2);
    float z2 = interpolate(p3_z, p4_z, gradientP3P4);

    for(int x = sx; x <= ex; x++) {
        // Skip point outside the screen.
        if(x >= 0 && x < w && y >= 0 && y < h) {
            int index = y * w + x;
            float gradientz = (x - sx) / (float)(ex - sx);
            float z = interpolate(z1, z2, gradientz);
            if(depthBuffer[index] > z) {
                depthBuffer[index] = z;
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void DrawSDLUtils::DrawScanLineTriangle(imgpp::Img &frame_buffer,
                                       imgpp::Img &depth_buffer,
                                        VertexData & v1,
                                        VertexData & v2,
                                        VertexData & v3,
                                        int w, int h,
                                        glm::ivec4 &color) {
    // After that, P1 has lower y value and P3 has higher y value.
    if(v1.screen_pos.y > v2.screen_pos.y) {
        SwapVerticeData(v1, v2);
    }
    if(v2.screen_pos.y > v3.screen_pos.y) {
        SwapVerticeData(v2, v3);
    }
    if(v1.screen_pos.y > v2.screen_pos.y) {
        SwapVerticeData(v1, v2);
    }

    float inv_slope_p1p2 = (v2.screen_pos.x - v1.screen_pos.x) / (float)(v2.screen_pos.y - v1.screen_pos.y);
    float inv_slope_p1p3 = (v3.screen_pos.x - v1.screen_pos.x) / (float)(v3.screen_pos.y - v1.screen_pos.y);

    VectF3 light_source(100.0f, 50.0f, -50.0f); // TODO Hard coded, should be moved outside
    VectF3 faceNormal   = (*v1.normal + *v2.normal + *v3.normal) / 3.0f;
    VectF3 faceCenter   = (v1.trans_pos + v2.trans_pos + v3.trans_pos) / 3.0f;
    float cosNormLight  = CalCosNormLight(faceCenter, faceNormal, light_source);

    // Calculate new color
    color->r *=  cosNormLight;
    color->g *=  cosNormLight;
    color->b *=  cosNormLight;

    // P2 at the right of P1P3
    if(inv_slope_p1p2 > inv_slope_p1p3) {
        for(int y = v1.screen_pos.y; y < v2.screen_pos.y; y++) {
            drawOneScanLineTriangle(renderer, depthBuffer, y,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    w, h, color);
        }
        for(int y = v2.screen_pos.y; y <= v3.screen_pos.y; y++) {
            drawOneScanLineTriangle(renderer, depthBuffer, y,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    w, h, color);
        }
    }
    // P2 at the left of P1P3
    else {
        for(int y = v1.screen_pos.y; y < v2.screen_pos.y; y++) {
            drawOneScanLineTriangle(renderer, depthBuffer, y,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    w, h, color);
        }
        for(int y = v2.screen_pos.y; y <= v3.screen_pos.y; y++) {
            drawOneScanLineTriangle(renderer, depthBuffer, y,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    w, h, color);
        }
    }
}


// -----------------------------------------------------------------------------
// Other functions
// -----------------------------------------------------------------------------

void DrawSDLUtils::DrawClippedPoint(imgpp::Img &frame_buffer, int x, int y, int w, int h) {
    if(x >= 0 && x <= w && y >= 0 && y <= h) {
        SDL_RenderDrawPoint(renderer, x, y);
    }
}


// -----------------------------------------------------------------------------
// Draw Filled triangle (Simple)
// -----------------------------------------------------------------------------

// TODO To clean
// Vertices must be ordonated
// P1 has lower y, P2 and P3 has same y and highter
static void drawScanlineBottomFlatTriangle(imgpp::Img &frame_buffer,
                                            int p1_x, int p1_y,
                                            int p2_x, int p2_y,
                                            int p3_x, int p3_y,
                                            int w, int h) {
    // Just for readability
    typedef struct { int x; int y; } P;
    P P1 = {p1_x, p1_y};
    P P2 = {p2_x, p2_y};
    P P3 = {p3_x, p3_y};
    const float inv_slope_p1p2 = (P2.x - P1.x) / (float)(P2.y - P1.y);
    const float inv_slope_p1p3 = (P3.x - P1.x) / (float)(P3.y - P1.y);
    float sx = P1.x, ex = P1.x;
    for(int y = P1.y; y <= P2.y; y++) {
        for(int x = sx; x <= ex; x++) {
            DrawSDLUtils::drawClippedPoint(renderer, (int)x, y, w, h);
        }
        sx += inv_slope_p1p2;
        ex += inv_slope_p1p3;
    }
}

// TODO To clean
// Vertices must be ordonated
// P1 has lower y, P2 and P3 has same y and highter
static void drawScanlineTopFlatTriangle(imgpp::Img &frame_buffer,
                                        int p1_x, int p1_y,
                                        int p2_x, int p2_y,
                                        int p3_x, int p3_y,
                                        int w, int h) {
    // Just for readability
    typedef struct { int x; int y; } P;
    P P1 = {p1_x, p1_y};
    P P2 = {p2_x, p2_y};
    P P3 = {p3_x, p3_y};
    const float inv_slope_p1p2 = (P2.x - P1.x) / (float)(P2.y - P1.y);
    const float inv_slope_p1p3 = (P3.x - P1.x) / (float)(P3.y - P1.y);
    float sx = P1.x, ex = P1.x;
    for(int y = P1.y; y >= P2.y; y--) {
        for(int x = sx; x <= ex; x++) {
            DrawSDLUtils::drawClippedPoint(renderer, (int)x, y, w, h);
        }
        sx -= inv_slope_p1p2;
        ex -= inv_slope_p1p3;
    }
}
void DrawSDLUtils::drawScanLineTriangle(imgpp::Img &frame_buffer,
                                        int p1_x, int p1_y,
                                        int p2_x, int p2_y,
                                        int p3_x, int p3_y,
                                        int w, int h) {
    // Just for readability
    typedef struct { int x; int y; } P;
    P P1 = {p1_x, p1_y};
    P P2 = {p2_x, p2_y};
    P P3 = {p3_x, p3_y};

    // After that, P1 has lower y value and P3 has higher y value.
    if(P1.y > P2.y) {
        P tmp = P1;
        P1 = P2;
        P2 = tmp;
    }
    if(P2.y > P3.y) {
        P tmp = P2;
        P2 = P3;
        P3 = tmp;
    }
    if(P1.y > P2.y) {
        P tmp = P1;
        P1 = P2;
        P2 = tmp;
    }

    if(P2.y == P3.y){
        if(P2.y > P1.y) {
            drawScanlineBottomFlatTriangle(renderer, P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, w, h);
        }
        else {
            drawScanlineTopFlatTriangle(renderer, P1.x, P1.y, P2.x, P2.y, P3.x, P3.y, w, h);
        }
    }
    else {
        const float inv_slope_p1p3 = (P3.x - P1.x) / (float)(P3.y - P1.y);
        P P4;
        P4.y = P2.y;
        P4.x = inv_slope_p1p3 * (P2.y - P3.y) + P3.x;

        // P4 at the right of P2
        if(P4.x > P2.x) {
            drawScanlineBottomFlatTriangle(renderer, P1.x, P1.y, P2.x, P2.y, P4.x, P4.y, w, h);
            drawScanlineTopFlatTriangle(renderer, P3.x, P3.y, P2.x, P2.y, P4.x, P4.y, w, h);
        }
        else {
            drawScanlineBottomFlatTriangle(renderer, P1.x, P1.y, P4.x, P4.y, P2.x, P2.y, w, h);
            drawScanlineTopFlatTriangle(renderer, P3.x, P3.y, P4.x, P4.y, P2.x, P2.y, w, h);
        }
    }
}
*/

// -----------------------------------------------------------------------------
// Draw Triangle with Z-Buffering and Gouraud Shading
// -----------------------------------------------------------------------------

// Draw line between 2 points, at y position, from left to right.
// Points are start point (On line p1p2) and end point (On line p3p4);
// p1p2 is at the left, p3p4 at the right
static void DrawOneGouraudTriangle(imgpp::Img &frame_buffer,
                                   imgpp::Img &depth_buffer,
                                   int y,
                                   int p1_x, int p1_y, float p1_z,
                                   int p2_x, int p2_y, float p2_z,
                                   int p3_x, int p3_y, float p3_z,
                                   int p4_x, int p4_y, float p4_z,
                                   float cos1, float cos2,
                                   float cos3, float cos4,
                                   int w, int h,
                                   const glm::ivec4 &color) {
    float gradient_p1p2 = ((p2_y - p1_y) != 0) ? (y - p1_y) / (float)(p2_y - p1_y) : 1;
    float gradient_p3p4 = ((p4_y - p3_y) != 0) ? (y - p3_y) / (float)(p4_y - p3_y) : 1;

    float sx = Interpolate(p1_x, p2_x, gradient_p1p2);
    float ex = Interpolate(p3_x, p4_x, gradient_p3p4);

    float z1 = Interpolate(p1_z, p2_z, gradient_p1p2);
    float z2 = Interpolate(p3_z, p4_z, gradient_p3p4);

    float c1 = Interpolate(cos1, cos2, gradient_p1p2);
    float c2 = Interpolate(cos3, cos4, gradient_p3p4);

    imgpp::ImgROI &frame_buffer_roi = frame_buffer.ROI();
    imgpp::ImgROI &depth_buffer_roi = depth_buffer.ROI();
    for(int x = sx; x < ex; ++x) {
        // Skip point outside the screen.
        if (x >= 0 && x < w && y >= 0 && y < h) {
            float gradient_x = (x - sx) / (float)(ex - sx);
            float z = Interpolate(z1, z2, gradient_x);
            // float c = Interpolate(c1, c2, gradient_x);
            if (depth_buffer_roi.At<float>(x, y) > z) {
                depth_buffer_roi.At<float>(x, y) = z;
                float c = Interpolate(c1, c2, gradient_x);
                frame_buffer_roi.At<uint8_t>(x, y, 0) = color.x * c;
                frame_buffer_roi.At<uint8_t>(x, y, 1) = color.y * c;
                frame_buffer_roi.At<uint8_t>(x, y, 2) = color.z * c;
                frame_buffer_roi.At<uint8_t>(x, y, 3) = color.w;
            }
        }
    }
}

void DrawUtils::DrawGouraudTriangle(imgpp::Img &frame_buffer,
                                       imgpp::Img &depth_buffer,
                                       VertexData &v1,
                                       VertexData &v2,
                                       VertexData &v3,
                                       int w, int h,
                                       const glm::ivec4 &color) {
    // After that, P1 has lower y value and P3 has higher y value.
    if (v1.screen_pos.y > v2.screen_pos.y) {
        SwapVerticeData(v1, v2);
    }
    if (v2.screen_pos.y > v3.screen_pos.y) {
        SwapVerticeData(v2, v3);
    }
    if (v1.screen_pos.y > v2.screen_pos.y) {
        SwapVerticeData(v1, v2);
    }

    const float inv_slope_p1p2 = (v2.screen_pos.x - v1.screen_pos.x) / (float)(v2.screen_pos.y - v1.screen_pos.y);
    const float inv_slope_p1p3 = (v3.screen_pos.x - v1.screen_pos.x) / (float)(v3.screen_pos.y - v1.screen_pos.y);

    const glm::vec3 light_source(100.0f, 50.0f, -50.0f); // TODO Hard coded, should be moved outside
    const float cos1 = CalCosNormLight(v1.trans_pos, v1.normal, light_source);
    const float cos2 = CalCosNormLight(v2.trans_pos, v2.normal, light_source);
    const float cos3 = CalCosNormLight(v3.trans_pos, v3.normal, light_source);

    // P2 at the right of P1P3
    if (inv_slope_p1p2 > inv_slope_p1p3) {
        for (int y = v1.screen_pos.y; y < v2.screen_pos.y; y++) {
            DrawOneGouraudTriangle(frame_buffer, depth_buffer, y,
                v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                cos1, cos3, cos1, cos2,
                w, h, color);
        }
        for (int y = v2.screen_pos.y; y <= v3.screen_pos.y; y++) {
            DrawOneGouraudTriangle(frame_buffer, depth_buffer, y,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    cos1, cos3, cos2, cos3,
                    w, h, color);
        }
    } else {   // P2 at the left of P1P3
        for (int y = v1.screen_pos.y; y < v2.screen_pos.y; y++) {
            DrawOneGouraudTriangle(frame_buffer, depth_buffer, y,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    cos1, cos2, cos1, cos3,
                    w, h, color);
        }
        for (int y = v2.screen_pos.y; y <= v3.screen_pos.y; y++) {
            DrawOneGouraudTriangle(frame_buffer, depth_buffer, y,
                    v2.screen_pos.x, v2.screen_pos.y, v2.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    v1.screen_pos.x, v1.screen_pos.y, v1.screen_pos.z,
                    v3.screen_pos.x, v3.screen_pos.y, v3.screen_pos.z,
                    cos2, cos3, cos1, cos3,
                    w, h, color);
        }
    }
}