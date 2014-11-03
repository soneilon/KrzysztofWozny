// Include standard headers
#include <stdio.h>
#include <stdlib.h>
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>
GLFWwindow* window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
// Shader configuration!
#include <common/shader.hpp>
#include <common/controls.hpp>


int main(){

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	//GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glDisable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	/*// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//ortho below:
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 4, -7), // Camera is at (0,4,-7), in World Space
		glm::vec3(0, 1, 0), // and looks at (0,1,0)
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	*/
	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {

		//Pyramid

		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f, //bottom
		1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,

		//Flat pyramid

		-1.0f + 3, 0.0f, -1.0f + 3,
		1.0f + 3, 0.0f, -1.0f + 3,
		0.0f + 3, 0.0f, 0.0f + 1,
		1.0f + 3, 0.0f, -1.0f + 3,
		1.0f + 3, 0.0f, 1.0f + 3,
		0.0f + 5, 0.0f, 0.0f + 3,
		1.0f + 3, 0.0f, 1.0f + 3,
		-1.0f + 3, 0.0f, 1.0f + 3,
		0.0f + 3, 0.0f, 0.0f + 5,
		-1.0f + 3, 0.0f, 1.0f + 3,
		-1.0f + 3, 0.0f, -1.0f + 3,
		0.0f + 1, 0.0f, 0.0f + 3,
		-1.0f + 3, 0.0f, -1.0f + 3, //bottom
		1.0f + 3, 0.0f, -1.0f + 3,
		-1.0f + 3, 0.0f, 1.0f + 3,
		1.0f + 3, 0.0f, 1.0f + 3,
		1.0f + 3, 0.0f, -1.0f + 3,
		-1.0f + 3, 0.0f, 1.0f + 3,

		//Cuboid

		0.0f - 4, 1.0f, -1.0f + 5, //1
		-1.0f - 4, -1.0f, -1.0f + 5,
		-1.0f - 4, 1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, -1.0f + 5,
		0.0f - 4, -1.0f, -1.0f + 5,
		-1.0f - 4, -1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, 1.0f + 5, //2
		0.0f - 4, -1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, -1.0f + 5,
		0.0f - 4, -1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, 1.0f + 5,
		0.0f - 4, -1.0f, 1.0f + 5,
		-1.0f - 4, 1.0f, 1.0f + 5, //3
		-1.0f - 4, -1.0f, 1.0f + 5,
		0.0f - 4, -1.0f, 1.0f + 5,
		0.0f - 4, 1.0f, 1.0f + 5,
		-1.0f - 4, 1.0f, 1.0f + 5,
		0.0f - 4, -1.0f, 1.0f + 5,
		-1.0f - 4, -1.0f, -1.0f + 5, //4
		-1.0f - 4, -1.0f, 1.0f + 5,
		-1.0f - 4, 1.0f, 1.0f + 5,
		-1.0f - 4, -1.0f, -1.0f + 5,
		-1.0f - 4, 1.0f, 1.0f + 5,
		-1.0f - 4, 1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, 1.0f + 5,//5
		0.0f - 4, 1.0f, -1.0f + 5,
		-1.0f - 4, 1.0f, -1.0f + 5,
		0.0f - 4, 1.0f, 1.0f + 5,
		-1.0f - 4, 1.0f, -1.0f + 5,
		-1.0f - 4, 1.0f, 1.0f + 5,
		0.0f - 4, -1.0f, 1.0f + 5, //6
		-1.0f - 4, -1.0f, -1.0f + 5,
		0.0f - 4, -1.0f, -1.0f + 5,
		0.0f - 4, -1.0f, 1.0f + 5,
		-1.0f - 4, -1.0f, 1.0f + 5,
		-1.0f - 4, -1.0f, -1.0f + 5,

	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {

		//Pyramid

		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		//Flat pyramid

		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,

		//Cuboid

		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.7, 0.7, 0.7,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.6, 0.6, 0.6,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,
		0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
		0.5, 0.5, 0.5,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 24 * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	//glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

}
