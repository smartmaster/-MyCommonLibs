call "D:\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
pushd "E:\MyProjects\~MyCommonLibs\WSASocketEchoClient"
msbuild /p:UseEnv=true /p:plfatform=x64 /p:configuration="Release" "WSASocketEchoClient.vcxproj" /flp:Summary;LogFile="E:\MyProjects\~MyCommonLibs\~myfiles\790\BuildScripts\MySimpleXml.exe.WSASocketEchoClient.vcxproj.x64.Release.log"
popd
