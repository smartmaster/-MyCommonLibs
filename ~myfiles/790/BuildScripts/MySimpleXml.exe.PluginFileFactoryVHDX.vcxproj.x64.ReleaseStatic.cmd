call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\PluginFileFactoryVHDX"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="ReleaseStatic" "PluginFileFactoryVHDX.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.PluginFileFactoryVHDX.vcxproj.x64.ReleaseStatic.log"
popd
