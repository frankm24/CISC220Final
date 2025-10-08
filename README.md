# CISC220 Final
Credits:
Created by Wyatt Erwin, Frank Murphy, Maya Perry, Ben Zifcak
UI/Graphics: Frank Murphy

## Setup Instructions (CLion)
1. Launch CLion. If you have a project currently open, then close the project to go to CLion's welcome screen. In the top right corner of the screen, click the "Clone Repository" button. In the URL field, paste: ```https://github.com/frankm24/CISC220Final.git```
Then, choose the directory you wish to clone the project in and press "Clone".

2. Next, we need to clone the relevant submodules containing project dependencies (just SDL3 as of 9/22). Open a terminal in CLion and run the following command:
```bash
git submodule update --init --recursive
```
3. CLion will automatically detect the CMake files and handle the build process, so there should be no other setup steps. Run the code by pressing run using the **CISC220Final** configuration.

**NOTE:** When writing commits, you may get a warning that the SDL3 submodule is set to a "detached head." This state is intentional because it means that we are using a specific version of SDL and not attaching our repository to a changing branch, meaning we will always use the same version of SDL unless we upgrade it manually.
