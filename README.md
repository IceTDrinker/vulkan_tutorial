My journey following the Vulkan tutorial at : https://vulkan-tutorial.com/

First init the submodules :
```bash
git submodule update --init
```

Check the README.txt in the textures directory to get the image used in the tutorial


Then create the build dir with what works for your system.

Configure cmake from inside the build dir, check the build options for glfw : https://www.glfw.org/docs/3.3/compile.html

I personally went for (on Windows) :

- GLFW_BUILD_DOCS : off
- GLFW_BUILD_EXAMPLES : off
- GLFW_BUILD_TESTS : off
- GLFW_INSTALL : off
- GLFW_USE_HYBRID_HPG : on (I don't have a laptop but my guess is you probably want this on to properly use your gpu)
- GLFW_VULKAN_STATIC : on (not sure quite yet what should be prefered)
- USE_MSVC_RUNTIME_LIBRARY_DLL : on (means you probably would require people to install vcredists)

If you are on windows and open the Visual Studio solution, make sure to set vulkan_tutorial as the Startup project

Then build and run depending on your system

Cheers
