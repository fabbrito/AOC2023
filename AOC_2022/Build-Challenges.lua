project "AOC_2022"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"

files {"src/**.h", "src/**.hpp", "src/**.cpp"}

pchheader "pch.h"
pchsource "src/pch.cpp"

includedirs {"src", "../Utils/src"}

links {"Utils"}

targetdir("../Binaries/" .. OutputDir .. "/%{prj.name}")
objdir("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

filter "system:windows"
systemversion "latest"
defines {"WINDOWS"}

filter "configurations:Debug"
defines {"DEBUG"}
runtime "Debug"
symbols "On"

filter "configurations:Release"
defines {"RELEASE"}
runtime "Release"
optimize "On"
symbols "On"
postbuildcommands {"{COPYDIR} .\\*.txt %{cfg.targetdir} "}

filter "configurations:Dist"
defines {"DIST"}
runtime "Release"
optimize "On"
symbols "Off"
postbuildcommands {"{COPYDIR} .\\*.txt %{cfg.targetdir} "}
