RegisterScene<Scene_BVHViewer>("BVH Animation Viewer");
RegisterScene<Scene_LoadModel>("Forward+");
RegisterScene<Scene06_ShadowMapping>("Shadow Mapping");
RegisterScene<Scene_ClothSimulationSS1>("Cloth Simulation SS1");
RegisterScene<Scene_ClothSimulationSS2>("Cloth Simulation SS2");


static void ExportOBJ(fs::path path, std::shared_ptr<Cloth> &cloth) {
    std::ofstream outAsset(path);
    cereal::BinaryOutputArchive ar(outAsset);
    auto nodes = cloth->GetNodes();
    ar(cereal::make_nvp("clothObject", nodes));
}

static std::vector<AttribVertex> ImportOBJ(fs::path path) {
    std::vector<AttribVertex> ret;
    std::ifstream inputAsset(path);
    cereal::BinaryInputArchive ar(inputAsset);
    ar(cereal::make_nvp("clothObject", ret));
    return std::move(ret);
}

struct MousePosition
{
	double x{}, y{};
};

struct MouseCursor
{
	bool bIsDragging{ false };
	MousePosition start{};
	MousePosition curr{};
};
void processInput(GLFWwindow* window, float deltaTime)
{
float cameraSpeed = 0.05f * deltaTime; // adjust accordingly
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera.cameraPos += mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera.cameraPos -= mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
        mainCamera.yaw -= mainCamera.keySensitivity * 5.0f;
if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
        mainCamera.yaw += mainCamera.keySensitivity * 5.0f;
if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        mainCamera.pitch -= mainCamera.keySensitivity * 5.0f;
if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        mainCamera.pitch += mainCamera.keySensitivity * 5.0f;
if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        mainCamera.FOV -= mainCamera.keySensitivity * 2.0f;
if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        mainCamera.FOV += mainCamera.keySensitivity * 2.0f;
if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        mainCamera.FOV *= -1.0f;
}

// https://learnopengl.com/Lighting/Multiple-lights