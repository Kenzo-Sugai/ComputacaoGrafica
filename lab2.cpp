#include <GL/freeglut.h>
#include <iostream>

#define LAR 400
#define ALT 400

#define MAIORX 200.0f
#define MENORX -200.0f
#define MAIORY 200.0f
#define MENORY -200.0f

#define POSWX 250
#define POSWY 150

#define tang1x 10.0f
#define tang1y -145.0f
#define tang2x 10.0f
#define tang2y -185.0f

GLfloat B[4][2];

GLfloat P[4][4];

GLboolean GET_POINTS = 0;

GLint TOTAL_POINTS = 0;

GLfloat bx, by;

GLfloat angle, fAspect, rot = 0;

GLfloat angleX = 0, angleY = 0;

GLfloat dx = 0, dy = 0, dz = 0;

void myMousefuncBezierHermitInterate(int button, int state, int x, int y) {

	switch (button) {
	case GLUT_LEFT_BUTTON: {
		bx = x - (LAR / 2);
		by = (ALT / 2) - y;
		GET_POINTS = 1;
		break;
	}
	case GLUT_RIGHT_BUTTON: {
		GET_POINTS = 0;
		TOTAL_POINTS = 0;
		break;
	}
	case GLUT_MIDDLE_BUTTON: {
		break;
	}
	}

	std::cout << bx << " " << by << std::endl;

	glutPostRedisplay();

}

void DesenhaHermiteGrau3() {

	GLfloat H[4][4] = {
		{2.0f, -2.0f, 1.0f, 1.0f},
		{-3.0f, 3.0f, -2.0f, -1.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f}
	};

	if ((TOTAL_POINTS == 0) && (GET_POINTS)) {
		P[0][0] = bx;
		P[0][1] = by;
		P[0][2] = tang1x;
		P[0][3] = tang1y;
		TOTAL_POINTS += 1;
	}
	if ((TOTAL_POINTS > 0) && (GET_POINTS) && (TOTAL_POINTS < 2)) {

		if ((P[TOTAL_POINTS - 1][0] != bx) && (P[TOTAL_POINTS - 1][1] != by)) {
			P[TOTAL_POINTS][0] = bx;
			P[TOTAL_POINTS][1] = by;
			P[TOTAL_POINTS][2] = tang2x;
			P[TOTAL_POINTS][3] = tang2y;
			TOTAL_POINTS += 1;
			if (TOTAL_POINTS > 2) TOTAL_POINTS = 2;
		}
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	for (int i = 0; i < TOTAL_POINTS; i++) {
		glBegin(GL_POINTS);
		glVertex2f(P[i][0], P[i][1]);
		glEnd();
	}

	GLfloat T[4];
	GLfloat M[4];

	if (TOTAL_POINTS == 2) {
		GLfloat xini, yini, xfin, yfin;

		float t = 0.0;
		float passo = 0.01;

		xini = P[0][0] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + P[1][0] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + P[0][2] * (pow(t, 3) - 2 * pow(t, 2) + t) + P[1][2] * (pow(t, 3) - pow(t, 2));
		yini = P[0][1] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + P[1][1] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + P[0][3] * (pow(t, 3) - 2 * pow(t, 2) + t) + P[1][3] * (pow(t, 3) - pow(t, 2));

		for (float t = 0.0 + passo; t <= 1.0; t += passo) {

			xfin = P[0][0] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + P[1][0] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + P[0][2] * (pow(t, 3) - 2 * pow(t, 2) + t) + P[1][2] * (pow(t, 3) - pow(t, 2));
			yfin = P[0][1] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + P[1][1] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + P[0][3] * (pow(t, 3) - 2 * pow(t, 2) + t) + P[1][3] * (pow(t, 3) - pow(t, 2));

			glBegin(GL_LINE_STRIP);
			glVertex2f(xini, yini);
			glVertex2f(xfin, yfin);
			glEnd();

			xini = xfin;
			yini = yfin;

		}

	}

	std::cout << "TOTAL POINTS: " << TOTAL_POINTS << std::endl;

}

void DesenhaBezierGrau3() {

	GLfloat B[4][4] = {
		{-1.0f, 3.0f, -3.0f, 1.0f},
		{3.0f, -6.0f, 3.0f, 0.0f},
		{-3.0f, 3.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f}
	};

	if ((TOTAL_POINTS == 0) && (GET_POINTS)) {
		P[0][0] = bx;
		P[0][1] = by;
		P[0][2] = tang1x;
		P[0][3] = tang1y;
		TOTAL_POINTS += 1;
	}
	if ((TOTAL_POINTS > 0) && (GET_POINTS) && (TOTAL_POINTS < 4)) {

		if ((P[TOTAL_POINTS - 1][0] != bx) && (P[TOTAL_POINTS - 1][1] != by)) {
			P[TOTAL_POINTS][0] = bx;
			P[TOTAL_POINTS][1] = by;
			P[TOTAL_POINTS][2] = tang2x;
			P[TOTAL_POINTS][3] = tang2y;
			TOTAL_POINTS += 1;
			if (TOTAL_POINTS > 4) TOTAL_POINTS = 4;
		}
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5.0f);
	for (int i = 0; i < TOTAL_POINTS; i++) {
		glBegin(GL_POINTS);
		glVertex2f(P[i][0], P[i][1]);
		glEnd();
	}

	GLfloat T[4];
	GLfloat M[4];

	if (TOTAL_POINTS == 4) {
		GLfloat xini, yini, xfin, yfin;

		float t = 0.0;
		float passo = 0.01;

		xini = P[0][0] * pow((1 - t), 3) + (3 * P[1][0] * pow(t, 1) * pow((1 - t), 2)) + (3 * P[0][2] * pow(t, 2) * pow((1 - t), 1)) + P[1][2] * pow(t, 3);
		yini = P[0][1] * pow((1 - t), 3) + (3 * P[1][1] * pow(t, 1) * pow((1 - t), 2)) + (3 * P[0][3] * pow(t, 2) * pow((1 - t), 1)) + P[1][3] * pow(t, 3);

		for (float t = 0.0 + passo; t <= 1.0; t += passo) {

			xfin = P[0][0] * pow((1 - t), 3) + (3 * P[1][0] * pow(t, 1) * pow((1 - t), 2)) + (3 * P[0][2] * pow(t, 2) * pow((1 - t), 1)) + P[1][2] * pow(t, 3);
			yfin = P[0][1] * pow((1 - t), 3) + (3 * P[1][1] * pow(t, 1) * pow((1 - t), 2)) + (3 * P[0][3] * pow(t, 2) * pow((1 - t), 1)) + P[1][3] * pow(t, 3);

			glBegin(GL_LINE_STRIP);
			glVertex2f(xini, yini);
			glVertex2f(xfin, yfin);
			glEnd();

			xini = xfin;
			yini = yfin;

		}

	}

	std::cout << "TOTAL POINTS: " << TOTAL_POINTS << std::endl;

}


void DesenhaCurvaHermiteBezier() {
	glClear(GL_COLOR_BUFFER_BIT);

	//glPushMatrix();
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	DesenhaHermiteGrau3();
	//glPopMatrix();

	//glLoadIdentity();

	//glutSwapBuffers();
	glFlush();
}

void DesenhaCurvasHermiteEBezier() {

	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DesenhaBezierGrau3(); /// usar

	glutSwapBuffers();
}

void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.1, 500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	//GLsizei largura, altura;
	GLfloat largura, altura;

	// Evita a divisao por zero
	if (h == 0)
		h = 1;

	// Atualiza as variáveis
	largura = w;
	altura = h;

	// Especifica as dimensões da Viewport
	glViewport(0, 0, largura, altura);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Estabelece a janela de seleção (esquerda, direita, inferior,
	// superior) mantendo a proporção com a janela de visualização
	if (largura <= altura)
		gluOrtho2D(MENORX, MAIORX, MENORY * altura / largura, MAIORY * altura / largura);
	//gluOrtho2D(MENORX_BEZIER, MAIORX_BEZIER, MENORY_BEZIER * altura / largura, MAIORY_BEZIER * altura / largura);
	else
		gluOrtho2D(MENORX * largura / altura, MAIORX * largura / altura, MENORY, MAIORY);
	//gluOrtho2D(MENORX_BEZIER * largura / altura, MAIORX_BEZIER * largura / altura, MENORY_BEZIER, MAIORY_BEZIER);
}

void Inicializa(void)
{
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(POSWX, POSWY);

	glutInitWindowSize(LAR, ALT);

	glutCreateWindow("Curvas");

	glutDisplayFunc(DesenhaCurvasHermiteEBezier);

	glutReshapeFunc(AlteraTamanhoJanela);

	glutMouseFunc(myMousefuncBezierHermitInterate);

	Inicializa();

	glutMainLoop();

}
