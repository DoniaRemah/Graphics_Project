#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

out Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} vs_out;

uniform mat4 transform;

void main()
{
    //TODO: (Req 3) Change the next line to apply the transformation matrix
    //added comment: applying the transformation matrix means multiplying the transformation matrix we created in transform.cpp to the position of the point that we want to apply that transformation to
    //the homogeneous coordinate of the position vector above is obtained by adding 1 to its w component
    vec4 homogeneous_position = vec4(position, 1.0);
    gl_Position = transform*homogeneous_position;
    //position=transform*homogeneous_position; ?????


    // No need to change any of the following lines
    vs_out.position = position;
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    vs_out.normal = normal;
}