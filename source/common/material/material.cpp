#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: (Req 7) Write this function

        pipelineState.setup(); // start setup for the pipelinestate
        shader->use();         // set the shader to be used here
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }
    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        Material ::setup();
        // setup for the tintedmaterial
        // shader->getUniformLocation();
        shader->set("tint", tint); // set the the uniform "tint" to value tint
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        TintedMaterial ::setup();                      // setup for the tintedmaterial
        shader->set("alphaThreshold", alphaThreshold); // set alpha to uniform
        if (texture != nullptr)
        {
            glActiveTexture(GL_TEXTURE0); // active texture unit
            texture->bind();              // set open glBindTexture(GL_TEXTURE_2D, texture);
        }
        if (sampler != nullptr)
        {
            sampler->bind(0); // set which texture 0,1,2
        }

        shader->set("tex", 0); // send the unit number to the tex variable
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    // void LitMaterial::setup() const
    // {
    //     // calling on the textured material setup function
    //     // since lit material inherits from textured material
    //     Material::setup();
    //     // going through the texture maps defined in the .hpp
    //     // binding the texture and sampler
    //     // and then setting the values in the shader file
    //     if (albedo)
    //     {
    //         glActiveTexture(GL_TEXTURE0);
    //         albedo->bind();
    //         if (sampler_albedo)
    //         {
    //             sampler_albedo->bind(0);
    //         }

    //         shader->set("material.albedo", 0);
    //     }
    //     if (specular)
    //     {
    //         glActiveTexture(GL_TEXTURE1);
    //         specular->bind();
    //         if (sampler_specular)
    //         {
    //             sampler_specular->bind(1);
    //         }
    //         shader->set("material.specular", 1);
    //     }
    //     if (ambient_occlusion)
    //     {
    //         glActiveTexture(GL_TEXTURE2);
    //         ambient_occlusion->bind();
    //         if (sampler_ambient_occlusion)
    //         {
    //             sampler_ambient_occlusion->bind(2);
    //         }
    //         shader->set("material.ambient_occlusion", 2);
    //     }
    //     if (roughness)
    //     {
    //         glActiveTexture(GL_TEXTURE3);
    //         roughness->bind();
    //         if (sampler_roughness)
    //         {
    //             sampler_roughness->bind(3);
    //         }

    //         shader->set("material.roughness", 3);
    //     }
    //     if (emissive)
    //     {
    //         glActiveTexture(GL_TEXTURE4);
    //         emissive->bind();
    //         if (sampler_emissive)
    //         {
    //             sampler_emissive->bind(4);
    //         }
    //         shader->set("material.emissive", 4);
    //     }
    //     glActiveTexture(GL_TEXTURE0);
    // }

    // // the deserializer function to read data from the json object
    // void LitMaterial::deserialize(const nlohmann::json &data)
    // {
    //     // calling on the textured material function
    //     Material::deserialize(data);

    //     if (!data.is_object())
    //     {
    //         return;
    //     }
    //     albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
    //     specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
    //     ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
    //     roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
    //     emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

    //     // at last get the sampler
    //     sampler_albedo = AssetLoader<Sampler>::get(data.value("sampleralbedo", ""));
    //     sampler_specular = AssetLoader<Sampler>::get(data.value("samplerspecular", ""));
    //     sampler_ambient_occlusion = AssetLoader<Sampler>::get(data.value("samplerambient", ""));
    //     sampler_roughness = AssetLoader<Sampler>::get(data.value("samplerroughness", ""));
    //     sampler_emissive = AssetLoader<Sampler>::get(data.value("sampleremissive", ""));
    // }

    void LitMaterial::setup() const
    {
        // calling on the textured material setup function
        // since lit material inherits from textured material
        Material::setup();
        // going through the texture maps defined in the .hpp
        // binding the texture and sampler
        // and then setting the values in the shader file
        if (albedo)
        {
            glActiveTexture(GL_TEXTURE0);
            albedo->bind();
            sampler->bind(0);
            shader->set("material.albedo", 0);
        }
        if (specular)
        {
            glActiveTexture(GL_TEXTURE1);
            specular->bind();
            sampler->bind(1);
            shader->set("material.specular", 1);
        }
        if (ambient_occlusion)
        {
            glActiveTexture(GL_TEXTURE2);
            ambient_occlusion->bind();
            sampler->bind(2);
            shader->set("material.ambient_occlusion", 2);
        }
        if (roughness)
        {
            glActiveTexture(GL_TEXTURE3);
            roughness->bind();
            sampler->bind(3);
            shader->set("material.roughness", 3);
        }
        if (emissive)
        {
            glActiveTexture(GL_TEXTURE4);
            emissive->bind();
            sampler->bind(4);
            shader->set("material.emissive", 4);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    // the deserializer function to read data from the json object
    void LitMaterial::deserialize(const nlohmann::json &data)
    {
        // calling on the textured material function
        Material::deserialize(data);

        if (!data.is_object())
        {
            return;
        }
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        // if(data.contains("albedo"))
        // {
        //    albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        // }
        // if(data.contains("specular"))
        // {
        //    specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        // }
        // if(data.contains("ambient_occlusion"))
        // {
        //    ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        // }
        // if(data.contains("roughness"))
        // {
        //    roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        // }
        // if(data.contains("emissive"))
        // {
        //    emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
        // }

        // at last get the sampler
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
}