/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2026-2   ---------------------------*/
/*--------- Morales Zaragoza Eric Francisco ---------------*/
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
		//Para terminar la práctica la pata derecha y la cabeza serán de color diferente
		// positions			//COLOR RGB de los vertices
		-0.00266, 0.6462, 0.0f,	1.0f, 1.0f, 1.0f,				//A //0
		-0.14995, 0.54173, 0.0f,1.0f, 0.0f, 0.0f,				//B //1
		0.15304, 0.53746, 0.0f,	1.0f, 1.0f, 0.0f,				//C //2
		-0.12434, 0.30701, 0.0f,0.0f, 1.0f, 0.0f,				//D //3
		-0.00516, 0.36081, 0.0f,0.0f, 0.0f, 1.0f,				//E //4
		0.11447, 0.30647, 0.0f,	0.0f, 0.1f, 0.0f,				//F //5
		-0.19384, 0.26007, 0.0f, 0.0f, 1.0f, 0.0f,				//G //6
		0.19227, 0.26073, 0.0f,	 0.0f, 1.0f, 0.0f,				//H //7
		-0.3, 0.4, 0.0f,		 0.0f, 1.0f, 0.0f,				//I //8
		-0.6, 0.4, 0.0f,		 0.0f, 1.0f, 0.0f,				//J //9
		-0.6724, 0.16524, 0.0f, 0.0f, 1.0f, 0.0f,				//K //10
		-0.81433, 0.24099, 0.0f, 0.0f, 1.0f, 0.0f,				//L //11
		-0.34892, 0.16117, 0.0f, 0.0f, 1.0f, 0.0f,				//N //12
		-0.15747, 0.07761, 0,	 0.0f, 0.0f, 1.0f,				//P //13
		0.14183, 0.0809, 0,	 0.0f, 0.0f, 1.0f,					//Q //14
		0.19227, 0.26073, 0,	 0.0f, 0.0f, 1.0f,				//H //15
		-0.12434, 0.30701, 0.0f,0.0f, 1.0f, 0.0f,				//D-verde //16
		0.11447, 0.30647, 0.0f,	0.0f, 1.0f, 0.0f,				//F-verde //17
		-0.51388, -0.28533, 0.0f,0.0f, 1.0f, 0.0f,				//V	//18
		-0.35818, -0.66253, 0.0f,0.0f, 1.0f, 0.0f,				//J1 //19
		-0.18766, -0.81822, 0.0f,0.0f, 1.0f, 0.0f,				//N1 //20
		-0.00845, -0.95502, 0.0f,0.0f, 1.0f, 0.0f,				//M1 //21
		0.17064, -0.82239, 0.0f,0.0f, 1.0f, 0.0f,				//R1 //22
		0.34578, -0.66034, 0.0f,0.0f, 1.0f, 0.0f,				//G1 //23
		0.49971, -0.28513, 0.0f,0.0f, 1.0f, 0.0f,				//F1 //24
		0.34578, 0.15985, 0.0f,0.0f, 1.0f, 0.0f,				//W //25
		-0.00516, 0.36081, 0.0f,0.0f, 1.0f, 0.0f,				//E-verde //26
		0.19227, 0.26073, 0.0f,0.0f, 0.0f, 1.0f,				//H-azul //27
		0.34578, 0.15985, 0.0f,0.0f, 0.0f, 1.0f,				//W-azul //28
		0.66517, 0.16178, 0.0f,0.0f, 0.0f, 1.0f,				//B1 //29
		0.84798, -0.03752, 0.0f,0.0f, 0.0f, 1.0f,				//D1 //30
		0.80192, 0.2388, 0.0f,0.0f, 0.0f, 1.0f,					//C1 //31
		0.6, 0.4, 0.0f,0.0f, 0.0f, 1.0f,						//A1 //32
		0.28656, 0.3945, 0.0f,0.0f, 0.0f, 1.0f,					//Z //33
		-0.3, 0.4, 0.0f,0.0f, 1.0f, 0.0f,						//I //34
		-0.6, 0.4, 0.0f,0.0f, 1.0f, 0.0f,						//L //35
		-0.86257, -0.01998, 0.0f,0.0f, 1.0f, 0.0f,				//M //36
		-0.67563, 0.16414, 0.0f,0.0f, 1.0f, 0.0f,				//K //37
		-0.49808, -0.86819, 0.0f,0.0f, 1.0f, 0.0f,				//Q1 //38
		-0.41567, -0.98862, 0.0f,0.0f, 1.0f, 0.0f,				//P1 //39
		-0.29326, -0.98942, 0.0f,0.0f, 1.0f, 0.0f,				//O1 //40
		0.27635, -0.99342, 0.0f,0.0f, 1.0f, 0.0f,				//S1 //41
		0.39636, -0.99342, 0.0f,0.0f, 1.0f, 0.0f,				//T1 //42
		0.48276, -0.87262, 0.0f,0.0f, 1.0f, 0.0f,				//U1 //43


	};

	unsigned int indices[] =
	{
		4, 5, 2, 0, 1, 3,
		26, 16, 6, 12, 18, 19, 20, 21, 22, 23, 24, 25, 7, 17,
		27, 28, 29, 30, 31, 32, 33,
		6, 8, 9, 11, 36, 10, 12,
		19, 38, 39, 40, 20,
		22, 41, 42, 43, 23
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
		glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLE_FAN, 14, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_INT, (void*)(20 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_INT, (void*)(27 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(34 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(39 * sizeof(float)));
		//glDrawArrays(GL_POINTS, 0, 1);


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