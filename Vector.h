//************************************************************************* //
// 3D Vector oszt�ly
//
// Szirmay-Kalos L�szl�, 2003. M�jus.
//************************************************************************* //
#ifndef VECTOR_H
#define VECTOR_H

//===============================================================
class Vector {
//===============================================================
public:
    float x, y, z;   // a Descartes koordin�t�k
	Vector( ) { x = y = z = 0.0; }
	Vector( float x0, float y0, float z0 ) { x = x0; y = y0; z = z0; }

	Vector operator+( const Vector& v ) { // k�t vektor �sszege
		float X = x + v.x, Y = y + v.y, Z = z + v.z;
		return Vector(X, Y, Z);
	}
	Vector operator-( const Vector& v ) { // k�t vektor k�l�nbs�ge
		float X = x - v.x, Y = y - v.y, Z = z - v.z;
		return Vector(X, Y, Z);
	}	
	Vector operator*( float f ) {         // vektor �s sz�m szorzata
		return Vector( x * f, y * f, z * f );
	}
	float operator*( const Vector& v ) {  // k�t vektor skal�ris szorzata
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%( const Vector& v ) { // k�t vektor vektori�lis szorzata
		float X = y * v.z - z * v.y, Y = z * v.x - x * v.z, Z = x * v.y - y * v.x;
		return Vector(X, Y, Z);
	}
	float Length( ) {                     // vektor abszol�t �rt�ke
		return (float)sqrt( x * x + y * y + z * z );
	}
	void operator+=( const Vector& v ) {  // vektor �sszead�s
		x += v.x; y += v.y; z += v.z; 
	}
	void operator-=( const Vector& v ) {  // vektor k�l�nbs�g
		x -= v.x; y -= v.y; z -= v.z; 
	}
	void operator*=( float f ) {		  // vektor �s sz�m szorzata
		x *= f; y *= f; z *= f; 
	}
	void operator/=( float f ) {		  // vektor �s sz�m szorzata
		x /= f; y /= f; z /= f; 
	}
	Vector operator/( float f ) {		  // vektor osztva egy sz�mmal
		return Vector( x/f, y/f, z/f ); 
	}
	void Min( Vector& v ) {		  // vektor osztva egy sz�mmal
		if (x > v.x) x = v.x;
		if (y > v.y) x = v.y;
		if (z > v.z) x = v.z;
	}
	void Max( Vector& v ) {		  // vektor osztva egy sz�mmal
		if (x < v.x) x = v.x;
		if (y < v.y) x = v.y;
		if (z < v.z) x = v.z;
	}
	void Normalize( ) {					  // vektor normaliz�l�sa
		float l = Length( );
		if ( l < 0.000001f) { x = 1; y = 0; z = 0; }
		else { x /= l; y /= l; z /= l; }
	}
	Vector UnitVector( ) {				  // egy vektorral p�rhuzamos egys�gvektor
		Vector r = * this;
		r.Normalize();
		return r;
	}
	Vector Rotate( Vector& axis, float angle ) {	// vektor forgat�sa egy tengely k�r�l
		Vector iv = this -> UnitVector();
		Vector jv = axis.UnitVector() % this -> UnitVector();
		float radian = (float)(angle * My_PI/180);
		return (iv * (float)cos(radian) +  jv * (float)sin(radian));
	}

	Vector Rotate(int axis, float angle) {
		Vector Ax;
		switch(axis) {
			case 1:
				 Ax = Vector(0,1,0);
				break;
			case 2:
				Ax = Vector(0,0,1);
				break;
			default:
			case 0:
				Ax = Vector(1,0,0);
				break;
		}
		return Rotate(Ax, angle);
	}

	void AttenuateRnd(float a) {
		while (x>a) {
			x=x*y;
		}
		x/=5;
		while (y>a) {
			y=y*z; ///10;
		}
		while (z>a) {
			z/=5;
		}
		z=z*x*3;
	}
	
	float * GetArray() { return &x; }

	float& X() { return x; }
	float& Y() { return y; }
	float& Z() { return z; }
};

#endif