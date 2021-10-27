#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Text.h"

class TextBlock {
public:
	TextBlock(float x, float y, float height, float width, ShaderProgram& program);
	~TextBlock();

	TextBlock* CreateTextBlock(float x, float y, float height, float width, ShaderProgram& program);
	void SetText(std::string text);
	void SetColorBlock(glm::vec3 color);
	void SetPosition(glm::vec2 pos);
	void Draw();

private:
	float width;
	float height;
	float x;
	float y;
	int totalVertices;
	std::string text;
	glm::vec3 color;
	glm::vec2 position;
	GLuint vao;
	GLuint vbo;
	GLint posLocation;
};
