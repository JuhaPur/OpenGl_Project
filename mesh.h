#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "gameobject.h"
#include "texture.h"
#include "shader.h"
#include <vector>


struct Vertex{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;};

class Shader;

class Mesh : public GameObject {
public:
	Mesh(const std::vector< Vertex >& vertices, const std::vector< unsigned int >& indices);
	~Mesh();

	void render(Shader* shader, glm::vec3 lightPos, glm::vec3 viewPos, const glm::mat4& viewMatrix = glm::mat4(1.0f), const glm::mat4& projectionMatrix = glm::mat4(1.0f), GLuint textureId = 0);
	std::vector<Mesh*> loadMeshes(const std::string& path);

	Mesh* processMesh(aiMesh* mesh);
	void processNode(std::vector<Mesh*>* meshes, aiNode* node, const aiScene* scene);
private:

	GLuint m_VAO;
	GLuint m_EBO;
	GLuint m_VBO;
	size_t m_indiceCount;
	
};