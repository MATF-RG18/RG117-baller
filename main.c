#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<stdbool.h>
#include<stdio.h>
#include<time.h>


#define pi 3.141592653589793
double jump = 0;
double move = 0;

bool ball_jump= false;
bool ball_move_r= false;
bool ball_move_l= false;
float br = 0;

//i - promenljiva koja odredjuje x koordinatu crtanja poligona
int i = 0;
// promenljive koje odredjuju koordinate crtanja parova prepreka
int prvi = 3, drugi = 7, treci = 4, cetvrti = 8;


/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
void specialInput(int key, int x, int y);
void ball_jump_f(int value);
void ball_move_r_f(int value);
void ball_move_l_f(int value);
void set_new_objects();
/* funkcija koja registruje kada se pusti taster na tastaturi koji smo drzali */
void keyboard_up(unsigned char key, int x, int y);


void draw_cube();
void draw_floor_1();
void draw_floor_2();


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
	glutKeyboardUpFunc(keyboard_up);

	/* 
	 * funkcija koja ne dozvoljava ponavljanje karaktera koji drzimo
	 * na ulazu, registruje ga samo jednom.
	 */
	glutIgnoreKeyRepeat(1);


	/* Na pocetku kretanje loptice levo i desno je false */
	ball_move_r = false;
	ball_move_l = false;

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
	case 'l':
		if (!ball_move_r){
			glutTimerFunc(20, ball_move_r_f, 1);
			ball_move_r = true;
		}
		break;
	case 'j':
		if (!ball_move_l){
			glutTimerFunc(20, ball_move_l_f, 2);
			ball_move_l = true;
		}
		break;
    }
}

/*
 * funkcija koja registruje pustanje tastera sa tastature.
 * Kada se to desi hocemo da prekinemo animaciju.
 */
void keyboard_up(unsigned char key, int x, int y){
	(void)x;
	(void)y;
	switch (key){
	case 'l':
		ball_move_r = false;
		break;
	case 'j':
		ball_move_l = false;
		break;
	}
}

//funkcija za implementaciju skoka loptice 
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


void floor_move_period(void){
	//funkcija za pomeranje podloge

	if (br >= 0.7){
		/*
		 * Podlogu transliramo ulevo samo ako se sama loptica
		 * pomera udesno i presla je polovinu ekrana
		 */
		if (ball_move_r){
			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glTranslatef(-0.02, 0, 0);
			glPopMatrix();

			//  povecavamo move za 0.02 kako bi loptica ostala 
			//  u istoj poziciji
			move += 0.02;
		}

		/*
		 * ako move dodje u celobrojnu vrednost i ta celobrojna vrednost je
		 * deljiva sa 6(duzinu polovine poligona) treba da pomerimo poligon udesno
		 */
		if (((move - (int)move) < 0.02)&&((int)(move+6) % 6 == 0)){ 
			//i - promenljiva koja odredjuje x koordinatu crtanja poligona
			i+=6;
			set_new_objects();
		}
	}else{
		br += 0.02;
	}
}

void ball_move_r_f(int value){
	if (value != 1) return;

	glutPostRedisplay();

	if (br <= 0.7) {
		/*
		 * dokle god loptica ne dodje u polozaj da kamera treba
		 * da je prati, izvrsavamo samo pomeranje loptice
		 */
		move += 0.02;
		br = br + 0.02;
		if (ball_move_r){
			glutTimerFunc(20,ball_move_r_f, 1);
		}
		else{
			ball_move_r = false;
		}
	}
	else {
		/*
		 * Kada loptica dodje u odredjenu poziciju, kamera pocinje
		 * da je "prati", i poziva se funkcija za kretanje podloge u levo
		 * a loptica stoji u mestu zajedno sa kamerom fiksirana
		 */
		floor_move_period();
		br = br + 0.02;
		if (ball_move_r){
			glutTimerFunc(20,ball_move_r_f, 1);
		}
		else{
			ball_move_r = false;
		}
	}

}

void ball_move_l_f(int value){
	if (value != 2) return;

	glutPostRedisplay();

	if (br <= 0.7) {
		/*
		 * dokle god loptica ne dodje u polozaj da kamera treba
		 * da je prati, izvrsavamo samo pomeranje loptice
		 */
		move -= 0.02;
		br = br - 0.02;
		if (ball_move_l){
			glutTimerFunc(20,ball_move_l_f, 2);
		}
		else{
			ball_move_l = false;
		}
	}
	else {
		/*
		 * Kada loptica dodje u odredjenu poziciju, kamera pocinje
		 * da je "prati", i poziva se funkcija za kretanje podloge u levo
		 * a loptica stoji u mestu zajedno sa kamerom fiksirana
		 */
		floor_move_period();
		br = 0.7 - 0.02;
		move -= 0.02;
		if (ball_move_l){
			glutTimerFunc(20,ball_move_l_f, 2);
		}
		else{
			ball_move_l = false;
		}
	}

}

static void on_reshape(int width, int height)
{
	/* pamtimo sirinu i visinu prozora */
	
	glViewport(0,0,width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/height, 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.7,0.3,1.22,
			  0.7,0.3,0,
			  0,1,0);
}

void draw_floor_2(){

	/* Crtamo podlogu */

	glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glVertex3f(i+4 , -0.05, -0.2);
		glVertex3f(i+4 , -0.05,  0.2);
		glVertex3f(i+10, -0.05,  0.2);
		glVertex3f(i+10, -0.05, -0.2);
	glEnd();
	
	/* crtanje prednje ivice podloge*/
	glBegin(GL_POLYGON);
		glNormal3f(0,0,-1);
		glVertex3f(i+4 , -0.05, 0.2 );
		glVertex3f(i+4 ,    -5, 0.2 );
		glVertex3f(i+10,    -5, 0.2 );
		glVertex3f(i+10, -0.05, 0.2 );
	glEnd();

	//iscrtavanje prepreka 
	glPushMatrix();
		glTranslatef(i+((float)prvi*(1/10.0)) + 1,0.1,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(i+((float)drugi*(1/10.0)) + 2,0.25,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(i+((float)treci*(1/10.0)) + 3,0.2,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(i+(float)cetvrti*(1/10.0) + 4,0.3,0);
		glScalef(2,1,1);
		draw_cube();
	glPopMatrix();
}
	
void draw_floor_1(){

	/* Crtamo podlogu */
	glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glVertex3f(i-2, -0.05, -0.2);
		glVertex3f(i-2, -0.05,  0.2);
		glVertex3f(i+4, -0.05,  0.2);
		glVertex3f(i+4, -0.05, -0.2);
	glEnd();
	
	/* crtanje prednje ivice podloge*/
	glBegin(GL_POLYGON);
		glNormal3f(0,0,-1);
		glVertex3f(i-2, -0.05, 0.2 );
		glVertex3f(i-2,    -5, 0.2 );
		glVertex3f(i+4,    -5, 0.2 );
		glVertex3f(i+4, -0.05, 0.2 );
	glEnd();

}

void set_new_objects(){
	/* postavljamo seme za random funkciju */
	srand(time(NULL));

	int k = rand();
	prvi = k%10;
	drugi = (k/10)%10;
	treci = (k/100)%10;
	cetvrti = (k/1000)%10;
}

void draw_sphere(){
	/* ambijentalna refleksija za sferu*/
	GLfloat ambient_lopta[] = {1, 0, 0, 1};      
	                                                      
	/* difuzna refleksija za sferu*/
	GLfloat diffuse_lopta[] = {1, 0, 0, 1};        

	/* spekularna refleksija za sferu*/
	GLfloat specular_lopta[] = { 1, 1, 1, 1 };           

	GLfloat shininess = 100;

	glPushMatrix();
	/* crtamo sferu */
	glTranslatef(move, sin(jump*pi / 180)*0.6, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_lopta);  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_lopta);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_lopta);
	glMaterialf( GL_FRONT, GL_SHININESS, shininess);      

    glutSolidSphere(0.05, 50,50);
	
	glPopMatrix();
}


void draw_cube(){
	// Funkcija za crtanje kocke
	glPushMatrix();
	glScalef(1,0.2,1);
	glutSolidCube(0.2);
	glPopMatrix();
}

static void on_display(void)
{
	/* postavljamo poziciju svetla */
	GLfloat light_position[] = {0,2,6,0};
														
	/* Ambijentalna boja svetla. */                       
	GLfloat light_ambient[] = { 0, 0, 0, 1 };       
	                                                      
	/* Difuzna boja svetla. */                            
	GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };       
	                                                      
	/* Spekularna boja svetla. */                         
	GLfloat light_specular[] = { 1, 1, 1, 1 };      

	// ----------------------------------------------------------

	/* ambijentalna refleksija za poligon */
	GLfloat ambient_coeffs[] = {(float)66/255, (float)244/255,(float)92/255, 1};      

	/* difuzna refleksija za poligon */
	GLfloat diffuse_coeffs[] = {(float)80/255, (float)244/255,(float)69/255,0, 0, 0, 1};        

	/* spekularna refleksija za poligon */
	GLfloat specular_coeffs[] = { 0, 0, 0, 1 };           


	/* Koeficijent glatkosti materijala. */               
	GLfloat shininess = 100;

	/* postavljanje svetla 0*/
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
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
    gluLookAt(0.7,0.3,1.22,
			  0.7,0.3,0,
			  0,1,0);

	draw_floor_1();
	draw_floor_2();


	draw_sphere();
    
	
	/* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
