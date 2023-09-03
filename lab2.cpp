
#include <GL/freeglut.h> 
#include <stdlib.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#define LAR 400 // largula da sua janela Windows
#define ALT 400 // altura da sua janela Windows

#define dimTx 256

using namespace std;
using namespace cv;

// Matriz RGB para guardar a imagem de textura
uchar imTex[dimTx][dimTx][4];

// variáveis para o ângulo de perspectiva e para o aspecto da projeção
// são usadas na função gluPerspective
//A variavel rangle é alterada pelo teclado se vc clicar em 'a', já a variável fAspect é alterada altomaticamente
//sempre que vc muda o aspecto da janela
GLfloat rangle, fAspect;


// variáveis que controlam o clique do mouse para operações geométricas
// são atualizadas nas funções de callback de mouse (GerenciaMouse) e
// operações geométricas como glTranslate
GLfloat dx = 0, dy = 0, dz = 0;

// essas variáveis aqui são controladas pela Callback de mouse
// servem para converter as coordenadas do Windows em coordenadas do OpenGL
GLfloat cx = 0, cy = 0;

void carregaTextura() {

	Mat imTexture, imTexture256;

	imTexture = imread("img24.jpg", 1);

	resize(imTexture, imTexture256, Size(256, 256));

	namedWindow("Teste Textura 256 x 256", 2);

	imshow("Teste Textura 256 x 256", imTexture256);

	for (int y = 0; y < imTexture256.rows; y++) {

		for (int x = 0; x < imTexture256.cols; x++) {

			Vec3b cor = imTexture256.at<Vec3b>(y, x);

			imTex[y][x][0] = 255 - cor[2]; // blue
			imTex[y][x][1] = 255 - cor[1]; // green
			imTex[y][x][2] = 255 - cor[0]; // red

		}
	}

	imTexture256.release(); // desaloca imagem
}

// função que desenha uma malha 3D
void DesenhaMalha();


// Essa função, que não é callback, serve para configurar a iluminação
// ela é chamada uma única vez na função de Incializa, chamada pela main()
void ConfiguraIluminacao(void)
{
	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 }; // definição de luz ambiente
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };	   // definição de luz difusa
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };// definição de luz especular (brilho) 
	GLfloat posicaoLuz[4] = { 0.0, 50.0, 50.0, 1.0 }; // posição do spot de luz

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}



// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// pinta a cor do fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// define a cor dos objetos que vão ser construídos
	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenha o teapot com a cor corrente (wire-frame)
	glPushMatrix(); // empacota as ações
	glTranslatef(dx, dy, dz); // essa função translada de dx, dy e dz tudo que vier abaixo dela e dentro da glPushMatrix-glPopMatrix
	DesenhaMalha();
	glPopMatrix();  // desempacota as ações

	// Executa os comandos OpenGL
	glutSwapBuffers();  // troca de buffer para acelerar a cena
}


void DesenhaMalha()
{
	//glutWireTeapot(50.0f);     // constrói uma malha Wire 
	//glutSolidTeapot(50.0f);     // constrói uma malha Solida 

	// esta macro desenha um simples quadrado
	glBegin(GL_QUADS);
	glVertex3f(-64, -64, 0);
	glVertex3f(64, -64, -50);
	glVertex3f(64, 64, -50);
	glVertex3f(-64, 64, 50);
	glEnd();
}



// Inicializa parâmetros iniciais, é chamada na main()
void Inicializa(void)
{
	// configura iluminação
	ConfiguraIluminacao();

	// define a cor de fundo
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	carregaTextura();

	// define o ângulo inicial de perspectiva, na função gluPerspective()
	//Essa variável é alterada pelo teclado se vc clicar em 'a'
	rangle = 45;
}


// Função usada para especificar o volume de visualização
// se vc alterou algum parâmetro de visualização, essa funçào é chamada
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(rangle, fAspect, 0.1, 500);

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

	// se vc alterou algum parâmetro de visualização, essa funçào é chamada
	EspecificaParametrosVisualizacao();
}


// calback de teclado
void Teclado(unsigned char key, int x, int y) {

	switch (key) {

	case 'r': dx = dx + 10; break;  //desloca para a direita

	case 'l': dx = dx - 10; break;  //desloca para a esquerda

	case 'a':                      // aumenta o ângulo de visualização da função gluPerspective(), provocando ZOO-IN
		rangle = rangle + 5;
		if (rangle > 360) rangle = 0;
		break;

	case 'b':                      // diminui o ângulo de visualização da função gluPerspective(), provocando ZOO-OUT
		rangle = rangle - 5;
		if (rangle < 0) rangle = 0;
		break;

	}

	// se vc alterou algum parâmetro de visualização, essa funçào é chamada
	EspecificaParametrosVisualizacao();

	// dispara a callback de Desenha() para desenhar o que alterou!
	glutPostRedisplay();
}


// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	// aqui tranaforma o x do windows no cx do OpenGL
	cx = x - (LAR / 2);

	// aqui tranaforma o y invertido do windows no cy do OpenGL
	cy = (ALT / 2) - y;

	/*	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
		// mude uma variável controlada pelo mouse aqui
	}

	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {
		// mude uma variável controlada pelo mouse aqui
		}
*/
// se vc alterou algum parâmetro de visualização, essa funçào é chamada
	EspecificaParametrosVisualizacao();

	// dispara a função callback de Desenho(), caso vc tenha alterado alguma coisa no visual
	glutPostRedisplay();
}



// Programa Principal
int main(int argc, char* argv[])
{
	// essa função inicializa a glut
	glutInit(&argc, argv);

	// ess função inicializa o Display com um buffer RGB duplo
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// essa função define as dimenções de sua janela Windows
	glutInitWindowSize(LAR, ALT);


	//aqui vc cria uma Janela Windows com as dimenções definidas acima
	glutCreateWindow("Visualizacao 3D");

	// aqui vc ativa uma callback de Desenho
	glutDisplayFunc(Desenha);

	// aqui vc ativa uma callBack para ajustar a janela de saída toda vez que vc alterar ela
	glutReshapeFunc(AlteraTamanhoJanela);

	// aqui vc ativa uma callbcak de mouse toda vez que vc clica nele
	glutMouseFunc(GerenciaMouse);

	// aqui vc ativa uma callback de teclado toda vez que vc clica nele
	glutKeyboardFunc(Teclado);

	// aqui vc inicializa qualquer coisa fixa (nào é callback)
	Inicializa();

	// quando tudo está pronto, aqui dispara tudo
	glutMainLoop();
}
