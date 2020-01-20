call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\VolumeDiskInfo"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Release" "VolumeDiskInfo.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.VolumeDiskInfo.vcxproj.x64.Release.log"
popd
