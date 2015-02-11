raytrace-et
===========

Real-time foveated raytracer using an EyeX controller with the EyeX SDK to render parts of a scene in higher resolution; depending on where the observer is looking by tracking his/her gaze.

When developing a real-time raytracer, I ran into issues with my older video card no longer keeping up to speed.
I optimized the raytracing to make use of foveated graphics by only rendering the part of the framebuffer (where the observer is looking) at high-resolution, thus only having to compute a fraction of the rays and taking most of the load off my graphics card.

For the most part implemented in C++ using DirectCompute.

Click [here](https://github.com/CaterHatterPillar/dv2519/releases/download/v1.1/fov_rt_et.pdf) to read more about it!
