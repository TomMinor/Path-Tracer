#Path Tracer

Tom Minor, Bournemouth, NCCA


##Summary

My original goals for this project were to create a path tracer with Open Shading Language integration, possibly using Photon Mapping as an optimisation technique. However, it's current state is simply a global illumination renderer that supports:

- Loading primitive/material data from simple text files.
- Global Illumination approximation in the scene, light is sampled from any primitive using an emissive material type.
- Controlling the sample rate and render size from the user interface.

Once the renderer is more mature I hope to carry out my original plans of a usable path tracer integrated with OSL.


##Usage

###3D Viewport

Displays a real time visualisation of the scene, the camera perspective of this view is used in the final render.

+ **Left Mouse Button + Drag -** Orbit around the viewport.

+ **Right Mouse Button + Drag -** Pan around the viewport

+ **Scroll wheel -** Zoom in/out.

###Render Settings

Control various aspects of the render output.

+ **Samples -** The amount of samples per pixel, higher values will result in longer render times but more accurate images.

+ **Output Size -** The resolution of the rendered image.

+ **File Name -** The output file name *(only **.ppm** files are supported)*

+ **Render Button -** Render the scene to the file. *Note - This will render in the background, right now the progress is only printed to standard output and there is no indication in the UI when the render is complete*

### Scene files

All scene data is stored in simple text files, empty/commented lines are ignored.

Comments begin with #.

####Data Types

+ **Primitives**

   Translation XYZ = `tx ty tz`<br>
   Rotation XYZ = `rx ry rz`<br>
   Scale XYZ = `sx sy sz`<br>
   Colour RGB = `cr cg cb`<br>
   Vertex XYZ = `vx vy vz`<br>
 + **Sphere**  `$sphere tx ty tz sx sy sz rx ry rz cr cg cb `
 + **Triangle** `$tri v0x v0y v0z v1x v1y v1z v2x v2y v2z tx ty tz sx sy sz rx ry rz cr cg cb`
 + **Plane** *Will be supported in future versions, right now two triangle primitives are used*

+ **Camera** `$camera  tx ty tz sx sy sz rx ry rz fov nearz far z`
+ **Background** `$background  cr cg cb`

##Algorithms

+ Refraction
- Schlick's approximation
- Snell's law

##Planned Features
- Photon Mapping optimisation
- Open Shading Language

##Known Problems

###File structure/problems
	- Fairly robust for a simple filetype
	- Tedious to extend
	- Does allow simple comments
	- Would have liked to look into RIB instead
	- Due to the pop nature of the file reading, the tokens are read in the wrong order for some values. Not a huge problem as the input can be inverted, XYZ becomes ZYX for example. All the more reason to use a well established format.

###Bugs
	- Depth
	- Sphere intersection

##Implementation Challenges
	- Understanding when to convert between object and world space produced many time consuming bugs, some still aren't fixed
	- Unsure where classes such as Scene, Camera & RenderContext should go, some parts are needed everywhere etc but should not logically be together
	- Threading, updating GUI, FBO (screw that)

##Research
	- Path tracing
	- Radiometry
	- Acceleration structures
	- Monte Carlo Sampling
	- 

+ Algorithms
	- Create ray in world space from camera into scene
	  Convert into object space for intersection
	  Convert impact point and light sample point to world space to calculate new direction vector
	  Convert shadow ray to object space for each intersection
	- Only lose energy whenever a ray bounces
	- Use inverse square rule to create light falloff
	- Sum all light sources
	- Tonemapping to scale brightnes down
	- Elaborate on monte carlo


+ Improvements
	- Acceleration structures such as KD-Trees for faster triangle intersection, other structures for faster object testing
	- Debugging was a challenge, hard to use breakpoints etc with lots of pixels


+ Design Decisions
	- Tried to make path tracer separate from visualisation, all ngl math classes could be replaced with glm or oiio for example

