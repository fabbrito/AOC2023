workspace "AdventOfCode"
architecture "x64"
configurations {"Debug", "Release", "Dist"}
startproject "AOC_2023"

-- Workspace-wide build options for MSVC
filter "system:windows"
buildoptions {"/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus"}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Utils"
include "Utils/Build-Utils.lua"
group ""

include "AOC_2023/Build-Challenges.lua"
include "AOC_2022/Build-Challenges.lua"
