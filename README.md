# Ray Tracer

This is a ray tracer developed as a free time project. It's based on the famous [Ray Tracing in One Weekend][web1]. It currently supports shadows, reflections, refractions, materials and depth of field. The only objects supported are spheres.

### Graphical User Interface

The GUI was done using Qt 5. The heavy rendering processes are done in a different thread (currently only one thread is used). Features include: Using the mouse scroll button you can zoom in/out the scene. Progress bar to indicate the rendering process. Elapsed time status bar display.

### Pictures

Basic Three Spheres - Camera location change (Samples per pixel = 50, Max depth = 25)

<img src="https://user-images.githubusercontent.com/46170128/109387683-e080cc00-78d0-11eb-99d0-fcfe4c05bc7c.png" width="300" height="191">

Showing Different Materials (Samples per pixel = 500, Max depth = 50)

<img src="https://user-images.githubusercontent.com/46170128/109387697-f2fb0580-78d0-11eb-814d-92fb9b3f14ff.png" width="300" height="191">

The Classic Big Scene Part 1 (Samples per pixel = 500, Max depth = 50)

<img src="https://user-images.githubusercontent.com/46170128/109387700-f55d5f80-78d0-11eb-8ee3-e2cd04b52860.png" width="300" height="191">

The Classic Big Scene Part 2 (Samples per pixel = 500, Max depth = 50)

<img src="https://user-images.githubusercontent.com/46170128/109387701-f68e8c80-78d0-11eb-9971-cdd9ef4cbb3c.png" width="300" height="191">

[web1]:            https://raytracing.github.io/books/RayTracingInOneWeekend.html
