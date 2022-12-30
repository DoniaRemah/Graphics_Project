#include "collisions.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads linearVelocity & angularVelocity from the given json object
    void CollisionComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        std::string obstucaseTypeStr = data.value("obstucaseType", "score");
        if (obstucaseTypeStr == "score")
        {
            obstucaseType = CollisionType::SCORE;
        }
        else if (obstucaseTypeStr == "wall")
        {
            obstucaseType = CollisionType::WALL;
        }
        else
        {
            obstucaseType = CollisionType::WIN;
        }
    }
}