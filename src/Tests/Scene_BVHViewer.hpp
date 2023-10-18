#pragma once

#include "ScenePreCompiled.h"
#include "Function/Base/Transform.hpp"
#include "Function/Animation/BVHAnim.hpp"
#include "Function/Render/Prototype/Cone.hpp"
#include "Function/Render/Prototype/Cylinder.hpp"
#include "glm/ext.hpp"

using namespace elf::anim;

class Scene_BVHViewer : public Scene {

    // Camera
    Camera mainCamera{glm::vec3(1.45, 0.348, 2.021), 240.f, -11.45f, 45.0f, 0.01f};

    // GUI Variables
    bool is_wireframe = false;
    bool reloadShaders = false;

    bool is_dragging = false;
    double start_x = 0, start_y = 0;
    double curr_x = 0, curr_y = 0;

    // plane
    Plane planeObject;
    std::shared_ptr<Mesh> planeMesh;

    // sphere
    UVSphere lightSphere;
    UVSphere sphereObject2;
    std::shared_ptr<Shader> sphereShader;

    // sphere
    std::shared_ptr<Cylinder> cylinderObject;
    std::shared_ptr<Cone> coneObject;

    // shader
    std::shared_ptr<Shader> mainShader;
    std::string vsPath = "./assets/shaders/plane/plane.vert";
    std::string fsPath = "./assets/shaders/plane/plane.frag";

    BVHRawData bvhRawData;

    BonePositions bvhBonePositions;

    std::vector<Cone> linkageCones;

public:
    Scene_BVHViewer(WindowSystem *windowSystem)
            : Scene(windowSystem) {
        Scene::RegisterInputs();

        // Init Plane
        sphereShader = std::make_shared<Shader>(vsPath, fsPath);
        mainShader = std::make_shared<Shader>(vsPath, fsPath);

        cylinderObject = std::make_shared<Cylinder>(glm::vec3(0, 0.5f, 0.f), glm::vec3(0.f, 0.001f, 0.f));
        coneObject = std::make_shared<Cone>(glm::vec3(0, 0.5f, 0.f), glm::vec3(0.f, 0.001f, 0.f));


//        auto data = ReadBVHAnimationFile(Path("./assets/animations/bvh/rest.bvh"));
        auto data = ReadBVHAnimationFile(Path("./assets/animations/bvh/rest.bvh"));
//        auto data = ReadBVHAnimationFile(Path("C:\\Users\\elecfrog\\Documents\\Sparrow-Renderer\\assets\\animations\\cmuconvert-daz-01-09\\01\\01_01.bvh"));
        if (data.has_value())
            bvhRawData = std::move(data.value());

        auto &dummyRoot = bvhRawData.skeleton.dummyRoot;
        auto &joints = bvhRawData.skeleton.joints;
        auto &motionFrames = bvhRawData.motionData.motionFrames;


        for (auto &motionFrame: motionFrames) {
            
            auto &rotations = motionFrame.jointRotation;
            for (int idx = 0; idx < rotations.size(); ++idx) {
                // TODO FIX HERE
                joints[idx]->rotationMatrix = BVHEulerToRotationMatrix(rotations[idx]);
            }
        }

        ApplyJointOffsets2(dummyRoot, bvhBonePositions);

        // bvhBonePositions itself is a dummy, we don't need to render this dummy point
        GenerateJointLinkages(0.1f, linkageCones, bvhBonePositions.childrens[0]);
    }


    ~Scene_BVHViewer() override = default;

    void OnUpdate(float _deltaTime = 0.0f) override {
        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        ProcessInput(deltaTime);

        mainCamera.UpdateCameraMatrix();

        fn_wireframeMode(is_wireframe);
    }

    void OnRender() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        if (reloadShaders) {
            sphereShader = std::make_shared<Shader>(vsPath, fsPath);
            reloadShaders = false;
            std::cout << "[Reload Shader]" << std::endl;
        }


        { // light sphere
            glm::mat4 M = glm::translate(glm::mat4(1.0f), light.position);
            M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
            lightSphere.Render(sphereShader, mainCamera, M, light);
        }

        for (auto &link: linkageCones) {
            glm::mat4 coneM = glm::mat4(0.25f);
            link.Render(*mainShader, mainCamera, coneM, light);
        }
    }

    void OnImGuiRender() override {
        ImGui::Checkbox("Wireframe Mode", &is_wireframe);
        if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F')) reloadShaders = true;

        ImGui::BeginChild("BVH Skeleton");
        DrawAnimationComponent();
        ImGui::EndChild();

        ImGui::BeginChild("Transform", ImVec2(0, 90));
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::BeginChild("Light", ImVec2(0, 90));
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.01f);
            ImGui::DragFloat3("Color", glm::value_ptr(light.color), 0.01f);
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::BeginChild("Camera", ImVec2(0, 90));
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Position", glm::value_ptr(mainCamera.cameraPos), 0.01f);
        }
        ImGui::EndChild();

        ImGui::BeginChild("Camera", ImVec2(0, 90));

        ImGui::EndChild();

        ImGui::SliderFloat("Sensitivity", &mainCamera.keySensitivity, 0.01f, 2.0f);
        ImGui::SliderFloat("Yaw Angle", &mainCamera.yaw, 0.0f, 360.0f);
        ImGui::SliderFloat("Pitch Angle", &mainCamera.pitch, -90.0f, 90.0f);
        ImGui::SliderFloat("FOV", &mainCamera.FOV, 10.0f, 120.0f);
    }



    void DrawAnimationComponent() const {
        ImGui::PushID("Animation");
        static const char *currGraphKey = "";

        const auto &skeleton = bvhRawData.skeleton;
        if (ImGui::TreeNode("BVH Skeleton")) {

            DrawHierarchyNode(skeleton.dummyRoot);

            DrawHierarchyNode2(bvhBonePositions);

            ImGui::TreePop();
        }
        ImGui::PopID();
    }


    void DrawHierarchyNode(std::shared_ptr<elf::anim::BVHJoint> node) const {
        std::string name = node->name + " " + glm::to_string(node->globalLocation);
        bool node_open = ImGui::TreeNode(name.c_str());

        if (node_open) {
            for (const auto &child: node->childrens) {
                DrawHierarchyNode(child);
            }
            ImGui::TreePop();
        }
    }

    void DrawHierarchyNode2(elf::anim::BonePositions node) const {
        std::string name = glm::to_string(node.current) /*+ " "*/ /*+  glm::to_string(node->globalLocation)*/;
        bool node_open = ImGui::TreeNode(name.c_str());

        if (node_open) {
            for (const auto &child: node.childrens) {
                DrawHierarchyNode2(child);
            }
            ImGui::TreePop();
        }
    }
};