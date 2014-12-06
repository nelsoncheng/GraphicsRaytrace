#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

/******************************************************************/
/*         Raytracer declarations                                 */
/******************************************************************/


/* constants */
#define TRUE 1
#define FALSE 0
#define SPHERE 0
#define PLANE 1
#define CYLINDER 2
#define CONE 3

#define PI 3.14159265358979323846264338327

/* data structures */

typedef struct point {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} point;

/* a vector is just a point */
typedef point vector;

/* a ray is a start point and a direction */
typedef struct ray {
  point* start;
  vector* dir;
} ray;

void normalize(vector* v);

typedef struct material {
  /* color */
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* ambient reflectivity */
  GLfloat amb;
  /* Diffuse Light */
  GLfloat kd;
  /* Specular Highlights*/
  GLfloat kss;
  /* Specular Highlight ns*/
  GLfloat ns;
  /* Reflection */
  GLfloat ksr;
  /* Refracted Rays */
  GLfloat kr;
} material;

typedef struct color {
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* these should be between 0 and 1 */
} color;

struct sphere {
  point* c;  /* center */
  GLfloat r;  /* radius */
};

struct plane {
	point* c;
	vector* n;
	color* tileColor;
	int texture;
	plane(){}
	plane(GLfloat x, GLfloat y, GLfloat z, GLfloat xr, GLfloat yr,GLfloat zr, int daTexture){
		c = new point;
		c->x = x;
		c->y = y;
		c->z = z;
		c->w = 1;
		n = new vector;
		n->x = xr;
		n->y = yr;
		n->z = zr;
		n->w = 0;
		normalize(n);
		texture = daTexture;
		tileColor = NULL;
	}
};

struct cylinder {
	point* c;
	vector* n;
	GLfloat r;
	GLfloat uBound;
	GLfloat bBound;
	cylinder(){}
	cylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat xr, GLfloat yr, GLfloat zr, GLfloat daR, GLfloat daUBound, GLfloat daBBound){
		c = new point;
		c->x = x;
		c->y = y;
		c->z = z;
		c->w = 1;
		n = new vector;
		n->x = xr;
		n->y = yr;
		n->z = zr;
		n->w = 0;
		normalize(n);
		r = daR;
		uBound = daUBound;
		bBound = daBBound;
	}
};

struct cone {
	point* c;
	vector* n;
	GLfloat r;
	GLfloat uBound;
	GLfloat bBound;
	cone(){}
	cone(GLfloat x, GLfloat y, GLfloat z, GLfloat xr, GLfloat yr, GLfloat zr, GLfloat daR, GLfloat daUBound, GLfloat daBBound){
		c = new point;
		c->x = x;
		c->y = y;
		c->z = z;
		c->w = 1;
		n = new vector;
		n->x = xr;
		n->y = yr;
		n->z = zr;
		n->w = 0;
		normalize(n);
		r = daR;
		uBound = daUBound;
		bBound = daBBound;
	}
};

struct object{
	int objectType;
	sphere *theSphere;
	plane *thePlane;
	cylinder *theCylinder; 
	cone *theCone;
	material* m;
	object(){}
	object(sphere *daSphere, plane *daPlane, cylinder *daCylinder, cone *daCone, material* daM){
		if(daSphere != NULL){
			objectType = SPHERE;
			theSphere = daSphere;
		}
		if(daPlane != NULL){
			objectType = PLANE;
			thePlane = daPlane;
		}
		if(daCylinder != NULL){
			objectType = CYLINDER;
			theCylinder = daCylinder;
		}
		if(daCone != NULL){
			objectType = CONE;
			theCone = daCone;
		}
		m = daM;
	}
};

struct objectNode{
	object *thisObject;
    objectNode* next;
};

class light {
  public:
    point *p;
    color *c;
    light(){};
    light(GLfloat daX, GLfloat daY, GLfloat daZ, GLfloat daR, GLfloat daG, GLfloat daB){
		p = new point;
        p->x = daX;
		p->y = daY;
		p->z = daZ;
		p->w = 1.0;
		c = new color;
		c->r = daR;
		c->g = daG;
		c->b = daB;
	}
};

struct lightNode{
	light *theLight;
	lightNode* next;
	lightNode(){};
	lightNode(lightNode* daNext, GLfloat daX, GLfloat daY, GLfloat daZ, GLfloat daR, GLfloat daG, GLfloat daB){
		theLight = new light(daX, daY, daZ, daR, daG, daB);
		next = daNext;
	}
};

/* functions in raytrace.cpp */
//void traceRay(ray*, color*, int);
color traceRay(ray* r, int d);
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**,double*);
int inShadow(ray* r);
void keyHandler(unsigned char ch, int x, int y);
void switchCamera();
void drawText(char* s);
/* functions in geometry.cpp */
sphere* makeSphere(GLfloat, GLfloat, GLfloat, GLfloat);
point* makePoint(GLfloat, GLfloat, GLfloat);
point* copyPoint(point *);
void freePoint(point *);
void calculateDirection(point*,point*,point*);
void findPointOnRay(ray*,double,point*);

int raySphereIntersect(ray*,sphere*,double*);
void findSphereNormal(sphere*,point*,vector*);
int rayPlaneIntersect(ray*,plane*,double*);
void findPlaneNormal(plane*,point*,vector*);
int rayConeIntersect(ray*,cone*,double*);
void findConeNormal(cone*,point*,vector*);
int rayCylinderIntersect(ray*,cylinder*,double*);
void findCylinderNormal(cylinder*,point*,vector*);

GLfloat dot(const vector* r, const vector* l);
/* functions in light.cpp */
material* makeMaterial(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void shade(point*,vector*,material*,vector*,color*,lightNode*, int);

/* global variables */
extern int width;
extern int height;

#endif	/* _RAYTRACE_H_ */