#include <stdio.h>			// Include stdio.h, which contains printf-function
#include <kgfw/Object.h>	// Include kgfw::Object
#include <glad/gl.h>		// Include glad
#include <GLFW/glfw3.h>		// Include glfw
#include <kgfw/GLUtils.h>	// Include GLUtils for checkGLError
#include "shader.h"			// Include Shader-class.
#include <vector>			// Include std::vector
#include "camera.h"			// Include Camera-class.
#include "teapot.h"			// Include Teapot-class.
#include "texture.h"		// Include Texture-class.
// Include STB-image library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gameobject.h"

class Application : public kgfw::Object
{
public:
	Application()
		: Object(__FUNCTION__)
		, m_shader(0) {
		const char* vertexShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 in_position;\n"
			"layout (location = 1) in vec2 in_texCoord;\n"
			"layout (location = 2) in vec3 in_normal;\n"
			"uniform mat4 MVP;\n"
			"uniform mat4 M;\n"
			"out vec3 normal;\n"
			"out vec3 fragPos;\n"
			"out vec2 texCoord;\n"
			"void main()\n"
			"{\n"
			"   normal = mat3(transpose(inverse(M))) * in_normal;\n"
			"   texCoord = in_texCoord;\n"
			"   gl_Position = MVP * vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
			"	fragPos = vec3(M * vec4(in_position, 1.0));\n"
			"}";

		const char* fragmentShaderSource =
			"#version 330 core\n"
			"uniform sampler2D texture0;\n"
			"uniform vec3 lightColor;\n"
			"uniform vec3 lightPos;\n"
			"uniform vec3 viewDir;\n"
			"uniform vec3 viewPos;\n"
			"uniform mat4 M;\n"
			"in vec2 texCoord;\n"
			"in vec3 normal;\n"
			"out vec4 FragColor;\n"
			"in vec3 fragPos;\n"

			"void main()\n"
			"{\n"
			"	float ambientStrength = 0.1;\n"
			"	vec3 ambient = ambientStrength * lightColor;\n"

			"	vec3 lightDir = normalize(lightPos - fragPos);\n"
			"	vec3 norm = normalize(normal);\n"

			"	float diff = max(dot(norm, lightDir), 0.0);\n"
			"	vec3 diffuse = diff * lightColor;\n"

			"   vec4 objectColor = texture2D(texture0, texCoord);\n"

			"	float specularStrength = 0.8;\n"
			"	vec3 viewDir = normalize(viewPos - fragPos);\n"
			"	vec3 reflectDir = reflect(-lightDir, norm);\n"

			"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);\n"

			"	vec3 specular = specularStrength * spec * lightColor;\n"

			"	vec3 result = vec3(ambient + diffuse) * objectColor.rgb + specular;\n"
			"	FragColor = vec4(result, 1.0);\n"

			"}";

		// Build and compile our shader program
		m_shader = new Shader(vertexShaderSource, fragmentShaderSource);

		// Create perspective camera with screen size 640x480
		m_camera = new Camera(glm::pi<float>() / 2, 640, 480, 0.1f, 100.0f);
		// Set camera transform (view transform)
		m_camera->setPosition(glm::vec3(0.0f, 0.4f, 0.4f));
		m_camera->setLookAt(glm::vec3(0.0f, 0.0f, -0.5f));


		m_teapots.push_back(new Teapot());
		m_teapots.push_back(new Teapot());
		m_teapots.push_back(new Teapot());
		m_teapots.push_back(new Teapot());

		m_teapots[0]->setPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
		m_teapots[1]->setPosition(glm::vec3(-0.5f, 0.0f, -1.0f));
		m_teapots[2]->setPosition(glm::vec3(0.5f, 0.0f, 0.0f));
		m_teapots[3]->setPosition(glm::vec3(0.5f, 0.0f, -1.0f));

		// Load the data for our texture using stb-image stbi_load-function
		int width, height, nrChannels;
		GLubyte* data = stbi_load("../photos/checkerboard.png", &width, &height, &nrChannels, 0);

		// Create texture
		m_texture = new Texture(width, height, nrChannels, data);

		// Enable depth buffering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// TODO: Try out different culling options
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		// glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CW);
		
	}

	~Application() {
		// Delete shader
		delete m_shader;
		m_shader = 0;

		// Delete teapot
		for (auto teapot : m_teapots)
		{
			delete teapot;
		}

		// Delete Camera
		delete m_camera;
		m_camera = 0;

		// Delete texture
		delete m_texture;
		m_texture = 0;
	}

	void render(GLFWwindow* window) {
		// Query the size of the framebuffer (window content) from glfw.
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Setup the opengl wiewport (i.e specify area to draw)
		glViewport(0, 0, width, height);
		checkGLError();
		// Set color to be used when clearing the screen
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		checkGLError();
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		checkGLError();

		for (auto teapot : m_teapots)
		{
			teapot->render(m_shader, m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture->getTextureId());
		}

	}

	void update(float deltaTime) {
		for (auto teapot : m_teapots)
		{
			teapot->setRotationZ(teapot->getRotationZ() + deltaTime);
		}

	}

private:
	Shader* m_shader;		// Pointer to the Shader object
	Camera* m_camera;		// Camera.
	//Teapot*						m_teapot;		// Teapot
	Texture* m_texture;
	std::vector<Teapot*>m_teapots;
};

// Global pointer to the application
Application* g_app = 0;



int main(void) {


	// Set c++-lambda as error call back function for glfw.
	glfwSetErrorCallback([](int error, const char* description) {
		fprintf(stderr, "Error %d: %s\n", error, description);
		});

	// Try to initialize glfw
	if (!glfwInit()) {
		return -1;
	}

	// Create window and check that creation was succesful.
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Set current context
	glfwMakeContextCurrent(window);
	// Load GL functions using glad
	gladLoadGL(glfwGetProcAddress);

	// Create application
	g_app = new Application();

	// Specify the key callback as c++-lambda to glfw
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		// Close window if escape is pressed by the user.
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		});

	// Get time using glfwGetTime-function, for delta time calculation.
	float prevTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		// Render the game frame and swap OpenGL back buffer to be as front buffer.
		g_app->render(window);
		glfwSwapBuffers(window);

		// Poll other window events.
		glfwPollEvents();

		// Compute application frame time (delta time) and update application
		float curTime = (float)glfwGetTime();
		float deltaTime = curTime - prevTime;
		prevTime = curTime;
		g_app->update(deltaTime);
	}

	// Delete application
	delete g_app;
	g_app = 0;

	// Destroy window
	glfwDestroyWindow(window);

	// Terminate glfw
	glfwTerminate();

	return 0;
}
