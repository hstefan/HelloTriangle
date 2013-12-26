#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>

void errorCallback(int error, const char* msg)
{
	fputs(msg, stderr);
}

int main(int argc, char* argv [])
{
	const int wWidth = 1280, wHeight = 720;

	if (!glfwInit())
	{
		return -1;
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
			return -1;
		}

		if (!gl3wIsSupported(3, 2))
		{
			std::fprintf(stderr, "Context doesn't support OpenGL 3.2\n");
			return -1;
		}

		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
}