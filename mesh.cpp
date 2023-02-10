#include "mesh.h"
#include <kgfw/GLUtils.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <string>
#include <stb_image.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	: GameObject(__FUNCTION__)
{
	m_indiceCount = indices.size();
	// create buffers/arrays
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}

Mesh::~Mesh(){}


void Mesh::render(Shader* shader, glm::vec3 lightPos, glm::vec3 viewPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint textureId)
{
	// Bind the shader
	shader->bind();

	// Set uniform values to the shader

	// MVP Matrix
	shader->setUniform("MVP", projectionMatrix * glm::inverse(viewMatrix) * getModelMatrix());
	checkGLError();

	// Model matrix
	shader->setUniform("M", getModelMatrix());
	checkGLError();

	// lightPos vector
	shader->setUniform("lightPos", lightPos.x, lightPos.y, lightPos.z);
	checkGLError();

	// lightPos vector
	shader->setUniform("viewPos", viewPos.x, viewPos.y, viewPos.z);

	shader->setUniform("lightColor", 1.0f, 1.0f, 1.0f);
	checkGLError();

	// Set texture, if set
	if (textureId > 0) {
		shader->setUniform("texture0", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);

}


Mesh* Mesh::processMesh(aiMesh* mesh)
{
	// add data containers for vertices and indices
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex; //temporable container for the data of each loop
		glm::vec3 vector;

		// load data from the Assimp mesh to our containers
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// Now retrieve the corresponding vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	// return with new Mesh of our own
	return new Mesh(vertices, indices);
}

void Mesh::processNode(std::vector<Mesh*>* meshes, aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		// the node object only contains indices to index the actual objects in the scene.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes->push_back(processMesh(mesh));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(meshes, node->mChildren[i], scene);
	}
}

std::vector<Mesh*> Mesh::loadMeshes(const std::string& path)
{
	// create the container that will be returned by this function
	std::vector<Mesh*> meshes; // it should at that point contain all the loaded meshes

	// read file with Assimp
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// Check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("Error loading model file \"%s\": \"%s\" ", path.c_str(), importer.GetErrorString());
		return meshes;
	}
	// retrieve the directory path of the filepath
	std::string dir = path.substr(0, path.find_last_of('/'));

	// process Assimp's root node recursively
	processNode(&meshes, scene->mRootNode, scene);

	return meshes;
}
