#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <stdio.h>	
#include <kgfw/Object.h>
#include <vector>			// Include vector
#include <glad/gl.h>		// Include glad
#include <glm/glm.hpp>      // Include glm
#include "shader.h"			// Include Shader-class.
#include "camera.h"			// Include Camera-class.
#include "texture.h"		// Include Texture-class.
#include "mesh.h"			// Include Mesh-class.
#include <GLFW/glfw3.h>
#include <kgfw/GLUtils.h>
#include <glm/gtx/transform.hpp>	// glm transform functions.
#include <stb_image.h>


class Application : public kgfw::Object
{
public:
	
	Application(int width, int height);

	~Application();

	void Meshes();
	void Textures();

	void render(GLFWwindow* window);

	void update(GLFWwindow* window,float deltaTime);

	bool getKeyState(GLFWwindow* window,float keyCode);

private:

	std::vector<Texture*> m_texture;
	Camera* m_camera; // Camera
	Mesh* m_mesh; 
	std::vector<Mesh*> m_meshes; 
	Shader* m_shader;		// Pointer to the Shader object
	std::vector<glm::vec3> path;
	GLFWwindow* m_window;
};