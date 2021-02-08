#ifndef SCENE_H
#define SCENE_H

#include <memory>

class Hittable_list;

namespace DFL {

class Scene
{
public:
    enum class Type { Basic, Advanced, Complex };

    std::unique_ptr<Hittable_list> create_world(Scene::Type scene_type) noexcept;

private:
    std::unique_ptr<Hittable_list> create_basic_world() noexcept;
    std::unique_ptr<Hittable_list> create_advanced_world() noexcept;
    std::unique_ptr<Hittable_list> create_complex_world() noexcept;
};

} // namespace DFL

#endif // SCENE_H
