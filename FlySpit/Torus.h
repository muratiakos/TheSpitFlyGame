#ifndef TORUS_H
#define TORUS_H

class Torus{ //Általános Tórusz osztály
public: 
	GLfloat r;
	GLfloat R;
	GLint seg;
	Vector** Vertex;
	Vector** Normal;

	float x, y, z;
	float sx, sy, sz;

	Torus(GLfloat _r, GLfloat _R,  GLint _seg, GLfloat _sx,  GLfloat _sy,  GLfloat _sz) {
		r=_r; R=_R;
		x=0; y=0; z=0;
		sx=_sx; sy=_sy; sz=_sz; //Tórusz skálázása
		seg=_seg;
		Tessellate();
	}

	void Tessellate() {
		Vertex = (Vector**) malloc( sizeof(Vector) *seg+1);
		Normal = (Vector**) malloc( sizeof(Vector) *seg+1);

		for (int i = 0;i < seg+1;i++) {
			Vertex[i] = (Vector*) malloc( sizeof(Vector) *seg+5);
			Normal[i] = (Vector*) malloc( sizeof(Vector) *seg+5);
		}
		//Pozitívba fordítás
		r = fabs(r);
		seg = abs(seg);
		//Torusz param-egyenlet		x(u,v)=(R+cos(2*PI*u))*cos(2*PI*v)
		//							y(u,v)=(R+cos(2*PI*u))*sin(2*PI*v)
		//							z(u,v)=r*sin(2*PI*u)
		// y és z a vilagban felcserelve

		for (int i=0; i<seg+1; i++) {		
			for (int j=0; j<seg+1; j++) {
				Vertex[i][j].x = (R + r * cos(2 * My_PI * i / seg)) * cos(2 * My_PI * j / seg);
				Vertex[i][j].y = r * sin(2 * My_PI * i / seg);
				Vertex[i][j].z = (R + r * cos(2 * My_PI * i / seg)) * sin(2 * My_PI * j / seg);

				glEnable(GL_NORMALIZE);
				Normal[i][j].x= CSILL * -1 * (Vertex[i][j].x-R*cos((2 * My_PI * j/ seg))) / (sx*sx);
				Normal[i][j].y= CSILL * -1 * (Vertex[i][j].y) / (sy*sy);
				Normal[i][j].z= CSILL * -1 * (Vertex[i][j].z-R*sin((2 * My_PI * j/ seg))) / (sz*sz);
				glDisable(GL_NORMALIZE);
			}
		} 
	}

	void Material() {
		//GLfloat aC[] = {0.05, 0.05, 0.02 , 1.0f};
		GLfloat aC[] = {0.43, 0.3, 0.01 , 1.0f};
		glMaterialfv(GL_FRONT, GL_AMBIENT, aC);
		GLfloat dC[] = {0.08, 0.07, 0.0 , 1.0f};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dC);
		GLfloat sC[] = {0.09, 0.09, 0.09 , 1.0f};
		glMaterialfv(GL_FRONT, GL_SPECULAR, sC);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.9);
		GLfloat e[] = {0.0, 0.0, 0.0, 1.0};
		glMaterialfv(GL_FRONT, GL_EMISSION, e);
	}

	float isCollide(float _r, float _x, float _y, float _z) {
		//Skalazas beszamitasa utkozesbe
		_x/=sx; _y/=sy; _z/=sz;

		//Paraméteres Egyenlet: terünkben y és z felcserélve
		// r^2 - z^2 - (R-sqrt(x^2+y^2))^2
		
		float res = (r-_r)*(r-_r) - (_y)*(_y) -
			(
				 (R-sqrt((_x)*(_x) + (_z)*(_z)))
				*(R-sqrt((_x)*(_x) + (_z)*(_z)))
			);
		return res;
	}

	void CollideVec(float* cp, float* n) {
		n[0]= 4*cp[0]/(sx*sx) * ( cp[0]*cp[0]/(sx*sx) + cp[1]*cp[1]/(sy*sy) + cp[2]*cp[2]/(sz*sz) - R*R - r*r);
		n[1]= 4*cp[1]/(sy*sy) * ( cp[0]*cp[0]/(sx*sx) + cp[1]*cp[1]/(sy*sy) + cp[2]*cp[2]/(sz*sz) + R*R - r*r);
		n[2]= 4*cp[2]/(sz*sz) * ( cp[0]*cp[0]/(sx*sx) + cp[1]*cp[1]/(sy*sy) + cp[2]*cp[2]/(sz*sz) - R*R - r*r);

		Normalize(&n[0],&n[1],&n[2]);
	}

	void Draw() {
		Material();
		for (int i = 0;i < seg;i++) {
			//Négyszög építés 3szögekbol
			glBegin(GL_TRIANGLES);
			for (int j = 0;j < seg;j++) {		
				glTexCoord2d(0, 0); //np - 1
				glNormal3f(Normal[i][j].x/sx, Normal[i][j].y/sy, Normal[i][j].z/sz);
				glVertex3f(Vertex[i][j].x, Vertex[i][j].y, Vertex[i][j].z);

				glTexCoord2d(1, 0); //np - 2
				glNormal3f(Normal[i + 1][j].x/sx, Normal[i + 1][j].y/sy, Normal[i + 1][j].z/sz);
				glVertex3f(Vertex[i + 1][j].x, Vertex[i + 1][j].y, Vertex[i + 1][j].z);

				
				glTexCoord2d(0, 1); //np-4
				glNormal3f(Normal[i][j + 1].x/sx, Normal[i][j + 1].y/sy, Normal[i][j + 1].z/sz);
				glVertex3f(Vertex[i][j + 1].x, Vertex[i][j + 1].y, Vertex[i][j + 1].z);

				glTexCoord2d(1, 0); //np - 2
				glNormal3f(Normal[i + 1][j].x/sx, Normal[i + 1][j].y/sy, Normal[i + 1][j].z/sz);
				glVertex3f(Vertex[i + 1][j].x, Vertex[i + 1][j].y, Vertex[i + 1][j].z);

				glTexCoord2d(1, 1); //np - 3
				glNormal3f(Normal[i + 1][j + 1].x/sx, Normal[i + 1][j + 1].y/sy, Normal[i + 1][j + 1].z/sz);
				glVertex3f(Vertex[i + 1][j + 1].x, Vertex[i + 1][j + 1].y, Vertex[i + 1][j + 1].z);

				glTexCoord2d(0, 1); //np - 4
				glNormal3f(Normal[i][j + 1].x/sx, Normal[i][j + 1].y/sy, Normal[i][j + 1].z/sz);
				glVertex3f(Vertex[i][j + 1].x, Vertex[i][j + 1].y, Vertex[i][j + 1].z);
			}
			glEnd();
		}
	}

	Vector RandomPos(int i) {
		Vector PR = Vector(0,0,0);
		int r1 = rand()*(1000+(110*i))*i;

		PR.x = ((R  * cos(2 * My_PI *  r1))*sx);
		PR.y = 0;
		PR.z = ((R  * sin(2 * My_PI *  r1))*sx);

		return PR;
	}
};

#endif