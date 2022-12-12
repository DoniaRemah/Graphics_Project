#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false; //culling option is set to false by default
            GLenum culledFace = GL_BACK; //cull (discard the back faces)
            GLenum frontFace = GL_CCW;  // the front face's winding order is gonna be counter clock-wise
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;  //this means that the fragment passes if its depth value is less than or equal to the stored depth value in the buffer
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const 
        {
            //TODO: (Req 4) Write this function

            //1- face culling
            if (faceCulling.enabled == false) //the default value is that we don't want this option to begin with
            {
                glDisable(GL_CULL_FACE); //disable the cull face option
            }
            else //we want to enable this option
            {
                glEnable(GL_CULL_FACE); 
                glFrontFace(faceCulling.frontFace); //specifying that the front face that will be rendered is considered to be the face whose vertices have counter clockwise winding
                glCullFace(faceCulling.culledFace); //specifying that the face we want to cull is the back face
                
            }



            //2- depth testing and depth mask
            if (depthTesting.enabled == false) //default is that we don't want this option
            {
                glDisable(GL_DEPTH_TEST); //disabling the depth test option

            }
            else //we want to enable this option
            {
                glEnable(GL_DEPTH_TEST); //enable depth testing
                glDepthFunc(depthTesting.function); //setting the function that will compare the depth value of the fragment to the values in the buffer to be the one specified in the struct
                glDepthMask(depthMask); //Setting the depth mask to true means that the depth buffer is updated each frame with new depth values (the depth buffer can be written)

            }
            //3- blending
            //4- color mask 

    
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}