call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\OpensslHelper"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "OpensslHelper.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.OpensslHelper.vcxproj.Win32.Release.log"
popd
