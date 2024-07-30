#pragma once
#include "geometry.hpp"


#include <memory>
#include <vector>


class world: public geometry {

public:

    std::vector<std::shared_ptr<geometry>> objects;

    world() {}
    world(std::shared_ptr<geometry> object) { 
        add(object); 
    }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<geometry> object) {
        objects.push_back(object);
    }

    
    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (const auto& object : objects) {
            if (object->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

  
};