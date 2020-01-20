call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\WmiHelpers"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="DebugStatic" "WmiHelpers.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.WmiHelpers.vcxproj.x64.DebugStatic.log"
popd
