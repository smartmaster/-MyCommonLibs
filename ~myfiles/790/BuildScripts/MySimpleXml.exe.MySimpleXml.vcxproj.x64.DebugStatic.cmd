call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\MySimpleXml"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="DebugStatic" "MySimpleXml.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.MySimpleXml.vcxproj.x64.DebugStatic.log"
popd
