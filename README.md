# Computer Graphics Homework 6

## Description
A C++ and OpenGL project simulating a simplified solar system with four planets (Sun, Earth, Moon, Mars). Features include rotation, orbiting, interactive controls, lighting, and camera setup.

## Features
- Four planets: Sun, Earth, Moon, and Mars, each with unique colors.
- Planets rotate on their own axes and follow their respective orbits.
- The Moon orbits the Earth; Earth and Mars orbit the Sun.
- Interactive controls for rotation, translation, scaling, and animation.
- Includes positional lighting, orthographic/perspective viewing volume, and camera setup.
- Displays "A simple solar system" in the window.

## Controls
- **Left Mouse Button + Move**: Rotate the scene.
- **Middle Mouse Button + Move**: Scale the scene.
- **Right Mouse Button + Move**: Translate the scene.
- **Space Bar**: Start/stop the animation.
- **'r' or 'R' Key**: Reset the scene.
- **'l' or 'L' Key**: Turn on the lights.
- **'s' or 'S' Key**: Turn off the lights.
- **'Esc' Key**: Quit the program.

## Code Overview
The main components of the project include:
- **Initialization**: Setting up the OpenGL environment, lighting, and materials.
- **Display Function**: Rendering the solar system, including planets and orbits.
- **Interaction**: Handling mouse and keyboard inputs for scene manipulation.
- **Animation**: Updating planet rotations and orbits.

### Initialization
The `init` function sets up the OpenGL environment, including background color, shading model, and lighting.

### Display Function
The `display` function clears the screen, sets up the scene, and draws the planets and their orbits. It also renders the string "A simple solar system".

### Interaction
Mouse and keyboard inputs are handled to allow rotation, translation, scaling, and toggling of animation and lighting.

### Animation
The `myTimerFunc` function updates the rotation and revolution of the planets periodically to create the animation effect.
