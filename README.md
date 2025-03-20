# Particles that move in OpenGL
This was my first attempt at making something in OpenGl.
Initially i had a goal to create particle simulation and have it rendered in OpenGL, which kinda happened, but i realized im complicating myself by first not learning OpenGL.

# How to control  Size Resolution and Number of particles:
## int main() is in Particles.cpp
## Size: Line 19
It controls the radius of the particle.
## Resolution : Line 141
It controls how many vertices the circle of a particle should have.
## Number of: Line 144

## Boundry check in case particles leave the screen bounds 
Line 224
Line 263
