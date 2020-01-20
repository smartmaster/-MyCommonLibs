call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\HighPerfTransfer"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="ReleaseStatic" "HighPerfTransfer.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.HighPerfTransfer.vcxproj.x64.ReleaseStatic.log"
popd
