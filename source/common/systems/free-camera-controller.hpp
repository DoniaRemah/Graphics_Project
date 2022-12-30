#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/collisions.hpp"
#include "../components/player.hpp"
#include <iostream>
#include "../application.hpp"
#include <string>
#include <glm/gtx/hash.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            PlayerComponent *player = nullptr;
            bool upCollider = false;
            bool downCollider = false;
            bool leftCollider = false;
            bool rightCollider = false;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }

            for (auto entity : world->getEntities())
            {
                player = entity->getComponent<PlayerComponent>();
                if (player)
                    break;
            }

            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;

            if (!player)
                return;

            // Get the entity that we found via getOwner of camera(player) (we could use controller->getOwner())
            Entity *entity = player->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            glm::mat4 matrix = entity->localTransform.toMat4();
            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));
            glm::vec3 current_sensitivity = controller->positionSensitivity;

            // Get Collision Entity
            CollisionComponent *collision = nullptr;
            Entity *Collision_entity = nullptr;


            std:: string act_Collision_Type;
            Entity *actual_collision_entity = nullptr;

            float min_distance = INT_MAX - 1;
            float act_ball;
            float act_coliding;
            float mov;
            float distance;

            for (auto entity : world->getEntities())
            {
                collision = entity->getComponent<CollisionComponent>();
                if (!(collision))
                    continue;
                Collision_entity = collision->getOwner();
                std::string name = Collision_entity->name;
                glm::vec3 &objPosition = Collision_entity->localTransform.position;
                glm::vec3 &objScale = Collision_entity->localTransform.scale;
                bool collisionX = false;
                // collisionX = position of collision(center) + half width && position of palyer(center) + radius

                // check z or y ??? Z 90 %%%
                bool collisionY = false;

                std::string horiz = "h";
                std::string vertical = "v";

                // down direction
                mov = deltaTime * current_sensitivity.z;

                // for checks
                float act_ballx_right = position.x - 0.5;
                float act_colidingX_right = objPosition.x - objScale.x;
                float distance_in_right = act_ballx_right - act_colidingX_right;
                float act_ballx_left = position.x + 0.5;
                float act_colidingX_left = objPosition.x + objScale.x;

                float act_ball_top = position.z + 0.5;
                float act_ball_below = position.z - 0.5;

                float act_wall_top = objPosition.z + objScale.z;
                float act_wall_below = objPosition.z - objScale.z;

                // COLLIDE FROM RIGHT

                if (app->getKeyboard().isPressed(GLFW_KEY_D))
                {
                    act_coliding = objPosition.x + objScale.x;
                    act_ball = position.x - 0.5;

                    distance = act_ball - act_coliding;

                    if (act_ball >= act_coliding) // ball is left of wall
                    {
                        if (act_ball_top > act_wall_below && act_ball_below < act_wall_top)
                        {
                            if (min_distance > distance)
                            {
                                if(collision->getobstucaseType() == CollisionType::SCORE){
                                    act_Collision_Type = "score";
                                    actual_collision_entity = Collision_entity;
                                }
                                
                                min_distance = distance;
                            }
                        }
                    }
                }

                // COLLIDE FROM LEFT

                if (app->getKeyboard().isPressed(GLFW_KEY_A))
                {
                    act_coliding = objPosition.x - objScale.x;
                    act_ball = position.x + 0.5;

                    distance = act_coliding - act_ball;

                    if (act_ball <= act_coliding) // ball is right of wall
                    {
                        if (act_ball_top > act_wall_below && act_ball_below < act_wall_top)
                        {
                            if (min_distance > distance)
                            {
                                min_distance = distance;
                            }
                        }
                    }
                }

                // COLLIDE FROM ABOVE BALL

                if (app->getKeyboard().isPressed(GLFW_KEY_W))
                {
                    act_ball = position.z + 0.5;
                    act_coliding = objPosition.z - objScale.z;
                    distance = act_coliding - act_ball;

                    if (act_ball <= act_coliding) // ball is below of wall
                    {
                        if (act_ballx_left > act_colidingX_right && act_ballx_right < act_colidingX_left)
                        {
                            if (min_distance > distance)
                            {
                                min_distance = distance;
                            }
                        }
                    }
                }

                // COLLIDE FROM BELOW OF BALL
                if (app->getKeyboard().isPressed(GLFW_KEY_S))
                {
                    act_ball = position.z - 0.5;
                    act_coliding = objPosition.z + objScale.z;
                    distance = act_ball - act_coliding;

                    if (act_ball >= act_coliding) // ball is above of wall
                    {
                        if (act_ballx_left > act_colidingX_right && act_ballx_right < act_colidingX_left)
                        {
                            if (min_distance > distance)
                            {
                                min_distance = distance;
                            }
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////// CHECKING TYPE OF COLLISION

            if (app->getKeyboard().isPressed(GLFW_KEY_S))
            {
                if (min_distance >= mov)
                {
                    position += front * (deltaTime * current_sensitivity.z);
                }
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_W))
            {
                if (min_distance >= mov)
                {
                    position -= front * (deltaTime * current_sensitivity.z);
                }
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_A))
            {
                if (min_distance >= mov)
                {
                    position += right * (deltaTime * current_sensitivity.x);
                }
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_D))
            {
                if (min_distance >= mov) // if it didn't collide
                {

                    position -= right * (deltaTime * current_sensitivity.x);
                }
                else // if it collided
                {
                    if (act_Collision_Type == "score")
                    {
                        app->score += 10;
                        std:: cout << "score is: " << app->score << std:: endl;
                        world->markForRemoval(actual_collision_entity);
                        position -= right * (deltaTime * current_sensitivity.x);
                    }
                }
            }

            if (collision->getobstucaseType() == CollisionType::WIN)
            {
                app->winner = true;
            }

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };
}

// If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
// if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
//     current_sensitivity *= controller->speedupFactor;