set gsoap_bin=e:\MyProjects\~MyCommonLibs\zzz-test\vsphere-gsoap\VIM-vSphere-SDK-Cpp-Wrapper-master
set vmware_sdk=e:\windows-server-backup-sonmi01-790-robocopy\QQQ\VMWare-install\6.5\VMware-vSphere-SDK-6.5.0-4571253-build-for-debug\SDK\vsphere-ws
set targetDir=e:\MyProjects\~MyCommonLibs\zzz-test\vsphere-gsoap\VIM-vSphere-SDK-Cpp-Wrapper-master\wrapper
set vimpl=vim-sonmi0-ex.pl
set path=%gsoap_bin%;%path%

echo "############################################"

md %targetDir%

%vimpl% -p %vmware_sdk%\wsdl\vim25\vimService.wsdl %targetDir%

%vimpl% -e %vmware_sdk%\docs\ReferenceGuide

%vimpl% -g %targetDir%


echo "############################################"
