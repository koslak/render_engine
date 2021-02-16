#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "core/hittable.h"

#include <memory>
#include <vector>

class Hittable_list : public Hittable
{
public:
    Hittable_list() = default;
    Hittable_list(std::shared_ptr<Hittable> object) { add(object); }

    void clear() noexcept { objects_vector.clear(); }
    void add(std::shared_ptr<Hittable> object) noexcept { objects_vector.emplace_back(object); }

    virtual bool hit(const DFL::Ray &ray, double t_min, double t_max, Hit_record &hit_record) const override;

    std::vector<std::shared_ptr<Hittable>> objects_vector;
};

bool Hittable_list::hit(const DFL::Ray &ray, double t_min, double t_max, Hit_record &hit_record) const
{
    Hit_record temporal_hit_record;
    bool is_hit_anything{ false };
    double closest_so_far{ t_max };

    for(const auto &object : objects_vector)
    {
        if(object != nullptr && object->hit(ray, t_min, closest_so_far, temporal_hit_record))
        {
            is_hit_anything = true;
            closest_so_far = temporal_hit_record.t;
            hit_record = temporal_hit_record;
        }
    }

    return is_hit_anything;
}



#endif // HITTABLE_LIST_H
