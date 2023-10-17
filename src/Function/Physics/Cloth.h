#pragma once

#include "Spring.h"
#include "MassNode.h"
#include <vector>
#include <random>
#include "Resource/Vertex_new.h"

class Cloth
{
public:
	glm::vec3 position;
	int width, length;
	int nodesPerRow, nodesPerColumn;
	int nodesDensity;

	std::vector<MassNode*> nodes;
	std::vector<Spring*> springs;
	std::vector<AttribVertex> vertices;

	// Hook  coefficients
	float structuralCoef = 2000.0;
	float shearCoef = 100.0;
	float bendingCoef = 40.0;

	// damp  coefficient
	float dampCoef = 0.01;

	glm::vec3 gravity = glm::vec3(0, -9.8, 0);

private:
	void InitMassNode(int rowIndex, int ColumnIndex);

	void CreateMassNodeByVertex(AttribVertex& vertex);

	void InitSpringStructures(int rowIndex, int ColumnIndex);


	static inline glm::vec3 ComputeTriangleNormal(AttribVertex& n1, AttribVertex& n2, AttribVertex& n3)
	{
		return glm::cross(n2.position - n1.position, n3.position - n1.position);
	}

public:
	Cloth(glm::vec3 position, int width, int height, int nodesDensity);

	Cloth(glm::vec3 position, std::vector<AttribVertex>& clothObj);

	std::vector<AttribVertex>& GetTriangles();
	std::vector<AttribVertex> GetNodes();
	inline MassNode* GetNode(int x, int y) { return nodes[x + y * nodesPerRow]; }

	void ComputeNodesForce(float timeStamp)
	{
		for (MassNode* node : nodes)
			UpdateNode_Verlet(node, timeStamp);
	}

	void ComputeSpringsForce(float timeStamp)
	{
		for (Spring* spring : springs)
			spring->ComputeForce(timeStamp);
	}

	void ComputeWindForce(glm::vec3 windDir)
	{
		for (MassNode* node : nodes)
			node->force += (windDir * (float)node->mass);
	}

	void ComputeAirForce()
	{
		const float maxAirResistance = 1.f;

		for (MassNode* node : nodes)
		{
			if (!node->isFixed)
			{
				std::default_random_engine generator;
				std::uniform_real_distribution<float> distribution(0.0, maxAirResistance);

				float f = distribution(generator);
				glm::vec3 air_force = -node->velocity * f * (float)node->mass;
				node->force += air_force;
			}
		}
	}

	void ComputeGravity()
	{
		for (MassNode* node : nodes)
			node->force += (gravity * node->mass);
	}

	void CollisionGround(float height)
	{
		const float friction_coef = 1.f;

		for (MassNode* node : nodes)
		{
			if (!node->isFixed && node->vertex.position.y < height)
			{
				float penetration_depth = height - node->vertex.position.y;
				glm::vec3 normal_response = glm::vec3(0.0f, penetration_depth, 0.0f);
				node->velocity.y *= -0.5f;

				glm::vec3 horizontal_velocity = glm::vec3(node->velocity.x, 0.0f, node->velocity.z);
				glm::vec3 friction_force = -friction_coef * horizontal_velocity;

				node->force += friction_force;
				node->vertex.position += normal_response;
			}
		}
	}

	void CollisionSphere(const glm::vec3& sphereCenter, float sphereRadius)
	{
		float restitutionCoefficient = 1.f;
		for (MassNode* node : nodes)
		{
			if (!node->isFixed)
			{
				glm::vec3 direction = node->vertex.position - sphereCenter;
				float distance = glm::length(direction);

				if (distance < sphereRadius)
				{
					glm::vec3 normal = glm::normalize(direction);
					float penetrationDepth = sphereRadius - distance;

					node->vertex.position += normal * penetrationDepth;

					node->velocity -= normal * glm::dot(node->velocity, normal) * (1.0f + restitutionCoefficient);
				}
			}
		}
	}

	void UpdateNode_Leapfrog(MassNode* node, float timeStamp)
	{
		if (node->isFixed) return;

		node->acceleration = node->force / node->mass;
		node->velocity += node->acceleration * timeStamp;
		glm::vec3 temp = node->vertex.position;
		node->vertex.position += node->velocity * timeStamp;

		node->oldPos = temp;

		node->force = glm::vec3(0); // reset
	}

	static void UpdateNode_Verlet(MassNode* node, float timeStamp)
	{
		if (node->isFixed) return;

		glm::vec3 tempPos = node->vertex.position;

		// Verlet integration step
		glm::vec3 acceleration = node->force / node->mass;
		node->vertex.position += (node->vertex.position - node->oldPos) + acceleration * (timeStamp * timeStamp);

		// Update velocity approximation if needed elsewhere
		node->velocity = (node->vertex.position - node->oldPos) / timeStamp;

		node->oldPos = tempPos;
		node->force = glm::vec3(0); // reset
	}

	int releaseSteps[4] = {0, 0, 0, 0}; // 释放所需的总步数
	int currentStep[4] = {0, 0, 0, 0}; // 当前步骤数

	void UpdateNormal();

	void UpdateFixedPoint(int i);

	void collision(glm::vec3 pos);

	void collision();
};
