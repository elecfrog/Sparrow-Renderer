#include "Spring.h"

Spring::Spring(MassNode* node1, MassNode* node2, float hookC, float dampC)
        :node1(node1), node2(node2), hookC(hookC), dampC(dampC)
{
    restLength = glm::length(node1->vertex.position - node2->vertex.position);
}


// Spring-damper pair
void Spring::ComputeForce(float timeStamp)
{
    float currentLength = glm::length(node1->vertex.position - node2->vertex.position);

    // one of the two force directions
    glm::vec3 forceDir = glm::normalize(node2->vertex.position - node1->vertex.position);

    glm::vec3 velocityDiff = node2->velocity - node1->velocity;
    
    // hooke's law without direction, only force magnitude
    // k_s*(L_c - L_r)
    float HookMagnitude = (float)(currentLength - restLength) * hookC;

    // damping force
    float dampMagnitude = glm::dot(forceDir, velocityDiff) * dampC;

    glm::vec3 force = forceDir * (HookMagnitude + dampMagnitude);

    node2->force += -force;
    node1->force += force;
}