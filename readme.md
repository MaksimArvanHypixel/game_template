# Minigame

## How to build

This project makes use of CMake to build.

If you're using Visual Studio, more info on how to build it is available [here](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio).
The workflow should basically be:
- Open the launcher for VS.
- Click on "Open a local folder" and select the folder of the repository you cloned.
- It should open the "CMake Overview Pages", and logs should be written in the "Output" tab at the bottom.
- Wait for "CMake generation finished." to be shown in the logs.
- Click on the dropdown arrow next to "Select Startup Item..." at the top, and select "minigame.exe".
- Then click on the green arrow, the game should boot up.

## Tech Stack
- Flecs
  - [Source](https://github.com/SanderMertens/flecs)
  - [Documentation](https://www.flecs.dev/flecs)
  - [Examples](https://github.com/SanderMertens/flecs/tree/master/examples/cpp)
- Raylib
  - [Source](https://github.com/raysan5/raylib)
  - [Documentation](https://www.raylib.com/cheatsheet/cheatsheet.html)
  - [Examples](https://www.raylib.com/examples.html)
- ImGui
  - [Source](https://github.com/ocornut/imgui)
  - [Documentation](https://github.com/ocornut/imgui/wiki)
  - [Examples](https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp)
