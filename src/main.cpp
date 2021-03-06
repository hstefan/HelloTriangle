#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <fstream>

#include "gl/Program.hpp"
#include "gl/Shader.hpp"
#include "gl/BufferObject.hpp"

struct AppData
{
	gl::Program<GLuint>* program;
	gl::Shader<GLuint>* vshader;
	gl::Shader<GLuint>* fshader;
	gl::BufferObject<GLuint>* pbo;
	GLuint vao;
};

AppData* appData;

static const int wWidth = 1280, wHeight = 720;

void errorCallback(int error, const char* msg)
{
	fputs(msg, stderr);
}

void reshape(GLFWwindow* window, int width, int height)
{
	std::fprintf(stdout, "Reshaping: %d x %d\n", width, height);
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

GLFWwindow* initContext()
{
	if (!glfwInit())
	{
		return nullptr;
	}

	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	auto window = glfwCreateWindow(wWidth, wHeight, "Hello Triangle", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (window)
	{
		if (gl3wInit())
		{
			std::fprintf(stderr, "Failed to initialize gl3w.\n");
			return nullptr;
		}

		if (!gl3wIsSupported(3, 2))
		{
			std::fprintf(stderr, "Context doesn't support OpenGL 3.2\n");
			return nullptr;
		}
		glfwSetFramebufferSizeCallback(window, reshape);
	}
	return window;
}

void render(GLFWwindow* window)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(appData->program->identifier());
	glBindBuffer(GL_ARRAY_BUFFER, appData->pbo->identifier());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 48);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

void eventLoop(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		render(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void initApplication(GLFWwindow* window)
{
	appData = new AppData();
	appData->program = new gl::Program<GLuint>();
	
	appData->vshader = new gl::Shader<GLuint>(GL_VERTEX_SHADER, "res/vertex.glsl");
	if (!appData->vshader->compile())
		fprintf(stderr, appData->vshader->infoLog().get());

	appData->fshader = new gl::Shader<GLuint>(GL_FRAGMENT_SHADER, "res/fragment.glsl");
	if (!appData->fshader->compile())
		fprintf(stderr, appData->fshader->infoLog().get());

	appData->program->attachShader(*appData->vshader);
	appData->program->attachShader(*appData->fshader);

	if (!appData->program->link())
		std::fprintf(stderr, "Link failure: %s\n", appData->program->infoLog());

	appData->program->detachShader(*appData->vshader);
	appData->program->detachShader(*appData->fshader);
	
	const float vertexData[] = {
		0.0f, 0.5f, 0.0f, 1.0f,
		0.5f, -0.366f, 0.0f, 1.0f,
		-0.5f, -0.366f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	appData->pbo = new gl::BufferObject<GLuint>(GL_STATIC_DRAW, vertexData,
		sizeof(vertexData));

	glGenVertexArrays(1, &appData->vao);
	glBindVertexArray(appData->vao);
}

int main(int argc, char* argv [])
{
	GLFWwindow* window = initContext();
	if (window)
	{
		initApplication(window);
		gl::Program<GLuint> prog;
		eventLoop(window);
	}

	delete appData->program;
	delete appData->fshader;
	delete appData->vshader;
	delete appData;

	glfwTerminate();
}