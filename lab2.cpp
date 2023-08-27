// TeaPot3D.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização 
// de objetos 3D.
// Este código está baseado nos exemplos disponíveis no livro 
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#include <gl/glut.h>
#include <GL/freeglut.h>
#include <iostream>

GLfloat angle, fAspect;

GLfloat dx = 0, dy = 0, dz = 0;

#define LARGURA 400
#define ALTURA 400

GLfloat* matrixMultiply(const GLfloat* M1, const GLfloat* M2) {

	const int FIXED_MATRIX_SIZE = 4;

	GLfloat* resultMatrix = new GLfloat[16];

	for (int i = 0; i < FIXED_MATRIX_SIZE; i++) {
		for (int j = 0; j < FIXED_MATRIX_SIZE; j++) {
			resultMatrix[i * FIXED_MATRIX_SIZE + j] = 0;
			for (int k = 0; k < FIXED_MATRIX_SIZE; k++)
				resultMatrix[i * FIXED_MATRIX_SIZE + j] += M1[i * FIXED_MATRIX_SIZE + k] * M2[k * FIXED_MATRIX_SIZE + j];
		}
	}

	return resultMatrix;

}

void matrixTranslateMoviment(GLfloat dx, GLfloat dy, GLfloat dz) {

	GLfloat currentMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);

	const GLfloat identityMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		dx, dy, dz, 1
	};

	const GLfloat* resultMatrix = matrixMultiply(currentMatrix, identityMatrix);

	glLoadMatrixf(resultMatrix);

}

void matrixTranslateScale(GLfloat dx, GLfloat dy, GLfloat dz) {

	GLfloat currentMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);

	const GLfloat identityMatrix[16] = {
		dx, 0, 0, 0,
		0, dy, 0, 0,
		0, 0, dz, 0,
		0, 0, 0, 1
	};

	const GLfloat* resultMatrix = matrixMultiply(currentMatrix, identityMatrix);

	glLoadMatrixf(resultMatrix);

}


void desenhaMalha() {

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(-50, 0, 0);
	glVertex3f(50, 0, 0);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0, 50, 0);
	glVertex3f(50, 50, 0);
	glEnd();
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// pinta a cor do fundo
	glClear(GL_COLOR_BUFFER_BIT);

	// define a cor dos objetos que vão ser construídos
	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (wire-frame)
	glPushMatrix(); // empacota as ações

	//desenhaMalha();

	//matrixTranslateMoviment(dx, dy, dz);
	glTranslatef(dx, dy, 0);
	glutWireTeapot(50.0f);     // constrói um objeto qualquer
	glPopMatrix();  // desempacota as ações

	//matrixTranslateScale(dx, dy, dz);
	//glScalef(dx, dy, dz);
	glLoadIdentity();

	// Executa os comandos OpenGL
	glutSwapBuffers();  // troca de buffer para acelerar a cena

	//glViewport(0, 200, 0, 0);
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle = 100;
}

// Função usada para especificar o volume de visualização
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

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

void mouseInteract(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) {

		if (state == GLUT_UP) {

			dx = x - (LARGURA / 2);
			dy = (ALTURA / 2) - y;

			std::cout << x << std::endl;
			std::cout << y << std::endl;

		}
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void teclado(unsigned char key, int x, int y) {

	switch (tolower(key)) {
	case 'd': dx += 10; break;
	case 'a': dx -= 10; break;
	case 's': dy -= 10; break;
	case 'w': dy += 10; break;
	}

	std::cout << "[Tecla: " << key << " = {" << dx << ", " << dy << "}]" << std::endl;

	glutPostRedisplay();


}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {

	if (dir > 0) {
		if (angle >= 10) angle -= 5;
	}
	else {
		if (angle <= 130) angle += 5;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();

}

// Programa Principal
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(mouseInteract);
	glutMouseWheelFunc(mouseWheel);
	//glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(teclado);

	Inicializa();
	glutMainLoop();
}
