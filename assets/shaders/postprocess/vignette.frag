#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?

    //NDC space for texture is scaling each pixel by 2 then translating it by -1

    vec4 scene_color = texture(tex,tex_coord);
    vec2 tex_coord_NDS = (tex_coord*2) -1;
    float 2D_Length = (tex_coord_NDS.x * tex_coord_NDS.x) + (tex_coord_NDS.y * tex_coord_NDS.y);

    vec4 vignette = scene_color / (1 + 2D_Length);

    frag_color = vignette;    
}