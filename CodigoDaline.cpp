// TeaPot3D.c - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização 
// de objetos 3D.
// Este código está baseado nos exemplos disponíveis no livro 
// "OpenGL SuperBible", 2nd Edition, de Richard S. e Wright Jr.

#include <gl/glut.h>
#include <iostream>

GLfloat angle, fAspect;

GLfloat dx = 0, dy = 0, dz = 0;

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// pinta a cor do fundo
	glClear(GL_COLOR_BUFFER_BIT);

	// define a cor dos objetos que vão ser construídos
	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (wire-frame)
	glPushMatrix(); // empacota as ações
	glTranslatef(dx, dy, dz);  // faz uma translação
	glutWireTeapot(50.0f);     // constrói um objeto qualquer
	glPopMatrix();  // desempacota as ações

	// Executa os comandos OpenGL
	glutSwapBuffers();  // troca de buffer para acelerar a cena
}

// Inicializa parâmetros de rendering
void Inicializa(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle = 45;
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
	gluLookAt(0, 80, 200, 0, 0, 0, 0, 1, 0);
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

void teclado(unsigned char key, int x, int y) {

	std::cout << "[Tecla: " << key << " = {" << dx << ", " << dy << "}]" << std::endl;

	switch (tolower(key)) {
		case 'r': dx += 10; break;
		case 'l': dx -= 10; break;
	}

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

// Programa Principal
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(350, 300);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);

	glutKeyboardFunc(teclado);

	Inicializa();
	glutMainLoop();
}
