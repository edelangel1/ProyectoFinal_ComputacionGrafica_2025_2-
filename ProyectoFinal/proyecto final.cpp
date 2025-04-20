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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void reiniciar();
void DoMovement();
void animacion();
void controlCamara();
void animacionSilla();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(20.0f, 10.0f, -22.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
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

// Control de animacion
bool animacionActiva = false;

// estructura modelo silla
struct Silla
{
	glm::vec3 posicionFinal;
	glm::vec3 posicionActual;
	float anguloActual;
	float anguloFinal;
	glm::vec3 scale;
	int estadoAnimacion;
	bool dibujar;
	bool sillaNueva;
};

//Animacion sillas
Silla sillas[31];

// Deltatime
GLfloat currentFrame = glfwGetTime();
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

	// Models
	Model sillaVieja((char*)"Models/sillaVieja/sillaVieja.obj");
	Model sillaNueva((char*)"Models/sillaNueva/sillaNueva.obj");
	Model salon((char*)"Models/salon/Estructura.obj");

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

	// Inicializar las sillas
	inicializarSillas();
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

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
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

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

		// Carga de modelo 
		// Salon
		glm::mat4 modelSalon(1);
		modelSalon = glm::mat4(1);
		modelSalon = glm::rotate(modelSalon, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSalon = glm::scale(modelSalon, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSalon));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		salon.Draw(lightingShader);
		// Sillas
		glm::mat4 modelSilla(1);
		for (size_t i = 0; i < 31; i++)
		{
			if (sillas[i].dibujar)
			{
				modelSilla = glm::mat4(1);
				modelSilla = glm::translate(modelSilla, glm::vec3(sillas[i].posicionActual.x, sillas[i].posicionActual.y, sillas[i].posicionActual.z));
				modelSilla = glm::rotate(modelSilla, glm::radians(sillas[i].anguloActual), glm::vec3(0.0f, 1.0f, 0.0f));
				modelSilla = glm::scale(modelSilla, glm::vec3(sillas[i].scale.x, sillas[i].scale.y, sillas[i].scale.z));
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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (animacionActiva) return;
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
		if (!animacionActiva)
		{
			animacionActiva = true;
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
	animacionActiva = false;
	inicializarSillas();
}

// Control general de animacion
void animacion()
{
	if (!animacionActiva) return;
	animacionSilla();
	controlCamara();
}

// Control de la camara
void controlCamara()
{
	float ciclo = fmod(currentFrame, 1500.0f);
	float radioMayor = 19.0;
	float radioMenor = 16.0f;
	float y = 8.0f;
	glm::vec3 centro = glm::vec3(18.5f, 0.0f, -22.0f);

	// Calcular nueva posición
	float x = (cos(ciclo) * radioMenor) + centro.x;
	float z = (sin(ciclo) * radioMayor) + centro.z;
	glm::vec3 newPosition = glm::vec3(x, y, z);

	// Establecer nueva posición
	camera.SetPosition(newPosition);

	// Calcular y establecer la dirección hacia el centro
	glm::vec3 newFront = glm::normalize(centro - newPosition);
	camera.SetFront(newFront);
}

// Inicializa las sillas
void inicializarSillas()
{
	float posSillasY = 0.4;
	float posSillasX19_30 = 33.5f;
	glm::vec3 posSillas[] = {
		// silla profesor
		glm::vec3(6.6f, posSillasY, -22.1f),
		// silas mesa 1
		glm::vec3(13.8f, posSillasY, -33.0f),
		glm::vec3(18.0f, posSillasY, -31.0f),
		glm::vec3(18.0f, posSillasY, -29.0f),
		// sillas mesa 2
		glm::vec3(18.0f, posSillasY, -24.5f),
		glm::vec3(20.0f, posSillasY, -22.0f),
		glm::vec3(17.6f, posSillasY, -19.0f),
		// sillas mesa 3
		glm::vec3(18.6f, posSillasY, -15.8f),
		glm::vec3(18.6f, posSillasY, -13.2f),
		glm::vec3(15.6f, posSillasY, -11.3f),
		// sillas mesa 4
		glm::vec3(20.9f, posSillasY, -38.9f),
		glm::vec3(24.6f, posSillasY, -36.6f),
		glm::vec3(24.6f, posSillasY, -34.4f),
		// sillas mesa 5
		glm::vec3(26.1f, posSillasY, -31.0f),
		glm::vec3(28.8f, posSillasY, -28.5f),
		glm::vec3(27.1f, posSillasY, -25.3f),
		// sillas mesa 6
		glm::vec3(28.8f, posSillasY, -18.5f),
		glm::vec3(28.8f, posSillasY, -16.5f),
		glm::vec3(25.7f, posSillasY, -14.5f),
		// sillas mesa 7
		glm::vec3(posSillasX19_30, posSillasY, -38.4f),
		glm::vec3(posSillasX19_30, posSillasY, -36.3f),
		glm::vec3(posSillasX19_30, posSillasY, -33.8f),
		// sillas mesa 8
		glm::vec3(posSillasX19_30, posSillasY, -28.88f),
		glm::vec3(posSillasX19_30, posSillasY, -26.58f),
		glm::vec3(posSillasX19_30, posSillasY, -24.28f),
		// sillas mesa 9
		glm::vec3(posSillasX19_30, posSillasY, -19.36f),
		glm::vec3(posSillasX19_30, posSillasY, -17.06f),
		glm::vec3(posSillasX19_30, posSillasY, -14.76f),
		// sillas mesa 10
		glm::vec3(posSillasX19_30, posSillasY, -9.84f),
		glm::vec3(posSillasX19_30, posSillasY, -7.54f),
		glm::vec3(posSillasX19_30, posSillasY, -5.24f)
	};
	float anguloSillas[] = {
		// silla profesor
		180.0f,
		// sillas mesa 1
		0.0f, 0.0f, 0.0f,
		// sillas mesa 2
		0.0f, 0.0f, 0.0f,
		// sillas mesa 3
		0.0f, 0.0f, 0.0f,
		// sillas mesa 4
		0.0f, 0.0f, 0.0f,
		// sillas mesa 5
		0.0f, 0.0f, 0.0f,
		// sillas mesa 6
		0.0f, 0.0f, 0.0f,
		// sillas mesa 7
		0.0f, 0.0f, 0.0f,
		// sillas mesa 8
		0.0f, 0.0f, 0.0f,
		// sillas mesa 9
		0.0f, 0.0f, 0.0f,
		// sillas mesa 10
		0.0f, 0.0f, 0.0f
	};
	int estadoAnimacion[] = {
		// silla profesor
		1,
		// sillas mesa 1
		3, 3, 3,
		// sillas mesa 2
		3, 3, 3,
		// sillas mesa 3
		3, 3, 3,
		// sillas mesa 4
		7, 7, 7,
		// sillas mesa 5
		7, 7, 7,
		// sillas mesa 6
		7, 7, 7,
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
		sillas[i].posicionFinal = posSillas[i];
		sillas[i].posicionActual = posSillas[i];
		sillas[i].anguloActual = anguloSillas[i];
		sillas[i].anguloFinal = anguloSillas[i];
		sillas[i].scale = glm::vec3(0.5f, 0.6f, 0.5f);
		sillas[i].estadoAnimacion = estadoAnimacion[i];
		sillas[i].dibujar = true;
		sillas[i].sillaNueva = false;
	}
}

// Animaciones
void animacionSilla() {
	// Limites de las sillas
	float x1 = 3.5f;
	float x2 = 21.0f;
	float x3 = 29.5f;
	float x4 = 35.5f;
	float z1 = -33.0f;
	float z2 = -3.0f;
	float z3 = 1.0f;
	// Distancia de giro 
	float distGiro = abs(sillas[30].posicionFinal.z - z2) * 1.3;
	// Cambios de posicion y rotacion
	float pasoPosicion = 0.03f;
	float pasoRotacion = 90 / ceil(distGiro / pasoPosicion);

	for (size_t i = 0; i < 31; i++)
	{
		switch (sillas[i].estadoAnimacion)
		{
		case 1: // Mover silla 0 a x1
			if (sillas[i].posicionActual.x > x1)
			{
				sillas[i].posicionActual.x -= pasoPosicion;
			}
			else
			{
				sillas[i].posicionActual.x = x1;
			}
			if (abs(sillas[i].posicionActual.x - x1) <= distGiro)
			{
				if (sillas[i].anguloActual < 270)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 270;
				}
			}
			if (sillas[i].anguloActual == 270 && sillas[i].posicionActual.x == x1)
			{
				sillas[i].estadoAnimacion = 2;
			}
			break;
		case 2: // Mover silla 0 a z1
			if (sillas[i].posicionActual.z <= z1)
			{
				sillas[i].posicionActual.z = z1;
			}
			else
			{
				sillas[i].posicionActual.z -= pasoPosicion;
			}
			if ((sillas[i].anguloActual != 180) && (abs(sillas[i].posicionActual.z - z1) <= distGiro))
			{
				if (sillas[i].anguloActual > 180)
				{
					sillas[i].anguloActual -= pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 180;
				}
			}
			if (sillas[i].anguloActual == 180 && sillas[i].posicionActual.x == x1)
			{
				sillas[i].estadoAnimacion = 3;
			}
			break;
		case 3: // Mover sillas 0-9 a z2
			if (sillas[i].posicionActual.x > x2)
			{
				sillas[i].posicionActual.x = x2;
			}
			else
			{
				sillas[i].posicionActual.x += pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.x - x2) <= distGiro)
			{
				if (i == 0)
				{
					if (sillas[i].anguloActual > 90)
					{
						sillas[i].anguloActual -= pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 90;
					}
				}
				else
				{
					if (sillas[i].anguloActual < 90)
					{
						sillas[i].anguloActual += pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 90;
					}
				}
			}
			if (sillas[i].anguloActual == 90 && sillas[i].posicionActual.x == x2)
			{
				if (i == 0)
				{
					sillas[i].estadoAnimacion = 5;
				}
				else
				{
					sillas[i].estadoAnimacion = 4;
				}
			}
			break;
		case 4: // Esperar que silla 0 llegue a estado 3
			if (abs(sillas[0].posicionActual.x - x2) <= distGiro)
			{
				sillas[i].estadoAnimacion = 5;
			}
			break;
		case 5: // Mover sillas 0-30 a z3
			if (sillas[i].posicionActual.z > z3)
			{
				sillas[i].posicionActual.z = z3;
				sillas[i].estadoAnimacion = 6;
			}
			else
			{
				sillas[i].posicionActual.z += pasoPosicion;
			}
			break;
		case 6: // Esperar que silla 19 llegue a z3
			sillas[i].dibujar = false;
			if (sillas[19].posicionActual.z < z3)
			{
				break;
			}
			sillas[i].scale = glm::vec3(3.0f, 3.0f, 3.0f);
			sillas[i].posicionActual.z = z3 + (distGiro * i);
			sillas[i].posicionActual.y = 0.1;
			sillas[i].sillaNueva = true;
			if (i < 10)
			{
				sillas[i].anguloActual = -90;
			}
			else
			{
				sillas[i].anguloActual = 270;
			}
			sillas[i].estadoAnimacion = 14;
			break;
		case 7: // Esperar que silla 0 llegue a z2
			if (sillas[9].posicionActual.z >= z2)
			{
				sillas[i].estadoAnimacion = 8;
			}
			break;
		case 8: // Mover sillas 10-18 a x3
			if (sillas[i].posicionActual.x >= x3)
			{
				sillas[i].posicionActual.x = x3;
			}
			else
			{
				sillas[i].posicionActual.x += pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.x - x3) <= distGiro)
			{
				if (sillas[i].anguloActual < 90)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 90;
				}
			}
			if (sillas[i].anguloActual == 90 && sillas[i].posicionActual.x == x3)
			{
				sillas[i].estadoAnimacion = 9;
			}
			break;
		case 9: // Esperar que silla 10 llegue a estado 9
			if (sillas[10].estadoAnimacion == 9)
			{
				sillas[i].estadoAnimacion = 10;
			}
			break;
		case 10: // Mover sillas 10-30 a z2
			if (sillas[i].posicionActual.z >= z2)
			{
				sillas[i].posicionActual.z = z2;
			}
			else
			{
				sillas[i].posicionActual.z += pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.z - z2) <= distGiro)
			{
				if (sillas[i].anguloActual > 0)
				{
					sillas[i].anguloActual -= pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 0;
				}
			}
			if (sillas[i].anguloActual == 0 && sillas[i].posicionActual.z == z2)
			{
				sillas[i].estadoAnimacion = 11;
			}
			break;
		case 11: // Mover sillas 10-30 a x2
			if (sillas[i].posicionActual.x <= x2)
			{
				sillas[i].posicionActual.x = x2;
			}
			else
			{
				sillas[i].posicionActual.x -= pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.x - x2) <= distGiro)
			{
				if (sillas[i].anguloActual < 90)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 90;
				}
			}
			if (sillas[i].anguloActual == 90 && sillas[i].posicionActual.x == x2)
			{
				sillas[i].estadoAnimacion = 5;
			}
			break;
		case 12: // Esperar a que la silla 10 llegue a z2
			if (sillas[12].posicionActual.z >= z2)
			{
				sillas[i].estadoAnimacion = 13;
			}
			break;
		case 13: // Mover sillas 19-30 a x4
			if (sillas[i].posicionActual.x >= x4)
			{
				sillas[i].posicionActual.x = x4;
			}
			else
			{
				sillas[i].posicionActual.x += pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.x - x4) <= distGiro)
			{
				if (sillas[i].anguloActual < 90)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 90;
				}
			}
			if (sillas[i].anguloActual == 90 && sillas[i].posicionActual.x == x4)
			{
				sillas[i].estadoAnimacion = 10;
			}
			break;
		case 14: // Mover sillas 0-30 sobre x2
			sillas[i].posicionActual.z -= pasoPosicion;
			if (sillas[i].posicionActual.z < z3)
			{
				sillas[i].dibujar = true;
			}
			if (i == 0)
			{
				if (sillas[i].posicionActual.z <= z1)
				{
					sillas[i].posicionActual.z = z1;
				}
				if (abs(sillas[i].posicionActual.z - z1) <= distGiro)
				{
					if (sillas[i].anguloActual < 0)
					{
						sillas[i].anguloActual += pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 0;
					}
				}
				if (sillas[i].anguloActual == 0 && sillas[i].posicionActual.z == z1)
				{
					sillas[i].estadoAnimacion = 15;
				}
			}
			else if (i <= 9)
			{
				if (sillas[i].posicionActual.z <= sillas[i].posicionFinal.z)
				{
					sillas[i].posicionActual.z = sillas[i].posicionFinal.z;
				}
				if (abs(sillas[i].posicionActual.z - sillas[i].posicionFinal.z) <= distGiro)
				{
					if (sillas[i].anguloActual < sillas[i].anguloFinal)
					{
						sillas[i].anguloActual += pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = sillas[i].anguloFinal;
					}
				}
				if (sillas[i].anguloActual == sillas[i].anguloFinal && sillas[i].posicionActual.z == sillas[i].posicionFinal.z)
				{
					sillas[i].estadoAnimacion = 18;
				}
			}
			else
			{
				if (sillas[i].posicionActual.z <= z2)
				{
					sillas[i].posicionActual.z = z2;
				}
				if (abs(sillas[i].posicionActual.z - z2) <= distGiro)
				{
					if (sillas[i].anguloActual > 180)
					{
						sillas[i].anguloActual -= pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 180;
					}
				}
				if (sillas[i].anguloActual == 180 && sillas[i].posicionActual.z == z2)
				{
					sillas[i].estadoAnimacion = 19;
				}
			}
			break;
		case 15: // Mover silla 0 a x1
			if (sillas[i].posicionActual.x < x1)
			{
				sillas[i].posicionActual.x = x1;
			}
			else
			{
				sillas[i].posicionActual.x -= pasoPosicion;
			}
			if (abs(sillas[i].posicionActual.x - x1) <= distGiro)
			{
				if (sillas[i].anguloActual < 90)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 90;
				}
			}
			if (sillas[i].anguloActual == 90 && sillas[i].posicionActual.x == x1)
			{
				sillas[i].estadoAnimacion = 16;
			}
			break;
		case 16: // Mover silla 0 a z final
			if (sillas[i].posicionActual.z > sillas[i].posicionFinal.z)
			{
				sillas[i].posicionActual.z = sillas[i].posicionFinal.z;
			}
			else
			{
				sillas[i].posicionActual.z += pasoPosicion;
			}
			if ((sillas[i].anguloActual != 180) && (abs(sillas[i].posicionActual.z - sillas[i].posicionFinal.z) <= distGiro))
			{
				if (sillas[i].anguloActual < 180)
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = 180;
				}
			}
			if (sillas[i].anguloActual == 180 && sillas[i].posicionActual.z == sillas[i].posicionFinal.z)
			{
				sillas[i].estadoAnimacion = 17;
			}
			break;
		case 17: // Mover silla 0 a x final
			if (sillas[i].posicionActual.x > sillas[i].posicionFinal.x)
			{
				sillas[i].posicionActual.x = sillas[i].posicionFinal.x;
			}
			else
			{
				sillas[i].posicionActual.x += pasoPosicion;
			}
			if (sillas[i].posicionActual.x == sillas[i].posicionFinal.x)
			{
				sillas[i].estadoAnimacion = 21;
			}
			break;
		case 18:
			if (sillas[i].posicionActual.x < sillas[i].posicionFinal.x)
			{
				sillas[i].posicionActual.x = sillas[i].posicionFinal.x;
			}
			else
			{
				sillas[i].posicionActual.x -= pasoPosicion;
			}
			if (sillas[i].posicionActual.x == sillas[i].posicionFinal.x)
			{
				sillas[i].estadoAnimacion = 21;
			}
			break;
		case 19:
			if (i <= 18)
			{
				if (sillas[i].posicionActual.x > x3)
				{
					sillas[i].posicionActual.x = x3;
				}
				else
				{
					sillas[i].posicionActual.x += pasoPosicion;
				}
				if (abs(sillas[i].posicionActual.x - x3) <= distGiro)
				{
					if (sillas[i].anguloActual < 270)
					{
						sillas[i].anguloActual += pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 270;
					}
				}
				if (sillas[i].anguloActual == 270 && sillas[i].posicionActual.x == x3)
				{
					sillas[i].estadoAnimacion = 20;
				}
			}
			else
			{
				if (sillas[i].posicionActual.x > x4)
				{
					sillas[i].posicionActual.x = x4;
				}
				else
				{
					sillas[i].posicionActual.x += pasoPosicion;
				}
				if (abs(sillas[i].posicionActual.x - x4) <= distGiro)
				{
					if (sillas[i].anguloActual < 270)
					{
						sillas[i].anguloActual += pasoRotacion;
					}
					else
					{
						sillas[i].anguloActual = 270;
					}
				}
				if (sillas[i].anguloActual == 270 && sillas[i].posicionActual.x == x4)
				{
					sillas[i].estadoAnimacion = 20;
				}
			}
			break;
		case 20:
			if (sillas[i].posicionActual.z < sillas[i].posicionFinal.z)
			{
				sillas[i].posicionActual.z = sillas[i].posicionFinal.z;
			}
			else
			{
				sillas[i].posicionActual.z -= pasoPosicion;
			}
			if ((sillas[i].anguloActual != sillas[i].anguloFinal) && (abs(sillas[i].posicionActual.z - sillas[i].posicionFinal.z) <= distGiro))
			{
				if (sillas[i].anguloActual < (sillas[i].anguloFinal + 360))
				{
					sillas[i].anguloActual += pasoRotacion;
				}
				else
				{
					sillas[i].anguloActual = sillas[i].anguloFinal;
				}
			}
			if (sillas[i].anguloActual == sillas[i].anguloFinal && sillas[i].posicionActual.z == sillas[i].posicionFinal.z)
			{
				sillas[i].estadoAnimacion = 18;
			}
			break;
		default:
			break;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (animacionActiva) return;
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