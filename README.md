# CISC220 Final
Wyatt Erwin, Frank Murphy, Maya Perry, Ben Zifcak

## Setup Instructions
1. Clone the repository **recursively** to ensure that the SDL3 submodule is cloned with our code.
```bash
git clone --recursive git@github.com:YOUR_USERNAME/CISC220Final.git
```
NOTE: You may get a warning that the SDL3 submodule is set to a "detached head." This state is intentional because it means that we are using a specific version of SDL and not attaching our repository to a changing branch, meaning we will always use the same version of SDL unless we upgrade it manually.

2. If you already cloned the repository non-recursively, you can always use the following terminal command to sync up:
```bash
git submodule update --init --recursive
```

