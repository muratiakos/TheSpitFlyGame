#ifndef FLY_H
#define FLY_H

#include "MovingObject.h"
#include "Utils.h"

class Fly: public MovingObject {
public:
	GLfloat orient; //Fej Orientáció
	int Time, LastShoot, LastDie;

	Sphere* head;
	Sphere* eye;
	Sphere* body;

	Fly( Vector _P) {
		P = _P;
		orient=0;
		
		V.z =0.02;

		Time =0;
		LastShoot =0;
		LastDie =0;
		Life = 100;

		head=new Sphere(1, 20);
		eye=new Sphere(1, 20);
		body=new Sphere(1, 20);


		//Matéria beállítása
		GLfloat _aC[] = {0.09, 0.05, 0.04 , 0.2};
		GLfloat _dC[] = {0.18, 0.11, 0.09 , 0.1};
		GLfloat _sC[] = {0.0, 0.0, 0.0 , 1.0};
		GLfloat _e[] = {0, 0, 0, 1.0};

		setMaterial(_aC, _dC, _sC, _e);
	}

	void TurnH(float i) {
		if (Alive) {
			//Kamera fordítása 5 fokkal jobbra vagy balra
			V.x=V.x*cos(i*My_PI/180)-V.z*sin(i*My_PI/180);
			V.z=V.x*sin(i*My_PI/180)+V.z*cos(i*My_PI/180);
		}
	}
	
	void TurnV(float i) {
		if (Alive) {
			//V.y=V.y*cos(i*My_PI/180); //-V.y*sin(i*My_PI/180);
			float newY=V.y+(i/10000);
			if (fabs(newY) >=V_TURN) {
				newY=mySig(newY)*V_TURN;
			}
			V.y = newY;
		}
	}

	void Draw() {
		float tmp;
		//Légy pozícionálása
		

		glPushMatrix();
			glTranslatef(P.x, P.y, P.z);
			glScalef(0.05, 0.05, 0.05);
			glRotatef(orient, 0, 1, 0);

			{ //Fej rajzolas
				Material();
				glPushMatrix();	
					glTranslatef(-2.2, 0.2, 0);
					glScalef(1.1, 1.0, 1.3);
					head->Draw();
				glPopMatrix();

				//szemek kirajzolása
				for (int i=0;i<2;i++) {
					if (i==0) tmp=-1;else tmp=1;
					szemMaterial();
					glPushMatrix();
						glTranslatef(-3, 0.5, tmp*0.2);
						glScalef(0.5, 0.5, 0.5);
						//glRotatef(-90, 0, 0, 1);
						eye->Draw();
					glPopMatrix();
				}


			}
			{ //Szárnyak kirajzolása
				glDisable(GL_LIGHTING);
				glEnable(GL_BLEND);
				glColor4f(1,1,1,0.3);
				for (int i=0;i<2;i++) {
					if (i==0) tmp=-1;else tmp=1;
					glPushMatrix();	
					
					if (Alive) tmp*=60+(Time%10); //Szárny lebegtetés
					else tmp*=100;

						glRotatef(tmp, 1, 0, 0);
						glBegin(GL_TRIANGLES);
							glVertex3f(0, 0, 0);
							glVertex3f(-2, 4, 0);
							glVertex3f( 2, 4, 0);
						glEnd();
					glPopMatrix();
				}
				
				glDisable(GL_BLEND);
				glEnable(GL_LIGHTING);
			}			
			{ //Tor-Potroh rajzolása
				Material();
				glPushMatrix();
					glScalef(1.5, 1.0, 1.0);
					glRotatef(-150, 0, 0, 1);
					body->Draw();
				glPopMatrix();
			}
		glPopMatrix();
	}

	void Move() {
		if (Alive) {
			Collide();
			Time=((Time+Td)%500)+1;
			LastShoot+=Td;
			LastDie+=Td;

			Vector P0, D;
			P0 = P;
			P = P+(V);
			D=P-P0;

			orient=-1*(atan2(D.z,D.x)*180/My_PI)+180;
		} else if (P.y > (- cso->r * cso->sy + 0.05)) P.y-=0.01;
	}

	void Spit() {
		if (Alive && LastShoot>300) {
			flyspit[flyspitCount] = new FlySpit(P, V);
			flyspitCount++;
			LastShoot=0;

			shootCount++;
		}
	}
	void szemMaterial() { // Szem
		GLfloat _aC[] = {0.00, 0.0, 0.0 , 0.5};
		glMaterialfv(GL_FRONT, GL_AMBIENT, _aC);
		GLfloat _dC[] = {0.02, 0.0, 0.01 , 1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, _dC);
		GLfloat _sC[] = {0.0, 0.0, 0.0 , 1};
		glMaterialfv(GL_FRONT, GL_SPECULAR, _sC);
		GLfloat _e[] = {0, 0, 0, 1.0};
		glMaterialfv(GL_FRONT, GL_EMISSION, _e);
	}

	bool Collide() {
		float s = 0.1;
		float p0[3] = {P.x,P.y,P.z};
		float p1[3];
		
		float cv = CollideSpeed;
		NextPoint(p0, p1, cv);
		
		if(cso->isCollide(s, p1[0], p1[1], p1[2])<0 ) {
			//van ütközés
				

				bool coll=true;
				while (coll && cv>=MovingSpeed) {
					cv=cv-(cv/20); //N-ad közelítés
					NextPoint(p0, p1, cv);
					coll = (cso->isCollide(s, p1[0], p1[1], p1[2])<0);
				}

				float n[3],cp[3];
				cp[0]=P.x; cp[1]=P.y; cp[2]=P.z;
				cso->CollideVec(cp, n);
				Vector nv = Vector(n[0], n[1], n[2]);

				float cosIn = -1 * (VectorAngle(nv,V));
				float v_size=sqrt(V.x*V.x+V.y*V.y+V.z*V.z);

				V.Normalize();
				Normalize(&n[0], &n[1], &n[2]);

				V.x=1.2*n[0]* cosIn + V.x;
				V.y=1.2*n[1]* cosIn + V.y;
				V.z=1.2*n[2]* cosIn + V.z;

				V.Normalize();
				V = V*v_size;

				Die();
			return true;
		} else return false;
	}
	void Die() {
		if (LastDie>2000) { //2 mp védelem
			LastDie=0;
			Life-=33;
			if (Life<=0) {
				V.x=0; V.y=0; V.z=0;
				Alive = false;

				float ac = acos(P.x/(cso->R*cso->sx));
				float z = ((cso->R  * sin(ac))*cso->sx);
				if (fabs(P.z - z) > 2) z = ((cso->R  * sin(ac+My_PI))*cso->sx);
				P.z = z;
			}
		}
		
	}

};
#endif