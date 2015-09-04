solution "DX12_ConservativeClustered"
	configurations { "Debug", "Release"}
	platforms { "x64" }
	startproject "DX12_ConservativeClustered"
	
	location "build/"
	debugdir "bin/"
	buildoptions { "/MP8" }

	configuration { "x64", "Debug" }
		flags { "Symbols" }
		targetsuffix "_debug"
		targetdir "bin/"
	configuration { "x64", "Release" }
		flags { "Optimize", "Symbols"}
		targetsuffix "_release"
		targetdir "bin/"

	project "DX12_ConservativeClustered"
		kind "ConsoleApp"
		language "C++"
		files { "source/DX12_ConservativeClustered/**.h", "source/DX12_ConservativeClustered/**.cpp" }
		includedirs { "external/SDL/include/", "external/DirectXTK/include/", "external/AntTweakBar/include/" }
		configuration { "x64", "Debug" }
			links { "external/SDL/lib/x64/SDL2", "external/SDL/lib/x64/SDL2_image", "external/DirectXTK/lib/x64/Debug/DirectXTK", "d3d12", "dxgi", "d3dcompiler", "external/AntTweakBar/lib/AntTweakBar64" }
			objdir "build/DX12_ConservativeClustered/Debug/obj/"
		configuration { "x64", "Release" }
			links { "external/SDL/lib/x64/SDL2", "external/SDL/lib/x64/SDL2_image", "external/DirectXTK/lib/x64/Release/DirectXTK", "d3d12", "dxgi", "d3dcompiler", "external/AntTweakBar/lib/AntTweakBar64"  }
			objdir "build/DX12_ConservativeClustered/Release/obj/"
	