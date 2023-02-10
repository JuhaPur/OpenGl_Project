#pragma once
#include "shader.h"
#include "application.h"
#include "gameobject.h"
#include <kgfw/GLUtils.h>
// Include STB-image library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Application::Application(int width, int heigth) 
	: Object(__FUNCTION__) {
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
		"	float ambientStrength = 0.03;\n"
		"	vec3 ambient = ambientStrength * lightColor;\n"

		"	vec3 lightDir = normalize(lightPos - fragPos);\n"
		"	vec3 norm = normalize(normal);\n"

		"	float diff = max(dot(norm, lightDir), 0.0);\n"
		"	vec3 diffuse = diff * lightColor;\n"

		"   vec4 objectColor = texture2D(texture0, texCoord);\n"

		"	float specularStrength = 1.5;\n"
		"	vec3 viewDir = normalize(viewPos - fragPos);\n"
		//"	vec3 reflectDir = reflect(-lightDir, norm);\n"

		"	vec3 halfwayVec = normalize(viewDir + lightDir);\n"
		"	float spec = pow(max(dot(norm, halfwayVec), 0.0), 16);\n"

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
	
	Textures();
	Meshes();
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
}

Application::~Application() {
	// Delete shader
	delete m_shader;
	m_shader = 0;

	// Delete Camera
	delete m_camera;
	m_camera = 0;

	// Delete textures
	for (auto texture : m_texture) {
		delete texture;
	}

	//Delete meshes
	for (auto mesh : m_meshes) {
		delete mesh;
	}

}

void Application::Textures() {
	// Load the data for our texture using stb-image stbi_load-function
	int width, height, nrChannels;
	GLubyte* data = stbi_load("../photos/wood3.png", &width, &height, &nrChannels, 0);
	m_texture.push_back(new Texture(width, height, nrChannels, data));

	data = stbi_load("../photos/Brick.png", &width, &height, &nrChannels, 0);
	m_texture.push_back(new Texture(width, height, nrChannels, data));

	data = stbi_load("../photos/checkerboard.png", &width, &height, &nrChannels, 0);
	m_texture.push_back(new Texture(width, height, nrChannels, data));

	data = stbi_load("../photos/sky.png", &width, &height, &nrChannels, 0);
	m_texture.push_back(new Texture(width, height, nrChannels, data));

	data = stbi_load("../photos/grass00.png", &width, &height, &nrChannels, 0);
	m_texture.push_back(new Texture(width, height, nrChannels, data));
}

void Application::Meshes()
{
	auto meshes = m_mesh->loadMeshes("../Objects/radio.obj");
	m_meshes.push_back(meshes[0]);
	m_meshes[0]->setScaling(glm::vec3(0.050f));
	m_meshes[0]->setRotationX(-3.14 / 2.0f);
	m_meshes[0]->setRotationZ(90.0f);
	m_meshes[0]->setPosition(glm::vec3(-0.5f, -0.5f, 0.0f));

	meshes = m_mesh->loadMeshes("../Objects/torusknot.obj");
	m_meshes.push_back(meshes[0]);
	m_meshes[1]->setScaling(glm::vec3(0.02f));
	m_meshes[1]->setPosition(glm::vec3(-0.5f, -0.05f, -1.0f));

	meshes = m_mesh->loadMeshes("../Objects/suzanne.obj");
	m_meshes.push_back(meshes[0]);
	m_meshes[2]->setScaling(glm::vec3(0.03f));
	m_meshes[2]->setPosition(glm::vec3(0.5f, -0.2f, 0.0f));

	meshes = m_mesh->loadMeshes("../Objects/cylinder.obj");
	m_meshes.push_back(meshes[0]);
	m_meshes[3]->setScaling(glm::vec3(0.03f));
	m_meshes[3]->setPosition(glm::vec3(0.5f, 0.0f, -1.0f));

	meshes = m_mesh->loadMeshes("../Objects/grass.obj");
	m_meshes.push_back(meshes[0]);
	m_meshes[4]->setScaling(glm::vec3(0.015f));
	m_meshes[4]->setRotationX(-3.14 / 2);
	m_meshes[4]->setRotationZ(90.0f);
	m_meshes[4]->setPosition(glm::vec3(0.0f, -0.63f, -1.0f));
}




void Application::render(GLFWwindow* window) {
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

	m_meshes[0]->render(m_shader, glm::vec3(1.0f), m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture[0]->getTextureId());
	m_meshes[1]->render(m_shader, glm::vec3(1.0f), m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture[1]->getTextureId());
	m_meshes[2]->render(m_shader, glm::vec3(1.0f), m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture[2]->getTextureId());
	m_meshes[3]->render(m_shader, glm::vec3(1.0f), m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture[3]->getTextureId());
	m_meshes[4]->render(m_shader, glm::vec3(1.0f), m_camera->getPosition(), m_camera->getModelMatrix(), m_camera->getProjectionMatrix(), m_texture[4]->getTextureId());
}

bool Application::getKeyState(GLFWwindow* window,float keyCode)
{
	return glfwGetKey(window, keyCode);
}

void Application::update(GLFWwindow* window, float deltaTime) {
	//movement with WASD and UP & DOWN KEYS
	float deltaX = getKeyState(window, GLFW_KEY_D) - getKeyState(window, GLFW_KEY_A);
	float deltaZ = getKeyState(window, GLFW_KEY_S) - getKeyState(window, GLFW_KEY_W);
	float deltaY = getKeyState(window, GLFW_KEY_UP) - getKeyState(window, GLFW_KEY_DOWN);

	float deltaV = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
	float deltaN = (deltaX + deltaY + deltaZ) / deltaV;

	glm::vec3 delta = deltaTime * glm::vec3(deltaX / deltaV, deltaY / deltaV, deltaZ / deltaV);

	if (deltaV != 0) {
		m_camera->setPosition(m_camera->getPosition() + delta);
	}
}