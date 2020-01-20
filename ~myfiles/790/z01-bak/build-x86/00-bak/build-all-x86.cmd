set __source_root=F:\MyProjects\~MyCommonLibs

rem call "E:\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
call "d:\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

rem set __Rebuild=/t:Rebuild
set __Rebuild=

cd /d %__source_root%

rem 0001 ////////////////////////////////////////////////////////////////////////////////////////////////
set __Configuration=Release

msbuild.exe "%__source_root%\DTraceLib\DTraceLib.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\DTraceLibTest\DTraceLibTest.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\FileLock\FileLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\GetCurrentModuleName\GetCurrentModuleName.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-dll-test\GetCurrentModuleName-dll-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-exe-test\GetCurrentModuleName-exe-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MTReadWrite\MTReadWrite.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MutexLock\MutexLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RWLock\RWLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketEchoClient\WSASocketEchoClient.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketServer\WSASocketServer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VolumeDiskInfo\VolumeDiskInfo.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VShadow\VShadow.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\NTFSParser\NTFSParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\VHDXParser\VHDXParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\MySimpleXml\MySimpleXml.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RPCStructStream\RPCStructStream.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\HighPerfTransfer\HighPerfTransfer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\SharedSocketProcess\SharedSocketProcess.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true




rem 0002 ////////////////////////////////////////////////////////////////////////////////////////////////
set __Configuration=ReleaseStatic

msbuild.exe "%__source_root%\DTraceLib\DTraceLib.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\DTraceLibTest\DTraceLibTest.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\FileLock\FileLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\GetCurrentModuleName\GetCurrentModuleName.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-dll-test\GetCurrentModuleName-dll-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-exe-test\GetCurrentModuleName-exe-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MTReadWrite\MTReadWrite.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MutexLock\MutexLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RWLock\RWLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketEchoClient\WSASocketEchoClient.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketServer\WSASocketServer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VolumeDiskInfo\VolumeDiskInfo.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VShadow\VShadow.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\NTFSParser\NTFSParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\VHDXParser\VHDXParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\MySimpleXml\MySimpleXml.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RPCStructStream\RPCStructStream.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\HighPerfTransfer\HighPerfTransfer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\SharedSocketProcess\SharedSocketProcess.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true




rem 0003 ////////////////////////////////////////////////////////////////////////////////////////////////
set __Configuration=DebugStatic

msbuild.exe "%__source_root%\DTraceLib\DTraceLib.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\DTraceLibTest\DTraceLibTest.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\FileLock\FileLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\GetCurrentModuleName\GetCurrentModuleName.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-dll-test\GetCurrentModuleName-dll-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-exe-test\GetCurrentModuleName-exe-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MTReadWrite\MTReadWrite.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MutexLock\MutexLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RWLock\RWLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketEchoClient\WSASocketEchoClient.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketServer\WSASocketServer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VolumeDiskInfo\VolumeDiskInfo.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VShadow\VShadow.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\NTFSParser\NTFSParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\VHDXParser\VHDXParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\MySimpleXml\MySimpleXml.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RPCStructStream\RPCStructStream.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\HighPerfTransfer\HighPerfTransfer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\SharedSocketProcess\SharedSocketProcess.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true




rem 0004 ////////////////////////////////////////////////////////////////////////////////////////////////
set __Configuration=Debug

msbuild.exe "%__source_root%\DTraceLib\DTraceLib.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\DTraceLibTest\DTraceLibTest.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\FileLock\FileLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\GetCurrentModuleName\GetCurrentModuleName.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-dll-test\GetCurrentModuleName-dll-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
rem msbuild.exe "%__source_root%\GetCurrentModuleName-exe-test\GetCurrentModuleName-exe-test.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MTReadWrite\MTReadWrite.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\MutexLock\MutexLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RWLock\RWLock.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketEchoClient\WSASocketEchoClient.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\WSASocketServer\WSASocketServer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VolumeDiskInfo\VolumeDiskInfo.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\VShadow\VShadow.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\NTFSParser\NTFSParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\VHDXParser\VHDXParser.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%
msbuild.exe "%__source_root%\MySimpleXml\MySimpleXml.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\RPCStructStream\RPCStructStream.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\HighPerfTransfer\HighPerfTransfer.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true
msbuild.exe "%__source_root%\SharedSocketProcess\SharedSocketProcess.vcxproj" %__Rebuild% /p:Configuration=%__Configuration%;useEnv=true


exit
rem pause
