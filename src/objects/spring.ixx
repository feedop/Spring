export module spring;

import std;

import <glad/glad.h>;
import <glm/mat4x4.hpp>;

import colors;
import glutils;
import object;

std::vector<glm::vec3> getVertices()
{
	return
	{
		{0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f}
	};
}

std::vector<unsigned int> getIndices()
{
	return
	{
		0,
		1
	};
}

export class Spring : public Object
{
public:
	Spring() : Object(getVertices(), getIndices(), colors::yellow)
	{}

	virtual void draw(const Shader* shader) const override
	{
		shader->setVector("color", color);
		shader->setMatrix("model", model);
		ScopedLineWidth(5.0f);
		ScopedBindArray ba(VAO);
		glDrawElements(GL_LINES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
	}
};