/*---------------------------------------------------------*/
/* ----------------   Proyecto Final   --------------------*/
/*-----------------    2026-2   ---------------------------*/
/*-------- Alumnos: ---------------------------------------*/
/*-------- - García Martínez Carlos Alfredo --------------*/
/*-------- - Medina Vaca Katia Alessandra    --------------*/
/*-------- - Morales Zaragoza Eric Francisco --------------*/
/*-------- - Dueñas Jarvio Pablo Alam        --------------*/
/*---------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture
#define MINIAUDIO_IMPLEMENTATION //Audio
#include "miniaudio.h"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>



ma_engine engine;
ma_sound music;

void initAudio() {
	ma_result result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		std::cout << "Error al inicial el motor de audio\n";
	}
}

void playSound(const char* file) {
	ma_sound_init_from_file(&engine, file, 0, NULL, NULL, &music);
	ma_sound_set_looping(&music, MA_TRUE);
	ma_sound_start(&music);
}

void closeAudio() {
	ma_engine_uninit(&engine);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(0.0f, 90.0f, 45.0f));
float MovementSpeed = 0.1f;
GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

float myTime = 0.0f;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Texture
unsigned int	
t_white,
t_techo,
t_piso;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, 0.0f, 0.0f);
glm::vec3 lightColorChange(0.0f, 0.0f, 0.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

//  POSICIONES	//

// Variables para brazo robotico
float	baseRotation = 0.0f,
		cil0Rotation = 0.0f,
		cil1Rotation = 0.0f,
		cil2Rotation = 0.0f,
		handRotation = 0.0f;
int armState = 0;

// Variables para dron
float	propellerRotation = 0.0f,
		dronPosition_x = -20.0f,
		dronPosition_y = 4.0f,
		dronPosition_z = -20.0f,
		dronRotation_xz = 0.0f,
		dronRotation_y = 0.0f;
int dronState = 0,
	helpIterator = 0;

// Variables para pájaros
float	redBirdPos_x = -68.5f,
		redBirdPos_y = 10.8f,
		redBirdPos_z = -160.0f,
		redBirdRotation = 180.0f,
		birdJmpAngle = 0.0f;
int	redBirdState = 0;

//Esfera

int estadoEsf = 1;
float
posIniEsf_x = 150.0f,
posIniEsf_y = 18.9f,
posIniEsf_z = -120.0f,
movEsf_x = posIniEsf_x,
movEsf_y = posIniEsf_y,
movEsf_z = posIniEsf_z,
incMovEsf = 0.0f,
orientaEsf_y = 0.0f,
orientaEsf = 90.0f;

//Pendulo

int estadoPen = 1;
float orientaPen1 = 0.0f,
	  orientaPen2 = 0.0f,
	  incPen = 5.0f;

//Auto

int estadoAutoLego = 1;
float
posIniAuto_x = 51.0f,
posIniAuto_y = 27.0f,
posIniAuto_z = 4.5f,
movAuto_x = posIniAuto_x,
movAuto_y = posIniAuto_y,
movAuto_z = posIniAuto_z,
incMovAuto = 0.0f,
orienta_y = 0.0f,
orienta = 90.0f;

//Dominos

int estadoDom = 0;
float  orientaDom[6] = { 0,0,0,0,0,0 },
incDom[6] = { 0,0,0,0,0,0 },
incA = 45.0f,
incB = 78.0f;




bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 3.0f,
		posZ = 0.0f,
		giroMonito = 0.0;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		incG = 0.0f;

#define MAX_FRAMES 15	//Cantidad máxima de Frames
int i_max_steps = 60;	//Cuadros intermedios
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroCabeza;	//Se agrega para la cabeza
	float giroBrazo;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 13;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].giroMonito = giroMonito;

	//Aqui podemos imprimir el valor de la posición para poseteriormente guardar la animación



	FrameIndex++;
}

void resetElements(void)  //Carga la posición inicial al objeto
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	giroMonito = KeyFrame[0].giroMonito;

}

void interpolation(void)	//PlayIndex define el cuadro inicial de la interpolación
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	incG = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void) 
{
	myTime += 0.005f;

	// Dominos
	if (estadoDom == 0) 
	{
		orientaDom[0] += incDom[0];
		incDom[0] += 0.03;

		if (orientaDom[0] >= incA) {
			incDom[0] = 0;
			estadoDom = 1;
		}
	}
	if (estadoDom == 1) 
	{
		orientaDom[0] += incDom[0];
		incDom[0] += 0.03;

		orientaDom[1] += incDom[1];
		incDom[1] += 0.03;

		if (orientaDom[0] >= incB) {
			incDom[0] = 0;
		}

		if (orientaDom[1] >= incA) {
			incDom[1] = 0;
			estadoDom = 3;
		}
	}
	if (estadoDom == 2)
	{
		orientaDom[0] += incDom[0];
		incDom[0] += 0.03;

		orientaDom[1] += incDom[1];
		incDom[1] += 0.03;

		orientaDom[2] += incDom[2];
		incDom[2] += 0.03;

		if (orientaDom[2] >= incA) {
			incDom[2] = 0;
			estadoDom = 3;
		}
	}
	if (estadoDom == 3)
	{
		orientaDom[1] += incDom[1];
		incDom[1] += 0.03;

		orientaDom[2] += incDom[2];
		incDom[2] += 0.03;

		if (orientaDom[1] >= incB) {
			incDom[1] = 0;
			estadoDom = 4;
		}

	}
	if (estadoDom == 4) {
		
		orientaDom[2] += incDom[2];
		incDom[2] += 0.03;

		if (orientaDom[2] >= 90.0f) {
			estadoDom = 0;
			for (int i = 0; i < 6; i++) {
				orientaDom[i] = 0.0f;
				incDom[i] = 0.0f;
			}
		}
	}


	//Animación Brazo Robotico

	switch(armState) {
		case 0: //Giro inicial
			if (baseRotation > -45.0f) {
				baseRotation -= 1.5f;
			} else {
				armState = 1;
			}
		break;
		case 1: // Extiende el brazo
			if (cil0Rotation < 90.0f) {
				cil0Rotation += 1.0f;
				cil1Rotation -= 1.5f;
				cil2Rotation += 2.0f;
			} else {
				armState = 2;
			}
		break;
		case 2:	// Abre pinzas
			if (handRotation < 45.0f) {
				handRotation += 5.0f;
			} else {
				armState = 3;
			}
		break;
		case 3:	// Cierra pinzas
			if (handRotation > 0.0f) {
				handRotation -= 5.0f;
			}
			else {
				armState = 4;
			}
		break;
		case 4:	// Abre pinzas
			if (handRotation < 45.0f) {
				handRotation += 5.0f;
			}
			else {
				armState = 5;
			}
		break;
		case 5:	// Cierra pinzas
			if (handRotation > 0.0f) {
				handRotation -= 5.0f;
			}
			else {
				armState = 6;
			}
		break;
		case 6:	// Giro 
			if (baseRotation < 45.0f) {
				baseRotation += 1.5f;
			}
			else {
				armState = 7;
			}
		break;
		case 7:	// Abre pinzas
			if (handRotation < 45.0f) {
				handRotation += 5.0f;
			}
			else {
				armState = 8;
			}
		break;
		case 8:	// Cierra pinzas
			if (handRotation > 0.0f) {
				handRotation -= 5.0f;
			}
			else {
				armState = 9;
			}
		break;
		case 9:	// Abre pinzas
			if (handRotation < 45.0f) {
				handRotation += 5.0f;
			}
			else {
				armState = 10;
			}
		break;
		case 10:// Cierra pinzas
			if (handRotation > 0.0f) {
				handRotation -= 5.0f;
			}
			else {
				armState = 11;
			}
		break;
		case 11:// Retrae el brazo
			if (cil0Rotation > 0.0f) {
				cil0Rotation -= 1.0f;
				cil1Rotation += 1.5f;
				cil2Rotation -= 2.0f;
			}
			else {
				armState = 0;
			}
		break;
	}
	
	//Animación Dron

	switch (dronState) {
		case 0:		// Acelera hélices
			if (helpIterator < 100){
				propellerRotation += 3.0f;
			} else if (helpIterator < 200) {
				propellerRotation += 13.0f;
			} else if (helpIterator < 300) {
				propellerRotation += 25.0f;
			} else if (helpIterator < 400) {
				propellerRotation += 50.0f;
			} else {
				dronState = 1;
				helpIterator = -1;
			}
			helpIterator++;
		break;
		case 1:		// Sube y rota
			propellerRotation += 50.0f;
			if (dronPosition_y < 92.0f) {
				dronPosition_y += 0.25f;
				dronRotation_y -= 0.103f;
			} else {
				dronState = 2;
			}
		break;
		case 2:		// Rota en el plano XZ
			propellerRotation += 50.0f;
			if (dronRotation_xz > -10.0f) {
				dronRotation_xz -= 0.25f;
			} else {
				dronState = 3;
			}
		break;
		case 3:		// Avanza hasta destino
			propellerRotation += 50.0f;
			if (dronPosition_x < 90.0f) {
				dronPosition_x += 0.5f;
				dronPosition_z -= 0.6818f;
			} else {
				dronState = 4;
			}
		break;
		case 4:		// Rota en el plano XZ
			propellerRotation += 50.0f;
			if (dronRotation_xz < 0.0f) {
				dronRotation_xz += 0.25f;
			} else {
				dronState = 5;
			}
		break;
		case 5:		// baja y rota
			propellerRotation += 50.0f;
			if (dronPosition_y > 4.0f) {
				dronPosition_y -= 0.25f;
				dronRotation_y += 0.103f;
			} else {
				dronState = 6;
			}
		break;
		case 6:		// Desacelera hélices
			if (helpIterator < 100) {
				propellerRotation += 50.0f;
			} else if (helpIterator < 200) {
				propellerRotation += 25.0f;
			} else if (helpIterator < 300) {
				propellerRotation += 13.0f;
			} else if (helpIterator < 400) {
				propellerRotation += 3.0f;
			} else {
				dronState = 7;
				helpIterator = -1;
			}
			helpIterator++;
		break;
		case 7:		// Acelera hélices
			if (helpIterator < 100) {
				propellerRotation += 3.0f;
			} else if (helpIterator < 200) {
				propellerRotation += 13.0f;
			} else if (helpIterator < 300) {
				propellerRotation += 25.0f;
			} else if (helpIterator < 400) {
				propellerRotation += 50.0f;
			} else {
				dronState = 8;
				helpIterator = -1;
			}
			helpIterator++;
		break;
		case 8:		// Sube y rota
			propellerRotation += 50.0f;
			if (dronPosition_y < 92.0f) {
				dronPosition_y += 0.25f;
				dronRotation_y -= 0.103f;
			} else {
				dronState = 9;
			}
		break;
		case 9:		// Rota en el plano XZ
			propellerRotation += 50.0f;
			if (dronRotation_xz < 10.0f) {
				dronRotation_xz += 0.25f;
			} else {
				dronState = 10;
			}
		break;
		case 10:	// Avanza hasta origen
			propellerRotation += 50.0f;
			if (dronPosition_x > -20.0f) {
				dronPosition_x -= 0.5f;
				dronPosition_z += 0.6818f;
			} else {
				dronState = 11;
			}
		break;
		case 11:	// Rota en el plano XZ
			propellerRotation += 50.0f;
			if (dronRotation_xz > 0.0f) {
				dronRotation_xz -= 0.25f;
			} else {
				dronState = 12;
			}
		break;
		case 12:	// baja y rota
			propellerRotation += 50.0f;
			if (dronPosition_y > 4.0f) {
				dronPosition_y -= 0.25f;
				dronRotation_y += 0.103f;
			} else {
				dronState = 13;
			}
		break;
		case 13:	// Desacelera hélices
			if (helpIterator < 100) {
				propellerRotation += 50.0f;
			} else if (helpIterator < 200) {
				propellerRotation += 25.0f;
			} else if (helpIterator < 300) {
				propellerRotation += 13.0f;
			} else if (helpIterator < 400) {
				propellerRotation += 3.0f;
			} else {
				dronState = 0;
				helpIterator = -1;
			}
			helpIterator++;
		break;
	}

	// Animación Pajaros
	birdJmpAngle += 0.1875f;
	if (birdJmpAngle < 90.0f) {
		redBirdPos_y = 10.9 + 0.2 * sin(birdJmpAngle);
	} else if(birdJmpAngle < 180) {
		redBirdPos_y = 10.9 + 0.2 * sin(birdJmpAngle);
	} else {
		birdJmpAngle = 0;
	}

	switch (redBirdState) {
		case 0:		// se mueve al punto 1
			if (redBirdPos_x < -60.0f) {
				redBirdPos_x += 1.25f;
			} else {
				redBirdState = 1;
			}
		break;
		case 1:		// rota
			if (redBirdRotation < 270.0f) {
				redBirdRotation += 5.0f;
			} else {
				redBirdState = 2;
			}
		break;
		case 2:		// se mueve al punto 2
			if (redBirdPos_z > -163.0f) {
				redBirdPos_z -= 0.5f;
			} else {
				redBirdState = 3;
			}
		break;
		case 3:		// rota
			if (redBirdRotation < 360.0f) {
				redBirdRotation += 5.0f;
			} else {
				redBirdRotation = 0.0f;
				redBirdState = 4;
			}
		break;
		case 4:		// se mueve al punto 3
			if (redBirdPos_x > -64.25f) {
				redBirdPos_x -= 0.25f;
			} else {
				redBirdState = 5;
			}
		break;
		case 5:		// rota
			if (redBirdRotation < 90.0f) {
				redBirdRotation += 5.0f;
			} else {
				redBirdState = 6;
			}
		break;
		case 6:		// se mueve al punto 4
			if (redBirdPos_z < -156.0f) {
				redBirdPos_z += 0.5f;
			} else {
				redBirdState = 7;
			}
		break;
		case 7:		// rota
			if (redBirdRotation > 0.0f) {
				redBirdRotation -= 5.0f;
			} else {
				redBirdState = 8;
			}
		break;
		case 8:		// se mueve al punto 5
			if (redBirdPos_x > -64.25f) {
				redBirdPos_x -= 0.25f;
			} else {
				redBirdState = 9;
			}
		break;
	}

	//Animación Esfera
	if (estadoEsf == 1) //Avanza
	{
		movEsf_y = posIniEsf_y + sin(myTime*15)*6.6;
		orientaEsf = myTime * 150;
	}

	//Animación Pendulo
	if (estadoPen == 1) //1
	{
		orientaPen1 += incPen;
		incPen -= 0.3;

		if (orientaPen1 <= 0.0f) {
			incPen = -5;
			estadoPen = 2;
		}
	}
	if (estadoPen == 2) //1
	{
		orientaPen2 += incPen;
		incPen += 0.3;
		if (orientaPen2 >= 0.0f) {
			estadoPen = 1;
		}
	}

	//Animación Auto
	incMovAuto = 0.3f;
	if (estadoAutoLego == 1) //Avanza
	{
		movAuto_x += incMovAuto;
		orienta = 90.0f;
		if (movAuto_x >= posIniAuto_x + 15.0f) {
			estadoAutoLego = 2;
		}
	}
	if (estadoAutoLego == 2) {
		movAuto_z += incMovAuto;
		orienta = 0.0f;
		if (movAuto_z >= posIniAuto_z + 15.0f) {
			estadoAutoLego = 3;
		}
	}
	if (estadoAutoLego == 3) {
		movAuto_x -= incMovAuto;
		orienta = -90.0f;
		if (movAuto_x <= posIniAuto_x + 3.0f) {
			estadoAutoLego = 4;
		}
	}
	if (estadoAutoLego == 4) {
		movAuto_x -= incMovAuto;
		movAuto_y += incMovAuto*0.6;
		orienta_y = -30.96;
		orienta = -90.0f;
		if (movAuto_x <= (posIniAuto_x - 3.0f) && movAuto_y >= (posIniAuto_y + 6.0f)) {
			estadoAutoLego = 5;
		}
	}
	if (estadoAutoLego == 5) {
		movAuto_x -= incMovAuto;
		movAuto_y -= incMovAuto * 2.1;
		orienta = -90.0f;
		orienta_y = 0.0f;
		if (movAuto_x <= (posIniAuto_x - 6.0f) && movAuto_y <= (posIniAuto_y + 0.0f)) {
			estadoAutoLego = 6;
		}
	}
	if (estadoAutoLego == 6) {
		movAuto_x -= incMovAuto;
		orienta = -90.0f;
		if (movAuto_x <= posIniAuto_x - 15.0f) {
			estadoAutoLego = 7;
		}
	}
	if (estadoAutoLego == 7) {
		movAuto_z -= incMovAuto;
		orienta = 180.0f;
		if (movAuto_z <= posIniAuto_z + 0.0f) {
			estadoAutoLego = 8;
		}
	}
	if (estadoAutoLego == 8) {
		movAuto_x += incMovAuto;
		orienta = 90.0f;
		if (movAuto_x >= posIniAuto_x + 0.0f) {
			estadoAutoLego = 1;
		}
	}



	if (myTime == 1.0f) {
		myTime = 0;
	}

	//if (play)
	//{
		
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				resetElements();
				i_curr_steps = 1;
				interpolation();
				//play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 1; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			giroMonito += incG;

			i_curr_steps++;
		}
	//}

	
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	if (isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{

	t_piso = generateTextures("Texturas/pisoTextura.png", 1, true);
	t_techo = generateTextures("Texturas/techo.png", 1, true);
	//This must be the last
	t_white = generateTextures("Texturas/white.jpg", 0, false);
}

void myData() {
	float vertices[] = {
		// positions          // texture coords
		 1.0f,  1.0f, 0.0f,   10.0f, 6.5f, // top right
		 1.0f, -1.0f, 0.0f,   10.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 6.5f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float verticesTecho[] = {
		// positions          // texture coords
		 1.0f,  1.0f, 0.0f,   2.0f, 2.0f, // top right
		 1.0f, -1.0f, 0.0f,   2.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   0.0f, 2.0f  // top left 
	};
	unsigned int indicesTecho[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(3, VAO);
	glGenBuffers(3, VBO);
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para Piso
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTecho), verticesTecho, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTecho), indicesTecho, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final CGeIHC 2026-2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	//AUDIO

	initAudio();
	playSound("resources/music/ambient.mp3");

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);

	

	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 
	
	vector<std::string> faces{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model carro("resources/objects/lego_car/PoliceCar.obj");
	Model esfera("resources/objects/Esfera/Esfera.obj");
	Model ventanillas("resources/objects/zona_cajas/ventanillas.obj");
	Model banquitas("resources/objects/zona_cajas/banquitas4.obj");
	Model basura("resources/objects/zona_cajas/boteBasura3.obj");
	Model columna("resources/objects/columna/columna.obj");
	Model columnaO("resources/objects/column/column.obj");
	Model stand("resources/objects/stand/stand.obj");
	Model pendulos("resources/objects/pendulo/pendulos.obj");
	Model pendulo("resources/objects/pendulo/pendulo.obj");
	Model domino("resources/objects/domino/domino.obj");

	// Modelos del brazo robotico
	Model raFixedBase("resources/objects/robotArm/ra_fixedBase.obj");
	Model raLeft("resources/objects/robotArm/ra_left.obj");
	Model raRight("resources/objects/robotArm/ra_right.obj");
	Model raCilinder03("resources/objects/robotArm/ra_cilinder03.obj");
	Model raCilinder("resources/objects/robotArm/ra_cilinder.obj");
	Model raShortArm("resources/objects/robotArm/ra_shortArm.obj");
	Model raCilinder02("resources/objects/robotArm/ra_cilinder02.obj");
	Model raMediumArm("resources/objects/robotArm/ra_mediumArm.obj");
	Model raCilinder01("resources/objects/robotArm/ra_cilinder01.obj");
	Model raLongArm("resources/objects/robotArm/ra_longArm.obj");
	Model raCilinder00("resources/objects/robotArm/ra_cilinder00.obj");
	Model raAxe("resources/objects/robotArm/ra_axe.obj");
	Model raRotativeBase("resources/objects/robotArm/ra_rotativeBase.obj");

	// Modelos del dron
	Model droneBody("resources/objects/drone/droneBody.obj");
	Model droneProp_1("resources/objects/drone/dronePropeller.obj");
	Model droneProp_2("resources/objects/drone/dronePropeller.obj");
	Model droneProp_3("resources/objects/drone/dronePropeller.obj");
	Model droneProp_4("resources/objects/drone/dronePropeller.obj");

	// Modelos de pájaros
	Model redBird("resources/objects/redBird/redBird.obj");
	Model yellowBird("resources/objects/yellowBird/yellowBird.obj");

	ModelAnim Caminar("resources/objects/Caminar/Caminar.dae");
	Caminar.initShaders(animShader.ID);

	ModelAnim Waving("resources/objects/Waving/Waving.dae");
	Waving.initShaders(animShader.ID);

	//Creación de animación

	KeyFrame[0].posX = 0;
	KeyFrame[0].posY = 3.0;
	KeyFrame[0].posZ = 0;
	KeyFrame[0].giroMonito = 0;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 3.0f;
	KeyFrame[1].posZ = 30.0f;
	KeyFrame[1].giroMonito = 0;

	KeyFrame[2].posX = 0.0f;
	KeyFrame[2].posY = 3.0f;
	KeyFrame[2].posZ = 30.0f;
	KeyFrame[2].giroMonito = 90.0f;

	KeyFrame[3].posX = 111.0f;
	KeyFrame[3].posY = 3.0f;
	KeyFrame[3].posZ = 30.0f;
	KeyFrame[3].giroMonito = 90.0;

	KeyFrame[4].posX = 111.0f;
	KeyFrame[4].posY = 3.0f;
	KeyFrame[4].posZ = 30.0f;
	KeyFrame[4].giroMonito = 180.0;

	KeyFrame[5].posX = 111.0f;
	KeyFrame[5].posY = 3.0f;
	KeyFrame[5].posZ = -132.0f;
	KeyFrame[5].giroMonito = 180.0;

	KeyFrame[6].posX = 111.0f;
	KeyFrame[6].posY = 3.0f;
	KeyFrame[6].posZ = -132.0f;
	KeyFrame[6].giroMonito = 270.0;

	KeyFrame[7].posX = -90.0f;
	KeyFrame[7].posY = 3.0f;
	KeyFrame[7].posZ = -132.0f;
	KeyFrame[7].giroMonito = 270.0;

	KeyFrame[8].posX = -90.0f;
	KeyFrame[8].posY = 3.0f;
	KeyFrame[8].posZ = -132.0f;
	KeyFrame[8].giroMonito = 360.0;

	KeyFrame[9].posX = -90.0f;
	KeyFrame[9].posY = 3.0f;
	KeyFrame[9].posZ = 0.0f;
	KeyFrame[9].giroMonito = 360.0;

	KeyFrame[10].posX = -90.0f;
	KeyFrame[10].posY = 3.0f;
	KeyFrame[10].posZ = 0.0f;
	KeyFrame[10].giroMonito = 450.0;

	KeyFrame[11].posX = 0.0f;
	KeyFrame[11].posY = 3.0f;
	KeyFrame[11].posZ = 0.0f;
	KeyFrame[11].giroMonito = 450.0;

	KeyFrame[12].posX = 0.0f;
	KeyFrame[12].posY = 3.0f;
	KeyFrame[12].posZ = 0.0f;
	KeyFrame[12].giroMonito = 360.0f;

	interpolation();


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.6f, 0.6f, 0.6f));		//Luz que viene de todas las direcciones
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));		//Luz que proviene de un punto. Con el cambio de los valores podemos cambiar el color
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));		//Luz de los reflejos		
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", lightColorChange);
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(15.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);
		
		staticShader.setFloat("material_shininess", 32.0f);
		
		
		glm::mat4 tmp = glm::mat4(1.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		viewOp = camera.GetViewMatrix();
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/


		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		
		glBindVertexArray(VAO[0]);
		//Piso
		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, -51.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(183.0f, 141.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_piso);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[2]);
		//Techo
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 114.0f, -51.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(183.0f, 141.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_techo);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);


		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------

		
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movAuto_x, movAuto_y, movAuto_z - 15.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orienta_y), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.1f, 2.1f, 2.1f));
		staticShader.setMat4("model", modelOp);
		carro.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Esfera
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(movEsf_x, movEsf_y, movEsf_z));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orientaEsf), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(orientaEsf_y), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.1f, 2.1f, 2.1f));
		staticShader.setMat4("model", modelOp);
		esfera.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Dominos
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 11.1f, -157.0f));
		tmp = modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaDom[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", modelOp);
		domino.Draw(staticShader);

		tmp = modelOp = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 3.9f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaDom[1]), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", modelOp);
		domino.Draw(staticShader);

		tmp = modelOp = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 3.9f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaDom[2]), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", modelOp);
		domino.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Pendulo
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(51.0f, 40.5f, -99.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader.setMat4("model", modelOp);
		pendulos.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 40.5f, -99.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaPen1), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader.setMat4("model", modelOp);
		pendulo.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(57.0f, 40.5f, -99.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(orientaPen2), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 1.5f, 1.5f));
		staticShader.setMat4("model", modelOp);
		pendulo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Brazo Robotico
		// -------------------------------------------------------------------------------------------------------------------------

		glm::mat4 jerarquia;

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-126.0f, 25.5f, -120.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.21f));
		jerarquia = modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raFixedBase.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(0.0f, 10.652f, 0.0f));
		jerarquia = modelOp = glm::rotate(modelOp, glm::radians(baseRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raRotativeBase.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(0.0f, 10.471f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raAxe.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(0.09f, 13.601f, 0.139));
		jerarquia = modelOp = glm::rotate(modelOp, glm::radians(cil0Rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raCilinder00.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(8.35f, 0.16f, 0.3f));
		staticShader.setMat4("model", modelOp);
		raLongArm.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(-8.0f, 51.3f, -27.0f));
		jerarquia = modelOp = glm::rotate(modelOp, glm::radians(cil1Rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raCilinder01.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(-6.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raMediumArm.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(6.0f, -30.0f, 38.0f));
		jerarquia = modelOp = glm::rotate(modelOp, glm::radians(cil2Rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raCilinder02.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(4.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raShortArm.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(-4.5f, 27.0f, -5.0f));
		staticShader.setMat4("model", modelOp);
		raCilinder.Draw(staticShader);
		
		jerarquia = modelOp = glm::translate(jerarquia, glm::vec3(4.0f, 11.0f, -13.0f));
		staticShader.setMat4("model", modelOp);
		raCilinder03.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(-5.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(handRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raLeft.Draw(staticShader);
		
		modelOp = glm::translate(jerarquia, glm::vec3(-5.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-handRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		raRight.Draw(staticShader);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Dron
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(dronPosition_x,dronPosition_y,dronPosition_z));
		modelOp = glm::rotate(modelOp, glm::radians(dronRotation_xz), glm::vec3(1.0f, 0.0f, 1.0f));
		modelOp = glm::rotate(modelOp, glm::radians(dronRotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
		jerarquia = modelOp = glm::scale(modelOp, glm::vec3(2.0f));
		staticShader.setMat4("model", modelOp);
		droneBody.Draw(staticShader);

		modelOp = glm::translate(jerarquia, glm::vec3(-4.0f, 1.1f, 3.9f));
		modelOp = glm::rotate(modelOp, glm::radians(-propellerRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		droneProp_1.Draw(staticShader);

		modelOp = glm::translate(jerarquia, glm::vec3(4.0f, 1.1f, 3.9f));
		modelOp = glm::rotate(modelOp, glm::radians(-propellerRotation + 70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		droneProp_2.Draw(staticShader);

		modelOp = glm::translate(jerarquia, glm::vec3(-4.0f, 1.1f, -3.6f));
		modelOp = glm::rotate(modelOp, glm::radians(-propellerRotation + 105.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		droneProp_3.Draw(staticShader);

		modelOp = glm::translate(jerarquia, glm::vec3(4.0f, 1.1f, -3.6f));
		modelOp = glm::rotate(modelOp, glm::radians(-propellerRotation + 35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", modelOp);
		droneProp_4.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Pájaros
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(redBirdPos_x, redBirdPos_y, redBirdPos_z));
		modelOp = glm::rotate(modelOp, glm::radians(redBirdRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.6f));
		staticShader.setMat4("model", modelOp);
		redBird.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 11.0f, -160.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.3f));
		staticShader.setMat4("model", modelOp);
		yellowBird.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 3.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		ventanillas.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 3.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		banquitas.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 3.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		basura.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-141.0f, 3.0f, -60.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		columnaO.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 3.0f, -60.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		columnaO.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(126.0f, 3.0f, -60.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		columnaO.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(126.0f, 3.0f, -171.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		columna.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 3.0f, -171.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", modelOp);
		columna.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(51.0f, 3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.6f, 0.75f, 0.6f));
		staticShader.setMat4("model", modelOp);
		stand.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(51.0f, 3.0f, -102.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.6f, 0.75f, 0.6f));
		staticShader.setMat4("model", modelOp);
		stand.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, 3.0f, -120.0f));
		modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.6f, 0.75f, 0.6f));
		staticShader.setMat4("model", modelOp);
		stand.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personajes
		// -------------------------------------------------------------------------------------------------------------------------

		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
		modelOp = glm::rotate(modelOp, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.27f));
		animShader.setMat4("model", modelOp);
		Caminar.Draw(animShader); 

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(51.0f, 3.0f, -27.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.27f));
		animShader.setMat4("model", modelOp);
		Waving.Draw(animShader);
		

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	glfwTerminate();
	return 0;
}

// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		std::cout << "X: "<< camera.Position.x << std::endl;
		std::cout << "Y: " << camera.Position.y << std::endl;
		std::cout << "Z: " << camera.Position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//Configurar botones de camaras
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) 
	{
		camera.Position.x = -180.0f;
		camera.Position.y = 60.0f;
		camera.Position.z = 45.0f;
		camera.Front.x = 1.0f;
		camera.Front.y = 0.0f;
		camera.Front.z = 0.0f;
	}
		
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		camera.Position.x = -35.16f;
		camera.Position.y = 44.78f;
		camera.Position.z = -120.0f;
		camera.Front.x = -1.0f;
		camera.Front.y = 0.0f;
		camera.Front.z = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		camera.Position.x = 51.29f;
		camera.Position.y = 39.16f;
		camera.Position.z = 73.8f;
		camera.Front.x = 0.0f;
		camera.Front.y = 0.0f;
		camera.Front.z = -1.0f;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
