call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\MySimpleXml"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "MySimpleXml.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.MySimpleXml.vcxproj.Win32.Release.log"
popd
