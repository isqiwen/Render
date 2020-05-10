#include "../include/MatrixTransform.h"
#include <iostream>
#include <cmath>

void PrintGLMMat4(const glm::mat4 &mat) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            std::cout << mat[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

glm::vec3 Normalize(const glm::vec3 &v)
{
        float k = glm::dot(v, v);
        k = sqrt(k);
        if (k < 0.0000000000001) {
                k = 1;
                std::cout << "Attention! Vector is zero!" << std::endl;
        }

        return glm::vec3(v.x / k, v.y / k, v.z / k);
}

// -----------------------------------------------------------------------------
// Translation methods
// -----------------------------------------------------------------------------

glm::mat4 MatrixTransform::CreateTranslate(const glm::vec3 &translate) {
    return MatrixTransform::CreateTranslate(translate.x, translate.y, translate.z);
}

glm::mat4 MatrixTransform::CreateTranslate(float translate_x, float translate_y, float translate_z) {
    glm::mat4 mat = glm::mat4(1.0f);
    mat[0][3] = translate_x;
    mat[1][3] = translate_y;
    mat[2][3] = translate_z;

    return mat;
}

// -----------------------------------------------------------------------------
// Scaling methods
// -----------------------------------------------------------------------------

glm::mat4 MatrixTransform::CreateScale(const glm::vec3 &scale) {
    return MatrixTransform::CreateScale(scale.x, scale.y, scale.z);
}

glm::mat4 MatrixTransform::CreateScale(float scale_x, float scale_y, float scale_z) {
    glm::mat4 mat = glm::mat4();
    mat[0][0] = scale_x;
    mat[1][1] = scale_y;
    mat[2][2] = scale_z;
    mat[3][3] = 1.0f;

    return mat;
}

// -----------------------------------------------------------------------------
// Rotation methods
// -----------------------------------------------------------------------------

glm::mat4 MatrixTransform::CreateRotateX(float radians_x) {
    glm::mat4 mat = glm::mat4(1.0f);
    const float cos = std::cos(radians_x);
    const float sin = std::sin(radians_x);
    mat[1][1] = cos;
    mat[1][2] = -sin;
    mat[2][1] = sin;
    mat[2][2] = cos;
    return mat;
}

glm::mat4 MatrixTransform::CreateRotateY(float radians_y) {
    glm::mat4 mat = glm::mat4(1.0f);
    const float cos = std::cos(radians_y);
    const float sin = std::sin(radians_y);
    mat[0][0] = cos;
    mat[0][2] = sin;
    mat[2][0] = -sin;
    mat[2][2] = cos;
    return mat;
}

glm::mat4 MatrixTransform::CreateRotateZ(float radians_z) {
    glm::mat4 mat = glm::mat4(1.0f);
    const float cos = std::cos(radians_z);
    const float sin = std::sin(radians_z);
    mat[0][0] = cos;
    mat[0][1] = -sin;
    mat[1][0] = sin;
    mat[1][1] = cos;
    return mat;
}

glm::mat4 MatrixTransform::CreateRotateZYX(float radians_z,
                                           float radians_y,
                                           float radians_x) {
    glm::mat4 mat = glm::mat4(1.0f);
    mat *= MatrixTransform::CreateRotateZ(radians_z);
    mat *= MatrixTransform::CreateRotateY(radians_y);
    mat *= MatrixTransform::CreateRotateX(radians_x);
    return mat;
}

glm::mat4 MatrixTransform::CreateRotateZYX(const glm::vec3 &rotate_zyx) {
    return MatrixTransform::CreateRotateZYX(rotate_zyx.x, rotate_zyx.y, rotate_zyx.z);
}

// -----------------------------------------------------------------------------
// Matrix Utils methods
// -----------------------------------------------------------------------------

glm::mat4 MatrixTransform::CreateLookAtLH(const glm::vec3 &camera_pos,
                                          const glm::vec3 &target_pos,
                                          const glm::vec3 &up_vector) {
    // Creates the 3 Camera's vectors.
    glm::vec3 cx, cy, cz;
    cz = target_pos - camera_pos;
    cx = glm::cross(up_vector, cz);
    cy = glm::cross(cz, cx);
    cz = Normalize(cz);
    cx = Normalize(cx);
    cy = Normalize(cy);

    // Creates the lookAt matrix from camera vectors.
    glm::mat3 m3(cx, cy, cz);
    glm::mat4 m_look_at(m3);
    m_look_at[0][3] = -(glm::dot(cx, camera_pos));
    m_look_at[1][3] = -(glm::dot(cy, camera_pos));
    m_look_at[2][3] = -(glm::dot(cz, camera_pos));
    return m_look_at;
}

glm::mat4 MatrixTransform::CreatePerspectiveFovLH(
    float radians_fov,
    float screen_width,
    float screen_height,
    float near,
    float far) {
    // TODO Add assert to check invalid values
    glm::mat4 mat = glm::mat4();
    const float cotan_fov = 1.0f / std::tan(radians_fov * 0.5f);
    const float depth = far - near;
    const float aspect = screen_height / screen_width;
    // Actual ratio is w/h but x / (w/h) == x * (h/w)
    mat[0][0] = cotan_fov * aspect;
    mat[1][1] = cotan_fov;
    mat[2][2] = far / depth;
    mat[2][3] = -(far * near) / depth;
    mat[3][2] = 1.0f;
    return mat;
}

glm::vec3 MatrixTransform::ProjectOnScreen(
    const glm::vec3 &point_3d,
    glm::mat4 const& matrix_transform,
    int screen_width,
    int screen_height) {
    glm::vec4 vec = glm::vec4(point_3d.x, point_3d.y, point_3d.z, 1.0f);
    vec = matrix_transform * vec;
    vec /= vec.w;
    vec.x = (vec.x * screen_width) + (screen_width * 0.5f);
    vec.y = (-vec.y * screen_height) + (screen_height * 0.5f);
    // upper-left is 0:0 thuz -vec.y
    return glm::vec3(vec.x, vec.y, vec.z);
}
