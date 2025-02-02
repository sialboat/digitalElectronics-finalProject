## Digital Electronics Final Project - Game-based CC/CV Controller
This file displays my code and other miscellaneous assets used for my NYU F24 Digital Electronics final project.
My final project draws a triangle on a ILI9341 screen and lets you control its direction and rotation using two joysticks.
These two joysticks have momentum-based movement, meaning that the triangle will smoothly accelerate and decelerate between
angular and positional values.

For more information on the concepts that the project is built upon, please refer to the project [report](https://docs.google.com/document/d/1EwBQhJmSSoC8HgRFptuw8qkn9xVgbsGXSP5RETaCfTY/edit?usp=sharing)

For a quick and concise overview video, please refer to this [video](https://youtu.be/SYEwovngMAE)

## Files Within this Repository
The reporistory contains multiple .ino files documenting the testing process that was used to test each individual feature from the DAC to the display and joystick. The main .ino file is titled `digitalElectronics-finalProject.ino`.

## Planned features
This project is a prototype and is currently in development. Future features include but may not be limited to the following:
- [ ] **CC + CV**: options within the device that allows for MIDI CC data and CV data to be sent out
- [ ] **OOP Refactor**: streamlining the code for expanding the scope of the program and its respective gameplay loop
- [ ] **Power Source + ON/OFF Switch:** removes the need to plug the device directly into a computer for the program to function.
- [ ] **Bluetooth Capability / Sync with Controllers**: a feature that enables the user to connect game controllers to control the game / parameters.
- [ ] **NeoPixel LED Visualizers:** color-synchronized LEDs that change color based on the position and direction the character was headed towards, purely for asthetic purposes.
- [ ] **Color-changing toggle:** both visual and functional, applies a bit of math into the position and angle values that are captured, varying the output a bit more.
- [ ] **Projectiles**: because we are that much closer to creating a video game and we may as well figure out how to make our little triangle shoot
- [ ] **Power-ups w/ Speed Boost**: extra elements that make the CV and CC data a bit more human through slowly incorporating a stimulating gameplay loop.
- [ ] **Collision Logic**: the foundation for implementing other objects into the display such as obstacles and objects that can be destroyed
- [ ] **Other Video Game-y Stuff**: implementing a health bar display, score display, mechanics such as dashing and projectile spray, enemies, side scrolling
- [ ] **Adjustable Parameters**: menus allowing for the user to readjust speed and rotation values
