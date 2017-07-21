#ifndef UTILS_H
#define UTILS_H


float mySig(float n) {
	if (n<0) return -1; else return 1;
}

void Message(float x, float y, char * buffer) {
       glRasterPos2f( x, y );
       for( int i = 0; i < 80; i++ ) {
               if ( buffer[i] == '\0' ) break;
               else glutBitmapCharacter( GLUT_BITMAP_8_BY_13, buffer[i] );
       }
}

bool SameDir(float x0, float x1) {
	if ((x0<=0 && x1<=0) || (x0>0 && x1>0) ) return true; else return false;
}

void Normalize(float* px, float* py, float* pz) {
	float x, y, z;
	x=*px; y=*py; z=*pz;
	float rn=sqrt(x*x + y*y + z*z);
	x /= rn; y /= rn; z /= rn;
	*px=x; *py=y; *pz=z;
}

float VectorAngle(Vector u, Vector v) {
	float angle;
	Vector tu =u ;
	Vector tv =v;

	tu.Normalize();
	tv.Normalize();

	float du = tu.Length(); //=sqrt(tu[0]*tu[0]+tu[1]*tu[1]+tu[2]*tu[2]);
	float dv= tv.Length(); //sqrt(tv[0]*tv[0]+tv[1]*tv[1]+tv[2]*tv[2]);

	angle=((tu.x*tv.x+tu.y*tv.y+tu.z*tv.z)/(du*dv));
	
	return angle;
}

#endif