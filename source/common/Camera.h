#ifndef COMMON_CAMERA_H
#define COMMON_CAMERA_H

#include "common/Node.h"
#include "common/Component.h"
#include "math/Vector3.h"
#include "math/Transform.h"
#include "shape/Frustum.h"

namespace te
{
    // Using right-handed coordinate system for world coordinate system
    // Y-axis is up direction
    struct WorldCoordinate
    {
        const static Vector3 WORLD_UP;
        const static Vector3 WORLD_RIGHT;
        const static Vector3 WORLD_FORWARD;
    };

    // Consider Camera as a special leaf node
    class Camera : public Node
    {
    public:
        ENABLE_LEAF_NODE
        Camera();
        virtual ~Camera();

        bool cull(Node* node);


    private:
        bool frustumCullingImpl(Node* node);
    };

    class CameraState : public Component
    {
    public:
        enum class CameraMode
        {
            ORTHOGONAL,
            PERSPECTIVE
        };

    public:
        struct CameraViewParas
        {
            CameraViewParas() = default;
            CameraViewParas(Vector3 position, Vector3 center, Vector3 up)
            {
                this->position = position;
                this->center = center;
                this->up = up;
            }

            Vector3 position;
            Vector3 center;
            Vector3 up;
        };

        struct CameraPersParas
        {
            CameraPersParas() = default;
            CameraPersParas(float fov, float aspect, float znear, float zfar)
            {
                this->fov = fov;
                this->aspect = aspect;
                this->znear = znear;
                this->zfar = zfar;
            }

            float fov;
            float aspect;
            float znear;
            float zfar;
        };

        struct CameraOrthoParas
        {
            CameraOrthoParas() = default;
            CameraOrthoParas(float left, float right, float bottom, float top, float znear, float zfar)
            {
                this->left = left;
                this->right = right;
                this->bottom = bottom;
                this->top = top;
                this->znear = znear;
                this->zfar = zfar;
            }

            float left;
            float right;
            float bottom;
            float top;
            float znear;
            float zfar;
        };

    public:
        CameraState();
        virtual ~CameraState();

        void setCameraMode(CameraMode mode);
        void setViewTransform(const CameraViewParas& view_paras);
        void setPersProjectTransform(const CameraPersParas& pers_paras);
        void setOrthoProjectTransform(const CameraOrthoParas& ortho_paras);

        CameraViewParas& getViewParas() { return _view_paras; }
        CameraPersParas& getPersParas() { return _pers_paras; }
        CameraOrthoParas& getOrthoParas() { return _ortho_paras; }

        Transform getViewTransform() const;
        Transform getProjectTransform() const;
        const Frustum& getFrustum();

    protected: 
        CameraViewParas     _view_paras;
        CameraPersParas     _pers_paras;
        CameraOrthoParas    _ortho_paras;
        Frustum             _frustum;

        CameraMode          _mode;
    };

    class FocusCamera : public Camera
    {
    public:
        FocusCamera();
        void onMouseLeft(Event* event);
        void onMouseMiddle(Event* event);

        bool isFocused() const { return _focused; }

        void setFocusMousePos(const Vector2& pos);
        Vector2 getFocusMousePos() const { return _focus_mouse_pos; }

    private:
        Vector2 _focus_mouse_pos;
        bool    _focused;
    };

    class FocusCameraBehavior : public Behavior
    {
    public:
        void update();
    };
}


#endif // COMMON_CAMERA_H