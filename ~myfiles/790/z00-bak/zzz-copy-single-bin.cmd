set __source_root=F:\MyProjects\~MyCommonLibs

set __dest_x86=%__source_root%\PublicLib\x86


set __config=Debug
rem xcopy "%__source_root%\MTReadWrite\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\HighPerfTransfer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt
xcopy "%__source_root%\WSASocketServer\%__config%\*.exe" "%__dest_x86%\%__config%\" /s /e /h /i /c /y /exclude:excluded.txt

