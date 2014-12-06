/******************************************************************/
/*         Main raytracer file                                    */
/*                                                                */
/* Group Members: Nelson Cheng                                    */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "lowlevel.h"
#include "raytrace.h"

/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void traceRay(ray*,color*);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**);

/* local data */

/* the scene: so far, just one sphere */
objectNode* objectList;
lightNode* lightList;

/* the viewing parameters: */
point* viewpoint;
GLfloat pnear;  /* distance from viewpoint to image plane */
GLfloat fovx;  /* x-angle of view frustum */
int width = 800;     /* width of window in pixels */
int height = 600;    /* height of window in pixels */

int main (int argc, char** argv) {
  int win;

  glutInit(&argc,argv);
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  win = glutCreateWindow("raytrace");
  glutSetWindow(win);
  init(width,height);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void init(int w, int h) {

  /* OpenGL setup */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 2.0, 0.0, 2.0, -2.0, 2.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);  

  /* low-level graphics setup */
  initCanvas(w,h);

  /* raytracer setup */
  initCamera(w,h);
  initScene();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawScene();  /* draws the picture in the canvas */
  flushCanvas();  /* draw the canvas to the OpenGL window */
  glFlush();
}

void initScene () {
	objectList = new objectNode;
	objectList->next = NULL;
	// R, G , B, Amb, Diff, Spec, NS, Reflect, Refract
	objectNode* tempObject = new objectNode;
	tempObject->next = objectList->next;
	sphere* s = makeSphere(-0.5,0.0,-2.5,0.1);
	object* o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
  
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	cylinder* c = new cylinder(-1.0,0.0,-2.5,0.0,0.0,0.0,0.085,0.5,1.0);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.1,0.1,0.1,0.3,0.8,0.7,200.0,0.2,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(1.0,0.0,-2.5,0.0,0.0,0.0,0.085,0.5,1.0);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.1,0.1,0.1,0.3,0.8,0.7,200.0,0.2,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;

	tempObject = new objectNode;
	tempObject->next = objectList->next;
	cone* con = new cone(-0.45,0.0,-4.2,0.0,0.0,0.0,0.8,0.0,1.0);
	o = new object(NULL,NULL,NULL,con,makeMaterial(0.3,0.6,0.5,0.3,0.8,1.0,100.0,0.2,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(-0.47,0.0,-1.1,0.0,0.0,0.0,0.025,0.1,1.0);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.11,0.21,0.82,0.3,0.8,0.7,200.0,0.2,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	con = new cone(0.65,0.15,-2.0,0.0,0.0,0.0,0.15,1.0,	1.0);
	o = new object(NULL,NULL,NULL,con,makeMaterial(1.0,0.2,0.7,0.3,0.8,1.0,100.0,0.2,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(0.65,0.15,-2.0,0.15);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.64,0.15,0.59,0.3,0.8,1.0,100.0,0.2,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(-0.3,0.0,-2.5,0.1);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(-0.3,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(-0.1,0.0,-2.5,0.1);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(-0.1,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
		tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(0.1,0.0,-2.5,0.1);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(0.1,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;

	tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(0.3,0.0,-2.5,0.1);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(0.3,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	s = makeSphere(0.5,0.0,-2.5,0.1);
	o = new object(s,NULL,NULL,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0)); 
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(0.5,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;

	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(-0.5,1.0,-2.5,0.0,0.0,0.0,0.002,0.5,0.1);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	c = new cylinder(0.0,-7.0,-3.9,1.0,1.0,5.5,0.8,0.5,0.4);
	o = new object(NULL,NULL,c,NULL,makeMaterial(0.2,0.2,0.2,0.0,0.0,1.0,100.0,1.0,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	plane* p = new plane(0.0,-0.35,-1.0,0.0,1.0,0.0,1);
	o = new object(NULL,p,NULL,NULL,makeMaterial(0.66,0.72,0.12,0.3,1.0,1.0,50.0,0.8,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
  	
	tempObject = new objectNode;
	tempObject->next = objectList->next;
	p = new plane(0.0,0.0,-5.0,1.0,0.0,1.0,0);
	o = new object(NULL,p,NULL,NULL,makeMaterial(0.85,0.12,0.26,0.0,1.0,1.0,50.0,0.5,0.0));
	tempObject->thisObject = o;
	objectList->next = tempObject;
	
	lightList = new lightNode;
	lightList->next = NULL;

	lightNode* tempLight = new lightNode(lightList->next,
										0.0,0.0,-2.0,
										0.7,0.7,0.7);
	lightList->next = tempLight;
  
	tempLight = new lightNode(lightList->next,
						     1.0,2.0,0.0,
							 0.6,0.6,0.6);
	lightList->next = tempLight;
}

void initCamera (int w, int h) {
  viewpoint = makePoint(0.0,0.0,0.0);
  pnear = 1.0;
  fovx = PI/3.5;
}

void drawScene () {
  int i,j;
  GLfloat imageWidth;
  /* declare data structures on stack to avoid dynamic allocation */
  point worldPix;  /* current pixel in world coordinates */
  point direction; 
  point worldPixAnti;
  ray r;
  color c;

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;
  worldPixAnti.w = 1.0;
  worldPixAnti.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;

  imageWidth = 2*pnear*tan(fovx/2);

  /* trace a ray for every pixel */
  for (i=0; i<width; i++) {
    for (j=0; j<height; j++) {
      /* find position of pixel in world coordinates */
      /* y position = (pixel height/middle) scaled to world coords */ 
      worldPix.y = (j-(height/2))*imageWidth/width;
      /* x position = (pixel width/middle) scaled to world coords */ 
      worldPix.x = (i-(width/2))*imageWidth/width;

      /* find direction */
      /* note: direction vector is NOT NORMALIZED */
      calculateDirection(viewpoint,&worldPix,&direction);
		normalize(&direction);
      /* trace the ray! */
      c = traceRay(&r,5);
	  /* anti-alias */
	  color antiColor[4];
		// Up
		worldPixAnti.y = worldPix.y + .001;
		worldPixAnti.x = worldPix.x;
		calculateDirection(viewpoint,&worldPixAnti,&direction);
		normalize(&direction);
		antiColor[0] = traceRay(&r,5);
		// Down
		worldPixAnti.y = worldPix.y - .001;
		worldPixAnti.x = worldPix.x;
		calculateDirection(viewpoint,&worldPixAnti,&direction);
		normalize(&direction);
		antiColor[1] = traceRay(&r,5);
		// Left
		worldPixAnti.y = worldPix.y;
		worldPixAnti.x = worldPix.x - .001;
		calculateDirection(viewpoint,&worldPixAnti,&direction);
		normalize(&direction);
		antiColor[2] = traceRay(&r,5);
		// Right
		worldPixAnti.y = worldPix.y;
		worldPixAnti.x = worldPix.x + .001;
		calculateDirection(viewpoint,&worldPixAnti,&direction);
		normalize(&direction);
		antiColor[3] = traceRay(&r,5);
		c.r = (c.r + antiColor[0].r + antiColor[1].r + antiColor[2].r + antiColor[3].r) / 5.0;
		c.g = (c.g + antiColor[0].g + antiColor[1].g + antiColor[2].g + antiColor[3].g) / 5.0;
		c.b = (c.b + antiColor[0].b + antiColor[1].b + antiColor[2].b + antiColor[3].b) / 5.0;
      /* write the pixel */
      drawPixel(i,j,c.r,c.g,c.b);
    }
  }
}

color traceRay(ray* r, int d) {
	point p;  /* first intersection point */
	vector n;
	material* m;
	color c;
	double t;
	c.r = 0;
	c.g = 0;
	c.b = 0;
	if(d >= 0){
		p.w = 0.0;  /* inialize to "no intersection" */
		firstHit(r,&p,&n,&m,&t);

		if (p.w != 0.0) {
			shade(&p,&n,m,r->dir,&c,lightList,d);  /* do the lighting calculations */
		} else {             /* nothing was hit */
			c.r = 0.0;
			c.g = 0.0;
			c.b = 0.0;
		}
	}	
	return c;
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m, double* overallT) {
	double t = 0;     /* parameter value at first hit */
	*overallT = -1.0;
	int hit = FALSE;
	int newHit = FALSE;
	objectNode* otherObject;
	otherObject = objectList->next;
	while(otherObject != NULL){
		switch(otherObject->thisObject->objectType){
			case SPHERE:
				newHit = raySphereIntersect(r,otherObject->thisObject->theSphere,&t);
			break;
			case PLANE:
				newHit = rayPlaneIntersect(r,otherObject->thisObject->thePlane,&t);
			break;
			case CONE:
				newHit = rayConeIntersect(r,otherObject->thisObject->theCone,&t);
			break;
			case CYLINDER:
				newHit = rayCylinderIntersect(r,otherObject->thisObject->theCylinder,&t);
		}
		if (newHit) {
			if(t < *overallT || *overallT == -1.0){						
				findPointOnRay(r,t,p);
				switch(otherObject->thisObject->objectType){
					case SPHERE:
						findSphereNormal(otherObject->thisObject->theSphere,p,n);
					break;
					case PLANE:
						findPlaneNormal(otherObject->thisObject->thePlane,p,n);
					break;
					case CONE:
						findConeNormal(otherObject->thisObject->theCone,p,n);
					break;
					case CYLINDER:
						findCylinderNormal(otherObject->thisObject->theCylinder,p,n);
				}
				if(otherObject->thisObject->objectType == PLANE && otherObject->thisObject->thePlane->texture == TRUE){
					GLfloat u = sqrt(p->x*p->x + p->y*p->y);
					GLfloat v = sqrt(p->y*p->y + p->z*p->z);
					int uInt = (int)(u*100.0);
					int vInt = (int)(v*100.0);
					uInt %= 100;
					vInt %= 100;
					if(p->x < 0.0 && p->y < 0.0)
						uInt = 100 - uInt;
					if(p->y < 0.0 && p->z < 0.0)			
						vInt = 100 - vInt;
					if((uInt >= 0 && uInt < 50 && vInt >=0 && vInt < 50) || (uInt >= 50 && uInt < 100 && vInt >= 50 && vInt < 100)){
						if(otherObject->thisObject->thePlane->tileColor == NULL){
							otherObject->thisObject->thePlane->tileColor = new color;
							otherObject->thisObject->thePlane->tileColor->r = otherObject->thisObject->m->r;	
							otherObject->thisObject->thePlane->tileColor->g = otherObject->thisObject->m->g;
							otherObject->thisObject->thePlane->tileColor->b = otherObject->thisObject->m->b;
						}
						otherObject->thisObject->m->r = 0.0;
						otherObject->thisObject->m->g = 0.0;
						otherObject->thisObject->m->b = 0.0;
					}
					else{
						if(otherObject->thisObject->thePlane->tileColor != NULL){
							otherObject->thisObject->m->r = otherObject->thisObject->thePlane->tileColor->r;
							otherObject->thisObject->m->g = otherObject->thisObject->thePlane->tileColor->g;
							otherObject->thisObject->m->b = otherObject->thisObject->thePlane->tileColor->b;
						}
					}
				}
				*m = otherObject->thisObject->m;
				newHit = FALSE;
				*overallT = t;
			}
			hit = TRUE;
		} 
		otherObject = otherObject->next;
	}
	
	if(!hit) p->w = 0.0;
}