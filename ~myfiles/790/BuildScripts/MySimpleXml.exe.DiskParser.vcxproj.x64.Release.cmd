call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\DiskParser"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Release" "DiskParser.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.DiskParser.vcxproj.x64.Release.log"
popd
