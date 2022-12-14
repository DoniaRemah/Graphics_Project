#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
     float col , row , total;
    col = floor((gl_FragCoord.x / size)) ; // calc if col is even or odd
    row = floor((gl_FragCoord.y / size)); //calc if row is even or odd
    total = col + row; 
    float isEven = mod(total,2); // even + odd = odd else even
    if(isEven == 0) // check if total is even or odd
    {frag_color = vec4(colors[0], 1.0);}
    else
    {frag_color = vec4(colors[1], 1.0);}
}