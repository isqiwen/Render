#ifndef RENDER_MATRIX_TRANSFORM_H
#define RENDER_MATRIX_TRANSFORM_H

#include <glm/glm.hpp>

/**
 * Abstract class that defines matrix utilities for transformations.
 * Meant to be used for operation like scaling, rotation, translation, lookAt...
 * (Specially with matrix 4x4).
 *
 * \remark
 * Matrix multiplication use 'Column vector' transformation.
 * Meaning that M1 * M2 * M3 * Vec will be actually (M1 * (M2 * (M3 * Vec))).
 * This change the structure of transform matrix.
 * For instance, translate matrix as tx, ty, and tz in the last column.
 * (In 'Row vector' multiplication, it is on the last row).
 *
 * \remark
 * For further informations about matrix transformations, some useful links.
 *
 * \see http://www.codinglabs.net/article_world_view_projection_matrix.aspx
 * \see https://web.archive.org/web/20131222170415/http:/robertokoci.com/world-view-projection-matrix-unveiled/
 * \see http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
 * \see http://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
 *
 * \author Qiwen Wang
 */


void PrintGLMMat4(const glm::mat4 &mat);

class MatrixTransform {

    private:
        // This is an abstract class
        MatrixTransform() = default;

    // -------------------------------------------------------------------------
    // Translation methods
    // -------------------------------------------------------------------------
    public:

        /**
         * Create a translation matrix from the given vector.
         *
         * \param translate The translation vector.
         * \return New translation matrix.
         */
        static glm::mat4 CreateTranslate(const glm::vec3 &translate);

        /**
         * Create a translation matrix with the given factors.
         *
         * \param translate_x The x translation value.
         * \param translate_y The y translation value.
         * \param translate_z The z translation value.
         * \return New translation matrix.
         */
        static glm::mat4 CreateTranslate(float translate_x,
                                         float translate_y,
                                         float translate_z);


    // -------------------------------------------------------------------------
    // Scaling methods
    // -------------------------------------------------------------------------
    public:

        /**
         * Create a scale matrix from given vector values.
         *
         * \param scale_xyz The vector with scale values (tx, ty, tz).
         * \return New scale matrix.
         */
        static glm::mat4 CreateScale(const glm::vec3 &scale_xyz);

        /**
         * Create a scale matrix from given factors.
         *
         * \param scale_x The x scale factor.
         * \param scale_y The y scale factor.
         * \param scale_z The z scale factor.
         * \return New scale matrix.
         */
        static glm::mat4 CreateScale(float scale_x,
                                     float scale_y,
                                     float scale_z);


    // -------------------------------------------------------------------------
    // Rotation methods
    // -------------------------------------------------------------------------
    public:

        /**
         * Create a rotation matrix on the x axe.
         *
         * \param radians_x The x rotation in radians.
         * \return The new rotation matrix.
         */
        static glm::mat4 CreateRotateX(float radians_x);

        /**
         * Create a rotation matrix on the y axe.
         *
         * \param radians_y The y rotation angle in radians.
         * \return The new rotation matrix.
         */
        static glm::mat4 CreateRotateY(float radians_y);

        /**
         * Create a rotation matrix on the z axe.
         *
         * \param radians_z The z rotation angle in radians.
         * \return The new rotation matrix.
         */
        static glm::mat4 CreateRotateZ(float radians_z);

        /**
         * Create a rotation matrix on the 3 axes.
         * It use the Yaw-Pitch-Roll order.
         *
         * \note
         * To learn more about, search for information about
         * "Converting Euler Angles to a Rotation Matrix".
         *
         * \see http://danceswithcode.net/engineeringnotes/rotations_in_3d/rotations_in_3d_part1.html
         *
         * \param radians_z The z rotation angle in radians. (Yaw)
         * \param radians_y The y rotation angle in radians. (Pitch)
         * \param radians_x The x rotation angle in radians. (Roll)
         * \return The new rotation matrix.
         */
        static glm::mat4 CreateRotateZYX(float radians_z,
                                         float radians_y,
                                         float radians_x);

        /**
         * Create a rotation matrix on the 3 axes.
         * It use the Yaw-Pitch-Roll order.
         *
         * \param rotate_zyx Vector that represents the YawPitchRoll rotation.
         * \return The new rotation matrix.
         */
        static glm::mat4 CreateRotateZYX(const glm::vec3 &rotate_zyx);


    // -------------------------------------------------------------------------
    // Matrix Utils methods
    // -------------------------------------------------------------------------
    public:

        /**
         * Create a left-handed (LH) LookAt matrix.
         *
         * \param camera_pos Position of the camera (Eye point).
         * \param target_pos Position of a point camera is pointing toward.
         * \param up_vector The up vector for current world. (Generally 0,1,0).
         * \return The left-handed lookAt matrix.
         */
        static glm::mat4 CreateLookAtLH(const glm::vec3 &camera_pos,
                                     const glm::vec3 &target_pos,
                                     const glm::vec3 &up_vector);

        /**
         * Create a left-handed (LH) perspective projection matrix.
         *
         * The viewed scene is a frustum defined by the Fov angle
         * and the depth distances (Near and far).
         * The field of view (Fov) is the angle the camera see the scene.
         *
         * \param radians_fov Camera scene angle in radians (Must be positive).
         * \param screen_width Screen width in pixels.
         * \param screen_height Screen height in pixels.
         * \param near Nearest distance camera can see (Must be positive).
         * \param far Farther distance camera can see (Must be positive).
         * \return The left-handed perspective matrix.
         */
        static glm::mat4 CreatePerspectiveFovLH(float radians_fov,
                                                float screen_width,
                                                float screen_height,
                                                float near,
                                                float far);
        /**
         * Project a 3D point into a 2D screen.
         * Resulting point is a 3D vector (x, y, z) with z is the Z-Buffering.
         *
         * \note
         * The screen use the common upper-left corner as 0:0.
         * Y coordinates are going down.
         *
         * \param point_3d The point to project.
         * \param matrix_transform Transformation matrix to apply 
         * \param screen_width The screen width in pixels.
         * \param screen_height The screen height in pixels.
         * \return Projected point on the screen.
         */
        static glm::vec3 ProjectOnScreen(const glm::vec3 &point_3d,
                                         const glm::mat4 &matrix_transform,
                                         int screen_width,
                                         int screen_height);
};

#endif