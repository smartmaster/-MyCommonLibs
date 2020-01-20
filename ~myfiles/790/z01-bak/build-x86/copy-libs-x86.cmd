set __source_root=F:\MyProjects\~MyCommonLibs

set __dest_x86=%__source_root%\PublicLib\x86


set __config=Release
xcopy "%__source_root%\DTraceLib\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt









set __config=ReleaseStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt








set __config=Debug
xcopy "%__source_root%\DTraceLib\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt









set __config=DebugStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.lib" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt








set __config=Release
xcopy "%__source_root%\DTraceLib\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=ReleaseStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





set __config=Debug
xcopy "%__source_root%\DTraceLib\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=DebugStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.pdb" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





set __config=Release
xcopy "%__source_root%\DTraceLib\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=ReleaseStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





set __config=Debug
xcopy "%__source_root%\DTraceLib\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=DebugStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.dll" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





set __config=Release
xcopy "%__source_root%\DTraceLib\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=ReleaseStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





set __config=Debug
xcopy "%__source_root%\DTraceLib\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt






set __config=DebugStatic
xcopy "%__source_root%\DTraceLib\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\SharedSocketProcess\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt





exit
