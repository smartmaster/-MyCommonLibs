set source=E:\MyProjects\~MyCommonLibs
set dest=E:\MyProjects\~MyCommonLibs-backup

md %dest%
 
date /t >> "%dest%\backup-logs.log"
time /t >> "%dest%\backup-logs.log"


xcopy "%source%\*.h" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.cpp" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.c" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.cc" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.hpp" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.hxx" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.hh" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.inl" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.idl" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.asm" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.inc" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"


xcopy "%source%\*.rc" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.resx" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"

xcopy "%source%\*.sln" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.vcxproj" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.vcxproj.filters" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"


xcopy "%source%\*.cmd" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
xcopy "%source%\*.txt" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"

rem xcopy "%source%\*.lib" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
rem xcopy "%source%\*.exe" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"
rem xcopy "%source%\*.dll" "%dest%\" /s /e /h /i /c /y >> "%dest%\backup-logs.log"


date /t >> "%dest%\backup-logs.log"
time /t >> "%dest%\backup-logs.log"

