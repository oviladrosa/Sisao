#include "TextBlock.h"

TextBlock::TextBlock(float x, float y, float height, float width, ShaderProgram &program) {
	this->width = width;
	this->height = height;
	text = "";
	color = glm::vec3(120.f, 0.f, 0.f);
	position = glm::vec2(250.f, 250.f);
	float vertices[12] = { x, y, x + width, y, x + width, y + height, x, y, x + width, y + height, x, y + height };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 2* sizeof(float), 0);
}

TextBlock *TextBlock::CreateTextBlock(float x, float y,float height, float width, ShaderProgram &program) {
	TextBlock* tb = new TextBlock(x,y,height, width, program);
	return tb;
}

TextBlock::~TextBlock() {

}

void TextBlock::SetText(std::string text) {
	this->text = text;
}

void TextBlock::SetColorBlock(glm::vec3 color) {
	this->color = color;
}

void TextBlock::SetPosition(glm::vec2 pos) {
	this->position = pos;
}

void TextBlock::Draw() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}