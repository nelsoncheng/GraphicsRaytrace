/******************************************************************/
/*         Lighting functions                                     */
/*                                                                */
/* Group Members: Nelson Cheng 				                      */
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

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat kd, GLfloat kss, GLfloat ns, GLfloat ksr, GLfloat kr) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
  m->kd = kd;
  m->kss = kss;
  m->ns = ns;
  m->ksr = ksr;
  m->kr = kr;
  return(m);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, lightNode* lightList, int d) {
	double distToIntersect, distToLight;
	vector toLightVector;
	vector nDummy;material* mDummy;
	point offsetP;
	ray toLightRay;
	point intersectPoint;
	GLfloat nDotHi, liDotN;
	/* ambient component of color */
	c->r = m->amb * m->r;
	c->g = m->amb * m->g;
	c->b = m->amb * m->b;
	
	vector nIn;
	nIn.x = -in->x;
	nIn.y = -in->y;
	nIn.z = -in->z;
	nIn.w = in->w;
	
	color reflectLight;
	reflectLight.r = 0.0;
	reflectLight.g = 0.0;
	reflectLight.b = 0.0;
	
	/* Diffuse Lighting */
	lightNode* aLight;
	aLight = lightList->next;
	while(aLight != NULL){
		calculateDirection(p, aLight->theLight->p, &toLightVector);
		normalize(&toLightVector);
		offsetP.x = p->x + toLightVector.x*0.0001;
		offsetP.y = p->y + toLightVector.y*0.0001;
		offsetP.z = p->z + toLightVector.z*0.0001;
		toLightRay.dir = &toLightVector;
		toLightRay.start = &offsetP;
		distToLight = sqrt(pow((offsetP.x-aLight->theLight->p->x),2)+pow((offsetP.y-aLight->theLight->p->y),2)+pow((offsetP.z-aLight->theLight->p->z),2));
		firstHit(&toLightRay,&intersectPoint,&nDummy,&mDummy,&distToIntersect);
		if((intersectPoint.w == 0.0) || (intersectPoint.w == 1.0 && distToIntersect > distToLight)){
			liDotN = dot(&toLightVector,n);
			if(liDotN > 0.0){
				// Diffuse Lighting
				c->r += m->kd * aLight->theLight->c->r * liDotN * m->r; 
				c->g += m->kd * aLight->theLight->c->g * liDotN * m->g; 
				c->b += m->kd * aLight->theLight->c->b * liDotN * m->b; 
				// For Specular Highlights
				vector Hi;
				Hi.x = toLightVector.x + nIn.x;
				Hi.y = toLightVector.y + nIn.y;
				Hi.z = toLightVector.z + nIn.z;
				normalize(&Hi);
				nDotHi = dot(n,&Hi);
				if(nDotHi > 0.0){
					reflectLight.r += pow(nDotHi,m->ns) * aLight->theLight->c->r;
					reflectLight.g += pow(nDotHi,m->ns) * aLight->theLight->c->g;
					reflectLight.b += pow(nDotHi,m->ns) * aLight->theLight->c->b;
				}
			}
		} 
		aLight = aLight->next;  
	}

	/* Specular */
	c->r += m->kss * reflectLight.r;
   	c->g += m->kss * reflectLight.g;
	c->b += m->kss * reflectLight.b;

	/* Reflections */
	vector rv;
	normalize(n);
	normalize(&nIn);
	GLfloat nDotV = 2.0*dot(n,&nIn);
	rv.x = (nDotV*n->x);
	rv.y = (nDotV*n->y);
	rv.z = (nDotV*n->z);
	rv.x -= nIn.x; 
	rv.y -= nIn.y; 
	rv.z -= nIn.z; 	
	rv.w = 0.0;
	normalize(&rv);
	offsetP.x = p->x + rv.x*0.0001;
	offsetP.y = p->y + rv.y*0.0001;
	offsetP.z = p->z + rv.z*0.0001;
	offsetP.w = 1.0;
	ray newRay;
	newRay.dir = &rv;
	newRay.start = &offsetP;
	color traceColor;
	if(m->ksr > 0.0){ 
		traceColor = traceRay(&newRay,d-1);
		c->r += m->ksr*traceColor.r;   
		c->g += m->ksr*traceColor.g;
		c->b += m->ksr*traceColor.b;
	}

	/* Refractions */
	offsetP.x = p->x + in->x*0.0001;
	offsetP.y = p->y + in->y*0.0001;
	offsetP.z = p->z + in->z*0.0001;
	newRay.dir = in;
	newRay.start = &offsetP;
	if(m->kr > 0.0){
		traceColor = traceRay(&newRay,--d);	
		c->r += m->kr*traceColor.r;   
		c->g += m->kr*traceColor.g;
		c->b += m->kr*traceColor.b;
	}

	/* clamp color values to 1.0 */
	if (c->r > 1.0) c->r = 1.0;
	if (c->g > 1.0) c->g = 1.0;
	if (c->b > 1.0) c->b = 1.0;
}


