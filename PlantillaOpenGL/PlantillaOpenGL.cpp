// PlantillaOpenGL.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "Shader.h"
#include "Vertice.h"

#include "Modelo.h"
#include "Carro.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

using namespace std;

Shader *shader;
GLuint posicionID;
GLuint colorID;
GLuint modeloID;
GLuint vistaID;
GLuint proyeccionID;

mat4 vista;
mat4 proyeccion;

Carro *carro;



//Declaramos apuntador de ventana
GLFWwindow *window;

GLfloat ancho = 1024.0f;
GLfloat alto = 768.0f;

vec3 posicionCamara = vec3(3.0f, 1.5f, 7.0f);

void establecerVista()
{
	vista = lookAt
	(
		posicionCamara, // Posicion de la camara
		vec3(0.0f, 0.0f, 0.0f), //Posicion del objetivo
		vec3(0.0f, 1.0f, 0.0f) //Vector hacia arriba
	);

	if (carro != NULL)
	{
		carro->vista = vista;
	}
}

void establecerProyeccion()
{
	proyeccion = perspective
	(
		90.0f, //Campo de vision (FoV) 
		ancho/alto, //Relacion de aspecto (Aspecto Ratio)
		0.1f, //Near Clipping (Desde donde renderea)
		150.0f //Far Clipping (Donde va a dejar de renderear)
	);

}

void dibujar() 
{
	carro->dibujar(GL_QUADS);
}

void actualizar()
{
	int estadoIzquierda =
		glfwGetKey(window, GLFW_KEY_LEFT);

	if (estadoIzquierda == GLFW_PRESS)
	{
		posicionCamara.x -= 0.01f;
		establecerVista();
	}

	int estadoDerecha =
		glfwGetKey(window, GLFW_KEY_RIGHT);

	if (estadoDerecha == GLFW_PRESS)
	{
		posicionCamara.x += 0.01f;
		establecerVista();
	}

	int estadoEspacio =
		glfwGetKey(window, GLFW_KEY_SPACE);
	if (estadoEspacio == GLFW_PRESS)
	{
		//Rotar el carro
		carro->modelo = rotate(carro->modelo, 0.1f,
			vec3(0.0f, 1.0f, 0.0f));
	}

	int estadoArriba =
		glfwGetKey(window, GLFW_KEY_UP);

	if (estadoArriba == GLFW_PRESS)
	{
		posicionCamara.y -= 0.01f;
		establecerVista();
	}

	int estadoAbajo =
		glfwGetKey(window, GLFW_KEY_DOWN);

	if (estadoAbajo == GLFW_PRESS)
	{
		posicionCamara.y += 0.01f;
		establecerVista();
	}
	
}

int main()
{
	
	
	//Si no se pudo iniciar glfw
	//terminamos ejcución
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo iniciar GLFW
	//entonces inicializamos la ventana
	window =
		glfwCreateWindow(1024, 768, "Ventana", 
			NULL, NULL);
	//Si no podemos iniciar la ventana
	//Entonces terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido  el contexto
	//Activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << 
			glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL =
		glGetString(GL_VERSION);
	cout << "Version OpenGL: "
		<< versionGL;

	

	const char *rutaVertex = 
		"VertexShader.shader";
	const char *rutaFragment = 
		"FragmentShader.shader";

	shader = new Shader(rutaVertex, rutaFragment);

	//Mapeo de atributos
	
	posicionID =
		glGetAttribLocation(shader->getID(), "posicion");
	colorID =
		glGetAttribLocation(shader->getID(), "color");
	modeloID =
		glGetUniformLocation(shader->getID(), "modelo");
	vistaID = 
		glGetUniformLocation(shader->getID(), "vista");
	proyeccionID =
		glGetUniformLocation(shader->getID(), "proyeccion");



	//Desenlazar el shader
	shader->desenlazar();

	establecerVista();
	establecerProyeccion();

	//Inicializar modelo y Establecer shader al modelo
	carro = new Carro();
	carro->vista = vista;
	carro->proyeccion = proyeccion;
	carro->shader = shader;
	carro->inicializarVertexArray(posicionID, colorID, modeloID, vistaID, proyeccionID);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LESS);


	//Ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Esablecer region de dibujo
		glViewport(0, 0, 1024, 768);
		//Establece el color de borrado
		glClearColor(0, 0, 0.5, 1);
		//Borramos
		glClear(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar los buffers
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

