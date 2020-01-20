call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
pushd "E:\MyProjects\~MyCommonLibs\PluginFileFactoryFileTest"
msbuild /p:UseEnv=true /p:plfatform=Win32 /p:configuration="Release" "PluginFileFactoryFileTest.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.PluginFileFactoryFileTest.vcxproj.Win32.Release.log"
popd
