//#include "teapot.h"
//#include <kgfw/GLUtils.h>           // Include GLUtils for checkGLError
//#include "shader.h"
//#include <glm/gtx/transform.hpp>	// glm transform functions.
//#include <vector>
//#include "teapot_data.h"
//
//
//Teapot::Teapot()
//    : GameObject(__FUNCTION__) {
//	
//	// 1. Create VAO
//	glGenVertexArrays(1, &m_vao);
//	checkGLError();
//	glBindVertexArray(m_vao);
//	checkGLError();
//	glGenBuffers(1, &m_positionsVbo);
//	checkGLError();
//	glGenBuffers(1, &m_normalsVbo);
//	checkGLError();
//	glGenBuffers(1, &m_coordinatesVbo);
//	checkGLError();
//
//
//
//	// 2. Create VBO for positions. Use TEAPOT_POSITIONS -variable (declader in teapot_data.h) for positions data.
//	glBindBuffer(GL_ARRAY_BUFFER, m_positionsVbo);
//	checkGLError();
//	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_POSITIONS), TEAPOT_POSITIONS, GL_STATIC_DRAW);
//	checkGLError();
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	checkGLError();
//	glEnableVertexAttribArray(0);
//	checkGLError();
//
//
//	// 3. Create VBO for texture coordinates Use TEAPOT_TEXTURE_COORDINATES -variable (declader in teapot_data.h) 
//	//    for texture coordinates data.
//	glBindBuffer(GL_ARRAY_BUFFER, m_coordinatesVbo);
//	checkGLError();
//	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_TEXTURE_COORDINATES), TEAPOT_TEXTURE_COORDINATES, GL_STATIC_DRAW);
//	checkGLError();
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//	checkGLError();
//	glEnableVertexAttribArray(1);
//	checkGLError();
//
//	
//	
//	// 4. Create VBO for normals. Use TEAPOT_NORMALS variable (declader in teapot_data.h) 
//	//    for normals data.
//	glBindBuffer(GL_ARRAY_BUFFER, m_normalsVbo);
//	checkGLError();
//	glBufferData(GL_ARRAY_BUFFER, sizeof(TEAPOT_NORMALS), TEAPOT_NORMALS, GL_STATIC_DRAW);
//	checkGLError();
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	checkGLError();
//	glEnableVertexAttribArray(2);
//	checkGLError();
//
//}
//
//Teapot::~Teapot() {
//	// TODO: Delete created VAO and VBOs here.
//	// Delete Vertex Array Object
//	glDeleteVertexArrays(1, &m_vao);
//	checkGLError();
//	glDeleteBuffers(1, &m_positionsVbo);
//	checkGLError();
//	glDeleteBuffers(1, &m_coordinatesVbo);
//	checkGLError();
//	glDeleteBuffers(1, &m_normalsVbo);
//	checkGLError();
//	
//
//}
//
//void Teapot::render(Shader* shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint textureId) {
//	// Bind the shader
//		shader->bind();
//
//	// Set uniform values to the shader (MVP Matrix)
//		shader->setUniform("MVP", projectionMatrix * glm::inverse(viewMatrix) * getModelMatrix());
//		shader->setUniform("M", getModelMatrix());
//		
//
//   	
//	// Set texture uniform to the shader
//		if (textureId > 0) {
//			shader->setUniform("texture0", 0);
//			glActiveTexture(GL_TEXTURE0);
//			glBindTexture(GL_TEXTURE_2D, textureId);
//		}
//
//		shader->setUniform("lightColor", 1, 1, 1);
//		shader->setUniform("lightPos", 0, 1, 0);
//
//
//	// Bind VAO.
//		glBindVertexArray(m_vao);
//		checkGLError();
//
//	// Draw teapot using glDrawArrays with TEAPOT_NUM_VERTICES.
//		glDrawArrays(GL_TRIANGLES,0,TEAPOT_NUM_VERTICES);
//		checkGLError();
//
//}
//
