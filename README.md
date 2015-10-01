# ClusteredShadingConservative
DirectX 12 light culling technique featured in GPU Pro 7

Platform: Windows 10 x64 on Graphics cards with D3D12 Conservative Rasterization feature (Currently Nvidia Maxwell 2.0 and Intel Skylake GPUs)

Build: 
1. Run "generate_vsproj.bat" and a /build folder will contain the Visual Studio 2015 solution.
2. Open solution and go to project properties to change "Target Platform Version" from 8.1 to 10.0.10240.0 (or relevant Windows 10 SDK version)

To build Debug you must install DirectX Graphics Tools from Windows 10 Feature-On-Demand (in Settings).


Constants.h contains some tweakable variables.

Screenshots and some outdated info: http://www.kevinortegren.com/portfolio/clustered-shading-assigning-arbitrarily-shaped-convex-light-volumes-using-conservative-rasterization/
