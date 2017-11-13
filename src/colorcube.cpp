#include "colorcube.h"


ColorCube::ColorCube()
{
	create_cube();
}

ColorCube::~ColorCube()
{

}


void ColorCube::create_cube() {


	glm::vec3 center = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 normal;
    
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    glm::vec3 v0 = glm::vec3(0);
    glm::vec3 v1 = glm::vec3(1,0,0);
    glm::vec3 v2 = glm::vec3(1,0,1);
    glm::vec3 v3 = glm::vec3(0,0,1);
    glm::vec3 v4 = glm::vec3(0,1,0);
    glm::vec3 v5 = glm::vec3(1,1,0);
    glm::vec3 v6 = glm::vec3(1,1,1);
    glm::vec3 v7 = glm::vec3(0,1,1);

    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);

    normal = normalize(v0 - center);
	normals.push_back(normal);
    normal = normalize(v1 - center);
	normals.push_back(normal);
    normal = normalize(v2 - center);
	normals.push_back(normal);
    normal = normalize(v3 - center);
	normals.push_back(normal);
    normal = normalize(v4 - center);
	normals.push_back(normal);
    normal = normalize(v5 - center);
	normals.push_back(normal);
    normal = normalize(v6 - center);
	normals.push_back(normal);
    normal = normalize(v7 - center);
	normals.push_back(normal);
	
    // f0
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    
    // f1
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);

    indices.push_back(5);
	indices.push_back(1);
	indices.push_back(0);

     // f2
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);

    indices.push_back(6);
    indices.push_back(2);
    indices.push_back(1);
    
     // f3
	indices.push_back(2);
	indices.push_back(6);
	indices.push_back(7);

    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(2);
    
     // f4
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(7);

    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(0);
    
     // f5
	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(6);

    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(4);


	// Model vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_STATIC_DRAW); // Give our vertices to OpenGL.
	glEnableVertexAttribArray(0); // 1rst attribute buffer : vertices
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float) * 3,  // stride
		(void*)0			// array buffer offset
	);

	// Model normals
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // 3rd attribute buffer : normals
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_TRUE,                         // normalized?
		sizeof(float) * 3,                // stride
		(void*)0                          // array buffer offset
	);

	// Model uv coords
	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, &uvs[0], GL_STATIC_DRAW); // Give our uvs to OpenGL.
	glEnableVertexAttribArray(2); // 2rst attribute buffer : uvs
	glVertexAttribPointer(
		2,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(float) * 2,  // stride
		(void*)0			// array buffer offset
	);

	// Model indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void ColorCube::draw() {

	//Draw Object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindVertexArray(VAO);

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glNormalPointer(GL_FLOAT, sizeof(glm::vec3), (void*)0);

    glPointSize(4.0);
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	glBindVertexArray(0);


}