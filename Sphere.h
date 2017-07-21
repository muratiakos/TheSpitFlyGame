#ifndef SPHERE_H
#define SPHERE_H

class Sphere { // Sphere osztaly
public: 
	Vector** Vertex;	// Gömbpontok koordinátái
	GLfloat R;
	GLint seg;

	//Gömb Konstruktorok
	Sphere() { Vertex = NULL; }

	Sphere(GLfloat _r, GLint _seg) {
		R=_r;
		seg=_seg;
		Tessellate();
	}

	void Tessellate() {
		Vertex = (Vector**) malloc( sizeof(Vector) *seg+1);
		for (int i = 0;i < seg+1;i++) Vertex[i] = (Vector*) malloc( sizeof(Vector) *seg+5);
		//Pozitívba fordítás
		seg = abs(seg);
		for (int i = 0;i < seg+1;i++) {
			for (int j = 0;j < seg+1;j++) {
					//Gömb param-egyenlete (x=r*sin(U)*cos(V)  y=r*cos(U)   z=r*sin(U)*sin(V))
					Vertex[i][j].x = sin(My_PI * i / seg) * cos(2 * My_PI * j / seg);
					Vertex[i][j].y = cos(My_PI * i / seg);
					Vertex[i][j].z = sin(My_PI * i / seg) * sin(2 * My_PI * j / seg);
			}
		} 
	}

	void Draw() {
		for (int i=0;i<seg;i++) {		
			//Négyszögalkotás háromszögekbol
			glBegin(GL_TRIANGLES);
			for (int j = 0;j < seg;j++) {
				//n-p - 1
				glNormal3f(Vertex[i][j].x , -Vertex[i][j].y , Vertex[i][j].z );
				glVertex3f(Vertex[i][j].x, -Vertex[i][j].y, Vertex[i][j].z);
				
				//n-p - 2
				glNormal3f(Vertex[i + 1][j].x , -Vertex[i + 1][j].y , Vertex[i + 1][j].z );
				glVertex3f(Vertex[i + 1][j].x, -Vertex[i + 1][j].y, Vertex[i + 1][j].z);
				
				//n-p - 4
				glNormal3f(Vertex[i][j + 1].x , -Vertex[i][j + 1].y , Vertex[i][j + 1].z );
				glVertex3f(Vertex[i][j + 1].x, -Vertex[i][j + 1].y, Vertex[i][j + 1].z);
				
				//n-p - 2
				glNormal3f(Vertex[i + 1][j].x , -Vertex[i + 1][j].y , Vertex[i + 1][j].z );
				glVertex3f(Vertex[i + 1][j].x, -Vertex[i + 1][j].y, Vertex[i + 1][j].z);
				
				//n-p - 3
				glNormal3f(Vertex[i + 1][j + 1].x , -Vertex[i + 1][j + 1].y , Vertex[i + 1][j + 1].z);
				glVertex3f(Vertex[i + 1][j + 1].x, -Vertex[i + 1][j + 1].y, Vertex[i + 1][j + 1].z);
				
				//n-p - 4
				glNormal3f(Vertex[i][j + 1].x , -Vertex[i][j + 1].y , Vertex[i][j + 1].z );
				glVertex3f(Vertex[i][j + 1].x, -Vertex[i][j + 1].y, Vertex[i][j + 1].z);
			}
			glEnd();
		}
	}
};

#endif