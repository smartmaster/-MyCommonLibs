set root=E:\MyProjects\~MyCommonLibs

del %root%\*.ncb /f /s /q /a
del %root%\*.res /f /s /q /a
del %root%\*.dep /f /s /q /a
del %root%\BuildLog*.htm /f /s /q /a
del %root%\*.ilk /f /s /q /a
del %root%\*.embed.manifest /f /s /q /a
del %root%\*.intermediate.manifest /f /s /q /a
del %root%\*.obj /f /s /q /a
del %root%\*.pch /f /s /q /a
del %root%\*.tlb /f /s /q /a
del %root%\*.idb /f /s /q /a
del %root%\*.log.xml /f /s /q /a
del %root%\*.aps /f /s /q /a
del %root%\*.old /f /s /q /a
del %root%\UpgradeLog*.XML /f /s /q /a
del %root%\UpgradeReport* /f /s /q /a
del %root%\*.tlog /f /s /q /a
del %root%\*.sdf /f /s /q /a
del %root%\*.ipch /f /s /q /a
del %root%\*.lastbuildstate /f /s /q /a
del %root%\*.log /f /s /q /a
del %root%\*.error /f /s /q /a
del %root%\*.unsuccessfulbuild /f /s /q /a
del %root%\*.exp /f /s /q /a
del %root%\vc???.pdb /f /s /q /a
del %root%\vc??.pdb /f /s /q /a
del %root%\*.VC.db /f /s /q /a

del %root%\lnk{*}.tmp /f /s /q /a
del %root%\*Browse.VC.db /f /s /q /a
del %root%\*.iobj /f /s /q /a
del %root%\*.ipdb /f /s /q /a

rem del %root%\*.suo /f /s /q /a
rem del %root%\*.user /f /s /q /a


del %root%\PublicLib\*.lib /f /s /q /a
del %root%\~myfiles\790\BuildScripts\*.* /f /s /q /a

set __rem=rem
rem set __rem=

%__rem% del %root%\*.pdb /f /s /q /a
%__rem% del %root%\*.pdb /f /s /q /a
%__rem% del %root%\*.lib /f /s /q /a

%__rem% del %root%\*.exe /f /s /q /a
%__rem% del %root%\*.dll /f /s /q /a
%__rem% del %root%\*.sys /f /s /q /a


