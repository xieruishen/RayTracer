# ThinkRayTracer
### Software Systems Project 2
#### Hyegi Bang, Sherrie Shen, Athmika Senthikumar

## Project Vision
The goal of the project is to implement a ray tracing program. The minimum viable product is a ray tracing program that renders a properly lighted image given a scene with only spheres in 2d and 3d. The stretch goal would be having the ray tracing program to render a properly lighted image given a scene with more diverse objects such as cubes and prisms.

### Learning Goal
Our learning goals for this project is to build up knowledge about graphics programming in C. Another goal is to learn the math behind ray tracing and be able to implement mathematical functions effectively using structs. We also want to improve our skills around writing well planned, well organized C code that is easy to read and modify.

### Project Workflow
Majority of the work was done through pair programming during our weekly meetings. We used "teletype” in atom, which allowed us to share the workspace with and collaborate on code in real time. Trello board contains a similar work environment. Most of the tasks were thought during the meeting and to prevent overlaps of tabs, one person became the scribe and added the cards. Therefore, the github commit history and trello board doesn’t fully represent the team’s contribution. However, all work were distributed and done equally within the teammates.


### Resources
To get started, we read the ray tracer tutorial on sphere from a ray tracer tutorial that covers the mathematics of ray tracing for sphere and provides example code: https://www.purplealienplanet.com/node/20
</br>

We also watched the video going over the maths of raytracing in 2D for a sphere: https://www.youtube.com/watch?v=hOeOuZGmeCM&t=892s
</br>

Other resources that provides examples for ray tracer program with more complex surface than spheres:
* [Example raytace program in C](https://www.purplealienplanet.com/node/20)
* [3D C/C++ tutorials - Ray tracing](http://www.3dcpptutorials.sk/index.php?id=16)
* [Introduction to Ray Tracing: a Simple Method for Creating 3D Images](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/ray-tracing-practical-example)
* [A small, simple path tracer written in C](https://github.com/VKoskiv/c-ray)

## Implementation
### Sphere and Rectangle in 2D

Our implementation is a modification of the code available on this website: https://www.purplealienplanet.com/node/20

#### Sphere

![Sphere2D](/image/sphere2d.jpg)
To determine if a ray will intersect a sphere or not we try to find if there exists a real value t where t is the magnitude of the line segment PA. The origin of the coordinate system is the center of the sphere. We define the unit vector of PA as d. The magnitude of OA is the radius of the circle r. To determine if there is a real value for t, we can solve the following equation:

|| P + dt || = r

The algorithm can be simplified by computing if the determinant of the equation is greater than zero of not.

#### Rectangle
### Sphere and Rectangle in 3D
#### Sphere
#### Rectangle

### Next Steps

### Project Reflection
