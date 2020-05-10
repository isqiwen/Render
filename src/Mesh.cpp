#include "../include/Mesh.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

namespace glm {
    void from_json(const nlohmann::json& j, glm::vec3& vec) {
        j.at(0).get_to(vec.x);
        j.at(1).get_to(vec.y);
        j.at(2).get_to(vec.z);
    }
}

Mesh::Mesh() {
    
}

Mesh::~Mesh() {

}

const glm::vec3 Mesh::GetPosition() {
    return position_;
}

const glm::vec3 Mesh::GetRotation() {
    return rotation_;
}

const glm::vec3 Mesh::GetScale() {
    return scale_;
}

const std::vector<glm::vec3> Mesh::GetVertices() {
    return vertices_;
}

const std::vector<glm::vec3> Mesh::GetNormals() {
    return normals_;
}

const std::vector<Face> Mesh::GetFaces() {
    return faces_;
}

bool Mesh::LoadMeshFromJSON(const std::string &file) {
    std::ifstream ifs(file);
    nlohmann::json j = nlohmann::json::parse(ifs);
    
    // name
    name_ = j["name"].get<std::string>();

    // position
    glm::from_json(j["position"], position_);

    // rotation
    glm::from_json(j["rotation"], rotation_);
    
    // scale
    glm::from_json(j["scale"], scale_);

    // vertices + normals
    auto &vertices_json = j["vertices"];
    auto &normals_json = j["normals"]; 
    vertices_.resize(vertices_json.size());
    normals_.resize(normals_json.size());
    for (int i = 0; i < vertices_.size(); ++i) {
        glm::from_json(vertices_json[i], vertices_[i]);
        glm::from_json(normals_json[i], normals_[i]);
    }

    // faces
    auto &faces_json = j["faces"];
    faces_.resize(faces_json.size());
    for (auto &face_json : faces_json) {
        Face face;
        face_json.at(0).get_to(face.a);
        face_json.at(1).get_to(face.b);
        face_json.at(2).get_to(face.c);
        faces_.push_back(face);
    }


    return true;
}