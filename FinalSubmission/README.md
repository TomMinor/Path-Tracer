Physically based global illumination renderer 

*Planned features*
- Photon Mapping optimisation
- Open Shading Language

+File structure/problems
	- Fairly robust for a simple filetype
	- Tedious to extend
	- Does allow simple comments
	- Would have liked to look into RIB instead
	- Due to the pop nature of the file reading, the tokens are read in the wrong order for some values. Not a huge problem as the input can be inverted, XYZ becomes ZYX for example. All the more reason to use a well established format.

+ Problems
	- Understanding when to convert between object and world space produced many time consuming bugs, some still aren't fixed
	- Unsure where classes such as Scene, Camera & RenderContext should go, some parts are needed everywhere etc but should not logically be together
	- Threading, updating GUI, FBO (screw that)

+ Didn't have time to
	- Integrate OSL
	- Photon Mapping

+ Did research
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

+ Bugs
	- Depth
	- Sphere intersection
