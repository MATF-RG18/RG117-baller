#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<stdbool.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include"pomocne.h"
#include "object.h"
#include "image.h"

// funkcija za inicijalizaciju teksture
static void initialize();

GLuint names;
//funkcija za iscrtavanje pozadine
void texture_pozadina(GLuint names);

#define pi 3.141592653589793
double jump = 0;
double move = 0;

#define POZADINA "pozadina3.bmp"

bool ball_jump= false;
bool ball_move_r= false;
bool ball_move_l= false;
float br = 0;
bool ball_free_fall = false;
//promenljiva koja odredjuje da li je skok pokrenut sa police
bool jump_from_hight = false;
// Posto se trenutno u poligon_x nalaze koordinate celih brojeva
// tj prepreke/police se iscrtavaju na koordinatama celih brojeva
// sirina_prepreke_min i sirina_prepreke_max oznacavaju koliko blizu
// loptica treba da se nalazi prepreci dok je u vazduhu da bi pri sletanju
// ostala na visini police umesto da prodje kroz nju i vrati se na poligon
double sirina_prepreke_min = 0.11;
double sirina_prepreke_max = 0.89;

//na_podlozi - promenljiva koja kada se lopta nalazi na podlozi 
//predstavlja visinu podloge i menja se kada je loptica na polici/prepreci
//i postaje jednaka visini prepreke. Onda i skok sa prepreke se implementira
//iz te pozicije umesto sa podloge (Ox=0).
double na_podlozi;

// promenljive za prikazivanje promenljivih
char tekst_poeni[100];
double br_poena=0;
double maks_poena = 0;

//promenljiva koja ce pamtiti poziciju sa koje je lopta skocila sa police kako bi izracunali da li je sletela
//na istu policu ili je skocila na policu ispred(ili eventualno preskocila i skocila na drugu po redu)
int pozicija_sa_koje_skace_loptica = 0;

//promenljiva koja odredjuje x koordinatu teksta sa poenima
double pos_score = 1;

//i - promenljiva koja odredjuje x koordinatu crtanja poligona
int i = 0;

// pocetna_pozicija promenljiva koja pomaze pri brojanju poena
double pocetna_pozicija = 0;

//promenljiva koja odredjuje kada pocinju ispocetka da se broje poeni
bool brojanje_pocinje = false;

// pomeraj loptice (brzina loptice na poligonou.)
double pomeraj_loptice = 0.02;

//h - promenljiva koja odredjuje visinu na kojoj loptica stoji
double h = 0;

// promenljiva koja odredjuje da li smo prvi put skocili na policu(detaljniji opis u
// funkciji 'animiraj_slobodan_pad')
bool prvi_skok = true;

// y koordinata loptice koja sluzi za simulaciju skoka
double ball_y_coord = 0;

// niz x i y koordinata za poligon
double broj_prepreka = 20;
double koordinata_poslednje_prepreke = 22;
int pomeraj = 10;
double *poligon_x, *poligon_y;

void alociraj_nizove(){
	poligon_x = (double*)malloc(broj_prepreka*sizeof(double));
	poligon_y = (double*)malloc(broj_prepreka*sizeof(double));

	for (int i=0; i<broj_prepreka; i++){
		poligon_x[i] = i+2;
		poligon_y[i] = 0.1;
	}
}

// promenljive za poziciju lopte pri skoku
int degree = 180;
double start_jump_pos = 0;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
void ball_jump_f(int value);
void ball_move_r_f(int value);
void ball_move_l_f(int value);
/* funkcija koja registruje kada se pusti taster na tastaturi koji smo drzali */
void keyboard_up(unsigned char key, int x, int y);
// funkcija koja implementira slobodan pad sa podloge.
void free_fall_f(int value);

// funkcija koja odredjuje da li se x kao koordinata nalazi u poziciji gde je polica
bool pozicija(double x);

// funkcija koja implementira ostajanje loptice na polici kada se nadje iznad iste
void provera_iznad_police();

// funkcija animiraj_slobodan_pad ima opis unutar definicije
void animiraj_slobodan_pad();

//prikazivanje poena i najboljeg rezultata
void tekst_trenutni_poeni_f(const char* s);
void tekst_maks_poeni_f(const char* s);

int main(int argc, char **argv)
{
	// prvo alociraj nizove koji imaju koordinate za iscrtavanje prepreka.
	alociraj_nizove();

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

	/* inicijalizacija teksture*/
	initialize();

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.75, 0.75, 0);

	


    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

// Funkcija za ispis koliko je trenutno osvojeno poena
void tekst_trenutni_poeni_f(const char* s) {
    /*iskljucujemo osvetljenje */
	glDisable(GL_LIGHTING);


	/*boja teksta.*/
    glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,1,1);

	glPushMatrix();
	//postavljamo poziciju teksta
	//posto hocemo da prati lopticu x-koordinata mora da zavisi od
	//pozicije loptice.
	glRasterPos2f(pos_score,0.8);
	glPopMatrix();
    int duzina=(int)strlen(s);
    for(int i=0;i<duzina;++i){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    /*iskljucujemo GL_COLOR_MATERIAL i Ukljucujemo opet svetlo. */
    glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}

// Funkcija za ispis koliko je maksimalno osvojeno poena
void tekst_maks_poeni_f(const char* s) {
    /*iskljucujemo osvetljenje */
	glDisable(GL_LIGHTING);


	/*boja teksta.*/
    glEnable(GL_COLOR_MATERIAL);
	glColor3f(1,1,1);

	glPushMatrix();
	//postavljamo poziciju teksta
	//posto hocemo da prati lopticu x-koordinata mora da zavisi od
	//pozicije loptice.
	glRasterPos2f(pos_score,0.7);
	glPopMatrix();
    int duzina=(int)strlen(s);
    for(int i=0;i<duzina;++i){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    /*iskljucujemo GL_COLOR_MATERIAL i Ukljucujemo opet svetlo. */
    glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}

static void on_keyboard(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
    switch (key) {
    case 27:
	case 'q':
        /* Zavrsava se program. */
		glDeleteTextures(2, &names);
        exit(0);
        break;
	case 32:
		// implementacija skoka
		if (!ball_jump){
			if (na_podlozi > 0) jump_from_hight = true;
			glutTimerFunc(30, ball_jump_f, 5);
			ball_jump = true;
		}
		break;
	case 'l':
		// implementacija kretanja udesno
		if (!ball_move_r){
			glutTimerFunc(20, ball_move_r_f, 1);
			ball_move_r = true;
		}
		break;
	case 'j':
		// implementacija kretanja ulevo
		if (!ball_move_l){
			glutTimerFunc(20, ball_move_l_f, 2);
			ball_move_l = true;
		}
		break;
	case 'f':
		pomeraj_loptice = 0.1;
		break;
    }
}

/*
 * funkcija koja registruje pustanje tastera sa tastature.
 * Kada se to desi hocemo da prekinemo animaciju tj prekinemo kretanje(kretanje se
 * dogadja samo kada se drze tasteri l i j).
 */
void keyboard_up(unsigned char key, int x, int y){
	//da nam -Wall i -Wextra ne bi prijavljivali warning kastujemo u void x i y.
	(void)x;
	(void)y;
	switch (key){
	case 'l':
		ball_move_r = false;
		break;
	case 'j':
		ball_move_l = false;
		break;
	case 'f':
		pomeraj_loptice = 0.02;
		break;
	}
}

void provera_iznad_police(){
	// ova funkcija ce imati efekat i izvrsiti nesto samo u slucaju da se
	// loptica nadje iznad podloge u letu i treba da "ostane na polici".

	if (pozicija(move)){
 		if (jump <= 24 && jump > 23){
			// u ovaj deo se ulazi ako je loptica u letu i nalazi se iznad police
			// onda se pocetna visina lopte povecava na 0.17 sto je visina police
			// da bi dala efekat da loptica stoji na polici. Takodje se prekida
			// animacija zato sto je loptica vrlo blizu podloge pa je neprimetan prekid.
			// (kada jump predje 24 onda 7*jump prelazi 180 stepeni pa se animacija sama po
			// sebi prekida.
			na_podlozi = 0.17;
			//promenljiva koja ce da se aktivirati tek kada loptica prvi put skoci na policu
			//Njom cemo da implementiramo to da se prvi skok na policu ne racuna kao dodatan poen
			jump = 0;
			ball_jump=false;
			if (!brojanje_pocinje){
				brojanje_pocinje = true;
			}
			//pozicija sa koje loptica skace se pamti
			if (prvi_skok) pozicija_sa_koje_skace_loptica = (int)(move + 0.15);
		}
	}else{
		if (!jump_from_hight)
			na_podlozi = 0;
	}
}

void provera_ispod_police(){
	// funkcija koja proverava da li je loptica pri skoku ispod police
	// i ne dozvoljava skok ako jeste.
	if (pozicija(move) && jump == 1 && na_podlozi == 0){
		jump = 25;
	}
}

//funkcija za implementaciju skoka loptice 
void ball_jump_f(int value){
	if (value != 5) return;

	jump += 1; 

	provera_iznad_police();

	provera_ispod_police();

	glutPostRedisplay();

	if (((ball_jump) && (jump*7 <= degree))||((jump_from_hight && ball_jump && jump*7 < degree + 20))){
		glutTimerFunc(30, ball_jump_f, 5);
	}
	else {
		jump = start_jump_pos;

		// ne zelimo da uvecavamo broj poena kada prvi put skocimo na policu
		if (brojanje_pocinje){
			if (!prvi_skok) {
				if ((int)(move + 0.15) > pozicija_sa_koje_skace_loptica){
					br_poena +=(int)(move + 0.15) - pozicija_sa_koje_skace_loptica;
					pozicija_sa_koje_skace_loptica = (int)(move+0.15);
				}
			}else{
				prvi_skok = false;
			}
		}

		// deo koda koji se izvrsava kada loptica skoci sa police na podlogu
		// tada se poeni resetuju
		if (jump_from_hight && !pozicija(move)) {
			na_podlozi = 0;
			brojanje_pocinje = false;
			if (br_poena > maks_poena) maks_poena = br_poena-1;
			prvi_skok = true;
			br_poena = 0;
		}
		ball_jump = false;
		jump_from_hight = false;
	}
} 
bool pozicija(double x){
	return (move >= 1.8 && ((apsolutno((x - ((int)x+1))) <= sirina_prepreke_min) || (apsolutno(x - ((int)x+1))) >= sirina_prepreke_max));
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
			glTranslatef(-pomeraj_loptice, 0, 0);
			glPopMatrix();

			//  povecavamo move za pomeraj_loptice(0.02) kako bi loptica ostala 
			//  u istoj poziciji
			move += pomeraj_loptice;
		}

		/*
		 * ako move dodje u celobrojnu vrednost i ta celobrojna vrednost je
		 * deljiva sa 6(duzinu polovine poligona) treba da pomerimo poligon udesno
		 * Ovaj korak radimo kako bi ustedeli na prostoru i vremenu u iscrtavanju, pa da
		 * se ceo poligon ne bi iscrtavao iscrtavaju se samo delovi koji su vidljivi i blizu ivica
		 * ovo se najlakse moze videti kada postavite trecu koordinatu gluLookAt-a postavite na 5 i odaljite kameru.
		 */
		if (((move - (int)move) < pomeraj_loptice)&&((int)(move+6) % 6 == 0)){ 
			//i - promenljiva koja odredjuje x koordinatu crtanja poligona
			i+=6;
		}
	}else{
		br += pomeraj_loptice;
	}
}

void free_fall_f(int value){
	if (value != 55) return;

	glutPostRedisplay();

	if (ball_free_fall && ball_y_coord >= 0.05 && jump < 25){
		/* 
		 * sve dok koordinata lopte ne dodje do poda pusta se animacija
		 * za padanje loptice sa police*/
		++jump;
		glutTimerFunc(30, free_fall_f, 55);
	}else{
		ball_free_fall = false;
		ball_y_coord = start_jump_pos;
		jump = 0;
	}
}

void animiraj_slobodan_pad(){
// ime ove funkcije je samo deo funkcionalnosti funkcije(svi znamo koliko je tesko
// dati naziv funkciji. Ime se odnosi na drugi deo velikog if-a dok prvi deo ima svoj komentar)
	if (pozicija(move)){
 		if (jump <= 24 && jump > 23){
			/*
			 * u ovaj deo if-a se ulazi kada se loptica pomera udesno i pri skoku (u padu) se
			 * nadje tik iznad police, tada visinu iscrtavanja lopte uvecavamo kako bi
			 * simulirali da je loptica na polici.
			 */
			na_podlozi = 0.17;
		}
	}else{
		if (jump == start_jump_pos && ball_y_coord > 0){
			/*
			 * u ovaj deo koda se ulazi kada se pomeramo udesno sa lopticom na polici i
			 * dodjemo do ivice kada treba da se implementira slobodan pad loptice sa police
			 */
			na_podlozi = 0;
			brojanje_pocinje = false;
			if (br_poena > maks_poena) maks_poena = br_poena;
			prvi_skok = true;
			br_poena = 0;
			jump = 23.6;
			glutTimerFunc(30, free_fall_f, 55);
			ball_free_fall = true;
		}
	}
}

void ball_move_r_f(int value){
	if (value != 1) return;

	glutPostRedisplay();

	animiraj_slobodan_pad();

	if (br <= 0.7) {
		/*
		 * dokle god loptica ne dodje u polozaj da kamera treba
		 * da je prati, izvrsavamo samo pomeranje loptice - ovo je samo na pocetku.
		 */
		move += pomeraj_loptice;
		br = br +  pomeraj_loptice;
		if (ball_move_r){
			glutTimerFunc(20,ball_move_r_f, 1);
		}
	}
	else {
		/*
		 * Kada loptica dodje u odredjenu poziciju, kamera pocinje
		 * da je "prati", i poziva se funkcija za kretanje podloge u levo
		 * a loptica stoji u mestu zajedno sa kamerom fiksirana
		 */
		floor_move_period();
		br = br + pomeraj_loptice;
		if (ball_move_r){
			glutTimerFunc(20,ball_move_r_f, 1);
			pos_score += pomeraj_loptice;
		}
	}
}

void ball_move_l_f(int value){
	if (value != 2) return;

	glutPostRedisplay();

	animiraj_slobodan_pad();

	if (br <= 0.7) {
		/*
		 * dokle god loptica ne dodje u polozaj da kamera treba
		 * da je prati, izvrsavamo samo pomeranje loptice
		 */
		move -= pomeraj_loptice;
		br = br - pomeraj_loptice;
		if (ball_move_l){
			glutTimerFunc(20,ball_move_l_f, 2);
		}
	}
	else {
		/*
		 * Kada loptica dodje u odredjenu poziciju, kamera pocinje
		 * da je "prati", i poziva se funkcija za kretanje podloge u levo
		 * a loptica stoji u mestu zajedno sa kamerom fiksirana
		 */
		floor_move_period();
		br = 0.7 - pomeraj_loptice;
		move -= pomeraj_loptice;
		if (ball_move_l){
			glutTimerFunc(20,ball_move_l_f, 2);
		}
	}
}
static void initialize(void){
	/*Prekopirano sa casa*/
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Postavlja se boja pozadine. */
    glClearColor(0, 0, 0, 0);

    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(move, 0);

    /* Kreira se tekstura. */
    image_read(image, POZADINA);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(1, &names);

    glBindTexture(GL_TEXTURE_2D, names);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
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

void texture_pozadina(GLuint names){
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);     
	glBindTexture(GL_TEXTURE_2D, names);                  

	//funkcija koja iscrtava pozadinu
	glPushMatrix();
	//prvo ide dva iscrtavanja prva dva dela pozadine(teksture) posebno
	//posto ostale zavise od koordinata polica pa ce se automatski ponovo 
	//iscrtavati sa promenom istih koordinata
	glBegin(GL_QUADS);
		glNormal3f(0, 1,0);
		glTexCoord2f(0, 0);
		glVertex3f(-2, -0.5, -0.2);
		glTexCoord2f(0, 1);
		glVertex3f(-2, 1.5, -0.2);
		glTexCoord2f(1, 1);
		glVertex3f(0, 1.5, -0.2);
		glTexCoord2f(1, 0);
		glVertex3f(0, -0.5, -0.2);
	glEnd();
	glBegin(GL_QUADS);
		glNormal3f(0, 1,0);
		glTexCoord2f(0, 0);
		glVertex3f(0, -0.5, -0.2);
		glTexCoord2f(0, 1);
		glVertex3f(0, 1.5, -0.2);
		glTexCoord2f(1, 1);
		glVertex3f(2, 1.5, -0.2);
		glTexCoord2f(1, 0);
		glVertex3f(2, -0.5, -0.2);
	glEnd();

	for (int i=0; i<broj_prepreka; ++i){
		glBegin(GL_QUADS);
			glNormal3f(0, 1,0);
			glTexCoord2f(0, 0);
			glVertex3f(poligon_x[i], -0.5, -0.2);
			glTexCoord2f(0, 1);
			glVertex3f(poligon_x[i], 1.5, -0.2);
			glTexCoord2f(1, 1);
			glVertex3f(poligon_x[i] + 2, 1.5, -0.2);
			glTexCoord2f(1, 0);
			glVertex3f(poligon_x[i] + 2, -0.5, -0.2);
		glEnd();
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	GLfloat shininess = 60;

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
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
	glMaterialf (GL_FRONT,  GL_SHININESS, shininess);      

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesavamo pogled */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.7,0.3,1.3,
			  0.7,0.3,0,
			  0,1,0);

	// postavljanje teksture
	glPushMatrix();
		texture_pozadina(names);
	glPopMatrix();

	iscrtaj_prepreke(poligon_x, poligon_y, &move, &broj_prepreka, &koordinata_poslednje_prepreke, broj_prepreka);
	draw_floor_1(&i);
	draw_floor_2(&i);

	ball_y_coord = sin((jump*7)*pi / 180)*0.6 + na_podlozi;
	draw_sphere(&move, ball_y_coord);

	/* Poziv funkcije za ispis poena na ekran */
	sprintf(tekst_poeni, "Poeni: %.f", br_poena);
	tekst_trenutni_poeni_f(tekst_poeni);

	/* Poziv funkcije za ispis maksimalnog broja osvojenih poena do sada*/
	sprintf(tekst_poeni, "High score: %.f", maks_poena);
	tekst_maks_poeni_f(tekst_poeni);

	/* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
