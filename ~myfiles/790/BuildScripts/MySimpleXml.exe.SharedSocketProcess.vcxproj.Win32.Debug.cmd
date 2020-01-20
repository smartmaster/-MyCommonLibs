call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\SharedSocketProcess"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Debug" "SharedSocketProcess.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.SharedSocketProcess.vcxproj.Win32.Debug.log"
popd
