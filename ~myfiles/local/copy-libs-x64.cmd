set __source_root=E:\MyProjects\~MyCommonLibs

set __dest_x64=%__source_root%\PublicLib\x64

xcopy "%__source_root%\DTraceLib\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\x64\*.lib" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt





xcopy "%__source_root%\DTraceLib\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\x64\*.pdb" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt





xcopy "%__source_root%\DTraceLib\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\x64\*.dll" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt





xcopy "%__source_root%\DTraceLib\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\FileLock\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\GetCurrentModuleName\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MTReadWrite\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MutexLock\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RWLock\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketEchoClient\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VolumeDiskInfo\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VShadow\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\NTFSParser\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\VHDXParser\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\MySimpleXml\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\RPCStructStream\x64\*.exe" "%__dest_x64%\" /s /e /h /i /c /y /exclude:excluded.txt





