
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
//#include <GL/gl.h>

#include <math.h>

struct Position { double x,y,z; };


/* PI */
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


void drawSphere(double r, int lats, int longs) {
	int i, j;
	for(i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r * cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}

void drawCapsule_(double r, int lats, int longs, double length) {
	int i, j;
    int halfLats = lats / 2;
	for(i = 0; i <= halfLats; i++) {
		double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
		double z0  = r * sin(lat0);
		double zr0 = r * cos(lat0);

		double lat1 = M_PI * (-0.5 + (double) i / lats);
		double z1  = r * sin(lat1);
		double zr1 = r * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}

    double lat = M_PI * (-0.5 + (double) halfLats / lats);
    double z  = r * sin(lat);
    double zr = r * cos(lat);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= longs; j++) {
        double lng = 2 * M_PI * (double) (j - 1) / longs;
        double x = cos(lng);
        double y = sin(lng);

        glNormal3f(x * zr, y * zr, z);
        glVertex3f(x * zr, y * zr, z);
        glNormal3f(x * zr, y * zr, z);
        glVertex3f(x * zr, y * zr, z + length);
    }
    glEnd();

    for(i = halfLats; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = r * sin(lat0);
        double zr0 = r * cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1  = r * sin(lat1);
        double zr1 = r * cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0 + length);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1 + length);
        }
        glEnd();
    }
}

void drawCapsule(double r, int lats, int longs, Position position1, Position position2) {
    double vx = position2.x - position1.x;
    double vy = position2.y - position1.y;
    double vz = position2.z - position1.z;
    double length = sqrt(vx * vx + vy * vy + vz * vz);

    double acosx = acos(vz / length);

    double axisx = -vy;
    double axisy = vx;
    double axisz = 0.0;

    printf("acos:%lf, axisx:%lf, axisy:%lf, axisz:%lf, length:%lf\n", acosx, axisx, axisy, axisz, length);

    glPushMatrix();
    glTranslatef( position1.x, position1.y, position1.z );
    glRotatef(acosx / M_PI * 180, axisx, axisy, axisz);
    drawCapsule_(r, lats, longs, length);
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    printf("現在のmatrix\n");
    printf("m[0]:% 7.5f m[4]:% 7.5f m[8] :% 7.5f m[12]:% 7.5f\n", m[0], m[4], m[8],  m[12]);
    printf("m[1]:% 7.5f m[5]:% 7.5f m[9] :% 7.5f m[13]:% 7.5f\n", m[1], m[5], m[9],  m[13]);
    printf("m[2]:% 7.5f m[6]:% 7.5f m[10]:% 7.5f m[14]:% 7.5f\n", m[2], m[6], m[10], m[14]);
    printf("m[3]:% 7.5f m[7]:% 7.5f m[11]:% 7.5f m[16]:% 7.5f\n", m[3], m[7], m[11], m[15]);
    glPopMatrix();

}

void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
    glBegin(GL_LINE_LOOP);
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);

    gluCylinder(quadric, base, 0, height, slices, stacks);

    gluDeleteQuadric(quadric);
    glEnd();
}

// Added in Exercise 9 - Start *****************************************************************
void drawSnowman( bool female )
{
    glRotatef( -90, 1, 0, 0 );
    glScalef(0.03, 0.03, 0.03);

    // draw 3 white spheres
    glColor4f( 0.0, 0.0, 1.0, 1.0 );
    drawSphere( 0.05, 3, 4 );
    //    glTranslatef( 0.0, 0.8, 0.0 );
    //    drawSphere( 0.6, 10, 10 );
    //    if(female)
    //    {
    //        glPushMatrix();
    //        glRotatef(90, 0, 1, 0);
    //        glTranslatef(-0.2, 0.05, 0.3);
    //        drawSphere( 0.32, 10, 10 );
    //        glTranslatef(0.4, 0, 0);
    //        drawSphere( 0.32, 10, 10 );
    //        glPopMatrix();
    //    }
    //    glTranslatef( 0.0, 0.6, 0.0 );
    //    drawSphere( 0.4, 10, 10 );
    //
    //    // draw the eyes
    //    glPushMatrix();
    //    glColor4f( 0.0, 0.0, 0.0, 1.0 );
    //    glTranslatef( 0.2, 0.2, 0.2 );
    //    drawSphere( 0.066, 10, 10 );
    //    glTranslatef( 0, 0, -0.4 );
    //    drawSphere( 0.066, 10, 10 );
    //    glPopMatrix();
    //
    //    // draw a nose
    //    glColor4f( 1.0, 0.5, 0.0, 1.0 );
    //    glTranslatef( 0.3, 0.0, 0.0 );
    //    glRotatef( 90, 0, 1, 0 );
    //    solidCone( 0.1, 0.3, 10, 10 );
}
// Added in Exercise 9 - End *****************************************************************
