# CISC220 Final
Credits:
Created by Wyatt Erwin, Frank Murphy, Maya Perry, Ben Zifcak
UI/Graphics: Frank Murphy

## Setup Instructions (CLion)
1. Clone this repository recursively with its dependency submodules using HTTPS in your desired folder via the following command:
```
git clone --recurse-submodules https://github.com/frankm24/CISC220Final.git
```
2. Open the project in
3. CLion will automatically detect the CMake files and handle the build process, so there should be no other setup steps. Once CLion has finished intitally indexing the project, which may take some time, run the code by pressing run using the **CISC220Final** configuration.

**NOTE:** When writing commits, you may get a warning that the SDL3 submodule is set to a "detached head." This state is intentional because it means that we are using a specific version of SDL and not attaching our repository to a changing branch, meaning we will always use the same version of SDL unless we upgrade it manually.