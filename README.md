# Ray Tracer

This is a ray tracer developed as a free time project. It's based on the famous [Ray Tracing in One Weekend][web1]. It currently supports shadows, reflections, refractions, materials and depth of field. The only objects supported are spheres.

### Graphical User Interface

The GUI was done using Qt 5. The heavy rendering processes are done in a different thread (currently only one thread is used). Features include: Using the mouse scroll button you can zoom in/out the scene. Progress bar to indicate the rendering process. Elapsed time status bar display.

[web1]:            https://raytracing.github.io/books/RayTracingInOneWeekend.html
