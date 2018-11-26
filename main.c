#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include<stdbool.h>

#define pi 3.141592653589793
double jump = 0;
double move = 0;

bool ball_jump= false;




/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
void specialInput(int key, int x, int y);
void ball_jump_f(int value);
void specialInput(int key, int x, int y);
int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreiramo prozor. */
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(100, 50);
    glutCreateWindow(argv[0]);

    /* callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
	glutIdleFunc(on_display);

	/* funkcija koja dozvoljava hvatanje specijalnih tastera
	 * tastature, kao sto su strelice */
	glutSpecialFunc(specialInput);


    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.75, 0.75, 0);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}


static void on_keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
	case 32:
		if (!ball_jump)
			glutTimerFunc(30, ball_jump_f, 5);
			ball_jump = true;
		break;
    }
		
}
void ball_jump_f(int value){
	if (value != 5) return;

	jump += 7; 

	glutPostRedisplay();

	if ((ball_jump) && (jump <= 180))
		glutTimerFunc(30, ball_jump_f, 5);
	else {
		jump = 0;
		ball_jump = false;
	}

}

void specialInput(int key, int x, int y){

    switch (key) {
	/* kretanje udesno */
	case GLUT_KEY_RIGHT:
		move += 0.01;
		glutPostRedisplay();
		break;
	/* kretanje ulevo */
	case GLUT_KEY_LEFT:
		move -= 0.01;
		glutPostRedisplay();
		break;

	}
	glutPostRedisplay();
}

static void on_reshape(int width, int height)
{
	/* pamtimo sirinu i visinu prozora */
	
	glViewport(0,0,width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/height, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.7,0.3,1,
			  0.7,0.3,0,
			  0,1,0);
}

static void on_display(void)
{
	/* postavljamo poziciju svetla */
	GLfloat light_position[] = {7,9,-7,1};
														
	/* Ambijentalna boja svetla. */                       
	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };       
	                                                      
	/* Difuzna boja svetla. */                            
	GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };       
	                                                      
	/* Spekularna boja svetla. */                         
	GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };      

	// ---------------------------------------------------------
	                                                      
	/* ambijentalna refleksija za sferu*/
	GLfloat ambient_coeffs_1[] = {255/255, 99/255, 71/255, 1};      
	                                                      
	/* difuzna refleksija za sferu*/
	GLfloat diffuse_coeffs_1[] = {255/255, 99/255, 71/255, 1};        

	/* spekularna refleksija za sferu*/
	GLfloat specular_coeffs_1[] = { 1, 1, 1, 1 };           

	// ----------------------------------------------------------

	/* ambijentalna refleksija za poligon */
	GLfloat ambient_coeffs[] = { (float)50/255, (float)205/255, (float)50/255, 1};      

	/* difuzna refleksija za poligon */
	GLfloat diffuse_coeffs[] = {(float)50/255, (float)205/255, (float)50/255, 1};        

	/* spekularna refleksija za poligon */
	GLfloat specular_coeffs[] = { 1, 1, 1, 1 };           


	/* Koeficijent glatkosti materijala. */               
	GLfloat shininess = 60;                               

	/* postavljanje svetla 0*/
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);  
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	/* Postavljaju se svojstva materijala */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);  
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);  
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
	glMaterialf (GL_FRONT_AND_BACK,  GL_SHININESS, shininess);      

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesavamo pogled */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.7,0.3,1,
			  0.7,0.3,0,
			  0,1,0);

	glShadeModel(GL_SMOOTH);


	/* Crtamo podlogu */

	glBegin(GL_POLYGON);
//		glNormal3f(0,1,0);
		glVertex3f(-10, -0.05, -0.2);
		glVertex3f(-10, -0.05,  0.2);
		glVertex3f( 10, -0.05,  0.2);
		glVertex3f( 10, -0.05, -0.2);
	glEnd();
	
	/* crtanje prednje ivice podloge*/
	glBegin(GL_POLYGON);
		glVertex3f(-100, -0.05, -0.2 );
		glVertex3f(-100, -50  , -0.2 );
		glVertex3f( 100, -50  , -0.2 );
		glVertex3f( 100, -0.05, -0.2 );
	glEnd();
	

	glPushMatrix();
	/* crtamo sferu */
	glTranslatef(move, sin(jump*pi / 180)*0.6, 0);
    glColor3f(1, 0, 0);

	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs_1);  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs_1);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs_1);
	glMaterialf( GL_FRONT, GL_SHININESS, shininess);      

    glutSolidSphere(0.05, 100,100);
	
	glPopMatrix();
    
	
	/* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
