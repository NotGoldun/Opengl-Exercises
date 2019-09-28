#include <Gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define Assert(func) if(!(func));

#define glVerify(func) glClearErrors(); \
		func;\
		Assert(glLogError(#func, __FILE__, __LINE__));

void glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glLogError(const char* functionName, const char* filename, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGl Error] " << std::endl <<
			"in function: " << functionName << std::endl <<
			"in file: " << filename << std::endl <<
			"in line: " << line << std::endl;
		return false;
	}
	return true;
}

unsigned int CompileShader(GLenum type, const std::string& source)
{
	glVerify(unsigned int id = glCreateShader(type));
	const char* cSource = source.c_str();
	glVerify(glShaderSource(id, 1, &cSource, nullptr));
	glVerify(glCompileShader(id));
	return id;
}



unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	glVerify(unsigned int program = glCreateProgram());
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	glVerify(glAttachShader(program, vertexShader));
	glVerify(glAttachShader(program, fragmentShader));
	glVerify(glLinkProgram(program));
	glVerify(glValidateProgram(program));

	glVerify(glDeleteShader(vertexShader));
	glVerify(glDeleteShader(fragmentShader));

	return program;
}

std::string ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream sourceStream;
	while (std::getline(stream, line))
	{
		sourceStream << line << "\n";
	}

	return sourceStream.str();
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing glew library!" << std::endl;
	}
	else
	{
		std::cout << "Glew initialized successfully!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] =
	{
		-0.5f, -0.5f, // 0
		0.3f, -0.3f, // 1
		0.3f, 0.3f, // 2
		-0.5f, 0.5f // 3
	};

	unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vertexBuffer;
	glVerify(glGenBuffers(1, &vertexBuffer));
	glVerify(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	glVerify(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, positions, GL_STATIC_DRAW));

	glVerify(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
	glVerify(glEnableVertexAttribArray(0));

	unsigned int indexBuffer;
	glVerify(glGenBuffers(1, &indexBuffer));
	glVerify(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
	glVerify(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	std::string vertexShaderPath = "Shaders/VertexShader.glsl";
	std::string vertexShader = ParseShader(vertexShaderPath);


	// Pinky triangle!
	std::string fragmentShaderPath = "Shaders/FragmentShader.glsl";
	std::string fragmentShader = ParseShader(fragmentShaderPath);
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glVerify(glUseProgram(shader));

	glVerify(int uniformLocation = glGetUniformLocation(shader, "uniformColor"));
	if (uniformLocation == -1)
	{
		std::cout << "Error capturing uniform location for uniformColor";
	}
	else
	{
		glVerify(glUniform4f(uniformLocation, 0.0f, 0.47f, 0.84f, 1.0f));
	}

	float red = 0.0f;
	float offset = 0.01f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glVerify(glClear(GL_COLOR_BUFFER_BIT));
		glVerify(glUniform4f(uniformLocation, red, 0.47f, 0.84f, 1.0f));
		glVerify(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		if (red >= 1.0f)
		{
			offset = -0.01f;
		}
		else if (red <= 0.0f)
		{
			offset = 0.01f;
		}

		red += offset;

		/* Poll for and process events */
		glfwPollEvents();
	}

	glVerify(glDeleteShader(shader));
	glfwTerminate();
	return 0;
}