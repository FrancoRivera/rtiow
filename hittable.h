#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"

class material;

// point, normal and t where the object is hit
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal){
        front_face = dot(r.direction(), outward_normal) < 0; // set if the ray is coming from the inside or outside
        normal = front_face ? outward_normal : -outward_normal; // set the normal ray
    }
};

// abstract class
class hittable {
    public:
    // we have t min and t max because sometimes we would only want
    // to search in that range of Ts for collitsion.
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif // HITTABLE_H
