#include "api.h"

#include "cameras/camera.h"
#include "core/scene.h"

class Scene;

namespace DFL
{

Render_options::Render_options()
{

}

std::unique_ptr<Camera> Render_options::make_camera()
{
    Camera *camera = new Camera;
    return std::unique_ptr<Camera>(camera);
}

std::unique_ptr<Scene> Render_options::make_scene()
{
    Scene *scene = new Scene;
    return std::unique_ptr<Scene>(scene);
}

} //namespace DFL
