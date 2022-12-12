#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    //Added comment: therefore the matrix multiplication will look like this: the output transformation matrix=translation*rotation*scaling
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const 
    {
        //TODO: (Req 3) Write this function
        glm::mat4 identity_matrix =glm::mat4(1.0f); //the identity matrix we will use as input parameter to the transformation functions (has to be initialized to the identity matrix to be multiplied by the transformation vectors giving a correct transformation matrix)

        //added comment: 1- constructing the scaling matrix
        glm::mat4 scaling_matrix = glm::scale(identity_matrix, scale); //scale is a vector defined to hold the scale factors
        //2- constructing the translation matrix
        glm::mat4 translation_matrix = glm::translate(identity_matrix, position);//translate also takes the identity matrix and the translation vector (position that is already defined)
        //3- constructing the rotation matrix
        glm::mat4 rotation_matrix = glm::yawPitchRoll(rotation.y,rotation.x,rotation.z); //this function takes the euler angles (in rotation vector that is already defined), and creates a rotation matrix from these angles (yaw: y, pitch: x, roll: z)


        //we should return the output transformation matrix which is the multiplication of the above three matrices
        return translation_matrix*rotation_matrix*scaling_matrix;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}