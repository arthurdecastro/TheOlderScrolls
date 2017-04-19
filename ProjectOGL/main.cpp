/*----------------------------------------------------
Visual Studio 2013 - Community
----------------------------------------------------
Trabalho 2 (GB) - The Older Scrolls 1

Arthur de Castro
Arthur Schunemann

TOS1 segue um gameplay que se assemelha a jogos de RPG
e Aventura da década de 1990. 

Controles:

W - Avançar
S - Retroceder
A - Virar para a esquerda
D - Virar para a direita

P - Modo Debug
R - Restart o game

ESC - sair do jogo

Quando estiver em diálogos:

Q - "Sim"
E - "Não"
------------------------------------------------------*/

// para abertura de arquivos através de métodos "ultrapassados"
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <glut.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

extern "C"{
	#include "glm.h"
}

/* ---------------------------------------------------------------------------------
------------------------------------------------------------------------------------*/

GLMmodel* pmodelo = NULL;
GLuint gTexturaID[4];
GLUquadricObj *qObj;

/* ---------------------------------------------------------------------------------*/

int fov = 60, velocidade = 1; // alteravel através do modo "debug"

int posx = 1, posz = -1; // para testes de colisão
float pos_x = 1.0, pos_y = 1.0, pos_z = -1.0, alvo_x = pos_x, alvo_y = 1.0, alvo_z = -14.0; // parâmetros da câmera

bool norte = true, sul = false, leste = false, oeste = false; // usado para mudar o alvo da câmera
bool debug = false, dialogo = false;

int y = 2; // animação da gota

int karma = 0; // karma = pontuação
int dialogoOption = -1; // numero atual da fala da personagem
int resposta[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // armazena os "sim" (0)  e "não" (1)
bool kill = false; // quando true, mata o personagem


/* ---------------------------------------------------------------------------------
------------------------------------------------------------------------------------*/

// Falas da NPC
void Escreve(void){
	char fala0[100] = "Voce nao me parece confiavel. Voce luta por Daria?";
	char fala1[100] = "Entao diga: Apoias o rei?";
	char fala2[100] = "A rainha?";
	char fala3[100] = "Voce busca a lua?";
	char fala4[100] = "Tem interesse na Rainha?";
	char fala5[100] = "Voce me parece confiavel. Mas tens algum interesse em mim?";
	char fala6[100] = "Quer se juntar a mim e derrotar Nemesmotren?";
	char fala7[100] = "Err... 'Se eu lhe ofereço o lírio, voce fica escarlate?'";
	char fala8[100] = "Imaginei. Esta aqui para me enfrentar?";
	char fala9[100] = "TOLO!";
		
	glColor3ub(255, 0, 0); //Cor da fonte
	
	glRasterPos3f(-3.5, 0.8, -1.6); //Posição da palavra
	
	// ============================== FALAS ================================
	if (dialogoOption == 0){	// fala 0
		//Uso do "for" para escrever mais de um caracter
		for (int i = 0; i <= strlen(fala0); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala0[i]);
		}		
	}

	if (dialogoOption == 1){ // fala 1	
		for (int i = 0; i <= strlen(fala1); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala1[i]);
		}
	}

	if (dialogoOption == 2){ // fala 2
		for (int i = 0; i <= strlen(fala2); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala2[i]);
		}
	}

	if (dialogoOption == 3) // fala 3
	{
		for (int i = 0; i <= strlen(fala3); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala3[i]);
		}
	}

	if (dialogoOption == 4) // fala 4
	{
		for (int i = 0; i <= strlen(fala4); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala4[i]);
		}
	}

	if (dialogoOption == 5) // fala 5
	{
		for (int i = 0; i <= strlen(fala5); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala5[i]);
		}
	}

	if (dialogoOption == 6) // fala 6
	{
		for (int i = 0; i <= strlen(fala6); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala6[i]);
		}
	}

	if (dialogoOption == 7) // fala 7
	{
		for (int i = 0; i <= strlen(fala7); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala7[i]);
		}
	}

	if (dialogoOption == 8) // fala 8
	{
		for (int i = 0; i <= strlen(fala8); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala8[i]);
		}
	}

	if (dialogoOption == 9) // fala 9
	{
		for (int i = 0; i <= strlen(fala9); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fala9[i]);
		}	
	}
}

void EscreveKarma(void)
{
	// para assegurar a posição
	pos_z = -1;
	pos_x = -5;
	leste = true;

	char Pontuacao[8] = "KARMA: ";
	char teste[100];
	
	//Conversão de inteiro para string, pois a OpenGL só escreve string ou char
	sprintf(teste, "%d", karma);

	glColor3ub(255, 0, 0); //Cor da fonte
	
	if (dialogoOption < 10)
	{
		glRasterPos3f(-3.5, 0.7, -1.7); 
	}

	else
	{
		glRasterPos3f(4, 0.7, 13);
	}

	for (int i = 0; i <= strlen(Pontuacao); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Pontuacao[i]);	

	for (int i = 0; i <= strlen(teste); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, teste[i]);
}

void EscreveGameOver(void)
{
	char gameOver1[40] = "GAME OVER! Morreste como um covarde";
	char gameOver2[50] = "GAME OVER! Voces continuam cada um por si";
	char gameOver3[55] = "GAME OVER! Voces continuam a aventura e se casam :3";
	
	glColor3ub(255, 0, 0);
	glRasterPos3f(4, 1, 13); 

	//==========================================================
	if (karma <= 0) // game over ruim
	{
		for (int i = 0; i <= strlen(gameOver1); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOver1[i]);
		}
	}

	if (karma > 0 && karma < 80) // game over neutro
	{
		for (int i = 0; i <= strlen(gameOver2); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOver2[i]);
		}
	}

	if (karma >= 80) // game over bom
	{
		for (int i = 0; i <= strlen(gameOver3); i++){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gameOver3[i]);
		}
	}
}

// ================= TEXTURE ====================

void Material_Luz(){
	GLfloat *tex;

	//Definição das propriedades dos materiais
	GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Cinza
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess[] = { 50.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	//Definição das propriedades das fontes de luz
	GLfloat lightIntensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

}

// função de carregamento de imagem
int  CarregarImagem(char *NomeArquivo){
#define SAIR {fclose(fp_arquivo); return -1;}
#define CTOI(C) (*(int*)&C)

	int Retorno, Auxiliar, Indice;

	GLubyte *Imagem, Cabecalho[0x54];
	GLuint PosicaoDados, ImagemTamanho;
	GLsizei ImagemLargura, ImagemAltura;

	// Abre o arquivo e efetua a leitura do Cabecalho do arquivo BMP
	FILE * fp_arquivo = fopen(NomeArquivo, "rb");
	if (!fp_arquivo)
		return -1;
	if (fread(Cabecalho, 1, 0x36, fp_arquivo) != 0x36)
		SAIR;
	if (Cabecalho[0] != 'B' || Cabecalho[1] != 'M')
		SAIR;
	if (CTOI(Cabecalho[0x1E]) != 0)
		SAIR;
	if (CTOI(Cabecalho[0x1C]) != 24)
		SAIR;

	// Recupera a informação dos atributos de altura e largura da imagem
	ImagemLargura = CTOI(Cabecalho[0x12]);
	ImagemAltura = CTOI(Cabecalho[0x16]);
	(CTOI(Cabecalho[0x0A]) == 0) ? (PosicaoDados = 0x36) : (PosicaoDados = CTOI(Cabecalho[0x0A]));

	ImagemTamanho = ImagemLargura*ImagemAltura * 3;

	// Efetura a Carga da Imagem
	Imagem = (GLubyte *)malloc(ImagemTamanho);
	Retorno = fread(Imagem, 1, ImagemTamanho, fp_arquivo);

	if (Retorno != ImagemTamanho)
	{
		free(Imagem);
		SAIR;
	}

	// Inverte os valores de R e B
	for (Indice = 0; Indice < ImagemTamanho; Indice += 3)
	{
		Auxiliar = Imagem[Indice];
		Imagem[Indice] = Imagem[Indice + 2];
		Imagem[Indice + 2] = Auxiliar;
	}

	// Tratamento da textura para o OpenGL
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Faz a geraçao da textura na memória
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImagemLargura, ImagemAltura, 0, GL_RGB, GL_UNSIGNED_BYTE, Imagem);

	fclose(fp_arquivo);
	free(Imagem);
	return 1;
}

// ================= OBJ =========================

void LuzMaterial() {
	//Definição das propriedades dos materiais
	float ambiente[] = { 0.7f, 0.7f, 0.7f, 1.0f }; //Cinza
	float difusa[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float especular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float brilho[] = { 50.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, brilho);

	//Definição das propriedades das fontes de luz
	GLfloat intensidade_luz[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat posicao_luz[] = { 2.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
	glLightfv(GL_LIGHT0, GL_AMBIENT, intensidade_luz);
}

void importaObj(void)
{
	if (!pmodelo)
	{
		pmodelo = glmReadOBJ("../ProjectOGL/data/gota.obj");
		
		if (!pmodelo) exit(0);
		glmUnitize(pmodelo);
		glmFacetNormals(pmodelo);
		glmVertexNormals(pmodelo, 90.0);
	}

	glmDraw(pmodelo, GLM_SMOOTH | GLM_MATERIAL);
}

void Animacion(int valor) // função para animar o obj
{
	// contagem incrementa animation;

	if (y <= -3)
	{
		y = 4;
	}
	
	else
	{
		y = y - 1;
	}	

	glutPostRedisplay();
	glutTimerFunc(200, Animacion, 1);
}

// ================= CORE =========================

void Inicio(void) // Inicializa aspectos do rendering
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // cor de fundo
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fov, 1, 0.5, 100); // função de perspectiva

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pos_x, pos_y, pos_z, alvo_x, alvo_y, alvo_z, 0, 1, 0); //Chama a câmera, passando parâmetros

	//glEnable(GL_TEXTURE_2D); //Habilita o uso de textura nos objetos
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Define a forma de armazenamento dos pixels na textura (1= alihamento por byte)
	glGenTextures(3, gTexturaID); //Define quantas texturas serão usadas no programa (3 = duas texturas); gTexturaID = vetor que guarda o número de texturas

	//Define que tipo de textura será usada (GL_TEXTURE_2D ==> define que será usada uma textura 2D (bitmaps))
	glBindTexture(GL_TEXTURE_2D, gTexturaID[0]); //gTexturaID[0]  ==> define o número da textura // parede
	CarregarImagem("../ProjectOGL/data/parede.bmp");
	
	glBindTexture(GL_TEXTURE_2D, gTexturaID[1]); // chao
	CarregarImagem("../ProjectOGL/data/chao.bmp");

	glBindTexture(GL_TEXTURE_2D, gTexturaID[2]); // porta
	CarregarImagem("../ProjectOGL/data/porta.bmp");

	glBindTexture(GL_TEXTURE_2D, gTexturaID[3]); // npc
	CarregarImagem("../ProjectOGL/data/npc.bmp");				
}

void Map() // vertices para as paredes, chao e teto
{
	glEnable(GL_TEXTURE_2D); //Habilita o uso de textura nos objetos
	// ====================== Paredes Especiais ==========================
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, gTexturaID[2]);
		glBegin(GL_QUADS);
			// parede A0
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, 0);
		glEnd();
	glPopMatrix();
	// ======================
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, gTexturaID[3]);
		glBegin(GL_QUADS);
			// parede NPC 
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-3, 0, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-3, 2, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-3, 2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-3, 0, 0);
		glEnd();
	glPopMatrix();

	// =================== teto ===================
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, gTexturaID[0]);
		glBegin(GL_QUADS);
			// teto 1
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, 0);
			// teto 2
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -2);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -2);
			// teto 3
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -4);
			// teto 4
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -6);
			// teto 5
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -8);
			// teto 6
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -10);
			// teto 7
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 2, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -4);
			// teto 8
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 2, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 2, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -4);
			// teto 9
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 2, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -4);
			// teto 10
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 2, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 2, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -2);
			// teto 11
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 2, -2);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, 0);
			// teto 12
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, 0);
			// teto 13
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 2, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -10);
			// teto 13b
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 2, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(6, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(6, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 2, -10);
			// teto 14
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 2, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -10);
			// teto 15
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 2, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -10);
			// teto 16 
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 2, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -10);
			// teto 17
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 2, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -8);
			// teto 18
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 2, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 2, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -6);
			// teto 19
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 2, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 2, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -4);
		glEnd();
	glPopMatrix();

	// ========== Chão ==========
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, gTexturaID[1]);
		glBegin(GL_QUADS); 
			// chao 1
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, 0);
			// chao 2
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -2);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -2);
			//chao 3
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -4);
			// chao 4
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -6);
			//chao 5
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -8);
			// chao 6
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 0, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -10);
			// chao 7
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 0, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 0, -4);
			// chao 8
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 0, -4);
			// chao 9
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 0, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 0, -4);
			// chao 10
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 0, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 0, -2);
			// chao 11 ---
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -2);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 0, 0);
			// chao 12
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 0, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 0, 0);
			// chao 13
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -10);
			// chao 13b
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(6, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(6, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 0, -10);
			// chao 14
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 0, -10);
			// chao 15
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 0, -10);
			// chao 16 
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 0, -10);
			// chao 17
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 0, -8);
			// chao 18
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 0, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 0, -6);
			// chao 19
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 0, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 0, -4);
		glEnd();
	glPopMatrix();

	// ============================== Paredes normais ============================== 
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, gTexturaID[0]);
		glBegin(GL_QUADS);
			// ================= Verticais =================
			// parede A1
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, 0);
			// parede A2
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(0, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, 0);
			// parede A3
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, 0);
			// parede A3b
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, 0);
			// parede A4
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(0, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, 0);
			// parede A4b
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -4);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -4);
			// parede A7
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -2);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -2);
			// parede A17
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -10);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -10);
			// parede A16
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -8);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -8);
			// parede A15
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -6);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -6);
			// parede A8
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -6);
			// parede A9
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -8);
			// parede FA
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 2, -10);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 0, -10);
			// parede A6b
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 2, -4);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 0, -4);
			// parede A19
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-6, 2, -6);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-6, 0, -6);
			// parede A23
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(0, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -6);
			// parede A24
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -8);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(0, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -8);

			// parede A77
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -8);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -8);

			// parede A77
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -8);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -8);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -6);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -6);

			// ================= Horizontais ======================

			// parede A5
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -4);
			// parede A5b
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -4);
			// parede A6
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -4);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -4);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -4);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -4);
			// parede A7
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -6);
			// parede A21
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -6);
			// parede A22
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -6);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -6);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -6);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -6);
			// parede F1
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(2, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(2, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -12);
			// parede F2
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -12);
			// parede F3
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(6, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(6, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 2, -12);
			// parede F4
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(0, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(0, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -12);
			// parede F5
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -12);
			// parede F6
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -12);
			// parede F7
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, -12);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, -12);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, -12);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, -12);
			// parede F8
			glTexCoord2f(0.0f, 0.0f); glVertex3i(2, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(4, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(2, 2, -10);
			// parede F9
			glTexCoord2f(0.0f, 0.0f); glVertex3i(4, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(6, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(6, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(4, 2, -10);
			// parede F10
			glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-2, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-2, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 2, -10);
			// parede F11
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -10);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -10);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -10);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -10);
			// parede n1
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, -2);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, -2);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, -2);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, -2);
			// parede n2
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-2, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-4, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-4, 2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-2, 2, 0);
			// parede n3
			glTexCoord2f(0.0f, 0.0f); glVertex3i(-4, 0, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(-6, 0, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(-6, 2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(-4, 2, 0);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); //Habilita o uso de textura nos objetos
}

void DesenhaCena(void)
{
	//Limpa a janela e habilita o teste para eliminar faces posteriores
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Trata oclusão de superfícies
	glEnable(GL_DEPTH_TEST);

	//Habilita a fonte de luz
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Normaliza vetores para sombreamento
	glEnable(GL_NORMALIZE);

	//Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);

	LuzMaterial();

	//Habilita o sombreamento
	glShadeModel(GL_SMOOTH);

	//Define as propriedades das superfícies dos materiais
	Material_Luz();

	Inicio();
	
	glColor3f(1.0, 1.0, 1.0);
	
	Map(); // desenha paredes, chão e teto
	
	// ====================== OBJs ==========================

	if (dialogo == false)
	{
		glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			glTranslatef(3, y, -5);
			glScalef(0.2,0.2,0.2);
			importaObj();
		glPopMatrix();
	}

	if (dialogo == true) // dialogo ativo
	{
		pos_z = -1;
		pos_x = -5;
		leste = true;

		Escreve();
		EscreveKarma();

		if (dialogoOption >= 10 || kill == true)
		{
			leste = false;
			pos_x = 1.0, pos_y = 1.0, pos_z = 3.0, alvo_x = pos_x, alvo_y = 1.0, alvo_z = 14.0;

			EscreveGameOver();
		}

		else if (dialogoOption < 0)
		{
			dialogoOption = 0;
		}
	}

	glutSwapBuffers();
}

// ================= COLISÕES =========================

void colisao2() // teste para a colisão para norte X sul
{
	if (posz < 0 && posz > -12)
	{
		if ((pos_x > 0 && pos_x < 2) || (pos_x > -6 && pos_x < -4))
		{
			pos_z = posz;
		}

		else
		{
			posz = pos_z;
		}
	}

	else
	{
		posz = pos_z;
	}
}

void colisao() // teste para a colisão para leste X oeste
{
	if (posx > -6 && posx < 4)
	{
		if (posz == -5 || posz == -11)
		{
			pos_x = posx;
		}

		else
		{
			posx = pos_x;
		}
	}

	else
	{
		posx = pos_x;
	}
}

// ================= GAMEPLAY =========================

void Direcao()
{
	// direção
	if (norte == true)
	{
		alvo_x = pos_x;
		alvo_z = alvo_z - 90.0;
	}

	else if (oeste == true)
	{
		alvo_x = pos_x - 14.0;
		alvo_z = pos_z;
	}

	else if (sul == true)
	{
		alvo_x = 1.0;
		alvo_z = alvo_z + 90.0;
	}

	else if (leste == true)
	{
		alvo_x = pos_x + 14.0;
		alvo_z = pos_z;
	}
}

// Função de movimentação
void TeclasNormais(unsigned char tecla, int x, int y)
{
	if ((pos_z >= -2 && pos_x == -5) && leste == true)
	{
		dialogo = true;
	}

	if (dialogo == false)
	{
		// == == == == Movimentação == == == ==

			// ============ Avançar =====================
			if (tecla == 'W' || tecla == 'w')
			{
				if (norte == true || sul == true) // norte sul
				{
					if (norte == true)
					{
						posz -= velocidade;
					}
					else // sul == true
					{
						posz += velocidade;
					}

					colisao2();
				}

				else if (leste == true || oeste == true) // leste oeste
				{
					if (leste == true)
					{
						posx += velocidade;
					}

					else // oeste == true
					{
						posx -= velocidade;
					}

					colisao();
				}
			}

		// ================= Retroceder =================

		if (tecla == 'S' || tecla == 's')
		{
			if (norte == true || sul == true)
			{
				if (norte == true)
				{
					posz += velocidade;
				}
				else
				{
					posz -= velocidade;
				}

				colisao2();
			}

			else if (leste == true || oeste == true)
			{
				if (leste == true){
					posx -= velocidade;
				}

				else{
					posx += velocidade;
				}

				colisao();
			}
		}

		// ===================== turn left and right =====================
		if (tecla == 'A' || tecla == 'a'){
			if (oeste == false && norte == true){
				oeste = true;
				leste = false;
				norte = false;
				sul = false;
			}

			else if (oeste == true){
				sul = true;
				leste = false;
				oeste = false;
				norte = false;
			}

			else if (oeste == false && sul == true){
				sul = false;
				oeste = false;
				leste = true;
				norte = false;
			}

			else if (leste == true){
				sul = false;
				leste = false;
				oeste = false;
				norte = true;
			}
		}

		else if (tecla == 'D' || tecla == 'd'){
			if (leste == false && norte == true)
			{
				leste = true;
				oeste = false;
				norte = false;
				sul = false;
			}

			else if (leste == true){
				sul = true;
				leste = false;
				oeste = false;
				norte = false;
			}

			else if (leste == false && sul == true){
				sul = false;
				leste = false;
				oeste = true;
				norte = false;
			}

			else if (oeste == true){
				sul = false;
				leste = false;
				oeste = false;
				norte = true;
			}
		}
	}

	Direcao(); //

	// resposta[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	if (dialogo == true){
		// Q == Sim == 0

		if (tecla == 'Q' || tecla == 'q' && kill != true){
			if (dialogoOption == 0){
				resposta[0] = 0;
				karma = karma + 10;

				dialogoOption = 1;
			}

			else if (dialogoOption == 1){
				resposta[1] = 0;
				kill = true;

				dialogoOption = 2;
			}

			else if (dialogoOption == 2){
				resposta[2] = 0;
				karma = karma + 10;

				dialogoOption = 3;
			}

			else if (dialogoOption == 3){
				resposta[3] = 0;
				kill = true;
				
				dialogoOption = 9;
			}

			else if (dialogoOption == 4){
				resposta[4] = 0;
				kill = true;

				dialogoOption = 5;
			}

			else if (dialogoOption == 5){
				resposta[5] = 0;
				karma = karma - 5;

				dialogoOption = 6;
			}

			else if (dialogoOption == 6){
				resposta[6] = 0;
				karma = karma + 10;

				dialogoOption = 7;
			}

			else if (dialogoOption == 7){
				resposta[7] = 0;
				karma = karma + 10;

				dialogoOption = 10;
			}

			else if (dialogoOption == 8){
				resposta[8] = 0;
				kill = true;

				dialogoOption = 9;
			}
			
			if (dialogoOption == 9){
				kill = true;
				dialogoOption = 10;
			}
		}

		// E == Não == 1

		if (tecla == 'E' || tecla == 'e' && kill != true){

			if (dialogoOption == 0){
				resposta[0] = 1;
				karma = karma - 10;

				dialogoOption = 8;
			}

			else if (dialogoOption == 1){
				resposta[1] = 1;
				karma = karma + 10;

				dialogoOption = 2;
			}

			else if (dialogoOption == 2){
				resposta[2] = 1;
				karma = karma - 10;

				dialogoOption = 3;
			}

			else if (dialogoOption == 3){
				resposta[3] = 1;
				karma = karma + 10;

				dialogoOption = 4;
			}

			else if (dialogoOption == 4){
				resposta[4] = 1;
				karma = karma + 10;

				dialogoOption = 5;
			}

			else if (dialogoOption == 5){
				resposta[5] = 1;
				karma = karma + 10;

				dialogoOption = 6;
			}

			else if (dialogoOption == 6){
				resposta[6] = 1;
				karma = karma - 5;

				dialogoOption = 7;
			}

			else if (dialogoOption == 7){
				resposta[7] = 1;
				karma = karma - 5;

				dialogoOption = 10;
			}

			else if (dialogoOption == 8){
				resposta[8] = 1;
				karma = karma + 5;

				dialogoOption = 3;
			}


			if (dialogoOption == 9){
				kill == true;
				dialogoOption = 10;
			}
		}
	}

	// reset no jogo
	if (tecla == 'R' || tecla == 'r'){
		posx = 1;
		posz = -1; 
		pos_x = 1.0;
		pos_y = 1.0;
		pos_z = -1.0;
		alvo_x = pos_x;
		alvo_y = 1.0;
		alvo_z = -14.0; 

		norte = true;
		sul = false;
		leste = false;
		oeste = false; 
		dialogo = false;
		
		kill = false;

		karma = 0;
		dialogoOption = -1;
	}
	
	// ativar debug
	if (tecla == 'P' || tecla == 'p'){
		if (debug == false){
			debug = true;
		}

		else{
			debug = false;
		}	
	}

	// debug
	if (debug == true){		
		cout << "============ DEBUG ==============" << endl;
		cout << "norte = " << norte << endl;
		cout << "sul = " << sul << endl;
		cout << "leste = " << leste << endl;
		cout << "oeste = " << oeste << endl << endl;

		cout << "posx = " << posx << endl;
		cout << "pos_x = " << pos_x << endl;
		cout << "posz = " << posz << endl;
		cout << "pos_z = " << pos_z << endl << endl;

		cout << "dialogo option = " << dialogoOption << endl;
		cout << "Karma = " << karma << endl;
		cout << "Kill = " << kill << endl << endl;

	}

	// 'ESC' = sair do jogo

	if (GetAsyncKeyState(VK_ESCAPE)){
		exit(0);
	}

	glutPostRedisplay();
}

//Função para tratamento de teclas especiais chamada pela main
void TeclasEspeciais(int tecla, int x, int y){
	if (debug == true){
		// mudar fov
		if (tecla == GLUT_KEY_RIGHT){
			fov += 1;
		}
		else if (tecla == GLUT_KEY_LEFT){
			fov -= 1;
		}

		// mudar o dialogo atual
		else if (tecla == GLUT_KEY_UP){
			dialogoOption += 1;
		}
		else if (tecla == GLUT_KEY_DOWN){
			dialogoOption -= 1;
		}

		cout << "============ DEBUG ==============" << endl;
		cout << "norte = " << norte << endl;
		cout << "sul = " << sul << endl;
		cout << "leste = " << leste << endl;
		cout << "oeste = " << oeste << endl << endl;

		cout << "posx = " << posx << endl;
		cout << "pos_x = " << pos_x << endl;
		cout << "posz = " << posz << endl;
		cout << "pos_z = " << pos_z << endl << endl;

		cout << "dialogo option = " << dialogoOption << endl;
		cout << "Karma = " << karma << endl;
		cout << "Kill = " << kill << endl << endl;
	}	

	else
	{
		cout << "Debug operation only" << endl;
	}

	glutPostRedisplay(); //Função deve ser chamada ao final para redesenhar a imagem com as alterações definidas aqui
}

// =================== MAIN ======================

void main() // Parte principal - ponto de início de execução, Cria janela, Inicializa aspectos relacionados a janela e a geracao da imagem, etc
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	glutInitWindowSize(800, 800); // tamanho da janela
	glutCreateWindow("The Older Scrolls 1 (beta 1.0)"); // Cria uma janela com o titulo especificado

	glutSpecialFunc(TeclasEspeciais); //Chama a função de tratamento de teclas especiais
	glutKeyboardFunc(TeclasNormais); //Chama a função de tratamento de teclas normais
	
	glutTimerFunc(200, Animacion, 1); // animação

	glutDisplayFunc(DesenhaCena); // Especifica para a OpenGL que funcao deve ser chamada para geracao da imagem
	Inicio(); // Executa a inicializacao de parametros de exibicao
	
	glutMainLoop(); // Dispara a "maquina de estados" de OpenGL
}