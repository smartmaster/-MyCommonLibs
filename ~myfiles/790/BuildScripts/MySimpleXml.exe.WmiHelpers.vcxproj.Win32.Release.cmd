call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\WmiHelpers"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "WmiHelpers.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.WmiHelpers.vcxproj.Win32.Release.log"
popd
