# ThinkRayTracer: C Based Ray Tracing Algorithm
#### Hyegi Bang, Sherrie Shen, Athmika Senthikumar

## Project Vision
The goal of the project is to implement a ray tracing program. The minimum viable product is a ray tracing program that renders a properly lighted image given a scene with objects in 2d and 3d. The stretch goal would be having the ray tracing program to render a properly lighted image given a scene with more diverse objects such as cubes and prisms.

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

#### General System Architecture
In order to visualize the ray interaction with the object, a for loop is implemented to iterate over every pixel of the screen so that we project eye array from every single pixel of an image. Different intensities of light lit points depending on how much light reaches the point. If light reaches a point, the intersection between the eye-ray, ray shoot through the screen, and the point is also calculated to determine whether the point is in the view. Furthermore, if the light reflects of from the objects we continue the intersect ray calculation until the intensity of the light dims.

![diagram](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/diagram.png)

```
for(HEIGHT){
  for(WIDTH){
      do for every ray:
         - Find closest ray/sphere intersection:
           * Iterate over every sphere

         - Check if we reach a light source from this point
           * Iterate over every light source
           * Find right color

         - Either go with reflected ray or go to next pixel. If go with reflected ray, update intensity of light source with reflectivity of object.
  }
}
```


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
  retval = false;
else{
  float sqrtdiscr = sqrtf(discr);
  float t0 = (-B + sqrtdiscr)/(2);
  float t1 = (-B - sqrtdiscr)/(2);

  if(t0 > t1)
    t0 = t1;

  if((t0 > 0.001f) && (t0 < *t)){
    *t = t0;
    retval = true;
  }else
    retval = false;
}

return retval;
}

```
After calculating the discriminant, if the discriminant is negative, since there are no real roots,return false in that case as the ray misses the sphere. All the other cases, return true as it implies that one or two intersections exists. In the code above, t represents the distance between the start of the ray and the point on the incidentRayCamera   intersects. Putting conditionals allows us to get solution with smaller magnitude, implying that it is the point of intersection.   

#### Rectangle
We used the slab method to calculate the intersection of a ray with the cube. In the slab method, we flatten out the cube and check if the ray falls within the bounding lines as shown in the diagram below.

![Rectangle](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/Rectangle.jpg)

To determine if it’s a hit or a miss, first, we calculate t1 and t2 using only the x coordinates. Let the minimum of t1 and t2 be tnear and the maximum be tfar. We can then calculate t1 and t2 using the y coordinates, but tnear is only updated if the minimum of t1 and t2 is greater than the previous tnear and tfar is only updated if the maximum of t1 and t2 is lesser than the previous tfar. After these two rounds of calculations are done if tnear > tfar then we know that the ray does not intersect with the cube. Below is an example dealing with the the slab in x planes.

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

// Same algorithm applies to y planes and z planes and we find the maximum tNear minimum tFar across all slabs.

return tFar >= tNear;
```
If there is a valid intersection, we store the distance from the eye ray to the point of intersection which is just tNear.

### Lambertian Reflectance
To make a more realistic scene, we not only need to compute whether the eye ray intersect with any object in scene but also determine how to illuminate the object based on its material. Material consists of color diffusion and reflectivity (0 - 100%) as its attributes. For each object we put in the scene, we assigned it both a position and material. The color diffusion property indicates the color the object will reflect when light shines it. For each RGB value, we scaled the 0-255 range to be between 0-1 since it is much more intuitive to specify a percentage. The reflectivity of an object is used to determine how 'shiny' an object is and acts as a mirror for other objects. The material of the object is defined as follows:

```
typedef struct{
    colour diffuse;
    float reflection;
}material;
```

If the eye ray coming from a specific pixel on the image will hit an object, we need to figure out the intensity of the pixel. We applied Lambertian reflectance to model diffuse reflection. By Lambertian reflectance, the apparent brightness of a Lambertian surface to an observer is the same regardless of the observer's angle of view. This model allows us to compute the color and intensity of each pixel. The model is described as follows:
![equation](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/ID_equation.gif)

where I<sub>D</sub> represents the intensity of one color for that pixel, N is the unit normal vector of the surface at the point of intersection , L is the unit vector from point of intersection to the light source, C is the diffuse color of the sphere, I<sub>L</sub> is the intensity of the incoming light. The code is outlined below:

```
for(j=0; j < 3; j++){
  light currentLight = lights[j];
  vector dist = vectorSub(&currentLight.pos, &newStart);
  if(vectorDot(&n, &dist) <= 0.0f) continue;
  float t = sqrtf(vectorDot(&dist,&dist));
  if(t <= 0.0f) continue;

  ray lightRay;
  lightRay.start = newStart;
  lightRay.dir = vectorScale((1/t), &dist);

  /* Lambert diffusion */
  float lambert = vectorDot(&lightRay.dir, &n) * coef; /*coef is the scaling factor of the intensity. It equals to 1 if it is not reflected light*/

  red += lambert * currentLight.intensity.red * currentMat.diffuse.red;
  green += lambert * currentLight.intensity.green * currentMat.diffuse.green;
  blue += lambert * currentLight.intensity.blue * currentMat.diffuse.blue;
}
```

### Reflection of Ray
Another key aspect we takes into consider when updating the pixel value of the image is reflection of light of the illuminated object. We use the normal vector of the surface at the point of intersection to compute the reflected ray. We then repeat the same process by treating this reflected ray as the new eye ray. To prevent the algorithm from infinitely being stuck in the loop of reflection rays, we keep track of the number of times this iteration takes place. In our implementation, we set the upper bound to be 14 iterations. We also terminate the iteration if the scaling factor of the light intensity is zero meaning no light is going to be reflected.

### Results
![cube_three](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/cube_three.jpg)
Three cubes generated with three light source. All cubes are visible.
![cube_dimmed](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/cube_dimmed.jpg)
Three cubes generated with two light source: one shining on right portion of the screen and the other light is located far in z-axis from the screen. All cubes are visible. However, the color of far-left cube is dimmed as the light's intensity when light array intersects the point is low.  

![sphere_reflection](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/sphere_reflection.jpg)
Five cubes generated with four light source. All spheres are visible, reflecting on each other.


### Reflection

As a team and an individual, we were able to meet all of our learning goals: to gain more confidence coding in C and deeper understanding of graphics. It was interesting to learn about physics in the field of computer graphics and perform implementation. However, we wished to have understood Lambertian Reflection more beforehand as implementing them. Unlike spheres, since cubes do not have gradients on her surface it was harder to calculate the diffuse reflection. We were only able to get general outlook of lights and reflection.

![working_cube](https://github.com/xieruishen/ThinkRayTracer/blob/master/reports/image/working_cube.jpg)
For future, we would like to drive formula that would provide more realistic light reflections on the cube. Overall, the project was good learning experience as it provided reasonable introduction to performing computer graphics in c.


### Trello
https://trello.com/b/mrNEzxu5/thinkraytracer
