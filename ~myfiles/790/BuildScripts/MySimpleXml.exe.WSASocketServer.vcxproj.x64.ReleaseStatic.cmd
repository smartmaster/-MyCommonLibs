call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\WSASocketServer"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="ReleaseStatic" "WSASocketServer.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.WSASocketServer.vcxproj.x64.ReleaseStatic.log"
popd
