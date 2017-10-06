#include "boundingcube.h"

BoundingCube::BoundingCube()
{
	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // Positions   // TexCoords
								 0.0f,  0.0f,  0.0f,
								 1.0f,  0.0f,  0.0f,
								 
								 0.0f,  1.0f,  0.0f,
								 1.0f,  1.0f,  0.0f,

								 0.0f,  0.0f,  1.0f,
								 1.0f,  0.0f,  1.0f,

								 0.0f,  1.0f,  1.0f,
								 1.0f,  1.0f,  1.0f,

								 // Z
								 0.0f,  0.0f,  0.0f,
								 0.0f,  0.0f,  1.0f,

								 1.0f,  0.0f,  0.0f,
								 1.0f,  0.0f,  1.0f,

								 0.0f,  1.0f,  0.0f,
								 0.0f,  1.0f,  1.0f,

								 1.0f,  1.0f,  0.0f,
								 1.0f,  1.0f,  1.0f,

								 // Y
								 0.0f,  0.0f,  0.0f,
								 0.0f,  1.0f,  0.0f,

								 1.0f,  0.0f,  0.0f,
								 1.0f,  1.0f,  0.0f,

								 0.0f,  0.0f,  1.0f,
								 0.0f,  1.0f,  1.0f,

								 1.0f,  0.0f,  1.0f,
								 1.0f,  1.0f,  1.0f,



	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}


BoundingCube::~BoundingCube()
{
}

void BoundingCube::draw() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBindVertexArray(quadVAO);
	glPointSize(4.0);
	glLineWidth(4.0);
	glDrawArrays(GL_LINES, 0, 32);
	glBindVertexArray(0);
}