export module object;

import std;

import <glad/glad.h>;
import <glm/mat4x4.hpp>;

import glutils;
import shader;

export class Object
{
public:
	virtual void draw(const Shader* shader) const = 0;
	
	inline void setModel(const glm::mat4& newModel)
	{
		model = newModel;
	}

	virtual ~Object()
	{
		if (VAO == 0)
			return;

		ScopedBindArray ba(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glDeleteBuffers(1, &VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDeleteBuffers(1, &EBO);

		glDeleteVertexArrays(1, &VAO);
	}

protected:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	glm::mat4 model{ 1.0f };
	glm::vec4 color;
	size_t vertexCount;
	size_t indexCount;

	Object(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices, const glm::vec4& color) : vertexCount(vertices.size()), indexCount(indices.size()), color(color)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		ScopedBindArray ba(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	}
	
	Object(const Object& other) = delete;
	Object& operator=(const Object& other) = delete;

};