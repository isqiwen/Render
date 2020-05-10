#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <glm/glm.hpp>

/**
 * The main camera component.
 * View point from where there scene is rendered.
 */
class Camera {
    private:
        // Position of the camera in world space.
        glm::vec3 position_;

        // Position of the target in world space (Camera look at target).
        glm::vec3 target_;

    public:
        Camera();

        glm::vec3 GetPosition();
        glm::vec3 GetTarget();
        void SetPosition(const glm::vec3 &position);
        void SetTarget(const glm::vec3 &target);
};

#endif