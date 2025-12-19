#include "Cloth.h"
#include <iostream>
#include "Core/Log/LogSystem.h"

Cloth::Cloth(glm::vec3 position, int width, int length, int nodesDensity)
        : position(position), width(width), length(length), nodesDensity(nodesDensity) {
    nodesPerRow = width * nodesDensity;
    nodesPerColumn = length * nodesDensity;

    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) InitMassNode(i, j);  // new mass node

    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) InitSpringStructures(i, j);     // new springs

    LogSystem::Info("Init Cloth with {0} mass nodes.", nodes.size());
    LogSystem::Info("Init Cloth with {0} springs.", springs.size());

    // left upper corner
    GetNode(0, 0)->isFixed = true;

    // right upper corner
    GetNode(nodesPerRow - 1, 0)->isFixed = true;

    // left bottom corner
    GetNode(0, (nodesPerColumn - 1))->isFixed = true;

    // right bottom corner
    GetNode(nodesPerRow - 1, (nodesPerColumn - 1))->isFixed = true;
}

Cloth::Cloth(glm::vec3 position, std::vector<AttribVertex> &clothObj)
        : position(position), width(16), length(16), nodesDensity(2) {
    nodesPerRow = width * nodesDensity;
    nodesPerColumn = length * nodesDensity;

    for (auto &vertex: clothObj) {
        CreateMassNodeByVertex(vertex);
    }

//    for (int i = 0; i < nodesPerRow; i++)
//        for (int j = 0; j < nodesPerColumn; j++) InitMassNode(i, j);  // new mass node

    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) InitSpringStructures(i, j);     // new springs

    LogSystem::Info("Init Cloth with {0} mass nodes.", nodes.size());
    LogSystem::Info("Init Cloth with {0} springs.", springs.size());

    // left upper corner
    GetNode(0, 0)->isFixed = true;

    // right upper corner
    GetNode(nodesPerRow - 1, 0)->isFixed = true;

    // left bottom corner
    GetNode(0, (nodesPerColumn - 1))->isFixed = true;

    // right bottom corner
    GetNode(nodesPerRow - 1, (nodesPerColumn - 1))->isFixed = true;
}

void Cloth::CreateMassNodeByVertex(AttribVertex&vertex) {
    glm::vec3 currPos = vertex.position;

    MassNode *node = new MassNode(currPos);

    nodes.push_back(node);
}

void Cloth::InitMassNode(int rowIndex, int ColumnIndex) {
    glm::vec3 currPos;
    currPos.x = (float) ColumnIndex / (float) nodesDensity - (float) width / 2.0f;
    currPos.y = 1;
    currPos.z = (float) (-rowIndex) / (float) nodesDensity + (float) length / 2.0f;

    MassNode *node = new MassNode(currPos);

    node->vertex.texCoords.x = (float) ColumnIndex / (nodesPerRow - 1);
    node->vertex.texCoords.y = (float) rowIndex / (1 - nodesPerColumn);

    nodes.push_back(node);
}


void Cloth::InitSpringStructures(int rowIndex, int columnIndex) {
    auto currentNode = GetNode(rowIndex, columnIndex);

    /*
     * Structural Springs 
     * */
    if (rowIndex < nodesPerRow - 1) {
        auto nextRowNode = GetNode(rowIndex + 1, columnIndex);
        springs.emplace_back(new Spring(currentNode, nextRowNode, structuralCoef, dampCoef));
    }

    if (columnIndex < nodesPerColumn - 1) {
        auto nextColNode = GetNode(rowIndex, columnIndex + 1);
        springs.emplace_back(new Spring(currentNode, nextColNode, structuralCoef, dampCoef));
    }

    /*
     * Sheering Springs 
     * */
    if (rowIndex < nodesPerRow - 1 && columnIndex < nodesPerColumn - 1) {
        auto nextDigNode = GetNode(rowIndex + 1, columnIndex + 1);
        springs.emplace_back(new Spring(currentNode, nextDigNode, shearCoef, dampCoef));
        springs.emplace_back(
                new Spring(GetNode(rowIndex + 1, columnIndex), GetNode(rowIndex, columnIndex + 1), shearCoef,
                           dampCoef));
    }

    /*
     * Bending Springs
     * */
    if (rowIndex < nodesPerRow - 2)
        springs.emplace_back(new Spring(currentNode, GetNode(rowIndex + 2, columnIndex), bendingCoef, dampCoef));

    if (columnIndex < nodesPerColumn - 2)
        springs.emplace_back(new Spring(currentNode, GetNode(rowIndex, columnIndex + 2), bendingCoef, dampCoef));
}


void Cloth::UpdateFixedPoint(int i) {
    if (i == 1) {
        // left upper corner
        GetNode(0, 0)->isFixed = !GetNode(0, 0)->isFixed;
    }

    if (i == 2) {
        // right upper corner
        GetNode(nodesPerRow - 1, 0)->isFixed = !GetNode(nodesPerRow - 1, 0)->isFixed;
    }

    if (i == 3) {
        // left bottom corner
        GetNode(0, (nodesPerColumn - 1))->isFixed = !GetNode(0, (nodesPerColumn - 1))->isFixed;
    }

    if (i == 4) {
        // right bottom corner
        GetNode(nodesPerRow - 1, (nodesPerColumn - 1))->isFixed = !GetNode(nodesPerRow - 1,
                                                                           (nodesPerColumn - 1))->isFixed;
    }

}


//there are two kinds of collision,one is with floor, one is with sphere
void Cloth::collision(glm::vec3 pos) {
    for (int i = 0; i < nodesPerRow; i++)
        for (int j = 0; j < nodesPerColumn; j++) {
            float dis = glm::distance(GetNode(i, j)->vertex.position, pos);
            float dis2 = GetNode(i, j)->vertex.position.y - pos.y;
            if (dis <= 0.2f) {
                GetNode(i, j)->acceleration = glm::vec3(0);
                GetNode(i, j)->velocity = glm::vec3(0);
            }

        }
}

void Cloth::collision() {
    for (int i = 0; i < nodesPerRow - 1; i++) {
        for (int j = 0; j < nodesPerColumn - 1; j++) {

            if (GetNode(i, j)->vertex.position.y - (-15.0f) < 1e-5) {
                GetNode(i, j)->acceleration = glm::vec3(0);
                GetNode(i, j)->velocity = glm::vec3(0);
            }

        }
    }
}

std::vector<AttribVertex> &Cloth::GetTriangles() {
    vertices.clear();

    for (int i = 0; i < nodesPerRow - 1; i++) {
        for (int j = 0; j < nodesPerColumn - 1; j++) {
            // Left upper triangle
            vertices.push_back(GetNode(i + 1, j)->vertex);
            vertices.push_back(GetNode(i, j)->vertex);
            vertices.push_back(GetNode(i, j + 1)->vertex);
            // Right bottom triangle
            vertices.push_back(GetNode(i + 1, j + 1)->vertex);
            vertices.push_back(GetNode(i + 1, j)->vertex);
            vertices.push_back(GetNode(i, j + 1)->vertex);
        }
    }
    return vertices;
}

std::vector<AttribVertex> Cloth::GetNodes() {

    std::vector<AttribVertex> ret;

    for (int i = 0; i < nodesPerRow; i++) {
        for (int j = 0; j < nodesPerColumn; j++) {
            ret.emplace_back(GetNode(i, j)->vertex);
        }
    }
    return ret;
}

void Cloth::UpdateNormal() {
    glm::vec3 normal(0.0, 0.0, 0.0);

    // reset normal
    for (MassNode *node: nodes) node->vertex.normal = glm::vec3(0.0);

    for (int i = 0; i < nodesPerRow - 1; i++) {
        for (int j = 0; j < nodesPerColumn - 1; j++) {
            // Left upper triangle
            MassNode *node1 = GetNode(i + 1, j);
            MassNode *node2 = GetNode(i, j);
            MassNode *node3 = GetNode(i, j + 1);

            normal = ComputeTriangleNormal(node1->vertex, node2->vertex, node3->vertex);

            node1->vertex.normal = normal;
            node2->vertex.normal = normal;
            node3->vertex.normal = normal;

            // Right bottom triangle
            node1 = GetNode(i + 1, j + 1);
            node2 = GetNode(i + 1, j);
            node3 = GetNode(i, j + 1);

            normal = ComputeTriangleNormal(node1->vertex, node2->vertex, node3->vertex);

            node1->vertex.normal = normal;
            node2->vertex.normal = normal;
            node3->vertex.normal = normal;
        }
    }
}