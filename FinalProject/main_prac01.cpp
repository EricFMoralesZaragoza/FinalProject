/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2026-2   ---------------------------*/
/*------------- (Dueñas Jarvio Pablo Alam) ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderYellow = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 1.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	float vertices[] = 
	{
		// positions         //
		//-0.8f,  0.8f, 0.0f,  //0  valores de x, y, z
		//0.8f,  0.8f, 0.0f,  //1  valores de x, y, z
		//0.8f,  0.6f, 0.0f,  //2  valores de x, y, z
		//-0.2f,  0.6f, 0.0f,  //3  valores de x, y, z
		//-0.2f,  0.1f, 0.0f,  //4  valores de x, y, z
		//0.3f,  0.1f, 0.0f,  //5  valores de x, y, z
		//0.3f,  -0.2f, 0.0f,  //6  valores de x, y, z
		//-0.2f,  -0.2f, 0.0f,  //7  valores de x, y, z
		//-0.2f,  -0.8f, 0.0f,  //8  valores de x, y, z
		//-0.8f,  -0.8f, 0.0f,  //9  valores de x, y, z

		//POSICIONES TORTUGA               COLORES RGB (todos los vertices deben contener un color) solo cabeza y aleta derecha diferente color

		-0.08833f,   0.73775f, 0.0f,       1.0f, 1.0f, 1.0f,  //A  valores de x, y, z  1
		0.05362f,    0.63808f, 0.0f,       1.0f, 0.0f, 0.0f,  //B  valores de x, y, z  2
		0.0204f,   0.42968f, 0.0f,         1.0f, 1.0f, 0.0f,  //C  valores de x, y, z  3
		-0.08531f,   0.48706f, 0.0f,       0.0f, 1.0f, 0.0f,  //D  valores de x, y, z  4
		-0.18498f, 0.43874f, 0.0f,         0.0f, 0.0f, 1.0f,  //E  valores de x, y, z  5
		-0.2182f, 0.63204f, 0.0f,          0.0f, 0.0f, 1.0f,  //F  valores de x, y, z  6

		0.08684f, 0.39344f, 0.0f,          1.0f, 1.0f, 1.0f, //G //7
		0.22577f, 0.30887f, 0.0f,          1.0f, 1.0f, 1.0f, //M //8
		0.37075f, -0.08679f, 0.0f,         1.0f, 1.0f, 1.0f, //N 9
		0.23483f, -0.42505f, 0.0f,         1.0f, 1.0f, 1.0f, //O 10 
		0.08382f, -0.56399f, 0.0f,         1.0f, 1.0f, 1.0f, //S 11
		-0.08229f, -0.67574f, 0.0f,         1.0f, 1.0f, 1.0f, //T 12
		-0.23331f, -0.55795f, 0.0f,         1.0f, 1.0f, 1.0f, //U 13
		-0.38432f, -0.42807f, 0.0f,         1.0f, 1.0f, 1.0f, //A1 14
		-0.52627f, -0.09585f, 0.0f,         1.0f, 1.0f, 1.0f, //B1 15
		-0.39338f, 0.29679f, 0.0f,         1.0f, 1.0f, 1.0f, //C1 16 
		-0.25445f, 0.39646f, 0.0f,         1.0f, 1.0f, 1.0f, //I1 17
		-0.18498f, 0.43874f, 0.0f,         1.0f, 1.0f, 1.0f, //E2 18


	};

	unsigned int indices[] =
	{
		0, 1, 2, 3, 4, 5, //CABEZA (coinciden el numero de los elementos con el numero de su orden)

		6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, //Caparazon

	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1 2026", NULL, NULL); //Bug?
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


	//My Functions
	//Setup Data to use
	myData();
	//To Setup Shaders
	setupShaders();
    
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Display Section
		glUseProgram(shaderProgramColor);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(10.0); 
		glLineWidth(4.0f);
		glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);//numero de elementos a leer (6)    comienza desde 0
		glDrawElements(GL_TRIANGLE_FAN, 12, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));
		//glDrawArrays(GL_LINE_LOOP, 0, 10); // comando encargado de dibujar la geometria (0 para indicar desde donde comienza a leer, el 2 para indicar donde termina)
		//glDrawArrays(GL_TRIANGLES, 3, 2); //que comience en el 3 y solo lea 2

		glBindVertexArray(0);
		glUseProgram(0);

		//End of Display Section

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
        glfwSetWindowShouldClose(window, true);  //Close
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

//Erase in order to work