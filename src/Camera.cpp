#include "../include/Camera.h"

Camera::Camera() {

}

glm::vec3 Camera::GetPosition() {
    return position_;
}

glm::vec3 Camera::GetTarget() {
    return target_;
}

void Camera::SetPosition(const glm::vec3 &position) {
    position_ = position;
}
void Camera::SetTarget(const glm::vec3 &target) {
    target_ = target;
}