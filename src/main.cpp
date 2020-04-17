#include <string.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangleApplication
{
public:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

private:
    VkInstance instance = nullptr;
    GLFWwindow* window = nullptr;

public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    bool glfwExtensionsAvailable(const std::vector<VkExtensionProperties>& availableVkExtensions, const char** reqglfwExtensions, uint32_t reqglfwExtensionsCount)
    {
        std::cout << "Checking " << reqglfwExtensionsCount << " required glfw extensions..." << std::endl;

        bool requiredExtensionsAreAvailable = true;

        for (uint32_t idx = 0; idx < reqglfwExtensionsCount; ++idx)
        {
            auto reqExt = reqglfwExtensions[idx];
            std::cout << "Checking " << reqExt << "... " << std::flush;

            bool found = false;
            for (auto& extProp : availableVkExtensions)
            {
                if (std::strcmp(extProp.extensionName, reqExt) == 0)
                {
                    found = true;
                    break;
                }
            }

            if (found)
            {
                std::cout << "Found !" << std::endl;
            }
            else
            {
                requiredExtensionsAreAvailable = false;
                std::cout << "Not found !" << std::endl;
            }
        }
        return requiredExtensionsAreAvailable;
    }

    void createInstance()
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Vulkan available extensions :" << std::endl;

        for (const auto& extension : extensions)
        {
            std::cout << "\t" << extension.extensionName << std::endl;
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        if (!glfwExtensionsAvailable(extensions, glfwExtensions, glfwExtensionCount))
        {
            throw std::runtime_error("Missing extensions required by glfw");
        }

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void initVulkan()
    {
        createInstance();
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
};

int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
