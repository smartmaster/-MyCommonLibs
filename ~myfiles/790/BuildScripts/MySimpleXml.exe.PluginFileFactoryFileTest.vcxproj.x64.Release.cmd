call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\PluginFileFactoryFileTest"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Release" "PluginFileFactoryFileTest.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.PluginFileFactoryFileTest.vcxproj.x64.Release.log"
popd
