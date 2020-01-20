call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\CabinetMagager-Source"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="DebugStatic" "CabinetMagager.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.CabinetMagager.vcxproj.Win32.DebugStatic.log"
popd
