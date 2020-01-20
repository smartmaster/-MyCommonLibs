call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\RWLock"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "RWLock.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.RWLock.vcxproj.Win32.Release.log"
popd
