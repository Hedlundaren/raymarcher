#include "marchingmesh.h"

MarchingMesh::MarchingMesh(Volume volData, glm::ivec3 gridRes, float *isoVal)
	: gridResolution(gridRes), data(volData), isoValue(isoVal)
{
	createSurface();
}

void MarchingMesh::createTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
	//normal = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back(vertices.size() - 3);
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);

	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
}

void MarchingMesh::createQuad(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
	//normal = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices.push_back(vertices.size() - 4); // v0
	indices.push_back(vertices.size() - 3); // v1
	indices.push_back(vertices.size() - 2); // v2

	indices.push_back(vertices.size() - 4); // v0
	indices.push_back(vertices.size() - 2); // v2
	indices.push_back(vertices.size() - 1); // v3

	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
}



void MarchingMesh::createSurface()
{
	vertices.clear();
	indices.clear();
	normals.clear();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// createQuad(glm::vec3(0, 0, 0), glm::vec3(0.0, 0.5, 0.0), glm::vec3(0.0, 0.5, 0.5), glm::vec3(0.0, 0, 0.5));
	// createTriangle(glm::vec3(0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0));

	glm::vec3 cubeSize = glm::vec3(1.0 / gridResolution.x, 1.0 / gridResolution.y, 1.0 / gridResolution.z);

	data.bindBuffer();
	for (int z = 0; z < gridResolution.z; z++)
	{
		for (int y = 0; y < gridResolution.y; y++)
		{
			for (int x = 0; x < gridResolution.x; x++)
			{

				glm::vec3 v[8];

				v[0].x = (float)x / gridResolution.x - 0.5f;
				v[0].y = (float)y / gridResolution.y - 0.5f;
				v[0].z = (float)z / gridResolution.z - 0.5f;

				v[1] = v[0] + glm::vec3(0, cubeSize.y, 0);
				v[2] = v[0] + glm::vec3(cubeSize.x, cubeSize.y, 0);
				v[3] = v[0] + glm::vec3(cubeSize.x, 0, 0);
				v[4] = v[0] + glm::vec3(0, 0, cubeSize.z);
				v[5] = v[0] + glm::vec3(cubeSize.x, 0, cubeSize.z);
				v[6] = v[0] + glm::vec3(cubeSize.x, cubeSize.y, cubeSize.z);
				v[7] = v[0] + glm::vec3(0, cubeSize.y, cubeSize.z);


				bool allEmpty = true;
				// bool allFilled = true;
				for (auto vert : v)
				{
					if (data.readData(vert) > *isoValue)
					{
						allEmpty = false;
					}
					else
					{
						//allFilled = false;
					}
				}

				if (!allEmpty)
				{
					createQuad(v[0], v[1], v[2], v[3]);
					createQuad(v[0], v[3], v[5], v[4]);
					createQuad(v[3], v[2], v[6], v[5]);
					createQuad(v[2], v[1], v[7], v[6]);
					createQuad(v[0], v[4], v[7], v[1]);
					createQuad(v[4], v[5], v[6], v[7]);
				}
			}
		}
	
		float percentage = 100.0 * z / gridResolution.z;
		std::cout << (percentage) << "% \r";
	}

	// Model vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_STATIC_DRAW); // Give our vertices to OpenGL.
	glEnableVertexAttribArray(0);																	  // 1rst attribute buffer : vertices
	glVertexAttribPointer(
		0,				   // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,				   // size
		GL_FLOAT,		   // type
		GL_FALSE,		   // normalized?
		sizeof(float) * 3, // stride
		(void *)0		   // array buffer offset
	);

	// Model normals
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // 3rd attribute buffer : normals
	glVertexAttribPointer(
		1,				   // attribute
		3,				   // size
		GL_FLOAT,		   // type
		GL_TRUE,		   // normalized?
		sizeof(float) * 3, // stride
		(void *)0		   // array buffer offset
	);

	// Model uv coords
	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, &uvs[0], GL_STATIC_DRAW); // Give our uvs to OpenGL.
	glEnableVertexAttribArray(2);															// 2rst attribute buffer : uvs
	glVertexAttribPointer(
		2,				   // attribute 1. No particular reason for 0, but must match the layout in the shader.
		2,				   // size
		GL_FLOAT,		   // type
		GL_FALSE,		   // normalized?
		sizeof(float) * 2, // stride
		(void *)0		   // array buffer offset
	);

	// Model indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void MarchingMesh::draw()
{

	//Draw Object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindVertexArray(VAO);

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glNormalPointer(GL_FLOAT, sizeof(glm::vec3), (void *)0);

	glDrawElements(
		GL_TRIANGLES,	// mode
		indices.size(),  // count
		GL_UNSIGNED_INT, // type
		(void *)0		 // element array buffer offset
	);

	glBindVertexArray(0);
}