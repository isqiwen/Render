#include "../include/Engine.h"
#include <imgpp/opencvbinding.hpp>
#include "../include/MatrixTransform.h"
#include "../include/DrawUtils.h"


// Do a projection but don't project on screen, stay un frustum
static glm::vec3 ProjectPoint(glm::vec3 const& v, glm::mat4 const& m) {
    glm::vec4 vec(v.x, v.y, v.z, 1.0f);
    vec = m * vec;
    vec /= vec.w;
    return glm::vec3(vec.x, vec.y, vec.z);
}

Engine::Engine() : render_window_(), is_runing_(true), camera_(), frame_buffer_(), depth_buffer_() {
    is_runing_ = false;
    depth_buffer_.SetSize(WINDOW_DEFAULT_SIZE_W, WINDOW_DEFAULT_SIZE_H, 1, 1, 32, true, true);
    frame_buffer_.SetSize(WINDOW_DEFAULT_SIZE_W, WINDOW_DEFAULT_SIZE_H, 1, 4, 8);
}

Engine::~Engine() {
    // render_window_.
}

Engine& Engine::GetInstance() {
    static Engine engine_;
    return engine_;
}

bool Engine::Init() {

    mesh_.LoadMeshFromJSON("/home/qiwen/project/Render/resources/meshes/test1.json");
    camera_.SetPosition(glm::vec3(0.f, 0.f, -4.0f));
    camera_.SetTarget(mesh_.GetPosition());

    render_window_.InitRenderWindow("Render", WINDOW_DEFAULT_SIZE_W, WINDOW_DEFAULT_SIZE_H, WINDOW_DEFAULT_LEFT, WINDOW_DEFAULT_TOP);

    return true;
}

void Engine::HandleEvent(int key){
    switch (key) {
        case 'q':
            StopRendering();
            break;
        case 'w':
            std::cout << "w pressed!" << std::endl;
            break;
        default:
            break;

    }
}

bool Engine::StartRendering() {
    if (is_runing_) {
        return false;
    }
    is_runing_ = true;
    while (is_runing_) {
        // handle key events
        int key = cv::waitKey(1);

        HandleEvent(key);

        RenderOneFrame();
    }
    
    return true;
}

bool Engine::RenderOneFrame() {
    for (int  x = 0; x < depth_buffer_.ROI().Width(); ++x) {
        for (int y = 0; y < depth_buffer_.ROI().Height(); ++y) {
            depth_buffer_.ROI().At<float>(x, y) = 2.0f;
        }
    }
    frame_buffer_.Zeros();
    RenderAll();
    render_window_.ShowImg(imgpp::RefCVMat(frame_buffer_.ROI()));

    return true;
}

bool Engine::StopRendering(){
    is_runing_ = false;

    return true;
}

void Engine::RenderAll() {
    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    const float w = WINDOW_DEFAULT_SIZE_W;
    const float h = WINDOW_DEFAULT_SIZE_H;
    glm::mat4 view_matrix = MatrixTransform::CreateLookAtLH(camera_.GetPosition(), camera_.GetTarget(), up);
    glm::mat4 projection_matrix = MatrixTransform::CreatePerspectiveFovLH(1.57f, w, h, 1.0f, 45.0f);

    // Project mesh
    const glm::mat4 pos_matrix = MatrixTransform::CreateTranslate(mesh_.GetPosition());
    const glm::mat4 rot_matrix = MatrixTransform::CreateRotateZYX(mesh_.GetRotation());
    const glm::mat4 sca_matrix = MatrixTransform::CreateScale(mesh_.GetScale());
    const glm::mat4 world_matrix = pos_matrix * rot_matrix * sca_matrix;
    const glm::mat4 camera_matrix = view_matrix * world_matrix;
    const glm::mat4 transform_matrix = projection_matrix * camera_matrix;
    auto &faces = mesh_.GetFaces();
    auto &vertices = mesh_.GetVertices();
    auto &normals = mesh_.GetNormals();
    for(auto &face : faces) {
        // Project 3 vertices in the 2D screen coordinates
        glm::vec3 p1_proj = MatrixTransform::ProjectOnScreen(vertices[face.a], transform_matrix, w, h);
        glm::vec3 p2_proj = MatrixTransform::ProjectOnScreen(vertices[face.b], transform_matrix, w, h);
        glm::vec3 p3_proj = MatrixTransform::ProjectOnScreen(vertices[face.c], transform_matrix, w, h);

        // Project normals in world coordinates
        glm::vec3 p1_norm = ProjectPoint(normals[face.a], world_matrix);
        glm::vec3 p2_norm = ProjectPoint(normals[face.b], world_matrix);
        glm::vec3 p3_norm = ProjectPoint(normals[face.c], world_matrix);

        // Project positions in world coordinates
        glm::vec3 p1_pos = ProjectPoint(vertices[face.a], world_matrix);
        glm::vec3 p2_pos = ProjectPoint(vertices[face.b], world_matrix);
        glm::vec3 p3_pos = ProjectPoint(vertices[face.c], world_matrix);

        VertexData v1 = {p1_proj, p1_norm, p1_pos};
        VertexData v2 = {p2_proj, p2_norm, p2_pos};
        VertexData v3 = {p3_proj, p3_norm, p3_pos};

        // Color of the light (To refactor)
        glm::ivec4 color;
        color.x = 255;
        color.y = 240;
        color.z = 42;
        color.w = 255;
        DrawUtils::DrawGouraudTriangle(frame_buffer_, depth_buffer_, v1, v2, v3, w, h, color);
    }
}




