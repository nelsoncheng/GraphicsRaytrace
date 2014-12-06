/******************************************************************/
/*         Geometry functions                                     */
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
#include "raytrace.h"

point* makePoint(GLfloat x, GLfloat y, GLfloat z) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));
  /* put stuff in it */
  p->x = x; p->y = y; p->z = z; 
  p->w = 1.0;
  return (p);
}

/* makes copy of point (or vector) */
point* copyPoint(point *p0) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));

  p->x = p0->x;
  p->y = p0->y;
  p->z = p0->z;
  p->w = p0->w;         /* copies over vector or point status */
  return (p);
}

/* unallocates a point */
void freePoint(point *p) {
  if (p != NULL) {
    free(p);
  }
}

/* vector from point p to point q is returned in v */
void calculateDirection(point* p, point* q, point* v) {
  v->x = q->x - p->x;
  v->y = q->y - p->y;
  v->z = q->z - p->z;
  /* a direction is a point at infinity */
  v->w = 0.0;
  normalize(v);
  /* NOTE: v is not unit length currently, but probably should be */
}

/* given a vector, sets its contents to unit length */
void normalize(vector* v) {
	GLfloat scaleFactor = sqrt((v->x*v->x)+(v->y*v->y)+(v->z*v->z));
	v->x = v->x / scaleFactor;
	v->y = v->y / scaleFactor;
	v->z = v->z / scaleFactor;
}

GLfloat dot(const vector* r, const vector* l){
	return r->x*l->x + r->y*l->y + r->z*l->z;
}

/* point on ray r parameterized by t is returned in p */
void findPointOnRay(ray* r,double t,point* p) {
  p->x = r->start->x + t * r->dir->x;
  p->y = r->start->y + t * r->dir->y;
  p->z = r->start->z + t * r->dir->z;
  p->w = 1.0;
}


/* SPHERES */

sphere* makeSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
  sphere* s;
  /* allocate memory */
  s = (sphere*) malloc(sizeof(sphere));

  /* put stuff in it */
  s->c = makePoint(x,y,z);   /* center */
  s->r = r;   /* radius */
  return(s);
}

/* returns TRUE if ray r hits sphere s, with parameter value in t */
int raySphereIntersect(ray* r,sphere* s,double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - s->c->x;
  p.y = r->start->y - s->c->y;
  p.z = r->start->z - s->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  +  v->y * v->y  +  v->z * v->z;
  b = 2*( v->x * p.x  +  v->y * p.y  +  v->z * p.z);
  c = p.x * p.x + p.y * p.y + p.z * p.z - s->r * s->r;

  D = b * b - 4 * a * c;
  
  if (D < 0) {  /* no intersection */
    return (FALSE);
  }
  else {
    D = sqrt(D);
    /* First check the root with the lower value of t: */
    /* this one, since D is positive */
    *t = (-b - D) / (2*a);
    /* ignore roots which are less than zero (behind viewpoint) */
    if (*t < 0) {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0) { return(FALSE); }
    else return(TRUE);
  }
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findSphereNormal(sphere* s, point* p, vector* n) {
  n->x = (p->x - s->c->x) / s->r;  
  n->y = (p->y - s->c->y) / s->r;
  n->z = (p->z - s->c->z) / s->r;
  n->w = 0.0;
  normalize(n);
}

int rayPlaneIntersect(ray* r,plane* p,double* t){
  point pt;   /* start of transformed ray */
  point* v;
  GLfloat A = p->n->x;
  GLfloat B = p->n->y;
  GLfloat C = p->n->z;
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  pt.x = r->start->x - p->c->x;
  pt.y = r->start->y - p->c->y;
  pt.z = r->start->z - p->c->z;
  v = r->dir; /* point to direction vector */
  *t = -(A*pt.x + B*pt.y + C*pt.z) / (A*v->x + B*v->y + C*v->z);

  if(*t > 0) return TRUE;
  else return FALSE;
}
void findPlaneNormal(plane* p ,point* pt,vector* n){
	n->x = p->n->x;
	n->y = p->n->y;
    n->z = p->n->z;
	n->w = 0.0;
}

int rayCylinderIntersect(ray* r,cylinder* theCylinder,double* t){
	point p;   /* start of transformed ray */
	double a,b,c;  /* coefficients of quadratic equation */
	double D;    /* discriminant */
	point* v;
  
	/* transform ray so that sphere center is at origin */
	/* don't use matrix, just translate! */
	p.x = r->start->x - theCylinder->c->x;
	p.y = r->start->y - theCylinder->c->y;
	p.z = r->start->z - theCylinder->c->z;
	v = r->dir; /* point to direction vector */

	GLfloat rs = theCylinder->r * theCylinder->r;
	a = (v->x * v->x)  +  (v->z * v->z) - rs;
	b = 2*((v->x * p.x)  +  (v->z * p.z) - rs);
	c = (p.x * p.x) + (p.z * p.z) - rs;

	D = b * b - 4 * a * c;
  
	if (D < 0) { 
		return FALSE;
	}
	else {
		D = sqrt(D);
		*t = (-b - D) / (2*a);
		if (*t < 0) {
			*t = (-b + D) / (2*a);
		}
		if (*t < 0) return FALSE; 
		else{ 
			GLfloat yVal = *t*v->y + r->start->y;
			if(yVal > theCylinder->uBound && theCylinder->uBound != -1.0)
				return FALSE;
			else{
				if(yVal < theCylinder->bBound && theCylinder->bBound != 1.0)
					return FALSE;
				else
					return TRUE;
			}
		}
	}
	
}
void findCylinderNormal(cylinder* v ,point* pt,vector* n){
	n->x = pt->x - v->c->x;  
	n->z = pt->z - v->c->z;
	n->y = 0.0;//(n->x*n->y)/(sqrt(n->x*n->x + n->y*n->y)); //pt->x/pt->y;
	n->w = 0.0;
	normalize(n);
}

int rayConeIntersect(ray* r,cone* theCone,double* t){
	point p;   /* start of transformed ray */
	double a,b,c;  /* coefficients of quadratic equation */
	double D;    /* discriminant */
	point* v;
  
	/* transform ray so that sphere center is at origin */
	/* don't use matrix, just translate! */
	p.x = r->start->x - theCone->c->x;
	p.y = r->start->y - theCone->c->y;
	p.z = r->start->z - theCone->c->z;
	v = r->dir; /* point to direction vector */

	GLfloat rs = theCone->r * theCone->r;
	a = (v->x * v->x)  +  (v->z * v->z) - (rs * v->y * v->y);
	b = 2*((v->x * p.x)  +  (v->z * p.z) - (rs *v->y * p.y));
	c = (p.x * p.x) + (p.z * p.z) - (rs * p.y * p.y);

	D = b * b - 4 * a * c;
  
	if (D < 0) { 
		return FALSE;
	}
	else {
		D = sqrt(D);
		*t = (-b - D) / (2*a);
		if (*t < 0) {
			*t = (-b + D) / (2*a);
		}
		if (*t < 0) return FALSE; 
		else{ 
			GLfloat yVal = *t*v->y + r->start->y;
			if(yVal > theCone->uBound && theCone->uBound != -1.0)
				return FALSE;
			else{
				if(yVal < theCone->bBound && theCone->bBound != 1.0)
					return FALSE;
				else
					return TRUE;
			}
		}
	}
	
}
void findConeNormal(cone* v ,point* pt,vector* n){
	point p;
	p.x = pt->x - v->c->x;
	p.y = pt->y - v->c->y;
	p.z = pt->z - v->c->z;
	n->x = p.x;  
	n->z = p.z;
	n->y = (p.x*-p.y)/(sqrt(p.x*p.x + p.y*p.y));
	n->w = 0.0;
	normalize(n);
}