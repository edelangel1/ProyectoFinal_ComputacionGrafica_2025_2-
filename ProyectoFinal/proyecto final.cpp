#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void inicializarSillas();
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void reiniciar();
void DoMovement();
void animacionSilla();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);

// estructura silla
struct Silla
{
	glm::vec3 posicionInicial;
	glm::vec3 posicionActual;
	float rotation;
	float scale;
	int estadoAnimacion;
	bool dibujar;
	bool sillaNueva;
};
//Animacion sillas mesas largas
bool animacionSilla1 = false;
Silla sillas[31];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	//models
	Model sillaVieja((char*)"Models/sillaVieja/sillaVieja.obj");
	Model sillaNueva((char*)"Models/sillaNueva/sillaNueva.obj");


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// inicializamos las sillas
	inicializarSillas();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacionSilla();

		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		
		
		
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.6f,0.6f,0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.3f, 0.3f, 0.3f);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        view = camera.GetViewMatrix();	
		glm::mat4 modelSilla(1);

	
		
		//Carga de modelo 
		for (size_t i = 0; i < 31; i++)
		{
			if (sillas[i].dibujar)
			{
				modelSilla = glm::mat4(1);
				modelSilla = glm::translate(modelSilla, glm::vec3(sillas[i].posicionActual.x, sillas[i].posicionActual.y, sillas[i].posicionActual.z));
				modelSilla = glm::rotate(modelSilla, glm::radians(sillas[i].rotation), glm::vec3(0.0f, 1.0f, 0.0f));
				modelSilla = glm::scale(modelSilla, glm::vec3(sillas[i].scale, sillas[i].scale, sillas[i].scale));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSilla));
				glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
				if (sillas[i].sillaNueva) {
					sillaNueva.Draw(lightingShader);
				}
				else {
					sillaVieja.Draw(lightingShader);
				}
			}
		}
	

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glm::mat4 model(1);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[0]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	if (key == GLFW_KEY_N)
	{
		if (!animacionSilla1)
		{
			animacionSilla1 = true;
		}
	}
	if (key == GLFW_KEY_R)
	{
		reiniciar();
	}

}

// Reiniciar la simulacion
void reiniciar()
{
	animacionSilla1 = false;
	inicializarSillas();
}

// Inicializa las sillas
void inicializarSillas()
{
	glm::vec3 posSillas[] = {
		// silla profesor
		glm::vec3(1.0f, 0.0f, -6.0f),
		// silas mesa 1
		glm::vec3(2.0f, 0.0f, -10.0f),
		glm::vec3(2.0f, 0.0f, -9.0f),
		glm::vec3(2.0f, 0.0f, -8.0f),
		// sillas mesa 2
		glm::vec3(2.0f, 0.0f, -7.0f),
		glm::vec3(2.0f, 0.0f, -6.0f),
		glm::vec3(2.0f, 0.0f, -5.0f),
		// sillas mesa 3
		glm::vec3(2.0f, 0.0f, -4.0f),
		glm::vec3(2.0f, 0.0f, -3.0f),
		glm::vec3(2.0f, 0.0f, -2.0f),
		// sillas mesa 4
		glm::vec3(4.0f, 0.0f, -10.0f),
		glm::vec3(4.0f, 0.0f, -9.0f),
		glm::vec3(4.0f, 0.0f, -8.0f),
		// sillas mesa 5
		glm::vec3(4.0f, 0.0f, -7.0f),
		glm::vec3(4.0f, 0.0f, -6.0f),
		glm::vec3(4.0f, 0.0f, -5.0f),
		// sillas mesa 6
		glm::vec3(4.0f, 0.0f, -4.0f),
		glm::vec3(4.0f, 0.0f, -3.0f),
		glm::vec3(4.0f, 0.0f, -2.0f),
		// sillas mesa 7
		glm::vec3(6.0f, 0.0f, -13.0f),
		glm::vec3(6.0f, 0.0f, -12.0f),
		glm::vec3(6.0f, 0.0f, -11.0f),
		// sillas mesa 8
		glm::vec3(6.0f, 0.0f, -10.0f),
		glm::vec3(6.0f, 0.0f, -9.0f),
		glm::vec3(6.0f, 0.0f, -8.0f),
		// sillas mesa 9
		glm::vec3(6.0f, 0.0f, -7.0f),
		glm::vec3(6.0f, 0.0f, -6.0f),
		glm::vec3(6.0f, 0.0f, -5.0f),
		// sillas mesa 10
		glm::vec3(6.0f, 0.0f, -4.0f),
		glm::vec3(6.0f, 0.0f, -3.0f),
		glm::vec3(6.0f, 0.0f, -2.0f)
	};
	float rotacionSillas[] = {
		// silla profesor
		130.0f,
		// sillas mesa 1
		130.0f, 130.0f, 130.0f,
		// sillas mesa 2
		130.0f, 130.0f, 130.0f,
		// sillas mesa 3
		130.0f, 130.0f, 130.0f,
		// sillas mesa 4
		130.0f, 130.0f, 130.0f,
		// sillas mesa 5
		130.0f, 130.0f, 130.0f,
		// sillas mesa 6
		130.0f, 130.0f, 130.0f,
		// sillas mesa 7
		130.0f, 130.0f, 130.0f,
		// sillas mesa 8
		130.0f, 130.0f, 130.0f,
		// sillas mesa 9
		130.0f, 130.0f, 130.0f,
		// sillas mesa 10
		130.0f, 130.0f, 130.0f
	};
	int estadoAnimacion[] = {
		// silla profesor
		1,
		// sillas mesa 1
		6, 6, 6,
		// sillas mesa 2
		6, 6, 6,
		// sillas mesa 3
		6, 6, 6,
		// sillas mesa 4
		8, 8, 8,
		// sillas mesa 5
		8, 8, 8,
		// sillas mesa 6
		8, 8, 8,
		// sillas mesa 7
		12, 12, 12,
		// sillas mesa 8
		12, 12, 12,
		// sillas mesa 9
		12, 12, 12,
		// sillas mesa 10
		12, 12, 12
	};
	for (size_t i = 0; i < 31; i++)
	{
		sillas[i].posicionInicial = posSillas[i];
		sillas[i].posicionActual = posSillas[i];
		sillas[i].rotation = rotacionSillas[i];
		sillas[i].scale = 1.0f;
		sillas[i].estadoAnimacion = estadoAnimacion[i];
		sillas[i].dibujar = true;
		sillas[i].sillaNueva = false;
	}
}

// Animaciones
void animacionSilla() {
	if (!animacionSilla1) return;

	// Puntos de giro
	float paso = 0.01f;
	float x1 = 0.5f;
	float x2 = 3.0f;
	float x3 = 5.0f;
	float x4 = 7.0f;
	float z1 = -1.0f;
	float z2 = 0.0f;
	
	for (size_t i = 0; i < 31; i++)
	{
		switch (sillas[i].estadoAnimacion)
		{
		case 1:
			sillas[i].posicionActual.x -= paso;
			if (sillas[i].posicionActual.x <= x1)
			{
				sillas[i].posicionActual.x = x1;
				sillas[i].estadoAnimacion = 2;
			}
			break;
		case 2:
			sillas[i].posicionActual.z += paso;
			if (sillas[i].posicionActual.z >= z1)
			{
				sillas[i].posicionActual.z = z1;
				sillas[i].estadoAnimacion = 3;
			}
			break;
		case 3:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x2)
			{
				sillas[i].posicionActual.x = x2;
				sillas[i].estadoAnimacion = 4;
			}
			break;
		case 4:
			sillas[i].posicionActual.z += paso;
			if (sillas[i].posicionActual.z >= z2)
			{
				sillas[i].posicionActual.z = z2;
				sillas[i].estadoAnimacion = 5;
			}
			break;
		case 5:
			sillas[i].dibujar = false;
			if (sillas[19].estadoAnimacion != 5) { break; }
			sillas[i].dibujar = true;
			sillas[i].sillaNueva = true;
			if (i == 0) { sillas[i].estadoAnimacion = 14; }
			else if(i < 10) { sillas[i].estadoAnimacion = 19; }
			else if (i < 19) { sillas[i].estadoAnimacion = 21; }
			else { sillas[i].estadoAnimacion = 23; }
			break;
		case 6:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x2)
			{
				sillas[i].posicionActual.x = x2;
				sillas[i].estadoAnimacion = 7;
			}
			break;
		case 7:
			if (sillas[0].posicionActual.x == x2)
			{
				sillas[i].estadoAnimacion = 4;
			}
			break;
		case 8:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x3)
			{
				sillas[i].posicionActual.x = x3;
				sillas[i].estadoAnimacion = 9;
			}
			break;
		case 9:
			if (sillas[1].posicionActual.z >= z1)
			{
				sillas[i].estadoAnimacion = 10;
			}
			break;
		case 10:
			sillas[i].posicionActual.z += paso;
			if (sillas[i].posicionActual.z >= z1)
			{
				sillas[i].posicionActual.z = z1;
				sillas[i].estadoAnimacion = 11;
			}
			break;
		case 11:
			sillas[i].posicionActual.x -= paso;
			if (sillas[i].posicionActual.x <= x2)
			{
				sillas[i].posicionActual.x = x2;
				sillas[i].estadoAnimacion = 4;
			}
			break;
		case 12:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x4)
			{
				sillas[i].posicionActual.x = x4;
				sillas[i].estadoAnimacion = 13;
			}
			break;
		case 13:
			if (sillas[10].posicionActual.z >= z1)
			{
				sillas[i].estadoAnimacion = 10;
			}
			break;
		case 14:
			sillas[i].posicionActual.z -= paso;
			if (sillas[i].posicionActual.z <= z1)
			{
				sillas[i].posicionActual.z = z1;
				sillas[i].estadoAnimacion = 15;
			}
			break;
		case 15:
			sillas[i].posicionActual.x -= paso;
			if (sillas[i].posicionActual.x <= x1)
			{
				sillas[i].posicionActual.x = x1;
				sillas[i].estadoAnimacion = 16;
			}
			break;
		case 16:
			sillas[i].posicionActual.z -= paso;
			if (sillas[i].posicionActual.z <= sillas[i].posicionInicial.z)
			{
				sillas[i].posicionActual.z = sillas[i].posicionInicial.z;
				sillas[i].estadoAnimacion = 17;
			}
			break;
		case 17:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= sillas[i].posicionInicial.x)
			{
				sillas[i].posicionActual.x = sillas[i].posicionInicial.x;
				sillas[i].estadoAnimacion = 18;
			}
			break;
		case 19:
			if (sillas[i-1].posicionActual.z > z1) { break; }
			sillas[i].posicionActual.z -= paso;
			if (sillas[i].posicionActual.z <= sillas[i].posicionInicial.z)
			{
				sillas[i].posicionActual.z = sillas[i].posicionInicial.z;
				sillas[i].estadoAnimacion = 20;
			}
			break;
		case 20:
			sillas[i].posicionActual.x -= paso;
			if (sillas[i].posicionActual.x <= sillas[i].posicionInicial.x)
			{
				sillas[i].posicionActual.x = sillas[i].posicionInicial.x;
				sillas[i].estadoAnimacion = 18;
			}
			break;
		case 21:
			if (sillas[i-1].posicionActual.z > z1) { break; }
			sillas[i].posicionActual.z -= paso;
			if (sillas[i].posicionActual.z <= z1)
			{
				sillas[i].posicionActual.z = z1;
				sillas[i].estadoAnimacion = 22;
			}
			break;
		case 22:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x3)
			{
				sillas[i].posicionActual.x = x3;
				sillas[i].estadoAnimacion = 19;
			}
			break;
		case 23:
			if (sillas[i-1].posicionActual.z > z1) { break; }
			sillas[i].posicionActual.z -= paso;
			if (sillas[i].posicionActual.z <= z1)
			{
				sillas[i].posicionActual.z = z1;
				sillas[i].estadoAnimacion = 24;
			}
			break;
		case 24:
			sillas[i].posicionActual.x += paso;
			if (sillas[i].posicionActual.x >= x4)
			{
				sillas[i].posicionActual.x = x4;
				sillas[i].estadoAnimacion = 19;
			}
			break;
		default:
			break;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}