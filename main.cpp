#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>     
#endif // Win32 platform

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define DEBUG			0
#define My_PI			3.141592
#define CSILL			30
#define QAt				5.0
#define GRAVITY			9.81
#define SPEED			0.01
#define V_TURN			0.005
#define TURN_ANGLE		5

//Játék változói
char buffer[80];
int Td=0;
int Tt=0;
int mX =0, mY =0;
bool Running = false;

int BugsCount = 4;
int Dead =0;
int EnemyCount =0;
int EnemyDead =0;


//Játék osztályai és példányai
#include "Textures.h"
#include "Vector.h"
#include "Utils.h"
#include "Sphere.h"

#include "Torus.h"
Torus* cso;

#include "FlySpit.h"
FlySpit* flyspit[100];
int flyspitCount=0;
int hitCount=0;
int shootCount=0;

#include "Fly.h"
Fly* fly;

#include "Bug.h"
Bug* bugs[10];

#include "Camera.h"
Camera* camera;

void onInitialization( ) {
	
	Dead =0;
	EnemyCount =0;
	EnemyDead =0;
	
	
	glEnable(GL_LIGHTING);
	float Ambiens[] = { 0.3f, 0.3f, 0.3f, 1.0f }; //Halvány fény dereng a barlangban
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambiens);
	
	//Környezet és nézet beállítása
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f, 1.0f, 0.8f, 50.0f);
	camera = new Camera(Vector(8.4,0,3), Vector(7,0,-3));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	//Textúra beállítása
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 1, WALL_TEXTURE_WIDTH, WALL_TEXTURE_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, WALL_TEXTURE_PIXEL_DATA);

	//Vilag objektumainak letrehozasa
	cso=new Torus(1,4,30,		1.0,1.0, 1.0); //Torus Merete es skalazasa
	
	//Bogarak letrehozasa veletlenül
	srand( (unsigned)time( NULL ));
	bool gonosz = true;
	Vector Pb = Vector(0,0,0);
	Vector Vb = Pb;

	for(int i=0;i<4;i++) {
		if (i>1) gonosz=false;

		Pb = cso->RandomPos(i);

		bugs[i]=new Bug(i, 0.05*(i+2), Pb, gonosz);
		Vb = Vector((rand()*2+100), (rand()*3+100),(rand()*2+100));
		Vb.Normalize();
		Vb.AttenuateRnd(0.001);

		bugs[i]->V = Vb;
		bugs[i]->V.x-=0.0005;
		bugs[i]->V.z+=0.001;
	}

	Pb = cso->RandomPos(3);
	fly=new Fly(Pb);

	Running = true;
}

void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor4f(0.0f, 1.2f, 0.3f, 1.0f);

    //cso kirajzolasa 
	glPushMatrix(); 
		glEnable(GL_TEXTURE_2D);
			glScalef(cso->sx, cso->sy, cso->sz);
			cso->Draw();
		glDisable(GL_TEXTURE_2D);
     glPopMatrix();

	//bogarak
	for(int i=0;i<BugsCount;i++) {
		bugs[i]->Draw();
		
	}
	
	fly->Draw();

    //kopetek
	for(int i=0;i<flyspitCount;i++) {
		 flyspit[i]->Draw();
	}

	//szövegek
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		glLoadIdentity();
			gluOrtho2D(0.0, 1.0, 0.0, 1.0);
			GLfloat betuszin[] = {1, 1, 1, 1};
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, betuszin);
			
			sprintf_s(buffer, "Enemy Bug: %d/%d Friendly Bug: %d/%d", EnemyDead, EnemyCount, Dead-EnemyDead, BugsCount-EnemyCount);
			Message(0.01f, 0.91f, buffer);

			float stat = 0;
			if (shootCount>0 ) stat = (((float)hitCount)/(float)shootCount)*100;

			sprintf_s(buffer, "Spits: %d/%d  Hits: %d Stat: %1.2f", flyspitCount,shootCount,  hitCount, stat);
			Message(0.01f, 0.93f, buffer);

			int prot = (2000-fly->LastDie);
			if (prot < 0) prot=0;

			if (fly->Life <=0) {
				fly->Life =0;
				sprintf_s(buffer, "Game Over - Press 'r' to play again");
				Message(0.25f, 0.5f, buffer);
			}

			sprintf_s(buffer, "Life: %d (Protection: %d)", fly->Life, prot);
			Message(0.01f, 0.95f, buffer);

		glPopMatrix( );
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

    // Buffercsere: rajzolas vege
    glFinish();
    glutSwapBuffers();
}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key==27) exit(0);

	if (key=='a') fly->TurnH(-TURN_ANGLE);
	if (key=='s' || key=='d') fly->TurnH(TURN_ANGLE);
	
	if (key=='w') fly->TurnV(-TURN_ANGLE); //fly->V = fly->V.Rotate(1,0.05); //fly->TurnV(2);
	if (key=='y') fly->TurnV(TURN_ANGLE); //fly->V = fly->V.Rotate(1,-0.05); //fly->TurnV(-2);
	
	if (key=='q')  fly->V*=1.5;
	if (key=='x')  fly->V*=0.8;

	if (key=='p')  Running=!Running;
	if (key=='r')  onInitialization(); 

	if (key==32) fly->Spit();
 
	glutPostRedisplay();
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
			if (state == GLUT_DOWN) {
				fly->Spit();
			}
	}
}

void MouseMotionFunc (int x, int y) { 
	int dX=x-mX;
	int dY=y-mY;

	if (abs(dX) > 5) dX=mySig(dX)*5;
	if (abs(dY) > 2) dY=mySig(dY)*2;
	
	fly->TurnH(dX/1.2);
	fly->TurnV(-1.2*dY);

	mX=x;
	mY=y;
}


void onIdle( ) {

	if (Running) {
		Td=glutGet(GLUT_ELAPSED_TIME)-Tt;
		Tt=glutGet(GLUT_ELAPSED_TIME);

		Vector tav =  fly->V;
		tav.Normalize();
			
		fly->Move();

		camera->P = fly->P - (tav*1.4);
		camera->P.y+=0.5;

		camera->C = fly->P + (tav*4);
		camera->Move();

		
		
		//Öreg lövedékek eltakarítása, ha már öreg
		int i=flyspitCount;
		while (i>0 && flyspitCount>0) {
			if (!flyspit[i-1]->Alive) {
				flyspit[i-1]=flyspit[flyspitCount-1];
				flyspitCount--;
			}
			i--;
		}

		for(int i=0;i<flyspitCount;i++) {
			flyspit[i]->Collide();
			flyspit[i]->Move();
		}

		//BOgarak 
		for(int i=0;i<BugsCount;i++) {
			if (!bugs[i]->Collide()) bugs[i]->Move();
		}
	} else {
		Td =0; Tt=0;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv); 
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("SpitFly - Grafika hazi feladat");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    onInitialization();

    glutDisplayFunc(onDisplay);
    glutMouseFunc(onMouse);
	glutMotionFunc(MouseMotionFunc);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();
    
    return 0;
}
