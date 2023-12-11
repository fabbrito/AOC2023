workspace "AOC_2023"
architecture "x64"
configurations {"Debug", "Release", "Dist"}
startproject "Challenges"

-- Workspace-wide build options for MSVC
filter "system:windows"
buildoptions {"/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus"}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Utils"
include "Utils/Build-Utils.lua"
group ""

include "AOC_2023/Build-Challenges.lua"
