#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            // 2- vertex array:
            glGenVertexArrays(1, &VAO); // define vertex array which have all info to deal with data in buffers as vertices and send them to attributes
            glBindVertexArray(VAO);     // bind vertex array
            // 1-Vertex buffer:
            glGenBuffers(1, &VBO);              // define vertex buffer which have all data
            glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind vertex buffer using  GL_ARRAY_BUFFER because there are many types of buffers
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            // put data into the buffer (binded pointer,size of data , data pointer ,GL_STATIC_DRAW )
            //  location
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // enable vertex attribute array using location of the required attribute to get data
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);
            // attribute gets data of position from(ATTRIB_LOC_POSITION,reading 3 floats as it is vec3 , disable normalize , stride (step) to move equal vertex (if put as 0 step will be 3 floats which size of read data) , the offset to start from which is zero in positions)
            // color
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));
            // attribute gets data of color from(ATTRIB_LOC_COLOR,reading 4 unsigned bytes (0 - 255 ) as it is vec4 of 8unit(1byte) , enable normalize(values are from 0 to 255 convert them to be from 0 to 1 by division by 255) , stride (step) to move equal vertex , the offset to start from which is offset where color start in vertex)
            // if normalize disabled it will assume any value greater than 1 as 1 so when applying Barycentric Coordinates it will give ones for all channels & it will be white
            // tex coord //same as previous
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord)); // norm
            // normal //same as previous
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
            // 3-Element buffer:
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
            glBindVertexArray(0);
            // set elementCount
            elementCount = elements.size();
        }

        // this function should render the mesh
        void draw()
        {
            // TODO: (Req 2) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}