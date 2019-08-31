#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINE);
		float radius = 0;
		glVertex2f(0.0f, 0.0f);
		int offset = 45;
		for (int i = offset; i < 360 - offset; i++)
		{
			glVertex2f(sin(i), cos(i));
			std::cout << "Sinus " << sin(i) << std::endl;
			std::cout << "Cosinus " << cos(i) << std::endl;
		}

		glEnd();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}