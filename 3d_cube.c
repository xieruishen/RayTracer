/* A simple ray tracer */

#include <stdio.h>
#include <stdbool.h> /* Needed for boolean datatype */
#include <math.h>
#include <float.h>

/* Width and height of out image */
#define WIDTH  1000
#define HEIGHT 1000

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
        int material;
}sphere;

/*The cube*/
typedef struct{
  vector pos;
  float length;
  float width;
  float height;
  int material;
}cube;

/* Colour */
typedef struct{
	float red, green, blue;
}colour;


/* Material Definition */
typedef struct{
	colour diffuse;
	float reflection;
}material;


/* The ray */
typedef struct{
        vector start;
        vector dir;
}ray;

/* Lightsource definition */
typedef struct{
	vector pos;
	colour intensity;
}light;

/* Subtract two vectors and return the resulting vector */
vector vectorSub(vector *v1, vector *v2){
	vector result = {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
	return result;
}

/* Multiply two vectors and return the resulting scalar (dot product) */
float vectorDot(vector *v1, vector *v2){
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

/* Calculate Vector x Scalar and return resulting Vector*/
vector vectorScale(float c, vector *v){
        vector result = {v->x * c, v->y * c, v->z * c };
        return result;
}

/* Add two vectors and return the resulting vector */
vector vectorAdd(vector *v1, vector *v2){
        vector result = {v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
        return result;
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
bool intersectRayCube(ray *r, cube *c, float *t)
{

  float x1 = c->pos.x - 0.5*c->length;
  float x2 = c->pos.x + 0.5*c->length;
  float y1 = c->pos.y - 0.5*c->width;
  float y2 = c->pos.y + 0.5*c->width;
  float z1 = c->pos.z - 0.5*c->height;
  float z2 = c->pos.z + 0.5*c->height;

  bool retval = false;

  float tNear = (float)INT_MIN;
  float tFar = (float)INT_MAX;

  float xd = r->dir.x;
  float xo = r->start.x;
  float yd = r->dir.y;
  float yo = r->start.y;
  float zd = r->dir.z;
  float zo = r->start.z;

  float minCubeX = min(x1,x2);
  float maxCubeX = max(x1,x2);
  float minCubeY = min(y1,y2);
  float maxCubeY = max(y1,x2);
  float minCubeZ = min(z1,z2);
  float maxCubeZ = max(z1,x2);


/*if the ray is parallel to the x axis and not inbetween the min and max x bounds of the cube then return false*/
  if (xd==0 && (xo<minCubeX || xo>maxCubeX))
  {
    return retval;
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
    return retval;
  }
  else{
    float t1 = (minCubeY - yo)/yd;
    float t2 = (maxCubeY - yo)/yd;

    tNear = max(tNear, min(t1, t2));
    tFar = min(tFar, max(t1, t2));

  }

/*if the ray is parallel to the z axis and not inbetween the min and max y bounds of the cube then return false*/
  if (zd==0 && (zo<minCubeZ || zo>maxCubeZ))
  {
    return retval;
  }
  else{
    float t1 = (minCubeZ - zo)/zd;
    float t2 = (maxCubeZ - zo)/zd;

    tNear = max(tNear, min(t1, t2));
    tFar = min(tFar, max(t1, t2));
  }

//if tFar is greater than tNear then we know that the ray does not pass through the cube.
  if(tFar>= tNear){
    // printf("tnear: %f\n", tNear);
    // printf("tfar: %f\n", tFar);
    *t = tNear;
    retval = true;
  }
  else{
    retval = false;
  }

  return retval;
}


/* Check if the ray and sphere intersect */
bool intersectRaySphere(ray *r, sphere *s){

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
	 */
	if(discr < 0)
		return false;
	else
		return true;
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

  material materials[3];

  // materials.diffuse.red = 1;
  // materials.diffuse.green = 0;
  // materials.diffuse.blue = 0;
  // materials.reflection = 0.2;

  materials[0].diffuse.red = 1;
  materials[0].diffuse.green = 0;
  materials[0].diffuse.blue = 0;
  materials[0].reflection = 0.2;

  materials[1].diffuse.red = 0;
  materials[1].diffuse.green = 1;
  materials[1].diffuse.blue = 0;
  materials[1].reflection = 0.5;

  materials[2].diffuse.red = 0;
	materials[2].diffuse.green = 0;
	materials[2].diffuse.blue = 1;
	materials[2].reflection = 0.9;

	// s.pos.x = 200;
	// s.pos.y = 300;
	// s.pos.z = 0;
	// s.radius = 100;
	// s.material = 0;

  cube cube[3];

  // cube.pos.x = 450;
  // cube.pos.y = 450;
  // cube.pos.z = 150;
  // cube.length = 300;
  // cube.width = 300;
  // cube.height = 100;
  // cube.material = 0;
  //
  cube[0].pos.x = 450;
  cube[0].pos.y = 450;
  cube[0].pos.z = 50;
  cube[0].length = 300;
  cube[0].width = 300;
  cube[0].height = 100;
  cube[0].material = 0;

  cube[1].pos.x = 150;
  cube[1].pos.y = 150;
  cube[1].pos.z = 350;
  cube[1].length = 100;
  cube[1].width = 100;
  cube[1].height = 100;
  cube[1].material = 1;

  cube[2].pos.x = 0;
  cube[2].pos.y = 0;
  cube[2].pos.z = 350;
  cube[2].length = 50;
  cube[2].width = 50;
  cube[2].height = 50;
  cube[2].material = 2;


  // c.pos.x = 450;
  // c.pos.y = 450;
  // c.pos.z = 150;
  // c.length = 300;
  // c.width = 300;
  // c.height = 100;
  // c.material = 1

  light lights[3];

  lights[0].pos.x = 0;
  lights[0].pos.y = 240;
  lights[0].pos.z = -100;
  lights[0].intensity.red = 1;
  lights[0].intensity.green = 1;
  lights[0].intensity.blue = 1;
  //
  lights[1].pos.x = 3200;
  lights[1].pos.y = 3000;
  lights[1].pos.z = -1000;
  lights[1].intensity.red = 0.6;
  lights[1].intensity.green = 0.7;
  lights[1].intensity.blue = 1;

  lights[2].pos.x = 600;
  lights[2].pos.y = 0;
  lights[2].pos.z = -100;
  lights[2].intensity.red = 0.3;
  lights[2].intensity.green = 0.5;
  lights[2].intensity.blue = 1;

	/* Direction of the ray */
	r.dir.x = 0;
	r.dir.y = 0;
	r.dir.z = 1;

	/* Start position of the ray, z coordinate */
	r.start.z = 0;

	for(y=0;y<HEIGHT;y++){
		for(x=0;x<WIDTH;x++){

			float red = 0;
			float green = 0;
			float blue = 0;

			int level = 0;
			float coef = 1.0;

			r.start.x = x;
			r.start.y = y;
			r.start.z = -2000;

			r.dir.x = 0;
			r.dir.y = 0;
			r.dir.z = 1;

      do{
				/* Find closest intersection */
				float t = 20000.0f;
				int currentCube = -1;

				unsigned int i;
				for(i = 0; i < 3; i++){
					if(intersectRayCube(&r, &cube[i], &t)){
            // printf("%d\n", i);
						currentCube = i;
            // printf("%d\n", i);
            // printf("%d\n", currentCube);
            // printf("distance: %f\n", t);
				}
      }
				if(currentCube == -1) {
            // printf("true");
            break;
        }else{
          // printf("%d\n", currentCube);
        }


				vector scaled = vectorScale(t, &r.dir);
				vector newStart = vectorAdd(&r.start, &scaled);

				/* Find the normal for this new vector at the point of intersection */
				vector n = vectorSub(&newStart, &cube[currentCube].pos);
				float temp = vectorDot(&n, &n);
				if(temp == 0) break;

				temp = 1.0f / sqrtf(temp);
				n = vectorScale(temp, &n);
        // printf("%f\n",n.x);

				/* Find the material to determine the colour */
				material currentMat = materials[cube[currentCube].material];

				/* Find the value of the light at this point */
				unsigned int j;
				for(j=0; j < 3; j++){
					light currentLight = lights[j];
					vector dist = vectorSub(&currentLight.pos, &newStart);
					if(vectorDot(&n, &dist) <= 0.0f) continue;
					float t = sqrtf(vectorDot(&dist,&dist));
          //printf("%lf\n",t );

					if(t <= 0.0f){
            continue;
          }

					ray lightRay;
					lightRay.start = newStart;
					lightRay.dir = vectorScale((1/t), &dist);

						/* Lambert diffusion */
						float lambert = vectorDot(&lightRay.dir, &n) * coef;
						red += lambert * currentLight.intensity.red * currentMat.diffuse.red * 100;
            // printf("%lf\n",red);
						green += lambert * currentLight.intensity.green * currentMat.diffuse.green * 40;
						blue += lambert * currentLight.intensity.blue * currentMat.diffuse.blue * 40;
					}

				/* Iterate over the reflection */
				coef *= currentMat.reflection;

				/* The reflected ray start and direction */
				r.start = newStart;
				float reflect = 2.0f * vectorDot(&r.dir, &n);
				vector tmp = vectorScale(reflect, &n);
				r.dir = vectorSub(&r.dir, &tmp);

				level++;

			}while((coef > 0.0f) && (level < 15));
			img[(x + y*WIDTH)*3 + 0] = (unsigned char)min(red*255.0f, 255.0f);
      if(red != 0){
        // printf("%lf\n", red);
        // printf("%d\n", img[(x + y*WIDTH)*3 + 0]);
      }
			img[(x + y*WIDTH)*3 + 1] = (unsigned char)min(green*255.0f, 255.0f);
			img[(x + y*WIDTH)*3 + 2] = (unsigned char)min(blue*255.0f, 255.0f);
		}
	}

	saveppm("image_cube.ppm", img, WIDTH, HEIGHT);

return 0;
}
