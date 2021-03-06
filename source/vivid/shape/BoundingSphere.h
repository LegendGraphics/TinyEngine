#ifndef VIVID_SHAPE_BOUNDING_SPHERE_H
#define VIVID_SHAPE_BOUNDING_SPHERE_H

#include "vivid/math/Vector3.h"

namespace vivid
{
    class BoundingSphere
    {
    public:
        BoundingSphere(const Vector3& center, float radius);

        bool contains(const Vector3& point);
        //bool intersectRay(const Ray& ray);
        bool intersect(const BoundingSphere& bs);

        inline Vector3 getCenter() const { return _center; }
        inline float   getRadius() const { return _radius; }

    protected:
        Vector3 _center;
        float _radius;
    };
}
#endif // VIVID_SHAPE_BOUNDING_SPHERE_H
