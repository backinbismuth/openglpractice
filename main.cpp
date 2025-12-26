#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"shader.h"

void processInput(GLFWwindow* window) { // Processes input
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { // Sets what happens to the viewport when resized
	glViewport(0, 0, width, height);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Initialising version and removing backwards support

	GLFWwindow* window = glfwCreateWindow(800, 600, "BetterThanUnity", NULL, NULL); // Creates window

	if (window == NULL) {
		std::cout << "Failed to initialise GLFW :[" << std::endl; // Window fails report
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Makes window current context (Forefront at screen)

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // Fancy way to say "if process for glad doesnt exist"
		std::cout << "Failed to initialise GLAD :[" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int indices[] = {
		0,1,3, // first tri
		1,2,3 // second tri (only applicable with rectangles! We're not using our EBO rn so it doesnt matter)
	};

	unsigned int VBO; // VBO = Vertex Buffer Object
	glGenBuffers(1, &VBO); // Generates 1 buffer ID, puts it into VBO's address
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copies vertices into GL_ARRAY_BUFFER
	// Since the vertices of the triangle does not change in the 3d world, we use STATIC_DRAW. Otherwise, we'd use DYNAMIC_DRAW.

	Shader currentShader("vertShader.vs", "fragShader.fs");

	unsigned int VAO;  // VAO set up, refer to VBO setup for details
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO; // Element Buffer Object (Currently unused, since we're not rendering a bunch of verts
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // For Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // For Color
	glEnableVertexAttribArray(1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Uncomment to make wireframe

	while (!glfwWindowShouldClose(window)) { // Render loop
		// Input
		processInput(window);

		// Rendering Commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentShader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check events, swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO); // Free up memory when program ends
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	std::cout << "Everything worked! :]\n";
	return 0;
}
