#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

// A face is a simple 3D triangle that constitutes a mesh.
typedef struct {
    int a;
    int b;
    int c;
} Face;

/**
 * Mesh is used to represent a 3D object.
 * It is made of a set of vertices.
 */
class Mesh {
    private:
        /** The nice mesh's name */
        std::string name_;

        /** Mesh's position in 3D world space */
        glm::vec3 position_;

        /** Mesh's rotation */
        glm::vec3 rotation_;

        /** Mesh's scale */
        glm::vec3 scale_;

        /** All mesh's vertices */
        std::vector<glm::vec3> vertices_;

        /** All normal vectors for vertices */
        std::vector<glm::vec3> normals_;

        /** All faces */
        std::vector<Face> faces_;

    public:
        Mesh();
        ~Mesh();

        const glm::vec3 GetPosition();
        const glm::vec3 GetRotation();
        const glm::vec3 GetScale();
        const std::vector<glm::vec3> GetVertices();
        const std::vector<glm::vec3> GetNormals();
        const std::vector<Face> GetFaces();
        bool LoadMeshFromJSON(const std::string &file);
};

#endif