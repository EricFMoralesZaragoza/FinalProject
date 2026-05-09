/*---------------------------------------------------------*/
/* ----------------   Práctica 3 --------------------------*/
/*-----------------    2026-2   ---------------------------*/
/*--Alumno:	Morales Zaragoza Eric Francisco ---------------*/
/*-------Cuenta:     316299840           ------------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];

void myData(void);
void getResolution(void);

//For Keyboard
float movX = 0.0f;
float movY = 0.0f;
float movZ = -6.0f;	//Esto para que las figuras no inicen pegadas a la cámara
float giroY = 0.0f;
float giroX = 0.0f;


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	

	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	GLfloat verticesLuis[] = {
		// positions			//Color
		-0.5f, 0.7f, -0.5f,		1.0f, 0.0f, 0.0f,	//V0
		0.5f, 0.7f, -0.5f,		1.0f, 0.0f, 1.0f,	//V1
		0.5f, 0.3f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		-0.2f, 0.3f, -0.5f,		1.0f, 1.0f, 0.0f,	//V3
		-0.2f, -0.3f, -0.5f,	0.0f, 1.0f, 0.0f,	//V4
		0.5f, -0.3f, -0.5f,		0.0f, 1.0f, 1.0f,	//V5
		0.5f, -0.7f, -0.5f,		0.0f, 0.0f, 1.0f,	//V6
		-0.5f, -0.7f, -0.5f,	1.0f, 1.0f, 1.0f,	//V7
	};

	unsigned int indicesLuis[] =
	{
		3, 2, 1, 0, 7, 4,
		7, 4, 5, 6
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);	//Only if we are going to work with index

	//Fill with the cube information
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Fill with the word information
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLuis), verticesLuis, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLuis), indicesLuis, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 3 20262", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	//Prepare to draw the scene with Projections
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	myShader.use();
	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projectionOp = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);	//Define el lienzo. El primer par para los valores de X, los segundos para el de Y y con los útimos dos los de Z

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		/*******************************************/
		//Use "view" in order to affect all models
		//	El segundo parámetro son los valores que queremos mover en los ejes x, y, z
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		//Rotación. Parámetros (Referencia, Cuanto queremos que gire en radianes, vector de dirección de giro que se maneja por el método de la mano derecha)
		viewOp = glm::rotate(viewOp, glm::radians(giroY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(giroX), glm::vec3(1.0f, 0.0f, 0.0f));
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		
		//Get the data from container 1
		glBindVertexArray(VAO[1]);	//Enable data array [1]
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]); //Only if we are going to work with index

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.7f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp);
		//glDrawArrays(GL_LINE_LOOP, 0, 8); //My C
		//glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));	//to Draw using index
		//glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));	//to Draw using index

		glm::mat4 temp; //Variable para guardar el estado del primer cubo

		/*-------------------Second figure-------------------*/
		//Get the data from container 0
		glBindVertexArray(VAO[0]);	//Enable data array [0]
		//IMPORTA mucho el orden en que se hace la traslación y la escala. Lo mejor es hacer primero la traslación y luego la rotación
		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));	//Matriz de modelo que permiten modificar ÚNICAMENTE al modelo del cubo
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //My Cube

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.0f, 0.0f));	//El primer parámetro es la matriz del origen
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //My Cube2

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -1.0f, 0.0f));	
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(9.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(11.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -4.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -4.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -4.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(13.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Quinta hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(6.5f, -5.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Sexta hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.5f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -6.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Séptima hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -7.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Octava hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(7.5f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -8.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));						//Rojo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Novena hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -9.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -9.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -9.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Décima hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.5f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.5f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -10.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(11.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Unodecima hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -11.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -11.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, -11.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(7.5f, -11.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -11.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(9.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Duodecima hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -12.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -12.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -12.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(6.5f, -12.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -12.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(7.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//treceava hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -13.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, -13.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -13.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -13.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -13.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(7.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Catorceava hilera

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -14.0f, 0.0f));	//El primer parámetro es la matriz del origen
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //My Cube2

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -14.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -14.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));						//Amarillo
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		//Quiceava hilera
		temp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -15.0f, 0.0f));	//Matriz de modelo que permiten modificar ÚNICAMENTE al modelo del cubo
		modelOp = glm::scale(modelOp, glm::vec3(5.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //My Cube

		glBindVertexArray(0);
		/*****************************************************************/
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.003f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.003f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.003f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.003f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.003f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.003f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		giroY += 0.06f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		giroY -= 0.06f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		giroX += 0.06f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		giroX -= 0.06f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}