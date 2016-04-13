# ClusteredShadingConservative
DirectX 12 light culling technique featured in GPU Pro 7


Platform: Windows 10 x64 on Graphics cards with D3D12 Conservative Rasterization feature (Currently Nvidia Maxwell 2.0 and Intel Skylake GPUs)


##Build

1. Run "generate_VS_solution.bat" and a /build folder will contain the Visual Studio 2015 solution.

2. Open solution and go to project properties to change "Target Platform Version" from 8.1 to 10.0.10240.0 (or 10.0.10586.0 or relevant Windows 10 SDK version)


To build Debug you must install DirectX Graphics Tools from Windows 10 Feature-On-Demand (in Settings).


Constants.h contains some tweakable variables.


##Controls and Shortcuts
###Keyboard & Mouse

Hold LMB + move mouse to look around

W, A, S, D  - Move forward, backward & strafe

LSHIFT      - Move down

SPACE       - Move up

C           - Print number of light indices in the Light Linked List

V           - Take a snap shot of one light and show all the clusters that light is assigned to

F           - Take a snap shot of current view and display all the clusters where lights were fetched from that frame

ESC         - Exit program


###Gamepad
![Alt text](http://kevinortegren.com/Downloads/Img/controller_mapping.png)


All shortcuts are accessible throughcan be altered through the UI:
Screenshots and some outdated info: http://www.kevinortegren.com/portfolio/clustered-shading-assigning-arbitrarily-shaped-convex-light-volumes-using-conservative-rasterization/
