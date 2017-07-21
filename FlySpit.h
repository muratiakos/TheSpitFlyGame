#ifndef FLYSPLIT_H
#define FLYSPLIT_H

#include "MovingObject.h"

class FlySpit: public MovingObject {
public:
	Sphere* spit;	//Gömb
	int Time;

	FlySpit(Vector _P, Vector _V) {
		//Bogár
		P = _P;
		Time=0;

		_V.Normalize();
		V=_V*0.3;
		V.y+=0.005;

		spit=new Sphere(0.04*sqrt(V.Length()), 10);

		GLfloat _aC[] = { 0.0, 0.03f, 0.f, 0.1f };
		GLfloat _dC[] = {0.00, 0.13, 0.0 , 0.4f};
		GLfloat _sC[] = {0.0, 0.0, 0.0 , 1.0f};
		GLfloat _e[] = {0.0, 0.0, 0.0, 0.0f};
		setMaterial(_aC, _dC, _sC, _e);
	}
	
	void Draw() {
		Time+=Td;
		if (Time>10000) Alive=false;

		Material();
		glPushMatrix();
			glTranslatef(P.x, P.y, P.z);
			glScalef(spit->R,spit->R,spit->R);
			spit->Draw();
		glPopMatrix();
	}

	void Move() {
		 P.x+=V.x*Td*SPEED*0.1;
		 P.y+=V.y*Td*SPEED*0.1;
		 P.z+=V.z*Td*SPEED*0.1;
		 //Gravitacios gyorsulas
		 V.y-=GRAVITY*Td*SPEED*0.0000005;
	}

	bool Collide() {
		float p0[3] = {P.x,P.y,P.z};
		float p1[3];
		
		float cv = CollideSpeed;
		NextPoint(p0, p1, cv);
		
		if(cso->isCollide(spit->R, p1[0], p1[1], p1[2])<0 ) {
			//van ütközés
				
				bool coll=true;
				while (coll) {
					cv=cv-(cv/20); //N-ad közelítés
					NextPoint(p0, p1, cv);
					coll = (cso->isCollide(spit->R, p1[0], p1[1], p1[2])<0);
				} 
				//NextPoint(p0, p1, cv); //cv/2 */

				//NextPoint(p0, p1, (MovingSpeed+CollideSpeed)/2);
				P.x=p1[0]; P.y=p1[1]; P.z=p1[2];

				V.x=0;
				V.y=0;
				V.z=0;

			return true;
		} else return false;
	}
};
#endif