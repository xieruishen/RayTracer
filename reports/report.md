# ThinkRayTracer: C Based Ray Tracing Algorithm
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

#### UML Diagram
Our implementation is a modification of the code available on this website: https://www.purplealienplanet.com/node/20
![UML](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/UML.jpg)

#### Sphere

![Sphere2D](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/sphere2d.jpg)


To determine if a ray will intersect a sphere or not we try to find if there exists a real value t where t is the magnitude of the line segment PA. The origin of the coordinate system is the center of the sphere. We define the unit vector of PA as d. The magnitude of OA is the radius of the circle r. To determine if there is a real value for t, we can solve the following equation:

                        || P + dt || = r

The algorithm can be simplified by computing if the determinant of the equation is greater than zero of not.
```
float A = vectorDot(&r->dir, &r->dir);
vector dist = vectorSub(&r->start, &s->pos);
float B = 2 * vectorDot(&r->dir, &dist);
float C = vectorDot(&dist, &dist) - (s->radius * s->radius);
float discr = B * B - 4 * A * C;

if(discr < 0)
  return false;
else
  return true;

```

#### Rectangle
We used the slab method to calculate the intersection of a ray with the cube. In the slab method, we flatten out the cube and check if the ray falls within the bounding lines as shown in the diagram below.

![Rectangle](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/Rectangle.jpg)

To determine if it’s a hit or a miss, first, we calculate t1 and t2 using only the x coordinates. Let the minimum of t1 and t2 be tnear and the maximum be tfar. We can then calculate t1 and t2 using the y coordinates, but tnear is only updated if the minimum of t1 and t2 is greater than the previous tnear and tfar is only updated if the maximum of t1 and t2 is lesser than the previous tfar. After these two rounds of calculations are done if tnear > tfar then we know that the ray does not intersect with the cube.

```
if (xd==0 && (xo<minCubeX || xo>maxCubeX))
{
  return false;
}
else{
  float t1 = (minCubeX - xo)/xd;
  float t2 = (maxCubeX - xo)/xd;

  tNear = max(tNear, min(t1, t2));
  tFar = min(tFar, max(t1, t2));
}

// Same algorithm applies to y-axis

return tFar >= tNear;
```


#### General System Architecture
In order to visualize the ray interaction with the object, a for loop is implemented to iterate over every pixel of the screen so that we project eye array from every single pixel of an image. Different intensities of light lit points depending on how much light reaches the point. If light reaches a point, the intersection between the eye-ray, ray shoot through the screen, and the point is also calculated to determine whether the point is in the view. Furthermore, if the light reflects of from the objects we continue the intersect ray calculation until the intensity of the light dims.

```
for(HEIGHT){
  for(WIDTH){
      do for every ray:
         - Find closest ray/sphere intersection:
           * Iterate over every sphere

         - Check if we reach a lightsource from this point
           * Iterate over every lightsource
           * Find right color

         - Either go with reflected ray or go to next pixel
  }
}
```

### Lambertian Reflectance
To make a more realistic scene, we not only need to compute whether the eye ray intersect with any object in scene but also determine how to illuminate the object based on its material. Material consists of color diffusion and reflectivity (0 - 100%) as its attributes. For each object we put in the scene, we assigned it both a position and material. The color diffusion property indicates the color the object will reflect when light shines it. For each RGB value, we scaled the 0-255 range to be between 0-1 since it is much more intuitive to specify a percentage. The reflectivity of an object is used to determine whether
Reflectivity is used to determine how 'shiny' an object is and will act as a mirror for other objects.

Once we determined whether a specific pixel should be illuminated or not, we need to figure out the intensity of the pixel


```
- For each lightsource that can be reached:
     * Calculate Lambert dot product with material reflection
     * Calculate each colour component, consisting of:
- Lambert dot product result
- Per colour intensity of the incoming light
```

### Results
![cube_three](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/cube_three.jpg)
![cube_dimmed](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/cube_dimmed.jpg)
![sphere_reflection](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/sphere_reflection.jpg)


### Reflection

### Trello
https://trello.com/b/mrNEzxu5/thinkraytracer
