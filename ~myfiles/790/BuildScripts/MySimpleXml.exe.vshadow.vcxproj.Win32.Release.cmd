call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\VShadow-Source"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "vshadow.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.vshadow.vcxproj.Win32.Release.log"
popd
