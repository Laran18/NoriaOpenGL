/*
*	Noria.cpp
*	Programa para dibujar y animar una noria
*
*	@author	scuesan
*/

// Bibliotecas de C++
#include <iostream>

// Bibliotecas graficas
#include <Utilidades.h>

#include <sstream>

//Variables dependientes del tiempo
static float angulo = 0;
static int tasaFps = 60;



using namespace std;

// Globales
static float long_barra_base = 1.3;
static float ancho_noria = 0.3;
static float radio_noria = 1;
static float N = 3; //Numero de cruces en una malla
static const int num_cabinas = 19;
static const float velocidad = 0.1;
static const int angulo_balanceo = 20;
static GLuint barra_base;
static GLuint lado_base;
static GLuint eje_base;
static GLuint base;
static GLuint cruz;
static GLuint malla;
static GLuint cabina;
static GLuint rueda;
static float balanceo[num_cabinas];
static float vel_balanceo[num_cabinas];
static float camara[3];
static float camx = 5;
static float camy = 0;
static float camz = 15;
static float radio_cam;




void init() {
	//funcion propia de inicializaciones

	glEnable(GL_DEPTH_TEST);

	//glutWireTorus

	//Barra individual de la base-----------------
	barra_base = glGenLists(1);
	glNewList(barra_base, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glColor3f(0.9,0.9,0.9);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.04, long_barra_base, 20, 20);

	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------



	//Lado de la base-----------------------------
	lado_base = glGenLists(1);
	glNewList(lado_base, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//Barra izquierda
	glPushMatrix();
	glRotatef(-30, 1, 0, 0);
	glCallList(barra_base);
	glPopMatrix();

	//Barra derecha
	glPushMatrix();
	glRotatef(30, 1, 0, 0);
	glCallList(barra_base);
	glPopMatrix();

	//Barra atravesada
	glPushMatrix();
	static float desp = -(cos(30 * PI / 180) * (long_barra_base / 1.5));
	glScalef(1.0, 1.0, (sin(30 * PI / 180) * (long_barra_base/1.5) *2)/long_barra_base);
	glTranslatef(0.0, desp, 0.0);
	glTranslatef(0.0, 0.0, long_barra_base/2);
	glRotatef(90, 1, 0, 0);
	glCallList(barra_base);
	glPopMatrix();

	//Barra inclinada derecha
	static float h = cos(30 * PI / 180) * long_barra_base;
	static float escalado1 = h / long_barra_base;
	static float x = h / cos(15 * PI / 180);
	static float escalado2 = x / h;
	static float y = h / cos(15 * PI / 180);
	static float escalado3 = y / x * 1.1;
	glPushMatrix();
	glRotatef(15, 0, 0, 1);
	glRotatef(-15, 1, 0, 0);
	glScalef(1.0, escalado3, 1.0);
	glScalef(1.0,escalado2, 1.0);
	glScalef(1.0, escalado1, 1.0);
	glCallList(barra_base);
	glPopMatrix();

	//Barra inclinada izquierda
	glPushMatrix();
	glRotatef(15, 0, 0, 1);
	glRotatef(15, 1, 0, 0);
	glScalef(1.0, escalado3, 1.0);
	glScalef(1.0, escalado2, 1.0);
	glScalef(1.0, escalado1, 1.0);
	glCallList(barra_base);
	glPopMatrix();

	//Barras union
	glPushMatrix();
	glTranslatef(0, desp, tan(15*PI/180)*desp);
	glRotatef(90, 0,0,1);
	glScalef(1.0, (tan(15*PI/180)*(-desp)) / long_barra_base, 1.0);
	glCallList(barra_base);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, desp, tan(15 * PI / 180) * (-desp));
	glRotatef(90, 0, 0, 1);
	glScalef(1.0, (tan(15 * PI / 180) * (-desp)) / long_barra_base, 1.0);
	glCallList(barra_base);
	glPopMatrix();


	//Cubo barra izquierda
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(0.0, -h, sqrt(pow(long_barra_base, 2) - pow(h, 2)));
	glutSolidCube(0.15);
	glPopMatrix();

	//Cubo barra derecha
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(0.0, -h, -sqrt(pow(long_barra_base, 2) - pow(h, 2)));
	glutSolidCube(0.15);
	glPopMatrix();

	//Cubo barra izquierda inclinada
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(tan(15*PI/180)*h, -h, sqrt(pow(x, 2) - pow(h, 2)));
	glutSolidCube(0.15);
	glPopMatrix();

	//Cubo barra derecha inclinada
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(tan(15 * PI / 180)* h, -h, -sqrt(pow(x, 2) - pow(h, 2)));
	glutSolidCube(0.15);
	glPopMatrix();



	glPopAttrib();
	glPopMatrix();
	glEndList();
	//--------------------------------------------

	//Eje central de la base----------------------
	eje_base = glGenLists(1);
	glNewList(eje_base, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//Cilindro central
	glPushMatrix();
	glColor3f(0.9,0.9,0.9);
	glTranslatef(0.0,0.0,-(ancho_noria + 0.31)/2);
	glutSolidCylinder(0.1, (ancho_noria + 0.3), 20, 20);
	glPopMatrix();

	//Tapa lateral
	glPushMatrix();
	glColor3f(0.8, 0.8, 0.8);
	glTranslatef(0.0, 0.0, (ancho_noria + 0.31) /2);
	glScalef(1.0,1.0,0.1);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//Tapa lateral
	glPushMatrix();
	glColor3f(0.8, 0.8, 0.8);
	glTranslatef(0.0, 0.0, -(ancho_noria + 0.31) / 2);
	glScalef(1.0, 1.0, 0.1);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------


	//Base----------------------------------------
	base = glGenLists(1);
	glNewList(base, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//Lateral 
	glPushMatrix();
	glTranslatef(0.0, -0.1, -(ancho_noria+0.2)/ 2);
	glRotatef(90, 0, 1, 0);
	glCallList(lado_base);
	glPopMatrix();

	//Lateral
	glPushMatrix();
	glTranslatef(0.0, -0.1, (ancho_noria+0.2) / 2);
	glRotatef(-90, 0, 1, 0);
	glCallList(lado_base);
	glPopMatrix();

	//Eje
	glPushMatrix();
	glCallList(eje_base);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------



	//Cruz----------------------------------------
	cruz = glGenLists(1);
	glNewList(cruz, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//Barra arriba
	glPushMatrix();
	glutSolidCylinder(0.01, ancho_noria, 20, 20);
	glPopMatrix();

	//Barra abajo
	glPushMatrix();
	glTranslatef(0,-radio_noria / (N + 1),0);
	glutSolidCylinder(0.01, ancho_noria, 20, 20);
	glPopMatrix();

	//Barra derecha
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, radio_noria/(N+1), 20, 20);
	glPopMatrix();

	//Barra izquierda
	glPushMatrix();
	glTranslatef(0,0, ancho_noria);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, radio_noria / (N + 1), 20, 20);
	glPopMatrix();

	//Barra diagonal
	static float hipotenusa = sqrt(pow(radio_noria / (N + 1), 2) + pow(ancho_noria, 2));
	glPushMatrix();
	glRotatef(90 - (acos((radio_noria/(N+1)) / hipotenusa)*180/PI), 1, 0, 0);
	glutSolidCylinder(0.01, hipotenusa, 20, 20);
	glPopMatrix();

	//Barra diagonal
	glPushMatrix();
	glTranslatef(0,0,ancho_noria);
	glRotatef(180 - (90 - (acos((radio_noria / (N + 1)) / hipotenusa) * 180 / PI)), 1, 0, 0);
	glutSolidCylinder(0.01, hipotenusa, 20, 20);
	glPopMatrix();


	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------

	//Malla---------------------------------------
	malla = glGenLists(1);
	glNewList(malla, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glColor3f(1.0, 1.0, 1.0);

	//Cruces
	for (auto i = 0; i < N; i++) {
		glPushMatrix();
		glTranslatef(0,-(radio_noria/(N+1))*i, 0);
		glCallList(cruz);
		glPopMatrix();
	}

	//Barra derecha
	glPushMatrix();
	glTranslatef(0, -((radio_noria / (N+1)) * (N)), 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, radio_noria / (N+1), 20, 20);
	glPopMatrix();

	//Barra izquierda
	glPushMatrix();
	glTranslatef(0, -((radio_noria/(N+1))*(N)), ancho_noria);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, radio_noria / (N+1), 20, 20);
	glPopMatrix();

	//Barra de cierre
	glPushMatrix();
	glTranslatef(0, -(radio_noria), 0);
	glutSolidCylinder(0.01, ancho_noria, 20, 20);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------


	//Cabina--------------------------------------
	cabina = glGenLists(1);
	glNewList(cabina, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glColor3f(0.6, 0.6, 0.6);
	glTranslatef(0, -((radio_noria / N - 0.1) / 3) * 2 - 0.05, 0);

	//Cubo
	glPushMatrix();
	glScalef(1, ((radio_noria/(N+1)-0.1) * 2 / 3)/(ancho_noria-0.1), 1);
	glutSolidCube(ancho_noria-0.1);
	glPopMatrix();

	//Soporte vertical abajo izq (visto desde un punto de vista +x, +y, z=0)
	glPushMatrix();
	glTranslatef((ancho_noria-0.1) / 2, ((radio_noria / (N) - 0.1) * 2 / 3), (ancho_noria - 0.1) / 2);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, (radio_noria/(N+2)), 20, 20);
	glPopMatrix();

	//Soporte verical arriba izq
	glPushMatrix();
	glTranslatef(-(ancho_noria - 0.1) / 2, ((radio_noria / (N)-0.1) * 2 / 3), (ancho_noria - 0.1) / 2);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, (radio_noria / (N+2)), 20, 20);
	glPopMatrix();

	//Soporte vertical arriba der
	glPushMatrix();
	glTranslatef(-(ancho_noria - 0.1) / 2, ((radio_noria / (N)-0.1) * 2 / 3), -(ancho_noria - 0.1) / 2);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, (radio_noria / (N + 2)), 20, 20);
	glPopMatrix();

	//Soporte vertical abajo der
	glPushMatrix();
	glTranslatef((ancho_noria - 0.1) / 2, ((radio_noria / (N)-0.1) * 2 / 3), -(ancho_noria - 0.1) / 2);
	glRotatef(90, 1, 0, 0);
	glutSolidCylinder(0.01, (radio_noria / (N + 2)), 20, 20);
	glPopMatrix();

	//Techo
	glPushMatrix();
	glTranslatef(0, ((radio_noria / N - 0.1) / 3) * 2, 0); //0.95* 1/3 del alto de la cabina ¿?
	glScalef(1, 0.1, 1);
	glutSolidCube(ancho_noria - 0.1);
	glPopMatrix();

	//Punto de union con la barra de la rueda
	glPushMatrix();
	glTranslatef(0, ((radio_noria / N - 0.1) / 3) * 2 + 0.03, 0);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();


	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------

	//Rueda---------------------------------------
	rueda = glGenLists(1);
	glNewList(rueda, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glColor3f(0.9, 0.9, 0.9);

	//Torus izquierda
	glPushMatrix();
	glTranslatef(0, 0, ancho_noria/2);
	glutSolidTorus(0.02, radio_noria, 20, num_cabinas);
	glPopMatrix();

	//Torus derecha
	glPushMatrix();
	glTranslatef(0, 0, -ancho_noria / 2);
	glutSolidTorus(0.02, radio_noria, 20, num_cabinas);
	glPopMatrix();

	//Barras radiales 
	for (auto i = 0; i < num_cabinas; i++) {
		glPushMatrix();
		glRotatef(i*360/num_cabinas, 0, 0, 1);
		glTranslatef(0, 0, -ancho_noria / 2);
		glCallList(malla);
		glPopMatrix();

	}

	glPopMatrix();
	glPopAttrib();
	glEndList();
	//--------------------------------------------



}

void fps() {
	//Muestra en el titulo de la ventana los fps actuales
	int ahora, tiempo_transcurrido;
	static int antes = glutGet(GLUT_ELAPSED_TIME);	
	static int fotogramas = 0;

	//Cada vez que llamo a fps se incrementa los fotogramas
	fotogramas++;

	//Calculo el tiempo que ha transcurrido entre dos fotogramas
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	//Si ha transcurrido más de 1 segundo, muestro los fps y reinicio el reloj
	if (tiempo_transcurrido > 1000) {
		//Modifico el titulo de la ventana
		stringstream titulo;
		titulo << "FPS = " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());

		//Reiniciar el reloj y los fotogramas
		fotogramas = 0;
		antes = ahora;
	}
}

void display()
{
	// Callback de dibujo
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Situar camara -> origen, punto interes, vertical subjetiva
	gluLookAt(camara[0], camara[1], camara[2], 0, 0.0, 0.0, 0, 1, 0);	//(0,1,0) Es la vertical que va a ver la camara (rotacion de la camara) 

	//ejes();

	glPushMatrix();
	glColor3f(0.0,0.5,0.5);
	//glutWireSphere(1, 20, 20);
	glPopMatrix();

	//Noria---------------------------------------
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//Rueda
	glPushMatrix();
	glRotatef(angulo, 0, 0, 1);
	glCallList(rueda);
	glPopMatrix();

	//Base
	glPushMatrix();
	glCallList(base);
	glPopMatrix();

	//Cabinas
	glPushMatrix();
	for (auto i = 0; i < num_cabinas;i++) {
		glPushMatrix();
		glRotatef(angulo + i * 360 / num_cabinas, 0, 0, 1);
		glTranslatef(0, -radio_noria, 0);
		glRotatef(-balanceo[i] - angulo - i * 360 / num_cabinas, 0, 0, 1);
		glScalef(0.75, 0.75, 0.75);
		glCallList(cabina);
		glPopMatrix();
	}
	glPopMatrix();
	glPopAttrib();
	//--------------------------------------------
	

	// Renovar el dibujo y mover el buffer trasero al frontal 
	glutSwapBuffers();

	fps();

}

void reshape(GLint w, GLint h)
{
	// w,h son las nuevas dimensiones de la ventana
	float ar = float(w) / h; //Relacion de aspecto

	//Mantenimiento de la relacion de aspecto para perspectiva (para ortografico poner tambien glViewport)
	glViewport(0, 0, w, h); 

	//Seleccionar camara: definir el volumen de la vista
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Ángulo de la perspectiva
	float ang = asin((long_barra_base+0.2)/radio_cam)*180/PI;


	//Perspectiva (relacion de aspecto -> float(w)/h)
	gluPerspective(2*ang, ar, 0.1, 100);
}

void onIdle() {
	//Callback de atención al evento idle

	//Sin control del tiempo
	//angulo += 0.1;

	//Con control del tiempo
	static const float vueltasSg = 0.06; //Vueltas por segundo
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);

	//Incremento de la variable temporal
	//delta = vueltas/sg * 360 * tiempo_en_segundos_transcurrido
	angulo += vueltasSg * 360 * (ahora - antes) / 1000;

	//Balanceo de las cabinas
	for (auto i = 0; i < num_cabinas;i++) {
		balanceo[i] += vel_balanceo[i] * 360 * (ahora - antes) / 1000; //50 en vez 360
		if (balanceo[i] >= angulo_balanceo) {
			vel_balanceo[i] = -velocidad;
		}
		else if (balanceo[i] <= -angulo_balanceo) {
			vel_balanceo[i] = velocidad;
		}
	}

	//Rotación de la cámara
	camara[0] = radio_cam * sin(angulo*PI/180);
	camara[2] = radio_cam * cos(angulo*PI/180);

	antes = ahora;

	//Encolar un evento de dibujo de display
	glutPostRedisplay();
}

void onTimer(int tiempo) {
	//Callback de atencion a la cuenta atras de un timer

	//Encolarse a sí misma
	glutTimerFunc(tiempo, onTimer, tiempo);

	onIdle();

}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);


	//Se inicia la posicion inicial de la camara
	camara[0] = camx;
	camara[1] = camy;
	camara[2] = camz;
	radio_cam = sqrt(pow(camx, 2) + pow(camy, 2) + pow(camz, 2));

	//Se inicializan los valores iniciales del balanceo de cada cabina 
	int valor;
	for (auto i = 0; i < num_cabinas;i++) {
		//Creamos un valor aleatorio dentro del rango del angulo limite del balanceo
		valor = rand() % (2*angulo_balanceo);
		balanceo[i] = valor - angulo_balanceo;
		
		valor = rand() % 10;
		if (valor >= 5) {
			vel_balanceo[i] = velocidad;
		}
		else {
			vel_balanceo[i] = -velocidad;
		}
	}

	// Crear la ventana
	glutCreateWindow("Noria");

	//init se tine que llamar después de crear la ventana
	init();

	// Seccion de registrode callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000 / tasaFps, onTimer, 1000 / tasaFps); 

	// Pone en marcha el bucle de atencion de evntos
	glutMainLoop();
}

