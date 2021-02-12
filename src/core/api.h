#ifndef API_H
#define API_H

#include "core/geometry.h"

namespace DFL
{

class Scene;
class Camera;

struct Render_options
{
    Render_options();
    std::unique_ptr<Scene> make_scene();
    std::unique_ptr<Camera> make_camera();
};

}  // namespace DFL

#endif // API_H
