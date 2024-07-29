#pragma once
#include "geometry.hpp"

class sphere : public geometry {
public:

    vec3 center;
    double radius;

    sphere() {}
    sphere(vec3 cen, double r) : center{ cen }, radius{ r } {};

    bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction());
        double b = dot(oc, r.direction());
        double c = dot(oc, oc) - radius * radius;
        double discriminant = b * b - a * c;
        if (discriminant > 0) {
            double temp = (-b - sqrt(discriminant)) / a;
            if (temp < tmax && temp > tmin) {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp < tmax && temp > tmin) {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                
                return true;
            }
        }
        return false;
    }
    
};