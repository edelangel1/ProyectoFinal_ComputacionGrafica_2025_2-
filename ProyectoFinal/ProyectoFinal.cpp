// Std. Includes
#include <iostream>
#include <string>
#include <vector>
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

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Music
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// ----------------- Function prototypes ----------------- //
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Sillas
void inicializarSillas();
void animarSilla();

// Computadoras
void inicializarComputadoras();
void animacionComputadora();
void reiniciarComputadoras();

// Salon
          void cargarTexturaRuido();
void UpdateAnimation(float deltaTime);
void ResetScene();
void manejarObjetos(float deltaTime);
void reiniciarObjetos();

// Humano
void animarHumano();
void interpolarHumano();

// Pizarron
void animarPizzaron();
void interpolarPizarron();

// Proyector
void animarProyector();
void interpolarProyector();

// Balon
void animarBalon();
void interpolarBalon();
// ------------------------------------------------------- //

// Music
bool isPlaying = false;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(20.0f, 10.0f, -22.0f));
float contadorCamara = 0.0;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 2.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f) };

float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f };

glm::vec3 Light1 = glm::vec3(0);

// ----------------- Control de animacion ----------------- //
bool animacionSillas = false;
bool animacionProyector = false;
bool animacionPizarron = false;
bool animacionBalon = false;
// ------------------------------------------------------- //

// --------------- Animacion de las sillas --------------- //
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
Silla sillas[31]; // sillas

// ------------ Animacion de las computadoras ------------ //
// Definición de los estados de animación
enum EstadoAnimacionComputadora
{
	ESTADO_INICIAL,
	ESTADO_ASCENDER,			  // Las viejas suben
	ESTADO_ROTAR,				  // Las viejas rotan
	ESTADO_REGRESAR_ANGULO,		  // Las viejas regresan al ángulo original
	ESTADO_AL_CENTRO,			  // Las viejas van al centro
	ESTADO_ESPERAR_CENTRO,		  // Las viejas esperan en el centro
	ESTADO_AL_ARMARIO,			  // Las viejas van hacia el armario
	ESTADO_FINAL_VIEJA,			  // Las viejas han completado su ciclo
	ESTADO_SALIR_NUEVA,			  // Las nuevas salen del armario
	ESTADO_AL_CENTRO_NUEVA,		  // Las nuevas van al centro
	ESTADO_ESPERAR_CENTRO_NUEVA,  // Las nuevas esperan en el centro
	ESTADO_A_POSICION,			  // Las nuevas van a su posición final
	ESTADO_ROTAR_NUEVA,			  // Las nuevas rotan
	ESTADO_REGRESAR_ANGULO_NUEVA, // Las nuevas regresan al ángulo original
	ESTADO_DESCENDER,			  // Las nuevas descienden
	ESTADO_FINAL_NUEVA			  // Las nuevas han completado su ciclo
};

// Estructura actualizada para las computadoras
struct Computadora
{
	glm::vec3 posicionInicial;					// Posición de inicio
	glm::vec3 posicionFinal;					// Posición objetivo actual
	glm::vec3 posicionActual;					// Posición actual en la animación
	float anguloInicial;						// Ángulo inicial
	float anguloFinal;							// Ángulo objetivo actual
	float anguloActual;							// Ángulo actual en la animación
	float anguloGiro;							// Ángulo para la rotación
	glm::vec3 scale;							// Escala del modelo
	EstadoAnimacionComputadora estadoAnimacion; // Estado actual
	bool dibujar;								// Si debe mostrarse
	bool computadoraNueva;						// Tipo de computadora (nueva/vieja)
	float tiempoEstado;							// Tiempo transcurrido en el estado actual
};

// Variables globales para la animación
const int NUMERO_COMPUTADORAS = 20;
const float RADIO_GIRO = 1.0f; // Radio de giro modificable
const float VELOCIDAD_ANIMACION = 5.0f;
const float ALTURA_ASCENSO = 3.25f; // Para escala de 1.0f
const float TIEMPO_ESPERA = 1.0f;	// Segundos en el centro
float tiempoEspera = 0.0f;
int contadorCompuViejasCompletadas = 0;
bool todasCompuViejasCompletadas = false;

// Posiciones clave
glm::vec3 posicionCentro(20.0f, 3.25f, -20.0f);	  // Centro del salón
glm::vec3 posicionArmario(5.5f, 3.25f, -42.625f); // Posición del armario
/*glm::vec3 posicionCentro(40.0f, 6.5f, -40.0f);
glm::vec3 posicionArmario(11.0f, 6.5f, -85.5f);*/

// Animación computadoras
bool estadoAnimacionComputadoras = false;
Computadora computadoras[NUMERO_COMPUTADORAS];

// Variables globales para las posiciones específicas
const float escala = 1.0f;			  // Ajusta este valor según necesites
const float posComputadorasY = 2.95f; // Altura inicial de las computadoras //2.95 para escala de 1.0f

// Arreglo con las posiciones específicas de las computadoras
const glm::vec3 posicionesComputadoras[10] = {
	glm::vec3(25.0f * escala, posComputadorasY, -16.5f * escala), // Mesa 1
	glm::vec3(27.0f * escala, posComputadorasY, -18.5f * escala), // Mesa 1
	glm::vec3(26.0f * escala, posComputadorasY, -26.4f * escala), // Mesa 2
	glm::vec3(26.0f * escala, posComputadorasY, -29.6f * escala), // Mesa 2
	glm::vec3(22.7f * escala, posComputadorasY, -34.5f * escala), // Mesa 3
	glm::vec3(20.7f * escala, posComputadorasY, -36.8f * escala), // Mesa 3
	glm::vec3(16.5f * escala, posComputadorasY, -29.6f * escala), // Mesa 4
	glm::vec3(18.0f * escala, posComputadorasY, -22.5f * escala), // Mesa 5
	glm::vec3(17.0f * escala, posComputadorasY, -15.5f * escala), // Mesa 6
	glm::vec3(15.5f * escala, posComputadorasY, -13.5f * escala)  // Mesa 6
};

// ----------------- Animación del salón ----------------- //

// Estados para objetos (sillas y computadoras)
enum EstadoObjetos
{
	OBJETOS_VISIBLES,	   // Estado inicial
	OBJETOS_DISOLVIENDO,   // Desapareciendo
	OBJETOS_ESPERANDO,	   // Esperando animación salón
	OBJETOS_REAPARECIENDO, // Reapareciendo
	OBJETOS_COMPLETO	   // Animación terminada
};

EstadoObjetos estadoObjetos = OBJETOS_VISIBLES;
float dissolveAmountObjetos = 0.0f;
float tiempoEsperaObjetos = 0.0f;
bool animacionSalonIniciada = false;
GLuint noiseTexture;

// Salón, Excavadora, Helicóptero

// Estados de animación para el salón
enum AnimationState
{
	INITIAL,		   // Estado inicial, salón viejo visible
	SHRINKING,		   // Salón viejo reduciéndose
	MOVING_OUT,		   // Salón viejo y excavadora moviéndose fuera
	HELICOPTER_MOVING, // Helicóptero moviéndose
	NEW_FALLING,	   // Salón nuevo cayendo con rebote
	FINAL			   // Estado final, salón nuevo visible y estable
};

// Variables globales para la animación del salón
AnimationState currentState = INITIAL;
float animationTimer = 0.0f;
bool animationActive = false;
bool salonCayendo = false;

// Posiciones y escalas para el salón
glm::vec3 oldSalonPosition = glm::vec3(0.0f, 0.0f, 0.0f);  // Posición inicial del salón viejo
glm::vec3 oldSalonScale = glm::vec3(1.0f);				   // Escala inicial del salón viejo
glm::vec3 newSalonPosition = glm::vec3(50.0f, 5.0f, 0.0f); // Posición inicial del salón nuevo (fuera de vista)
glm::vec3 newSalonScale = glm::vec3(0.4f);				   // Escala inicial del salón nuevo
glm::vec3 newSalonFallScale = glm::vec3(0.07);

// Posición inicial del bulldozer (fuera de vista)
glm::vec3 bulldozerBasePosition = glm::vec3(-30.5f, -0.3f, -9.0f);
glm::vec3 bulldozerBelt1Position = glm::vec3(-2.6f, 2.0f, 3.7);
glm::vec3 bulldozerBelt2Position = glm::vec3(-2.6f, 2.0f, -3.5);
glm::vec3 bulldozerBelt3Position = glm::vec3(-2.6f, 2.0f, -3.7f);
glm::vec3 bulldozerBelt4Position = glm::vec3(-2.6f, 2.0f, 3.5f);

// Posición inicial del helicóptero (fuera de vista)
glm::vec3 helicopterBasePosition = glm::vec3(30.0f, 10.0f, -9.0f);
glm::vec3 mainRotorOffset(0.0f, 3.615f, 5.15f);
glm::vec3 tailRotorOffset(0.0f, 4.34f, -4.3f);

// Velocidades y otros parámetros del helicóptero
float helicopterSpeed = 7.0f;	// Velocidad del helicóptero
bool helicopterVisible = false; // Inicialmente, el helicóptero no es visible
float helicopterRotationY = 270.0f;
bool oldSalonVisible = true;
bool newSalonVisible = false;
bool bulldozerVisible = false;
bool salonEmpezoAMoverse = false;
bool hasLanded = false;
float landingTimer = 0.0f;

// Rotación de la excavadora
float beltRotationAngle = 0.0f;
float beltRotationSpeed = 100.0f;

// Rotacion de las helices
float mainRotorAngle = 0.0;
float tailRotorAngle = 0.0f;
float mainRotorSpeed = 900.0f;
float tailRotorSpeed = 900.0f;

// Variables para el efecto de rebote
float bounceHeight = 0.0f;
float bounceVelocity = 0.0f;
int bounceCount = 0;
const float GRAVITY = -5.8f; //-9.8f;
const float BOUNCE_DAMPING = 0.6f;
const float BOUNCE_HEIGHT = 0.3f; // Altura máxima del rebote

// ----------------- Animacion del humano ----------------- //
// Variables de animacion
int frameActualHumano = 0;
int pasoActualInterpolacionHumano = 0;
bool animacionHumano = false;
bool humanoDisolviendo = false;
bool humanoReapareciendo = false;
bool humanoVisible = true;
float dissolveAmountHumano = 0.0f;

// Estructura de keyframes
struct KeyFrameHumano
{
	float posX, posY, posZ;
	float torsoRot_y;
	float cabezaRot_x, cabezaRot_y;
	float brazoDerRot_x, brazoDerRot_y;
	float brazoIzqRot_x, brazoIzqRot_y;
	float piernaRot;

	float segundosAnimacion;
	int nPasos;

	float incPosX, incPosY, incPosZ;
	float incTorsoRot_y;
	float incCabezaRot_x, incCabezaRot_y;
	float incBrazoDerRot_x, incBrazoDerRot_y;
	float incBrazoIzqRot_x, incBrazoIzqRot_y;
	float incPiernaRot;
};

KeyFrameHumano humanoKF[] = {
	// x   , y    , z     , torso , cX   , cY   , bDX   , bDY   , bIX  , bIY   , p    , segAnim
	{6.5f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1},
	{6.5f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{6.0f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{5.5f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{5.0f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{4.5f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{4.0f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.5f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -19.0f, 270.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.0f, 3.15f, -19.0f, 160.0f, 0.0f, -30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5},
	{3.0f, 3.15f, -19.0f, 160.0f, 0.0f, -30.0f, 0.0f, 0.0f, -100.0f, 5.0f, 0.0f, 3},
	{3.0f, 3.15f, -19.0f, 160.0f, 0.0f, -30.0f, 0.0f, 0.0f, -100.0f, 5.0f, 0.0f, 1.5},
	{3.0f, 3.15f, -19.0f, 160.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, -30.0f, -5.0f, -30.0f, 5.0f, 0.0f, 2},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, -50.0f, -60.0f, -5.0f, -30.0f, 5.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, -50.0f, -60.0f, -5.0f, -30.0f, 5.0f, 0.0f, 4},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, 50.0f, -30.0f, -5.0f, -60.0f, 5.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, 50.0f, -30.0f, -5.0f, -60.0f, 5.0f, 0.0f, 1},
	{3.0f, 3.15f, -19.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.0f, 3.15f, -18.5f, 0.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -18.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -17.5f, 0.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -17.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -16.5f, 0.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -16.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -15.5f, 0.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -15.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -14.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{4.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{4.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{5.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{5.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{6.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{6.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{7.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{7.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{8.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{8.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{9.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{9.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{10.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{10.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{11.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{11.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{12.0f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{12.5f, 3.15f, -14.5f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4},
	{12.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5},
	{12.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{11.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{11.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{10.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{10.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{9.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{9.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{8.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{8.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{7.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{7.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{6.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{6.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{5.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{5.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{4.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{4.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.5f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -14.5f, 270.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.0f, 3.15f, -14.5f, 180.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5},
	{3.0f, 3.15f, -15.0f, 180.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -15.5f, 180.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -16.0f, 180.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -16.5f, 180.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -17.0f, 180.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -17.5f, 180.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -18.0f, 180.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{3.0f, 3.15f, -18.5f, 180.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{3.0f, 3.15f, -19.0f, 180.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75},
	{3.5f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{4.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{4.5f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{5.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{5.5f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 15.0f, 0.0f, -15.0f, 0.0f, -15.0f, 0.75},
	{6.0f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, -15.0f, 0.0f, 15.0f, 0.0f, 15.0f, 0.75},
	{6.5f, 3.15f, -19.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.75} };
int nFramesHumano = sizeof(humanoKF) / sizeof(KeyFrameHumano);

// Variables para transformaciones de los modelos
glm::vec3 humanoPos(6.5f, 3.15f, -19.0f);
float humanoTorsoRot_y = 90.0f;
float humanoCabezaRot_x = 0.0f;
float humanoCabezaRot_y = 0.0f;
float humanoBrazoDerRot_x = 0.0f;
float humanoBrazoDerRot_y = 0.0f;
float humanoBrazoIzqRot_x = 0.0f;
float humanoBrazoIzqRot_y = 0.0f;
float humanoPiernaRot = 0.0f;
// ----------------- Animación del proyector ----------------- //
#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;

struct KeyFrameProyector {
	float posX, posY, posZ;
	float rotY;
	float incX, incY, incZ;
	float incRotY;
};

KeyFrameProyector ProyectorKF[MAX_FRAMES] = {
	{ 6.0f, 5.0f, -11.0f, 0.0f },   // posición inicial
	{ 6.0f, -9.6f, -11.0f, 90.0f },    
	{ 6.0f, -9.6f, -9.5f, 90.0f }, // esta en el suelo
	{ 6.0f, -9.6f, -9.5f, 90.0f },
	{ 8.5f, -9.6f, -9.0f, 90.0f },
	{ 8.5f, -9.6f, 15.0f, 90.0f },
};

int FrameIndexProyector = 6;
int PlayIndexProyector = 0;

float proyectorPosX = 6.0f, proyectorPosY = 5.0f, proyectorPosZ = -11.0f;
float proyectorRotY = 0.0f;

// ----------------- Animación del pizarron ----------------- //
const int MAX_FRAMES_PIZARRON = 18;

struct KeyFramePizarron {
	float x, y, z, rotY;
	float incX, incY, incZ, incRotY;
};

KeyFramePizarron pizKF[MAX_FRAMES_PIZARRON] = {
	{ 22.0f, 0.0f, 15.0f, 0.0f },  // Inicio
	{ 22.0f, 0.0f, -28.0f, 0.0f },  // entra al salon
	{ 22.0f, 0.0f, -28.0f, 35.0f },  
	{ 17.0f, 0.0f, -35.0f, 35.0f },
	{ 17.0f, 0.0f, -35.0f, 45.0f },
	{ 17.0f, 0.0f, -35.0f, 90.0f },
	{ 10.0f, 0.0f, -35.0f, 90.0f },
	{ 10.0f, 0.0f, -37.0f, 45.0f },
	{ 10.0f, 0.0f, -36.0f, 15.0f }, 
	{ 10.0f, 0.0f, -36.0f, 0.0f },
	{ 10.0f, 0.0f, -34.0f, 0.0f },
	{ 3.0f, 0.0f, -34.0f, 0.0f }, // se acerca a la pared
	{ 3.0f, 0.0f, -23.0f, 0.0f },
	{ 3.0f, 3.0f, -23.0f, 0.0f },
	{ 3.1f, 3.0f, -23.0f, 0.0f },
	{ 3.1f, 3.0f, -23.0f, 0.0f },
	{ 3.2f, 3.0f, -23.0f, 0.0f },
	{ 2.0f, 3.0f, -23.0f, 0.0f },
};
bool mostrarPizarron = false;
int FrameIndexPizarron = MAX_FRAMES_PIZARRON;
int PlayIndexPizarron = 0;
int pasosPizarron = 0;
int maxPasosPizarron = 90;

float pizarronPosX = 0.0f, pizarronPosY = 0.0f, pizarronPosZ = 0.0f;
float pizarronRotY = 0.0f;

// ----------------- Animación del balon ----------------- //
bool mostrarBalon = false;
float balonRot = 0.0f;
float balonIncRot = 0.0f;

struct KeyFrameBalon {
	float x, y, z;
	float incX, incY, incZ;
	float rot;
};

KeyFrameBalon balonKF[10] = {
	{ 35.0f, 5.5f, -21.0f, 0.0f },
	{ 6.0f, 7.0f, -11.0f, 180.0f }, // aqui pega al proyector
	{ 5.5f, 0.0f, -10.0f, 360.0f }, // aca cae al suele rebote 1
	{ 5.0f, 2.0f, -9.0f, 540.0f }, 
	{ 4.5f, 0.0f, -8.0f, 720.0f }, // rebote 2
	{ 4.3f, 1.5f, -7.0f, 540.0f },
	{ 4.1f, 0.0f, -6.5f, 720.0f },
	{ 3.9f, 0.5f, -6.0f, 540.0f },
	{ 3.8f, 0.0f, -5.5f, 720.0f },
	{ 3.7f, 0.0f, -5.3f, 720.0f }
};
int FrameIndexBalon = 10;
int PlayIndexBalon = 0;
int pasosBalon = 0;
int maxPasosBalon = 60;
float balonX = balonKF[0].x, balonY = balonKF[0].y, balonZ = balonKF[0].z;

int main()
{
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "iOS Development Lab - Equipo 10", nullptr, nullptr);

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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Setup and compile our shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");
	Shader dissolveShader("Shader/model_dissolve.vs", "Shader/model_dissolve.frag");

	// Load models
	// Animación del salon
	Model salonViejo((char*)"Models/salon/EstructuraVieja.obj");
	Model salonNuevo((char*)"Models/salon/Estructura.obj");
	Model bulldozerBase((char*)"Models/Bulldozer/Bulldozer.obj");
	Model bulldozerBelt((char*)"Models/Bulldozer/bulldozerLlanta.obj");
	Model helicopterBase((char*)"Models/Helicoptero/helicopterBase.obj");
	Model helicopterRotorMain((char*)"Models/Helicoptero/RotorMain.obj");
	Model helicopterRotorTail((char*)"Models/Helicoptero/RotorTail.obj");

	// Animación de las sillas
	Model sillaVieja((char*)"Models/sillaVieja/sillaVieja.obj");
	Model sillaNueva((char*)"Models/sillaNueva/sillaNueva.obj");

	// Animación de las computadoras
	Model computadoraVieja((char*)"Models/compuVieja/compuVieja.obj");
	Model computadoraNueva((char*)"Models/compuNueva/compuNueva.obj");

	// Animación del humano
	Model humanoBrazoDer((char*)"Models/humano/brazoDer.obj");
	Model humanoBrazoIzq((char*)"Models/humano/brazoIzq.obj");
	Model humanoCabeza((char*)"Models/humano/cabeza.obj");
	Model humanoPiernaDer((char*)"Models/humano/piernaDer.obj");
	Model humanoPiernaIzq((char*)"Models/humano/piernaIzq.obj");
	Model humanoTorso((char*)"Models/humano/torso.obj");
	Model iPad((char*)"Models/iPad/iPad.obj");

	// Animación del proyector
	Model proyector((char*)"Models/proyectorViejo/proyector.obj");

	// Animación del pizarron
	Model pizarron((char*)"Models/pizarronNuevo/pizarron.obj");

	// Animación del balon
	Model balon((char*)"Models/balon/balon.obj");

	// ------------------------------------------------------- //
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f };

	GLuint indices[] = {// Note that we start from 0!
						0, 1, 2, 3,
						4, 5, 6, 7,
						8, 9, 10, 11,
						12, 13, 14, 15,
						16, 17, 18, 19,
						20, 21, 22, 23,
						24, 25, 26, 27,
						28, 29, 30, 31,
						32, 33, 34, 35 };
	//------------------

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	// ------------------------------------------------------- //
	// Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/px.png"); // +X
	faces.push_back("SkyBox/nx.png"); // -X
	faces.push_back("SkyBox/py.png"); // +Y
	faces.push_back("SkyBox/ny.png"); // -Y
	faces.push_back("SkyBox/pz.png"); // +Z
	faces.push_back("SkyBox/nz.png"); // -Z

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	// ------------------------------------------------------- //

	// Inicializa la proyección con la relación de aspecto actual
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Inicializar animaciones
	inicializarSillas();
	inicializarComputadoras();
	cargarTexturaRuido();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// ------------ Funciones de las animaciones ------------ //
		// animacion();
		animarSilla();
		animacionComputadora();
		animarHumano();
		animarProyector();
		animarPizzaron();
		animarBalon();
		manejarObjetos(deltaTime);
		if (animationActive)
		{
			UpdateAnimation(deltaTime);
		}
		// ------------------------------------------------------- //

		// Actualiza el tamaño de la ventana si cambia
		glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);															   // Actualiza el tamaño del viewport
		projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f); // Recalcula la proyección

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f); // Temp

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		// glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

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

		glm::mat4 model(1);

		// ---------------- Draw the loaded model ---------------- //

		// Sillas
		for (size_t i = 0; i < 31; i++)
		{
			if (sillas[i].dibujar)
			{
				if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
				{
					dissolveShader.Use();
					glUniform1f(glGetUniformLocation(dissolveShader.Program, "dissolveAmount"), dissolveAmountObjetos);
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

					// Bind noise texture
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, noiseTexture);
					glUniform1i(glGetUniformLocation(dissolveShader.Program, "noiseMap"), 1);
				}
				else
				{
					lightingShader.Use();
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				}

				glm::mat4 modelSilla(1);
				modelSilla = glm::translate(modelSilla, sillas[i].posicionActual);
				modelSilla = glm::rotate(modelSilla, glm::radians(sillas[i].anguloActual), glm::vec3(0.0f, 1.0f, 0.0f));
				modelSilla = glm::scale(modelSilla, sillas[i].scale);

				if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
				{
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla));
				}
				else
				{
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSilla));
				}

				if (sillas[i].sillaNueva)
				{
					sillaNueva.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
				}
				else
				{
					sillaVieja.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
				}
			}
		}

		// Computadoras
		for (int i = 0; i < NUMERO_COMPUTADORAS; i++)
		{
			if (computadoras[i].dibujar)
			{
				if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
				{
					dissolveShader.Use();
					glUniform1f(glGetUniformLocation(dissolveShader.Program, "dissolveAmount"), dissolveAmountObjetos);
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

					// Bind noise texture
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, noiseTexture);
					glUniform1i(glGetUniformLocation(dissolveShader.Program, "noiseMap"), 1);
				}
				else
				{
					lightingShader.Use();
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				}

				glm::mat4 modelComputadora(1);
				modelComputadora = glm::translate(modelComputadora, computadoras[i].posicionActual);
				modelComputadora = glm::rotate(modelComputadora, computadoras[i].anguloActual, glm::vec3(0.0f, 1.0f, 0.0f));
				modelComputadora = glm::scale(modelComputadora, computadoras[i].scale);

				if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
				{
					glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelComputadora));
				}
				else
				{
					glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelComputadora));
				}

				if (computadoras[i].computadoraNueva)
				{
					computadoraNueva.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
				}
				else
				{
					computadoraVieja.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
				}
			}
		}

		// Humano
		if (humanoVisible)
		{
			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				dissolveShader.Use();
				glUniform1f(glGetUniformLocation(dissolveShader.Program, "dissolveAmount"), dissolveAmountObjetos);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, noiseTexture);
				glUniform1i(glGetUniformLocation(dissolveShader.Program, "noiseMap"), 1);

				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			}
			else
			{
				lightingShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			}

			// Configuración común para todas las partes del humano
			float escalaHumano = 3.5f;
			glm::mat4 modelHumanoBase = glm::mat4(1.0f);
			modelHumanoBase = glm::translate(modelHumanoBase, humanoPos);
			modelHumanoBase = glm::rotate(modelHumanoBase, glm::radians(humanoTorsoRot_y), glm::vec3(0.0f, 1.0f, 0.0f));
			modelHumanoBase = glm::scale(modelHumanoBase, glm::vec3(escalaHumano, escalaHumano, escalaHumano));

			// Dibujar torso
			glm::mat4 modelTorso = modelHumanoBase;
			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTorso));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTorso));
			}
			humanoTorso.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar cabeza
			glm::mat4 modelCabeza = modelHumanoBase;
			modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, 0.541f, 0.022f));
			modelCabeza = glm::rotate(modelCabeza, glm::radians(humanoCabezaRot_x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelCabeza = glm::rotate(modelCabeza, glm::radians(humanoCabezaRot_y), glm::vec3(0.0f, 1.0f, 0.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCabeza));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCabeza));
			}
			humanoCabeza.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar brazo derecho
			glm::mat4 modelBrazoDer = modelHumanoBase;
			modelBrazoDer = glm::translate(modelBrazoDer, glm::vec3(-0.14f, 0.434f, 0.027f));
			modelBrazoDer = glm::rotate(modelBrazoDer, glm::radians(humanoBrazoDerRot_x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelBrazoDer = glm::rotate(modelBrazoDer, glm::radians(humanoBrazoDerRot_y), glm::vec3(0.0f, 1.0f, 0.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBrazoDer));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBrazoDer));
			}
			humanoBrazoDer.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar iPad (en el brazo derecho)
			glm::mat4 modeliPad = modelBrazoDer;
			modeliPad = glm::translate(modeliPad, glm::vec3(-0.061f, -0.617f, 0.0f));
			modeliPad = glm::rotate(modeliPad, glm::radians(-77.7f), glm::vec3(0.0f, 0.0f, 1.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeliPad));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeliPad));
			}
			iPad.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar brazo izquierdo
			glm::mat4 modelBrazoIzq = modelHumanoBase;
			modelBrazoIzq = glm::translate(modelBrazoIzq, glm::vec3(0.14f, 0.434f, 0.027f));
			modelBrazoIzq = glm::rotate(modelBrazoIzq, glm::radians(humanoBrazoIzqRot_x), glm::vec3(1.0f, 0.0f, 0.0f));
			modelBrazoIzq = glm::rotate(modelBrazoIzq, glm::radians(humanoBrazoIzqRot_y), glm::vec3(0.0f, 1.0f, 0.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBrazoIzq));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBrazoIzq));
			}
			humanoBrazoIzq.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar pierna derecha
			glm::mat4 modelPiernaDer = modelHumanoBase;
			modelPiernaDer = glm::translate(modelPiernaDer, glm::vec3(-0.073f, 0.028f, 0.027f));
			modelPiernaDer = glm::rotate(modelPiernaDer, glm::radians(humanoPiernaRot), glm::vec3(1.0f, 0.0f, 0.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiernaDer));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiernaDer));
			}
			humanoPiernaDer.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);

			// Dibujar pierna izquierda
			glm::mat4 modelPiernaIzq = modelHumanoBase;
			modelPiernaIzq = glm::translate(modelPiernaIzq, glm::vec3(0.073f, 0.028f, 0.027f));
			modelPiernaIzq = glm::rotate(modelPiernaIzq, glm::radians(-humanoPiernaRot), glm::vec3(1.0f, 0.0f, 0.0f));

			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiernaIzq));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiernaIzq));
			}
			humanoPiernaIzq.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
		}

		// Draw salon
		lightingShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Old salon
		if (oldSalonVisible)
		{
			model = glm::mat4(1);
			model = glm::translate(model, oldSalonPosition);
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, oldSalonScale);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			salonViejo.Draw(lightingShader);
		}

		// New salon
		if (newSalonVisible)
		{
			model = glm::mat4(1);
			model = glm::translate(model, newSalonPosition);
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, newSalonScale);
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			salonNuevo.Draw(lightingShader);
		}

		// Excavadora (Bulldozer)
		if (bulldozerVisible)
		{
			// Base del bulldozer
			model = glm::mat4(1);
			model = glm::translate(model, bulldozerBasePosition);
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			bulldozerBase.Draw(lightingShader);

			// Llanta 1
			model = glm::mat4(1);
			model = glm::translate(model, bulldozerBasePosition);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, bulldozerBelt1Position);
			model = glm::rotate(model, glm::radians(beltRotationAngle), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación en X
			model = glm::scale(model, glm::vec3(1.5f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			bulldozerBelt.Draw(lightingShader);

			// Llanta 2
			model = glm::mat4(1);
			model = glm::translate(model, bulldozerBasePosition);
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, bulldozerBelt2Position);
			model = glm::rotate(model, glm::radians(beltRotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			bulldozerBelt.Draw(lightingShader);

			// Llanta 3
			model = glm::mat4(1);
			model = glm::translate(model, bulldozerBasePosition);
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, bulldozerBelt3Position);
			model = glm::rotate(model, glm::radians(-beltRotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			bulldozerBelt.Draw(lightingShader);

			// Llanta 4
			model = glm::mat4(1);
			model = glm::translate(model, bulldozerBasePosition);
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, bulldozerBelt4Position);
			model = glm::rotate(model, glm::radians(-beltRotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			bulldozerBelt.Draw(lightingShader);
		}

		// Helicoptero
		if (helicopterVisible)
		{
			// Base del helicóptero
			glm::mat4 helicopterBaseModel = glm::mat4(1.0f);
			helicopterBaseModel = glm::translate(helicopterBaseModel, helicopterBasePosition);
			helicopterBaseModel = glm::rotate(helicopterBaseModel, glm::radians(helicopterRotationY), glm::vec3(0.0f, 1.0f, 0.0f));

			// Base del helicóptero
			glm::mat4 model = helicopterBaseModel;
			model = glm::scale(model, glm::vec3(1.0f));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			helicopterBase.Draw(lightingShader);

			// Hélice principal (main rotor)
			glm::mat4 rotorModel = helicopterBaseModel;														 // Partimos de la transformación base
			rotorModel = glm::translate(rotorModel, mainRotorOffset);										 // Aplicamos el offset relativo
			rotorModel = glm::rotate(rotorModel, glm::radians(mainRotorAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la hélice
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(rotorModel));
			helicopterRotorMain.Draw(lightingShader);

			// Hélice de cola (tail rotor)
			glm::mat4 tailModel = helicopterBaseModel;													   // Partimos de la transformación base
			tailModel = glm::translate(tailModel, tailRotorOffset);										   // Aplicamos el offset relativo
			tailModel = glm::rotate(tailModel, glm::radians(tailRotorAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación de la hélice
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(tailModel));
			helicopterRotorTail.Draw(lightingShader);
		}

		// Dibujar proyector
		if (animacionProyector || PlayIndexProyector < FrameIndexProyector - 1) {
			glm::mat4 modelProyector(1);
			modelProyector = glm::mat4(1);
			modelProyector = glm::scale(modelProyector, glm::vec3(1.0f));
			modelProyector = glm::translate(modelProyector, glm::vec3(proyectorPosX, proyectorPosY, proyectorPosZ));
			modelProyector = glm::rotate(modelProyector, glm::radians(proyectorRotY), glm::vec3(1.0f, 0.0f, 0.0f));
			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelProyector));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelProyector));
			}
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			proyector.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
		}

		// Dibujar pizarron
		if (animacionPizarron || mostrarPizarron) {
			glm::mat4 modelPiz(1);
			modelPiz = glm::mat4(1);
			modelPiz = glm::translate(modelPiz, glm::vec3(pizarronPosX, pizarronPosY, pizarronPosZ));
			modelPiz = glm::rotate(modelPiz, glm::radians(pizarronRotY), glm::vec3(0.0f, 1.0f, 0.0f));
			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiz));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPiz));
			}
			glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
			pizarron.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
		}

		// Dibujar balon
		if (mostrarBalon) {    
			glm::mat4 modelBalon = glm::mat4(1.0f);
			modelBalon = glm::translate(modelBalon, glm::vec3(balonX, balonY, balonZ));
			modelBalon = glm::rotate(modelBalon, glm::radians(balonRot), glm::vec3(0.0f, 0.0f, 1.0f));
			modelBalon = glm::scale(modelBalon, glm::vec3(1.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBalon));
			if (estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO)
			{
				glUniformMatrix4fv(glGetUniformLocation(dissolveShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBalon));
			}
			else
			{
				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBalon));
			}
			balon.Draw(estadoObjetos != OBJETOS_VISIBLES && estadoObjetos != OBJETOS_COMPLETO ? dissolveShader : lightingShader);
		}

		// ------------------------------------------------------- //

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
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
		// glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// ------------------------------------------------------- //
		// Draw SkyBox
		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		// ------------------------------------------------------- //

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
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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

	// Iniciar animacion de las sillas
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		bool haySillasViejas = false;
		for (size_t i = 0; i < 31; i++)
		{
			if (!sillas[i].sillaNueva && sillas[i].dibujar)
			{
				haySillasViejas = true;
				break;
			}
		}

		if (haySillasViejas && !animacionSillas)
		{
			inicializarSillas();
			animacionSillas = true;
		}
	}

	// Reiniciar animación de sillas
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		inicializarSillas();
		animacionSillas = false;
	}

	// Iniciar animacion de las computadoras
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (!estadoAnimacionComputadoras)
		{
			estadoAnimacionComputadoras = true;
		}
	}

	// Reiniciar animacion de las computadoras
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		reiniciarComputadoras();
	}

	// Iniciar animacion del salón
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		if (estadoObjetos == OBJETOS_VISIBLES)
		{
			estadoObjetos = OBJETOS_DISOLVIENDO;
			dissolveAmountObjetos = 0.0f;
			// Iniciar disolución del humano también
			humanoDisolviendo = true;
			dissolveAmountHumano = 0.0f;
		}
	}

	// Reiniciar animacion del salón
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		if (currentState == FINAL)
		{
			// Reiniciar la escena solo si estamos en el estado final
			ResetScene();
		}
		// No hacer nada si el salón visible es el viejo
	}

	// Iniciar animación del humano
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		if (!animacionHumano)
		{
			frameActualHumano = 0;
			pasoActualInterpolacionHumano = 0;
			humanoTorsoRot_y = 90.0f;
			humanoBrazoDerRot_x = humanoBrazoDerRot_y = 0;
			humanoBrazoIzqRot_x = humanoBrazoIzqRot_y = 0;
			humanoCabezaRot_x = humanoCabezaRot_y = 0;
			humanoPiernaRot = 0.0f;
			interpolarHumano();
			animacionHumano = true;
		}
	}

	// Reiniciar animación del humano
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		frameActualHumano = 0;
		pasoActualInterpolacionHumano = 0;
		humanoPos = glm::vec3(6.5f, 3.15f, -19.0f);
		humanoTorsoRot_y = 90.0f;
		humanoBrazoDerRot_x = humanoBrazoDerRot_y = 0;
		humanoBrazoIzqRot_x = humanoBrazoIzqRot_y = 0;
		humanoCabezaRot_x = humanoCabezaRot_y = 0;
		humanoPiernaRot = 0.0f;
		animacionHumano = false;
	}

	// Iniciar animación del proyector
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		if (!animacionBalon)
		{
			// Reiniciar balón
			PlayIndexBalon = 0;
			pasosBalon = 0;
			balonX = balonKF[0].x;
			balonY = balonKF[0].y;
			balonZ = balonKF[0].z;
			animacionBalon = true;
			mostrarBalon = true;
			interpolarBalon();
			// Reiniciar pizarrón
			animacionPizarron = false;
			mostrarPizarron = false;
			pasosPizarron = 0;
			PlayIndexPizarron = 0;
			pizarronPosX = 100.0f;
			pizarronPosY = 100.0f;
			pizarronPosZ = 100.0f;
			pizarronRotY = pizKF[0].rotY;
			// Reiniciar proyector
			animacionProyector = false;
			PlayIndexProyector = 0;
			i_curr_steps = 0;
			proyectorPosX = ProyectorKF[0].posX;
			proyectorPosY = ProyectorKF[0].posY;
			proyectorPosZ = ProyectorKF[0].posZ;
			proyectorRotY = ProyectorKF[0].rotY;
		}
	}

	// Reiniciar animación del proyector
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		// Reiniciar balón
		PlayIndexBalon = 0;
		pasosBalon = 0;
		balonX = balonKF[0].x;
		balonY = balonKF[0].y;
		balonZ = balonKF[0].z;
		animacionBalon = false;
		mostrarBalon = false;
		// Reiniciar pizarrón
		animacionPizarron = false;
		mostrarPizarron = false;
		pasosPizarron = 0;
		PlayIndexPizarron = 0;
		pizarronPosX = 100.0f;
		pizarronPosY = 100.0f;
		pizarronPosZ = 100.0f;
		pizarronRotY = pizKF[0].rotY;
		// Reiniciar proyector
		animacionProyector = false;
		PlayIndexProyector = 0;
		i_curr_steps = 0;
		proyectorPosX = ProyectorKF[0].posX;
		proyectorPosY = ProyectorKF[0].posY;
		proyectorPosZ = ProyectorKF[0].posZ;
		proyectorRotY = ProyectorKF[0].rotY;
	}

	// Reproducir musica
	if (action == GLFW_PRESS && key == GLFW_KEY_G && !isPlaying)
	{
		PlaySound(TEXT("just-relax-11157.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		isPlaying = true;
	}
	else if (action == GLFW_PRESS && key == GLFW_KEY_O && isPlaying)
	{
		PlaySound(NULL, 0, 0); // Detiene la reproducción
		isPlaying = false;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos; // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
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
		glm::vec3(posSillasX19_30, posSillasY, -5.24f) };
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
		0.0f, 0.0f, 0.0f };
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
		12, 12, 12 };

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
void animarSilla()
{
	if (!animacionSillas)
		return;
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
		case 21:
			if (i == 30)
			{
				animacionSillas = false;
			}
			break;
		default:
			break;
		}
	}
}

// ------------ Animacion de las computadoras ------------ //
// Función para inicializar las computadoras con las posiciones específicas
void inicializarComputadoras()
{
	for (int i = 0; i < NUMERO_COMPUTADORAS; ++i)
	{
		if (i < NUMERO_COMPUTADORAS / 2)
		{
			// Computadoras viejas en sus posiciones específicas
			computadoras[i].posicionInicial = posicionesComputadoras[i];
			computadoras[i].computadoraNueva = false;
		}
		else
		{
			// Computadoras nuevas (inicialmente en el armario)
			computadoras[i].posicionInicial = posicionArmario;
			computadoras[i].computadoraNueva = true;
		}
		computadoras[i].posicionActual = computadoras[i].posicionInicial;
		computadoras[i].posicionFinal = computadoras[i].posicionInicial;
		computadoras[i].anguloInicial = 0.0f;
		computadoras[i].anguloFinal = 0.0f;
		computadoras[i].anguloActual = 0.0f;
		computadoras[i].anguloGiro = 0.0f;
		computadoras[i].scale = glm::vec3(1.0f, 1.0f, 1.0f); // Escala de las computadoras
		computadoras[i].estadoAnimacion = ESTADO_INICIAL;
		computadoras[i].dibujar = true;
		computadoras[i].tiempoEstado = 0.0f;
		// Las computadoras nuevas no se dibujan inicialmente
		if (computadoras[i].computadoraNueva)
		{
			computadoras[i].dibujar = false;
		}
	}
	estadoAnimacionComputadoras = false;
	contadorCompuViejasCompletadas = 0;
	todasCompuViejasCompletadas = false;
}

// Modificación en la función animacionComputadora() para las computadoras nuevas
void animacionComputadora()
{
	if (!estadoAnimacionComputadoras)
		return;
	for (int i = 0; i < NUMERO_COMPUTADORAS; ++i)
	{
		Computadora& comp = computadoras[i];
		comp.tiempoEstado += deltaTime;
		if (!comp.computadoraNueva)
		{
			// Animación para computadoras viejas
			switch (comp.estadoAnimacion)
			{
			case ESTADO_INICIAL:
				comp.estadoAnimacion = ESTADO_ASCENDER;
				comp.posicionFinal = comp.posicionActual + glm::vec3(0.0f, ALTURA_ASCENSO, 0.0f);
				break;

			case ESTADO_ASCENDER:
				comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
				if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
				{
					comp.posicionActual = comp.posicionFinal;
					comp.estadoAnimacion = ESTADO_ROTAR;
					comp.anguloGiro = 0.0f;
				}
				break;

			case ESTADO_ROTAR:
				comp.anguloGiro += deltaTime * 2.0f; // Velocidad de rotación
				if (comp.anguloGiro >= 2.0f * 3.14159f)
				{
					comp.estadoAnimacion = ESTADO_REGRESAR_ANGULO;
					comp.anguloFinal = comp.anguloInicial;
				}
				// Aplica la rotación circular
				comp.posicionActual.x = comp.posicionFinal.x + RADIO_GIRO * cos(comp.anguloGiro);
				comp.posicionActual.z = comp.posicionFinal.z + RADIO_GIRO * sin(comp.anguloGiro);
				comp.anguloActual = comp.anguloGiro;
				break;

			case ESTADO_REGRESAR_ANGULO:
				comp.anguloActual = glm::mix(comp.anguloActual, comp.anguloFinal, VELOCIDAD_ANIMACION * deltaTime);
				if (abs(comp.anguloActual - comp.anguloFinal) < 0.01f)
				{
					comp.anguloActual = comp.anguloFinal;
					comp.estadoAnimacion = ESTADO_AL_CENTRO;
					comp.posicionFinal = posicionCentro + glm::vec3(0.0f, ALTURA_ASCENSO, 0.0f);
				}
				break;

			case ESTADO_AL_CENTRO:
				comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
				if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
				{
					comp.posicionActual = comp.posicionFinal;
					comp.estadoAnimacion = ESTADO_ESPERAR_CENTRO;
					comp.tiempoEstado = 0.0f;
				}
				break;

			case ESTADO_ESPERAR_CENTRO:
				if (comp.tiempoEstado >= TIEMPO_ESPERA)
				{
					comp.estadoAnimacion = ESTADO_AL_ARMARIO;
					comp.posicionFinal = posicionArmario + glm::vec3(0.0f, ALTURA_ASCENSO, 0.0f);
				}
				break;

			case ESTADO_AL_ARMARIO:
				comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
				if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
				{
					comp.posicionActual = comp.posicionFinal;
					comp.estadoAnimacion = ESTADO_FINAL_VIEJA;
					comp.dibujar = false; // Ocultar la computadora vieja
					contadorCompuViejasCompletadas++;

					// Si todas las computadoras viejas completaron su animación
					if (contadorCompuViejasCompletadas >= NUMERO_COMPUTADORAS / 2)
					{
						todasCompuViejasCompletadas = true;
					}
				}
				break;

			case ESTADO_FINAL_VIEJA:
				// Estado final, no hace nada
				break;
			}
		}
		else
		{
			// Animación para computadoras nuevas
			if (todasCompuViejasCompletadas)
			{
				switch (comp.estadoAnimacion)
				{
				case ESTADO_INICIAL:
					comp.dibujar = true; // Hacer visible la computadora nueva
					comp.estadoAnimacion = ESTADO_SALIR_NUEVA;
					comp.posicionFinal = posicionCentro + glm::vec3(0.0f, ALTURA_ASCENSO, 0.0f);
					break;

				case ESTADO_SALIR_NUEVA:
					comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
					if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
					{
						comp.posicionActual = comp.posicionFinal;
						comp.estadoAnimacion = ESTADO_ESPERAR_CENTRO_NUEVA;
						comp.tiempoEstado = 0.0f;
					}
					break;

				case ESTADO_ESPERAR_CENTRO_NUEVA:
					if (comp.tiempoEstado >= TIEMPO_ESPERA)
					{
						comp.estadoAnimacion = ESTADO_A_POSICION;
						// La posición final es la misma que tenía la computadora vieja correspondiente
						int indiceVieja = i - NUMERO_COMPUTADORAS / 2;
						comp.posicionFinal = posicionesComputadoras[indiceVieja] + glm::vec3(0.0f, ALTURA_ASCENSO, 0.0f);
					}
					break;

				case ESTADO_A_POSICION:
					comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
					if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
					{
						comp.posicionActual = comp.posicionFinal;
						comp.estadoAnimacion = ESTADO_ROTAR_NUEVA;
						comp.anguloGiro = 0.0f;
					}
					break;

				case ESTADO_ROTAR_NUEVA:
					comp.anguloGiro += deltaTime * 2.0f;
					if (comp.anguloGiro >= 2.0f * 3.14159f)
					{
						comp.estadoAnimacion = ESTADO_REGRESAR_ANGULO_NUEVA;
						comp.anguloFinal = comp.anguloInicial;
					}
					// Aplica la rotación circular
					comp.posicionActual.x = comp.posicionFinal.x + RADIO_GIRO * cos(comp.anguloGiro);
					comp.posicionActual.z = comp.posicionFinal.z + RADIO_GIRO * sin(comp.anguloGiro);
					comp.anguloActual = comp.anguloGiro;
					break;

				case ESTADO_REGRESAR_ANGULO_NUEVA:
					comp.anguloActual = glm::mix(comp.anguloActual, comp.anguloFinal, VELOCIDAD_ANIMACION * deltaTime);
					if (abs(comp.anguloActual - comp.anguloFinal) < 0.01f)
					{
						comp.anguloActual = comp.anguloFinal;
						comp.estadoAnimacion = ESTADO_DESCENDER;
						// Ajuste para asegurar que desciende a la posición exacta en la mesa
						comp.posicionFinal = posicionesComputadoras[i - NUMERO_COMPUTADORAS / 2];
					}
					break;

				case ESTADO_DESCENDER:
					comp.posicionActual = glm::mix(comp.posicionActual, comp.posicionFinal, VELOCIDAD_ANIMACION * deltaTime);
					if (glm::distance(comp.posicionActual, comp.posicionFinal) < 0.01f)
					{
						comp.posicionActual = comp.posicionFinal;
						comp.estadoAnimacion = ESTADO_FINAL_NUEVA;
					}
					break;

				case ESTADO_FINAL_NUEVA:
					// Estado final, no hace nada
					break;
				}
			}
		}
	}
}

// Función para reiniciar las computadoras
void reiniciarComputadoras()
{
	estadoAnimacionComputadoras = false;
	contadorCompuViejasCompletadas = 0;
	todasCompuViejasCompletadas = false;
	inicializarComputadoras();
}

// ------------ Animacion del salon  ------------ //
void cargarTexturaRuido()
{
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width = 512, height = 512;
	std::vector<unsigned char> noiseData(width * height);
	for (int i = 0; i < width * height; ++i)
	{
		noiseData[i] = rand() % 256;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, noiseData.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void manejarObjetos(float deltaTime)
{
	// Manejar disolución del humano
	if (humanoDisolviendo)
	{
		dissolveAmountHumano += deltaTime * 1.5f;
		if (dissolveAmountHumano >= 1.0f)
		{
			dissolveAmountHumano = 1.0f;
			humanoDisolviendo = false;
		}
	}
	else if (humanoReapareciendo)
	{
		dissolveAmountHumano -= deltaTime * 1.5f;
		if (dissolveAmountHumano <= 0.0f)
		{
			dissolveAmountHumano = 0.0f;
			humanoReapareciendo = false;
		}
	}

	switch (estadoObjetos)
	{
	case OBJETOS_VISIBLES:
		break;

	case OBJETOS_DISOLVIENDO:
		dissolveAmountObjetos += deltaTime * 1.5f;

		if (dissolveAmountObjetos >= 1.0f)
		{
			dissolveAmountObjetos = 1.0f;
			estadoObjetos = OBJETOS_ESPERANDO;
			// Iniciar animación del salón
			animationActive = true;
			currentState = SHRINKING;
			animationTimer = 0.0f;
		}
		break;

	case OBJETOS_ESPERANDO:
		// Esperar a que la animación del salón termine
		if (currentState == FINAL)
		{
			tiempoEsperaObjetos += deltaTime;

			// Esperar 2 segundos antes de reaparecer
			if (tiempoEsperaObjetos >= 2.0f)
			{
				estadoObjetos = OBJETOS_REAPARECIENDO;
				humanoReapareciendo = true;
			}
		}
		break;

	case OBJETOS_REAPARECIENDO:
		dissolveAmountObjetos -= deltaTime * 1.5f;

		if (dissolveAmountObjetos <= 0.0f)
		{
			dissolveAmountObjetos = 0.0f;
			estadoObjetos = OBJETOS_COMPLETO;
		}
		break;

	case OBJETOS_COMPLETO:
		break;
	}
}

// Función para actualizar la animación según el estado actual
void UpdateAnimation(float deltaTime)
{
	if (!animationActive)
	{
		return;
	}

	animationTimer += deltaTime;

	switch (currentState)
	{
	case INITIAL:
		break;

	case SHRINKING:
		// Salón viejo reduciéndose
	{
		float shrinkFactor = 1.0f - (animationTimer / 1.0f); // 1 segundo para reducir
		if (shrinkFactor < 0.4f)
		{
			shrinkFactor = 0.4f;
		}
		oldSalonScale = glm::vec3(shrinkFactor);

		if (animationTimer >= 1.0f)
		{
			currentState = MOVING_OUT;
			animationTimer = 0.0f;
			bulldozerVisible = true;
		}
	}
	break;

	case MOVING_OUT:
	{
		float moveSpeed = 8.0f * deltaTime;

		// Mover la base del bulldozer
		bulldozerBasePosition.x += moveSpeed;

		// Rotar las cintas
		beltRotationAngle += beltRotationSpeed * deltaTime;
		if (beltRotationAngle > 360.0f)
			beltRotationAngle -= 360.0f;

		if (bulldozerBasePosition.x >= -10.5f)
		{
			salonEmpezoAMoverse = true;
		}

		if (salonEmpezoAMoverse)
		{
			oldSalonPosition.x += moveSpeed;
		}

		if (oldSalonPosition.x > 60.0f)
		{
			oldSalonVisible = false;
			bulldozerVisible = false;
			helicopterVisible = true; // El helicóptero se vuelve visible
			currentState = HELICOPTER_MOVING;
			animationTimer = 0.0f;
			newSalonVisible = false;
		}
	}
	break;

	case HELICOPTER_MOVING:
	{
		// Movimiento del helicóptero (siempre se actualiza)
		helicopterBasePosition.x -= helicopterSpeed * deltaTime;
		helicopterBasePosition.y = 10.0f + 3.0f * sin(helicopterBasePosition.x / 10.0f);

		// Cuando el helicóptero pasa por (0,0,0), activar la caída del salón
		if (helicopterBasePosition.x <= 0.0f && !newSalonVisible)
		{
			salonCayendo = true;
			newSalonVisible = true;
			newSalonPosition = glm::vec3(0.0f, 5.0f, 0.0f); // Posición inicial en el aire
			newSalonScale = glm::vec3(0.1f);
			currentState = NEW_FALLING; // Transición al estado de caída
			animationTimer = 0.0f;
			bounceVelocity = 0.0f;
			bounceCount = 0;
		}

		// El helicóptero desaparece cuando sale completamente de escena
		if (helicopterBasePosition.x <= -50.0f)
		{
			helicopterVisible = false;
		}
	}
	break;

	case NEW_FALLING:
	{
		// Actualizar posición del helicóptero (movimiento continuo)
		helicopterBasePosition.x -= helicopterSpeed * deltaTime;
		helicopterBasePosition.y = 10.0f + 3.0f * sin(helicopterBasePosition.x / 10.0f);

		if (helicopterBasePosition.x <= -50.0f)
		{
			helicopterVisible = false;
		}

		// Física de caída y rebote
		if (!hasLanded)
		{ // Solo aplicar física si no ha terminado de rebotar
			bounceVelocity += GRAVITY * deltaTime;
			newSalonPosition.y += bounceVelocity * deltaTime;

			// Cuando toca el suelo
			if (newSalonPosition.y <= 0.0f)
			{
				newSalonPosition.y = 0.0f;

				// Rebote si aún tiene energía suficiente
				if (abs(bounceVelocity) > 0.3f)
				{
					bounceVelocity = -bounceVelocity * BOUNCE_DAMPING;
					bounceCount++;

					// Escala durante el rebote
					newSalonScale = glm::vec3(1.0f);
				}
				else
				{
					// Finalizar rebotes cuando la energía es baja
					bounceVelocity = 0.0f;
					hasLanded = true;
					landingTimer = 0.0f;
				}
			}
		}
		else
		{
			// Transición suave después del último rebote
			landingTimer += deltaTime;
			float progress = glm::clamp(landingTimer / 0.5f, 0.0f, 1.0f);
			newSalonScale = glm::mix(glm::vec3(1.0f), glm::vec3(1.0f), progress); // Mantener escala 1.0
		}

		// Condición para terminar la animación
		if (hasLanded && landingTimer >= 0.5f)
		{
			currentState = FINAL;
			newSalonPosition.y = 0.0f; // Asegurar posición exacta
		}
	}
	break;

	case FINAL:
	{
		// Interpolación gradual a la escala final
		if (animationTimer < 1.0f)
		{ // 1 segundo para la transición
			float progress = animationTimer / 1.0f;
			newSalonScale = glm::mix(glm::vec3(0.8f), glm::vec3(1.0f), progress);
		}
		else
		{
			newSalonScale = glm::vec3(1.0f);
		}

		animationTimer += deltaTime;

		// Continuar moviendo el helicóptero hasta que desaparezca
		if (helicopterVisible)
		{
			// Movimiento horizontal
			helicopterBasePosition.x -= helicopterSpeed * deltaTime;

			// Efecto de flotación suave
			helicopterBasePosition.y = 10.0f + 1.5f * sin(glfwGetTime() * 2.0f);

			// Desaparecer cuando sale de escena
			if (helicopterBasePosition.x <= -50.0f)
			{
				helicopterVisible = false;
			}
		}
	}
	break;
	}

	if (helicopterVisible)
	{
		mainRotorAngle += mainRotorSpeed * deltaTime;
		if (mainRotorAngle > 360.0f)
			mainRotorAngle -= 360.0f;

		tailRotorAngle += tailRotorSpeed * deltaTime;
		if (tailRotorAngle > 360.0f)
			tailRotorAngle -= 360.0f;
	}
}

// Función para reiniciar la escena al estado inicial
void ResetScene()
{
	// Restablecer estado
	currentState = INITIAL;
	animationTimer = 0.0f;
	animationActive = false;

	// Restablecer posiciones y escalas
	oldSalonPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	oldSalonScale = glm::vec3(1.0f);
	newSalonPosition = glm::vec3(50.0f, 5.0f, 0.0f); // Fuera de la vista
	newSalonScale = glm::vec3(0.4f);
	bulldozerBasePosition = glm::vec3(-30.5f, -0.5f, -9.0f);
	bulldozerBelt1Position = glm::vec3(-2.6f, 2.0f, 3.7);
	bulldozerBelt2Position = glm::vec3(-2.6f, 2.0f, -3.5);
	bulldozerBelt3Position = glm::vec3(-2.6f, 2.0f, -3.7f);
	bulldozerBelt4Position = glm::vec3(-2.6f, 2.0f, 3.5f);
	helicopterBasePosition = glm::vec3(30.0f, 10.0f, -9.0f);
	beltRotationAngle = 0.0f;
	helicopterRotationY = 270.0f;
	mainRotorAngle = 0.0f;
	tailRotorAngle = 0.0f;

	// Restablecer visibilidad
	oldSalonVisible = true;
	newSalonVisible = false;
	bulldozerVisible = false;
	helicopterVisible = false;
	salonEmpezoAMoverse = false;

	// Restablecer variables de rebote
	bounceHeight = 0.0f;
	bounceVelocity = 0.0f;
	bounceCount = 0;
}

void reiniciarObjetos()
{
	estadoObjetos = OBJETOS_VISIBLES;
	dissolveAmountObjetos = 0.0f;
	tiempoEsperaObjetos = 0.0f;
	animacionSalonIniciada = false;
}

// ------------ Animacion del humano  ------------ //
void interpolarHumano()
{
	humanoKF[frameActualHumano].nPasos = ceil(humanoKF[frameActualHumano].segundosAnimacion / deltaTime);
	humanoKF[frameActualHumano].incPosX = (humanoKF[frameActualHumano + 1].posX - humanoKF[frameActualHumano].posX) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incPosY = (humanoKF[frameActualHumano + 1].posY - humanoKF[frameActualHumano].posY) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incPosZ = (humanoKF[frameActualHumano + 1].posZ - humanoKF[frameActualHumano].posZ) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incTorsoRot_y = (humanoKF[frameActualHumano + 1].torsoRot_y - humanoKF[frameActualHumano].torsoRot_y) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incCabezaRot_x = (humanoKF[frameActualHumano + 1].cabezaRot_x - humanoKF[frameActualHumano].cabezaRot_x) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incCabezaRot_y = (humanoKF[frameActualHumano + 1].cabezaRot_y - humanoKF[frameActualHumano].cabezaRot_y) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incBrazoDerRot_x = (humanoKF[frameActualHumano + 1].brazoDerRot_x - humanoKF[frameActualHumano].brazoDerRot_x) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incBrazoDerRot_y = (humanoKF[frameActualHumano + 1].brazoDerRot_y - humanoKF[frameActualHumano].brazoDerRot_y) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incBrazoIzqRot_x = (humanoKF[frameActualHumano + 1].brazoIzqRot_x - humanoKF[frameActualHumano].brazoIzqRot_x) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incBrazoIzqRot_y = (humanoKF[frameActualHumano + 1].brazoIzqRot_y - humanoKF[frameActualHumano].brazoIzqRot_y) / humanoKF[frameActualHumano].nPasos;
	humanoKF[frameActualHumano].incPiernaRot = (humanoKF[frameActualHumano + 1].piernaRot - humanoKF[frameActualHumano].piernaRot) / humanoKF[frameActualHumano].nPasos;
}

void animarHumano()
{
	if (!animacionHumano)
		return;
	if (pasoActualInterpolacionHumano >= humanoKF[frameActualHumano].nPasos)
	{
		frameActualHumano++;
		if (frameActualHumano >= nFramesHumano - 1)
		{
			animacionHumano = false;
		}
		else
		{
			pasoActualInterpolacionHumano = 0;
			interpolarHumano();
		}
	}
	else
	{
		humanoPos.x += humanoKF[frameActualHumano].incPosX;
		humanoPos.y += humanoKF[frameActualHumano].incPosY;
		humanoPos.z += humanoKF[frameActualHumano].incPosZ;
		humanoTorsoRot_y += humanoKF[frameActualHumano].incTorsoRot_y;
		humanoCabezaRot_x += humanoKF[frameActualHumano].incCabezaRot_x;
		humanoCabezaRot_y += humanoKF[frameActualHumano].incCabezaRot_y;
		humanoBrazoDerRot_x += humanoKF[frameActualHumano].incBrazoDerRot_x;
		humanoBrazoDerRot_y += humanoKF[frameActualHumano].incBrazoDerRot_y;
		humanoBrazoIzqRot_x += humanoKF[frameActualHumano].incBrazoIzqRot_x;
		humanoBrazoIzqRot_y += humanoKF[frameActualHumano].incBrazoIzqRot_y;
		humanoPiernaRot += humanoKF[frameActualHumano].incPiernaRot;

		pasoActualInterpolacionHumano++;
	}
}

// ------------ Animacion del balon  ------------ //
void interpolarBalon() {
	balonKF[PlayIndexBalon].incX = (balonKF[PlayIndexBalon + 1].x - balonKF[PlayIndexBalon].x) / maxPasosBalon;
	balonKF[PlayIndexBalon].incY = (balonKF[PlayIndexBalon + 1].y - balonKF[PlayIndexBalon].y) / maxPasosBalon;
	balonKF[PlayIndexBalon].incZ = (balonKF[PlayIndexBalon + 1].z - balonKF[PlayIndexBalon].z) / maxPasosBalon;
	balonIncRot = (balonKF[PlayIndexBalon + 1].rot - balonKF[PlayIndexBalon].rot) / maxPasosBalon; 
}

void animarBalon() {
	if (animacionBalon) {
		balonX += balonKF[PlayIndexBalon].incX;
		balonY += balonKF[PlayIndexBalon].incY;
		balonZ += balonKF[PlayIndexBalon].incZ;
		balonRot += balonIncRot;
		pasosBalon++;

		if (pasosBalon >= maxPasosBalon) {
			pasosBalon = 0;
			PlayIndexBalon++;
			if (PlayIndexBalon == 1) {
				animacionProyector = true;
				PlayIndexProyector = 0;
				i_curr_steps = 0;
				proyectorPosX = ProyectorKF[0].posX;
				proyectorPosY = ProyectorKF[0].posY;
				proyectorPosZ = ProyectorKF[0].posZ;
				proyectorRotY = ProyectorKF[0].rotY;
				interpolarProyector();
			}
			if (PlayIndexBalon >= FrameIndexBalon - 1) {
				animacionBalon = false;
			}
			else {
				interpolarBalon();
			}
		}
	}
}

// ------------ Animacion del pizarron  ------------ //
void interpolarPizarron() {
	pizKF[PlayIndexPizarron].incX = (pizKF[PlayIndexPizarron + 1].x - pizKF[PlayIndexPizarron].x) / maxPasosPizarron;
	pizKF[PlayIndexPizarron].incY = (pizKF[PlayIndexPizarron + 1].y - pizKF[PlayIndexPizarron].y) / maxPasosPizarron;
	pizKF[PlayIndexPizarron].incZ = (pizKF[PlayIndexPizarron + 1].z - pizKF[PlayIndexPizarron].z) / maxPasosPizarron;
	pizKF[PlayIndexPizarron].incRotY = (pizKF[PlayIndexPizarron + 1].rotY - pizKF[PlayIndexPizarron].rotY) / maxPasosPizarron;

	pizarronPosX = pizKF[PlayIndexPizarron].x;
	pizarronPosY = pizKF[PlayIndexPizarron].y;
	pizarronPosZ = pizKF[PlayIndexPizarron].z;
	pizarronRotY = pizKF[PlayIndexPizarron].rotY;
}

void animarPizzaron() {
	if (animacionPizarron) {
		if (pasosPizarron >= maxPasosPizarron) {
			PlayIndexPizarron++;
			if (PlayIndexPizarron >= FrameIndexPizarron - 1) {
				animacionPizarron = false;
			}
			else {
				pasosPizarron = 0;
				interpolarPizarron();
			}
		}
		else {
			pizarronPosX += pizKF[PlayIndexPizarron].incX;
			pizarronPosY += pizKF[PlayIndexPizarron].incY;
			pizarronPosZ += pizKF[PlayIndexPizarron].incZ;
			pizarronRotY += pizKF[PlayIndexPizarron].incRotY;
			pasosPizarron++;
		}
	}
}

// ------------ Animacion del proyector  ------------ //
void interpolarProyector() {
	ProyectorKF[PlayIndexProyector].incX = (ProyectorKF[PlayIndexProyector + 1].posX - ProyectorKF[PlayIndexProyector].posX) / i_max_steps;
	ProyectorKF[PlayIndexProyector].incY = (ProyectorKF[PlayIndexProyector + 1].posY - ProyectorKF[PlayIndexProyector].posY) / i_max_steps;
	ProyectorKF[PlayIndexProyector].incZ = (ProyectorKF[PlayIndexProyector + 1].posZ - ProyectorKF[PlayIndexProyector].posZ) / i_max_steps;
	ProyectorKF[PlayIndexProyector].incRotY = (ProyectorKF[PlayIndexProyector + 1].rotY - ProyectorKF[PlayIndexProyector].rotY) / i_max_steps;
}

void animarProyector() {
	if (animacionProyector) {
		if (i_curr_steps >= i_max_steps) {
			PlayIndexProyector++;
			if (PlayIndexProyector >= FrameIndexProyector - 1) {
				animacionProyector = false;
				//Inicia la animación del pizarrón usando los keyframes definidos en pizKF
				animacionPizarron = true;
				mostrarPizarron = true;
				pasosPizarron = 0;
				PlayIndexPizarron = 0;
				//Posición inicial del pizarrón (primer keyframe)
				pizarronPosX = pizKF[0].x;
				pizarronPosY = pizKF[0].y;
				pizarronPosZ = pizKF[0].z;
				pizarronRotY = pizKF[0].rotY;

				//Interpolación hacia el siguiente keyframe
				interpolarPizarron();
			}
			else {
				i_curr_steps = 0;
				interpolarProyector();
			}
		}
		else {
			proyectorPosX += ProyectorKF[PlayIndexProyector].incX;
			proyectorPosY += ProyectorKF[PlayIndexProyector].incY;
			proyectorPosZ += ProyectorKF[PlayIndexProyector].incZ;
			proyectorRotY += ProyectorKF[PlayIndexProyector].incRotY;
			i_curr_steps++;
		}
	}
}