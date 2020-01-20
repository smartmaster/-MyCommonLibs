set gsoap_bin=E:\MyProjects\gsoap-vmware\vim25
set vmware_sdk=Q:\VMWare-install\older-vmware-sdk\VMware-vSphere-SDK-5.5.0-1284541\SDK\vsphere-ws
set targetDir=E:\MyProjects\gsoap-vmware\VIM-vSphere-SDK-Cpp-Wrapper-master\wrapper
set vimpl=vim-sonmi01-no-static-instance.pl

echo "############################################"

set path=%path%;%gsoap_bin%

md %targetDir%

%vimpl% -p %vmware_sdk%\wsdl\vim25\vimService.wsdl %targetDir%

%vimpl% -e %vmware_sdk%\docs\ReferenceGuide

%vimpl% -g %targetDir%


echo "############################################"
