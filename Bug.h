#ifndef BUG_H
#define BUG_H

#include "MovingObject.h"

class Bug: public MovingObject {
public:
	Sphere* bogar;
	GLenum Light;
	bool EvilBug;

	Bug(int i, GLfloat _r, Vector _P, bool _EvilBug) {
		P = _P;
		bogar=new Sphere(_r, 10);
		EvilBug=_EvilBug;
		if (EvilBug) EnemyCount++;
		setLightSource((GLenum)(GL_LIGHT0+i));
	}

	void setLightSource(GLenum _light) {
		Light=_light;
		setLightPos();
		setLightColor();
	}
	
	void Die() {
		if (Alive) hitCount++;
		else fly->Life+=3;

		Life-=35;
		if (Life<=0) {
			Alive=false;
			Dead++;
			if (EvilBug) EnemyDead++;
			V.x=0;
			V.z=0;
		}

		//zöldítés
		e[0] -= 0.2;
		e[1] += 0.3;
		e[2] -= 0.2;
	}

	void setLightColor() {
		if (!EvilBug) { //Jó bogár
			GLfloat dc[] = {0.91f, 0.93f, 0.39f, 1.0f};
			GLfloat qa[] = {QAt};
			setLight(dc,dc,qa);

			GLfloat _aC[] = { 0.91f, 0.93f, 0.39f, 1.0f };
			GLfloat _dC[] = {0.91, 0.93, 0.39 , 1.0f};
			GLfloat _sC[] = {0.0, 0.0, 0.0 , 1.0f};
			GLfloat _e[] = {1.0, 1.0, 1.0, 1.0f};
			setMaterial(_aC, _dC, _sC, _e);
		} else { //Gonosz bogár
			GLfloat dc[] = {0.91f, 0.13f, 0.19f, 1.0f};
			GLfloat qa[] = {QAt};
			setLight(dc,dc,qa);

			GLfloat _aC[] = { 0.91f, 0.13f, 0.19f, 1.0f };
			GLfloat _dC[] = {0.91, 0.13, 0.19 , 1.0f};
			GLfloat _sC[] = {0.0, 0.0, 0.0 , 1.0f};
			GLfloat _e[] = {0.9, 0.0, 0.0, 1.0f};
			setMaterial(_aC, _dC, _sC, _e);
		}
		if (Light<=GL_LIGHT4) glEnable(Light);
	}

	void setLightPos() { 
		if (Light<=GL_LIGHT4) {
			GLfloat lightPos[] = {P.x, P.y, P.z, 1.0f};
			glLightfv(Light, GL_POSITION, lightPos);
		}
	}

	void setLight(GLfloat d[], GLfloat s[], GLfloat qa[]) {
		if (Light<=GL_LIGHT4) {
			glLightfv(Light, GL_DIFFUSE, d);
			glLightfv(Light, GL_SPECULAR, s);
			glLightfv(Light, GL_QUADRATIC_ATTENUATION, qa);
		}
	}

	void Draw() {
		//Gömb beállítása a megfelelo helyre, feny kikapcs a gombre
		if (Light<=GL_LIGHT4)  glDisable(Light);
		setLightPos();

		Material();
		//glEnable(GL_BLEND);
		glPushMatrix();
			glTranslatef(P.x, P.y, P.z);
			glScalef(bogar->R,bogar->R,bogar->R);
			bogar->Draw();
		glPopMatrix();
		//glDisable(GL_BLEND);
		//Fény visszakapcsolása
		if (Light<=GL_LIGHT4) glEnable(Light);
	}

	void Move() {
		 P.x+=V.x*Td*SPEED*MovingSpeed;
		 P.y+=V.y*Td*SPEED*MovingSpeed;
		 V.y-=GRAVITY*Td*SPEED*0.0000015;
		 P.z+=V.z*Td*SPEED*MovingSpeed;
	}

	bool isCollide(float r1, Vector P1) {
		Vector C = P1 - P;
		float h = C.Length();

		if (h<=bogar->R+r1) return true;
		else return false;
	}

	bool Collide() {
		//Légy ütközik-e bogárral?
		if (isCollide(0.2, (fly->P+fly->V))) fly->Die();
		
		//Van-e lövedékkel ütközés?
		for(int i=0;i<flyspitCount;i++) {
			if (isCollide(flyspit[i]->spit->R*1.2, flyspit[i]->P)){
				Die();
				flyspit[i]->Alive=false;
			}
		}

		//Mozgása
		float p0[3] = {P.x,P.y,P.z};
		float p1[3];

		NextPoint(p0, p1, CollideSpeed);
		p1[1]-=GRAVITY*Td*SPEED*0.0000015;

		if(cso->isCollide(bogar->R, p1[0], p1[1], p1[2])<0 ) {
			//van ütközés
			float n[3],cp[3];
			cp[0]=P.x; cp[1]=P.y; cp[2]=P.z;

			if (Alive) {
			cso->CollideVec(cp, n);
			Vector nv = Vector(n[0], n[1], n[2]);

			float cosIn = -1 * (VectorAngle(nv,V));
			float v_size=sqrt(V.x*V.x+V.y*V.y+V.z*V.z);

			V.Normalize();
			Normalize(&n[0], &n[1], &n[2]);

			V.x=2*n[0]* cosIn + V.x;
			V.y=2*n[1]* cosIn + V.y;
			V.z=2*n[2]* cosIn + V.z;

			V.Normalize();
			V = V*v_size;

			//Visszairanyitjuk az uj vektorral tavolabbi pontra a beragadas ellen
			P.x+=V.x*Td*SPEED*0.1;
			P.y+=V.y*Td*SPEED*0.1;
			P.z+=V.z*Td*SPEED*0.1;
			} else {
				//nem el
				V.x=0;
				V.y=0;
				V.z=0;
			}
			return true;
		} else return false;
	}
};

#endif