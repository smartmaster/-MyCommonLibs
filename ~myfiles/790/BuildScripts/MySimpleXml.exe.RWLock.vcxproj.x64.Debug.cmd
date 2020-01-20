call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\RWLock"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Debug" "RWLock.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.RWLock.vcxproj.x64.Debug.log"
popd
