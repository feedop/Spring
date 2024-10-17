export module weight;

import std;

import <glad/glad.h>;
import <glm/mat4x4.hpp>;
import <glm/gtx/transform.hpp>;

import colors;
import glutils;
import object;

std::vector<glm::vec3> getVertices()
{
	return
	{
		{-0.5f, -0.5f, 0.0f},
		{ 0.5f, -0.5f, 0.0f},
		{ -0.5f, 0.5f, 0.0f},
		{ 0.5f, 0.5f, 0.0f}
	};
}

std::vector<unsigned int> getIndices()
{
	return
	{
		0, 1, 2,
		1, 2, 3
	};
}

export class Weight : public Object
{
public:
	Weight() : Object(getVertices(), getIndices(), colors::red)
	{}

	virtual void draw(const Shader* shader) const override
	{
		shader->setVector("color", color);
		shader->setMatrix("model", model * baseModel);
		ScopedBindArray ba(VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
	}
private:
	glm::mat4 baseModel = glm::scale(glm::mat4{ 1.0f }, {0.5f, 0.2f, 1.0f});
};