#version 330

///////////////////////////////////UNIFORMS////////////////////////////
//uniforms are values that will be set in our cpp code
//and then they will be sent to the shader


//The eye vector is the position of the camera
uniform vec3 eye; 

//the VP matrix is the view point matrix 
//Which = the projection matrix * the camera view matrix
//which when multiplied by a point
//will transform it to its final location on the screen
//in the NDC space
uniform mat4 VP;

//M is the model matrix 
//which transforms the object from local to world
uniform mat4 M;

//M inverse transpose is the inverse transpose of the model matrix
//used to transform the normal to the object 
//since non uniform scaling messes with the normal vectors
uniform mat4 M_IT;



////////////////////////////////////////////////////////////////////

//describing where each vertex attribute is stored in the Vertex buffer object
//the VAO (vertex array object), only stores where is who
//putting every location in a variable that describes the attribute

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

//the texture coordinate is the point on the texture 
//where our vertex will map from
//it is a 2d vector where the texture image is uv space
layout(location=2) in vec2 tex_coord;

layout(location=3) in vec3 normal;

////////////////////////////////////Varyings////////////////////////////

//varyings are out from a shader and in to another shader
//here we are sending the info about the vertex 
//from the vertex shader
//to the fragment shader

out Varyings 
{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;


//setting all the varying data
//to send to the vertex shader
void main()
{
    //to get the world coordinate of the vertex, 
    //multiply the local vertex position by the the model matrix to transform it to the world coordinates
    vec3 world = (M * vec4(position, 1.0)).xyz;

    //the final position of the vertex is going to be 
    //the view point matrix multiplied by the world matrix 
    gl_Position = VP * vec4(world, 1.0);
    fs_in.color = color;

    fs_in.tex_coord = tex_coord;

    //transforming the normal vector of the vertex
    //using the model matrix inverse transpose for non uniform scaling
    //for uniform scaling, no difference between the the M and M_IT
    //normalize the vector 
    //it retains the direction of the normal and makes its length
    fs_in.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);

    //the view matrix is the direction vector 
    //from the eye (camera) to the world (object)
    fs_in.view = eye - world;

    fs_in.world = world;
}
