#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <fstream>

struct AppData
{
	GLuint program;
	GLuint pbo;
	GLuint vao;
	GLuint vshader;
	GLuint fshader;
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

	glUseProgram(appData->program);
	glBindBuffer(GL_ARRAY_BUFFER, appData->pbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
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

GLuint createShader(GLenum shaderType, const char* filename)
{
	auto shader = glCreateShader(shaderType);
	
	std::ifstream file(filename);
	std::string content(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>())
	);
	const char* rawContent = content.c_str();
	glShaderSource(shader, 1, &rawContent, nullptr);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
		GLchar* logMsg = new GLchar[infoLength + 1];
		glGetShaderInfoLog(shader, infoLength, nullptr, logMsg);
		fprintf(stderr, "Failed compiling %s shader: %s\n", filename, logMsg);
		delete[] logMsg;
	}
	return shader;
}

void initApplication(GLFWwindow* window)
{
	appData = new AppData();
	
	appData->program = glCreateProgram();
	appData->vshader = createShader(GL_VERTEX_SHADER, "res/vertex.glsl");
	appData->fshader = createShader(GL_FRAGMENT_SHADER, "res/fragment.glsl");

	glAttachShader(appData->program, appData->vshader);
	glAttachShader(appData->program, appData->fshader);
	glLinkProgram(appData->program);

	GLint status;
	glGetProgramiv(appData->program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(appData->program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* msg = new GLchar[logLength + 1];
		glGetProgramInfoLog(appData->program, logLength, nullptr, msg);
		std::fprintf(stderr, "Link failure: %s\n", msg);
		delete[] msg;
	}
	
	glDetachShader(appData->program, appData->vshader);
	glDetachShader(appData->program, appData->fshader);

	const float vertexPositions[] = 
		{
			0.75f, 0.75f, 0.0f, 1.0f,
			0.75f, -0.75f, 0.0f, 1.0f,
			-0.75f, -0.75f, 0.0f, 1.0f,
		};

	glGenBuffers(1, &appData->pbo);
	glBindBuffer(GL_ARRAY_BUFFER, appData->pbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &appData->vao);
	glBindVertexArray(appData->vao);
}

int main(int argc, char* argv [])
{
	GLFWwindow* window = initContext();
	if (window)
	{
		initApplication(window);
		eventLoop(window);
	}
	glfwTerminate();
}