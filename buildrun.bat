@echo off

setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

msbuild "C:\Users\hamis\Documents\Dev\AudioDevelopment\LetsLearnDSP\test\Builds\VisualStudio2019\test_StandalonePlugin.vcxproj"

PAUSE
"C:\Users\hamis\Documents\Dev\AudioDevelopment\LetsLearnDSP\test\Builds\VisualStudio2019\x64\Debug\Standalone Plugin\test.exe"
endlocal
