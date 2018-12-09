#include<GL/glut.h>
#include<math.h>
#include<stdbool.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include "object.h"

#define pi 3.141592653589793

// promenljive koje odredjuju koordinate crtanja parova prepreka
int prvi = 3, drugi = 7, treci = 4, cetvrti = 8;

void draw_cube(){
	// Funkcija za crtanje kocke
	glPushMatrix();
	glScalef(1,0.2,1);
	glutSolidCube(0.2);
	glPopMatrix();
}

void draw_sphere(double* move, double* jump){
	/* ambijentalna refleksija za sferu*/
	GLfloat ambient_lopta[] = {1, 0, 0, 1};      
	                                                      
	/* difuzna refleksija za sferu*/
	GLfloat diffuse_lopta[] = {1, 0, 0, 1};        

	/* spekularna refleksija za sferu*/
	GLfloat specular_lopta[] = { 9/10, 0, 0, 1 };           

	GLfloat shininess = 5;

	glPushMatrix();
	/* crtamo sferu */
	glTranslatef(*move, sin((*jump)*pi / 180)*0.6, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_lopta);  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_lopta);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_lopta);
	glMaterialf( GL_FRONT, GL_SHININESS, shininess);      

    glutSolidSphere(0.05, 50,50);
	
	glPopMatrix();
}

void draw_floor_1(int* i){

	/* Crtamo podlogu */
	glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glVertex3f((*i)-2, -0.05, -0.2);
		glVertex3f((*i)-2, -0.05,  0.2);
		glVertex3f((*i)+4, -0.05,  0.2);
		glVertex3f((*i)+4, -0.05, -0.2);
	glEnd();
	
	/* crtanje prednje ivice podloge*/
	glBegin(GL_POLYGON);
		glNormal3f(0,0,-1);
		glVertex3f((*i)-2, -0.05, 0.2 );
		glVertex3f((*i)-2,    -5, 0.2 );
		glVertex3f((*i)+4,    -5, 0.2 );
		glVertex3f((*i)+4, -0.05, 0.2 );
	glEnd();

}

void draw_floor_2(int* i){

	/* Crtamo podlogu */

	glBegin(GL_POLYGON);
		glNormal3f(0,1,0);
		glVertex3f((*i)+4 , -0.05, -0.2);
		glVertex3f((*i)+4 , -0.05,  0.2);
		glVertex3f((*i)+10, -0.05,  0.2);
		glVertex3f((*i)+10, -0.05, -0.2);
	glEnd();
	
	/* crtanje prednje ivice podloge*/
	glBegin(GL_POLYGON);
		glNormal3f(0,0,-1);
		glVertex3f((*i)+4 , -0.05, 0.2 );
		glVertex3f((*i)+4 ,    -5, 0.2 );
		glVertex3f((*i)+10,    -5, 0.2 );
		glVertex3f((*i)+10, -0.05, 0.2 );
	glEnd();

	//iscrtavanje prepreka 
	glPushMatrix();
		glTranslatef((*i)+((float)prvi*(1/10.0)) + 1,0.1,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef((*i)+((float)drugi*(1/10.0)) + 2,0.25,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef((*i)+((float)treci*(1/10.0)) + 3,0.2,0);
		draw_cube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef((*i)+(float)cetvrti*(1/10.0) + 4,0.3,0);
		glScalef(2,1,1);
		draw_cube();
	glPopMatrix();
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
