call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\GetCurrentModuleName"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "GetCurrentModuleName.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.GetCurrentModuleName.vcxproj.Win32.Release.log"
popd
