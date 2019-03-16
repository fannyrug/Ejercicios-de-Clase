/*---------------------------------------------------------*/
/* ----------------   Práctica 6 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
//#include "Sphere.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

float	sol = 0.0f;
float   mercurio = 0.0f;
float   lunatierra = 0.0f;
float   venus = 0.0f;
float   tierra = 0.0f;
float   marte = 0.0f;
float   luna1marte = 0.0f;
float   luna2marte = 0.0f;
float   jupiter = 0.0f, luna1jupiter = 0.0f, luna2jupiter = 0.0f, luna3jupiter = 0.0f, luna4jupiter = 0.0f;
float   saturno = 0.0f, anillosaturno = 0.0f, anillo2saturno = 0.0f;
float   urano = 0.0f; 
float luna1urano = 0.0f; 
float luna2urano = 0.0f;
float neptuno = 0.0f, lunaneptuno = 0.0f;



//To Buid the Sphere
//---------------------------------------------------------------------------
//const int na = 36;        // vertex grid size
const int meridianos = 46;			// vertex grid size   na -> meridianos
const int paralelos = 18;			// nb -> paralelos
const int meridianos3 = meridianos * 3;     // line in grid size
const int nn = paralelos * meridianos3;    // whole grid size
GLfloat sphere_pos[nn]; // vertex
GLfloat sphere_nor[nn]; // normal
//GLfloat sphere_col[nn];   // color
GLuint  sphere_ix[meridianos*(paralelos - 1) * 6];    // indices
GLuint sphere_vbo[4] = { -1,-1,-1,-1 };
GLuint sphere_vao[4] = { -1,-1,-1,-1 };

void sphere_init()
{
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(meridianos);
	db = (GLfloat)M_PI / GLfloat(paralelos - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < paralelos; ib++, b += db)
		for (a = 0.0, ia = 0; ia < meridianos; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = cos(b)*sin(a);
			z = sin(b);
			sphere_pos[ix + 0] = x * r;
			sphere_pos[ix + 1] = y * r;
			sphere_pos[ix + 2] = z * r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < paralelos; ib++)
	{
		for (ia = 1; ia < meridianos; ia++, iy++)
		{
			// first half of QUAD
			sphere_ix[ix] = iy;      ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos;   ix++;
			// second half of QUAD
			sphere_ix[ix] = iy + meridianos;   ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos + 1; ix++;
		}
		// first half of QUAD
		sphere_ix[ix] = iy;       ix++;
		sphere_ix[ix] = iy + 1 - meridianos;  ix++;
		sphere_ix[ix] = iy + meridianos;    ix++;
		// second half of QUAD
		sphere_ix[ix] = iy + meridianos;    ix++;
		sphere_ix[ix] = iy - meridianos + 1;  ix++;
		sphere_ix[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, sphere_vao);
	glGenBuffers(4, sphere_vbo);
	glBindVertexArray(sphere_vao[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_pos), sphere_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_ix), sphere_ix, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	i = 2; // normal
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*
		i=3; // color
		glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
	*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void sphere_exit()
{
	glDeleteVertexArrays(4, sphere_vao);
	glDeleteBuffers(4, sphere_vbo);
}
void sphere_draw()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(sphere_vao[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_LINE_LOOP, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}



void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void animate(void)
{
	sol += 0.5f;
	mercurio += 0.8f;
	venus += 0.8f;
	tierra += 0.8f;
	lunatierra += 0.3f;
	marte += 0.8f;
	luna1marte += 0.3f;
	luna2marte += 0.3f;
	jupiter += 0.8f;
	luna1jupiter += 0.3;
	luna2jupiter += 0.3;
	luna3jupiter += 0.3;
	luna4jupiter += 0.3;
	saturno += 0.8f, anillosaturno += 0.8f, anillo2saturno += 0.8f;
	urano += 0.8, luna1urano += 0.3f, luna2urano += 0.3f;
	neptuno += 0.8, lunaneptuno += 0.3f;
}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 temp01 = glm::mat4(1.0f); //Temp

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	sphere_draw();	//Sun

					//mercurio
	model = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(3, 0, 0));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.3f, 0.1f));
	sphere_draw();	//Mercurio



	//VENUS
	model = glm::mat4(1.0f);

	//model = glm::rotate(model, glm::radians(venus), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(5, 0, 0));
	model = glm::rotate(model, glm::radians(venus), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.9f, 0.6f, 0.1f));
	sphere_draw();	//Venus

	//Tierra
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(8, 0, 0));
	model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.3f, 0.6f, 1.0f));
	sphere_draw();	//Tierra

	//Para la Luna
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();	//Luna-Tierra


					//Marte
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(10, 0, 0));
	model = glm::rotate(model, glm::radians(marte), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.0f, 0.0f));
	sphere_draw();	//Marte

					//Para la Luna 1 Marte
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(luna1marte), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna1Marte

					//Para la Luna 2 Marte
	model = glm::translate(model, glm::vec3(1.9, 0, 0));
	model = glm::rotate(model, glm::radians(luna2marte), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna2Marte


					//Jupiter
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(13, 0, 0));
	model = glm::rotate(model, glm::radians(jupiter), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.8f, 0.7f, 0.5f));
	sphere_draw();	//Jupiter

					//Para la Luna 1 Jupiter
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(luna1jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna1Jupiter

					//Para la Luna 2 Jupiter
	model = glm::translate(model, glm::vec3(1.9, 0, 0));
	model = glm::rotate(model, glm::radians(luna2jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna2Jupiter


					//Para la Luna 3 Jupiter
	model = glm::translate(model, glm::vec3(2.2, 0, 0));
	model = glm::rotate(model, glm::radians(luna3jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna3Jupiter


					//Para la Luna 4 Jupiter
	model = glm::translate(model, glm::vec3(2.4, 0, 0));
	model = glm::rotate(model, glm::radians(luna4jupiter), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna4Jupiter

	// Saturno 

	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(17, 0, 0));
	model = glm::rotate(model, glm::radians(saturno), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.9f, 0.5f));
	sphere_draw();	//Saturno 

	//AnilloSaturno 


	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(17, 0, 0));
	model = glm::rotate(model, glm::radians(anillosaturno), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(1.5f, 0.05f, 1.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();	//Saturno 

					//Anillo2Saturno 


	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(17, 0, 0));
	model = glm::rotate(model, glm::radians(anillo2saturno), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(1.5f, 0.25f, 1.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.8f, 1.0f));
	sphere_draw();	//Saturno 

	//URANO 
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(21, 0, 0));
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.9f, 0.9f));
	sphere_draw();	//Urano 

	//Luna1Urano
	
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(luna1urano), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna1Marte

//Para la Luna 2 Urano
	model = glm::translate(model, glm::vec3(2.0, 0, 0));
	model = glm::rotate(model, glm::radians(luna2urano ), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5f, 0.5f, 0.5f));
	sphere_draw();	//Luna2Marte

	//NEPTUNO 
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(tierra), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(24, 0, 0));
	model = glm::rotate(model, glm::radians(neptuno), glm::vec3(0, 1, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.3f, 0.3f, 0.9f));
	sphere_draw();	//Urano 

	//Luna1Neptuno

	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(lunaneptuno), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.4f, 0.4f, 0.4f));
	sphere_draw();	//Luna1Marte


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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
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
	sphere_init();
	glEnable(GL_DEPTH_TEST);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);
		animate();

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	sphere_exit();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}