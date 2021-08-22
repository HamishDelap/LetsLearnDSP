# Install script for directory: C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/AUDIO_PLUGIN_EXAMPLE")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.8" TYPE FILE FILES
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/build/JUCE/tools/JUCEConfigVersion.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/build/JUCE/tools/JUCEConfig.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/JUCECheckAtomic.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/copyDir.cmake"
    "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/JUCE/extras/Build/CMake/juce_runtime_arch_detection.cpp"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/build/JUCE/tools/modules/cmake_install.cmake")
  include("C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/build/JUCE/tools/extras/Build/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/hamis/Documents/Dev/AudioDevelopment/LetsLearnDSP/test/build/JUCE/tools/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
