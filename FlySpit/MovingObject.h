#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

class MovingObject{
public:
	Vector P;
	Vector V;
	float MovingSpeed;
	float CollideSpeed;

	bool Alive;
	int Life;

	//Material
	GLfloat aC[4];
	GLfloat dC[4];
	GLfloat sC[4];
	GLfloat e[4];

	MovingObject() {
		P = Vector(0,0,0);
		V = Vector(0,0,0);

		MovingSpeed=0.1;
		CollideSpeed=0.2;
		Life = 100;

		Alive=true;
	}
	
	void setMaterial(GLfloat _aC[4], GLfloat _dC[4], GLfloat _sC[4], GLfloat _e[4]) {
		for(int i=0;i<4;i++) {
			aC[i]=_aC[i];
			dC[i]=_dC[i];
			sC[i]=_sC[i];
			e[i]=_e[i];
		}
	}
	
	void Material() {
		glMaterialfv(GL_FRONT, GL_AMBIENT, aC);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dC);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sC);
		glMaterialfv(GL_FRONT, GL_EMISSION, e);
	}

	void NextPoint(float* p0, float* p1, float v) {
		p1[0]=p0[0]+(V.x*Td*SPEED*v);
		p1[1]=p0[1]+(V.y*Td*SPEED*v);
		p1[2]=p0[2]+(V.z*Td*SPEED*v);
	}

};

#endif