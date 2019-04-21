/* A simple ray tracer */

#include <stdio.h>
#include <stdbool.h> /* Needed for boolean datatype */

/* Width and height of out image */
#define WIDTH  800
#define HEIGHT 600

/*max and min values needed for the intersectRayCube() function*/
#define INT_MIN -2147000000
#define INT_MAX 2147000000


/* The vector structure */
typedef struct{
      float x,y,z;
}vector;

/* The sphere */
typedef struct{
        vector pos;
        float  radius;
}sphere;

/*The cube*/
typedef struct{
        float x1;
        float x2;
        float y1;
        float y2;
        float z1;
        float z2;
}cube;

/* The ray */
typedef struct{
        vector start;
        vector dir;
}ray;

/* Subtract two vectors and return the resulting vector */
vector vectorSub(vector *v1, vector *v2){
	vector result = {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
	return result;
}

/* Multiply two vectors and return the resulting scalar (dot product) */
float vectorDot(vector *v1, vector *v2){
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

/*min and max are two helper functions needed for the intersectRayCube() function*/

float min(float x, float y)
{
  if (x<y){return x;}
  return y;
}
float max(float x, float y)
{
  if (x>y){return x;}
  return y;
}

/*
The intersect Ray Cube function works by using the slab method.
In the slab method, we flatten out the cube and check if the ray passes through the bounding lines.
*/
bool intersectRayCube(ray *r, cube *c)
{
  int tNear = INT_MIN;
  int tFar = INT_MAX;

  float xd = r->dir.x;
  float xo = r->start.x;
  float yd = r->dir.y;
  float yo = r->start.y;
  float zd = r->dir.z;
  float zo = r->start.z;

  float minCubeX = min(c->x1,c->x2);
  float maxCubeX = max(c->x1,c->x2);
  float minCubeY = min(c->y1,c->y2);
  float maxCubeY = max(c->y1,c->x2);
  float minCubeZ = min(c->z1,c->z2);
  float maxCubeZ = max(c->z1,c->z2);

/*if the ray is parallel to the x axis and not inbetween the min and max x bounds of the cube then return false*/
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

/*if the ray is parallel to the y axis and not inbetween the min and max y bounds of the cube then return false*/
  if (yd==0 && (yo<minCubeY || yo>maxCubeY))
  {
    return false;
  }
  else{
    float t1 = (minCubeY - yo)/yd;
    float t2 = (maxCubeY - yo)/yd;

    tNear = max(tNear, min(t1, t2));
    tFar = min(tFar, max(t1, t2));

  }

  /*if the ray is parallel to the z axis and not inbetween the min and max z bounds of the cube then return false*/
    if (zd==0 && (zo<minCubeZ || zo>maxCubeZ))
    {
      return false;
    }
    else{
      float t1 = (minCubeZ - zo)/zd;
      float t2 = (maxCubeZ - zo)/zd;

      tNear = max(tNear, min(t1, t2));
      tFar = min(tFar, max(t1, t2));

    }

//if tFar is greater than tNear then we know that the ray does not pass through the cube.
  return tFar >= tNear;
}


bool intersectRaySphere(ray *r, sphere *s, float *t){

	bool retval = false;

	/* A = d.d, the vector dot product of the direction */
	float A = vectorDot(&r->dir, &r->dir);

	/* We need a vector representing the distance between the start of
	 * the ray and the position of the circle.
	 * This is the term (p0 - c)
	 */
	vector dist = vectorSub(&r->start, &s->pos);

	/* 2d.(p0 - c) */
	float B = 2 * vectorDot(&r->dir, &dist);

	/* (p0 - c).(p0 - c) - r^2 */
	float C = vectorDot(&dist, &dist) - (s->radius * s->radius);

	/* Solving the discriminant */
	float discr = B * B - 4 * A * C;

	/* If the discriminant is negative, there are no real roots.
	 * Return false in that case as the ray misses the sphere.
	 * Return true in all other cases (can be one or two intersections)
	 * t represents the distance between the start of the ray and
	 * the point on the sphere where it intersects.
	 */
	if(discr < 0)
		retval = false;
	else{
		float sqrtdiscr = sqrtf(discr);
		float t0 = (-B + sqrtdiscr)/(2);
		float t1 = (-B - sqrtdiscr)/(2);

		/* We want the closest one */
		if(t0 > t1)
			t0 = t1;

		/* Verify t1 larger than 0 and less than the original t */
		if((t0 > 0.001f) && (t0 < *t)){
			*t = t0;
			retval = true;
		}else
			retval = false;
	}

return retval;
}

/* Output data as PPM file */
void saveppm(char *filename, unsigned char *img, int width, int height){
	/* FILE pointer */
	FILE *f;

	/* Open file for writing */
	f = fopen(filename, "wb");

	/* PPM header info, including the size of the image */
	fprintf(f, "P6 %d %d %d\n", width, height, 255);

	/* Write the image data to the file - remember 3 byte per pixel */
	fwrite(img, 3, width*height, f);

	/* Make sure you close the file */
	fclose(f);
}

int main(int argc, char *argv[]){

	/* Image data */
	unsigned char img[3*WIDTH*HEIGHT];

	/* Our ray and a sphere */
	sphere s;
	ray r;
  cube c;

	/* x, y for screen 'resolution' */
	int x,y;

	/* Intersect ray/sphere or not */
	bool hitcube;
  bool hitsphere;

	/* Position the sphere */
	s.pos.x = 200;
	s.pos.y = 200;
	s.pos.z = 200;

  c.x1 = 300;
  c.y1 = 300;
  c.x2 = 600;
  c.y2 = 600;
  c.z1 = 100;
  c.z2 = 200;

	/* Sphere radius */
	s.radius = 100;

	/* Direction of the ray */
	r.dir.x = 0;
	r.dir.y = 0;
	r.dir.z = 1;

	/* Start position of the ray, z coordinate */
	r.start.z = 0;

	/* Iterate over every pixel of our screen */
	for(y=0;y<HEIGHT;y++){
		/* Set the y-coordinate of the start position of the ray */
		r.start.y = y;
		for(x=0;x<WIDTH;x++){
			/* Set the x-coordinate of the start position of the ray */
			r.start.x = x;

			/* Check if the ray intersects with the shpere */
			hitcube = intersectRayCube(&r, &c);
      hitsphere = intersectRaySphere(&r,&s);
			if(hitcube || hitsphere){
        if (hitcube){
				img[(x + y*WIDTH)*3 + 0] = 0;
				img[(x + y*WIDTH)*3 + 1] = 255;
				img[(x + y*WIDTH)*3 + 2] = 255;
        }else{
          img[(x + y*WIDTH)*3 + 0] = 0;
  				img[(x + y*WIDTH)*3 + 1] = 255;
  				img[(x + y*WIDTH)*3 + 2] = 0;
			}}else{
				img[(x + y*WIDTH)*3 + 0] = 0;
				img[(x + y*WIDTH)*3 + 1] = 0;
				img[(x + y*WIDTH)*3 + 2] = 0;
			}

		}
	}


	saveppm("image.ppm", img, WIDTH, HEIGHT);

	return 0;
}
