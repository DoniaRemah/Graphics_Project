#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        //TODO: (Req 8) Write this function

       glm::mat4 local_world = this->localTransform.toMat4(); // Getting entity Transformation Matrix 
       
       Entity* Curr_parent = this->parent; // getting this entity's parent
    
       while(Curr_parent !=nullptr){ // Combining matrices till root
            glm::mat4 parent_transform = Curr_parent->localTransform.toMat4();
            local_world =  parent_transform * local_world;
            Curr_parent = Curr_parent->parent;
       }

        return local_world;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}