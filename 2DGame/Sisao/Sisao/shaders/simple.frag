#version 330

uniform vec4 color;
uniform vec4 arrow_color;

out vec4 outColor;
in vec2 Position;

void main()
{
	outColor = color;
	  if( Position.x < 350  || Position.x > 590)
    {
        outColor = arrow_color;
    }

}
