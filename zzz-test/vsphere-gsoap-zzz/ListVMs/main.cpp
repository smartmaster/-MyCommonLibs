#include <strstream>
#include <sstream>
#include <fstream>
#include "browseHypervisor.h"
#include "searchHypervisor.h"
#include "VimHelper.h"

int main_0(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);


		list_all(vimMgrInstance, sc);

		//Folder rf(vimMgrInstance, sc.rootFolder);
		//std::vector<ManagedEntity> mes = rf.get_childEntity();
		//for (size_t i = 0; i < mes.size(); ++i)
		//{
		//	cout << i << ". " << mes[i].get_name() << ":" << endl;
		//	if (mes[i].is_type_of(Datacenter::type))
		//		list_datacenter(vimMgrInstance, mes[i]);
		//	else if (mes[i].is_type_of(Folder::type))
		//		list_folder(vimMgrInstance, mes[i]);
		//}

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}


int main_1(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		vector<Datacenter> vecDc;
		const char * dcName = "datacenter.pheonix";
		search_datacenter_from_root_folder(vimMgrInstance, vecDc, dcName, sc);
		//52d699d7-b65f-a821-8357-55f367102678

		
		char uuid[] = "52d699d7-b65f-a821-8357-55f367102678";
		for (auto & datacenter : vecDc)
		{
			vector<VirtualMachine> vecVM;
			search_vm_by_id(vimMgrInstance, vecVM, datacenter, uuid, sc);

			gnvim1__VirtualMachineConfigInfo vmci;
			gnvim1__GuestInfo gi;
			for (auto & vm : vecVM )
			{
				vmci = vm.get_config();
				gi = vm.get_guest();
			}
		}


		char path[] = "[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx";
		for (auto & datacenter : vecDc)
		{
			vector<VirtualMachine> vecVM;
			search_vm_by_path(vimMgrInstance, vecVM, datacenter, path, sc);

			gnvim1__VirtualMachineConfigInfo vmci;
			gnvim1__GuestInfo gi;
			for (auto & vm : vecVM)
			{
				vmci = vm.get_config();
				gi = vm.get_guest();
				VimNoOp();
			}
		}


		{
			std::string vmmoid = "vm-5900";
			std::string vmtype = VirtualMachine::type;
			gnvim1__ManagedObjectReference mor;
			mor.__item = (char*)vmmoid.c_str();
			mor.type = (char*)vmtype.c_str();

			VirtualMachine vmById(vimMgrInstance, &mor);
			gnvim1__VirtualMachineConfigInfo vmci = vmById.get_config();
			gnvim1__GuestInfo gi = vmById.get_guest();
			VimNoOp();
		}
		

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}



int main_2(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		vector<Datacenter> vecDc;
		const char * dcName = "datacenter.pheonix";
		search_datacenter_from_root_folder(vimMgrInstance, vecDc, dcName, sc);
		//52d699d7-b65f-a821-8357-55f367102678


		//char uuid[] = "52d699d7-b65f-a821-8357-55f367102678";
		//for (auto & datacenter : vecDc)
		//{
		//	vector<VirtualMachine> vecVM;
		//	search_vm_by_id(vimMgrInstance, vecVM, datacenter, uuid, sc);

		//	gnvim1__VirtualMachineConfigInfo vmci;
		//	gnvim1__GuestInfo gi;
		//	for (auto & vm : vecVM)
		//	{
		//		vmci = vm.get_config();
		//		gi = vm.get_guest();
		//	}
		//}


		char path[] = "[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx";
		for (auto & datacenter : vecDc)
		{
			vector<VirtualMachine> vecVM;
			search_vm_by_path(vimMgrInstance, vecVM, datacenter, path, sc);

			gnvim1__VirtualMachineConfigInfo vmci;
			gnvim1__GuestInfo gi;
			for (auto & vm : vecVM)
			{
				vmci = vm.get_config();
				gi = vm.get_guest();

				Task taskCS = vm.CreateSnapshot_Task("gsoap test by sonmi01 (name)", "gsoap test by sonmi01 (desc)", false, true);
				gnvim1__TaskInfo ti;
				VimWaitForTask(taskCS, ti, 3000, 1000);

				gnvim1__VirtualMachineSnapshotInfo vmsi;
				if (gnvim1__TaskInfoState__success == ti.state)
				{
					//vmsi = vm.get_snapshot();

					VirtualMachineSnapshot vmsnapshot(vimMgrInstance, ((gnvim1__ManagedObjectReference*)(ti.result)));
					gnvim1__VirtualMachineConfigInfo vmcisnap =  vmsnapshot.get_config();

					bool  consolidate = true;
					Task taskRS = vmsnapshot.RemoveSnapshot_Task(true, &consolidate);
					gnvim1__TaskInfo tiRS;
					VimWaitForTask(taskRS, tiRS, 3000, 1000);

					VimNoOp();
				}
			}
		}


		{
			std::string vmmoid = "vm-5900";
			std::string vmtype = VirtualMachine::type;
			gnvim1__ManagedObjectReference mor;
			mor.__item = (char*)vmmoid.c_str();
			mor.type = (char*)vmtype.c_str();

			VirtualMachine vmById(vimMgrInstance, &mor);
			gnvim1__VirtualMachineConfigInfo vmci = vmById.get_config();
			gnvim1__GuestInfo gi = vmById.get_guest();

			Task taskCS = vmById.CreateSnapshot_Task("gsoap test by sonmi01 (name) moref", "gsoap test by sonmi01 (desc) moref", false, true);
			gnvim1__TaskInfo ti;
			VimWaitForTask(taskCS, ti, 3000, 1000);

			gnvim1__VirtualMachineSnapshotInfo vmsi;
			if (gnvim1__TaskInfoState__success == ti.state)
			{
				//vmsi = vm.get_snapshot();

				VirtualMachineSnapshot vmsnapshot(vimMgrInstance, ((gnvim1__ManagedObjectReference*)(ti.result)));
				gnvim1__VirtualMachineConfigInfo vmcisnap = vmsnapshot.get_config();

				bool  consolidate = true;
				Task taskRS = vmsnapshot.RemoveSnapshot_Task(true, &consolidate);
				gnvim1__TaskInfo tiRS;
				VimWaitForTask(taskRS, tiRS, 3000, 1000);

				VimNoOp();
			}


			VimNoOp();
		}



		//char moid[] = "moref=vm-5900";
		//for (auto & datacenter : vecDc)
		//{
		//	vector<VirtualMachine> vecVM;
		//	search_vm_by_moid(vimMgrInstance, vecVM, datacenter, moid, sc);

		//	gnvim1__VirtualMachineConfigInfo vmci;
		//	gnvim1__GuestInfo gi;
		//	for (auto & vm : vecVM)
		//	{
		//		vmci = vm.get_config();
		//		gi = vm.get_guest();
		//	}
		//}

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}


int main_3(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		//////////////////////////////////////////////////////////////////////////
		std::string vmmoid = "vm-288";// "vm-5900";
		std::string vmtype = VirtualMachine::type;
		gnvim1__ManagedObjectReference mor;
		mor.__item = (char*)vmmoid.c_str();
		mor.type = (char*)vmtype.c_str();

		VirtualMachine vmById(vimMgrInstance, &mor);
		gnvim1__VirtualMachineConfigInfo vmci =vmById.get_config();

		//////////////////////////////////////////////////////////////////////////
		GuestOperationsManager gopm(vimMgrInstance, sc.guestOperationsManager);
		GuestProcessManager pm = gopm.get_processManager();

		GuestFileManager fm = gopm.get_fileManager();



		//////////////////////////////////////////////////////////////////////////
		gnvim1__NamePasswordAuthentication auth;
		auth.interactiveSession = false;
		auth.username = "admin";
		auth.password = "c@123456";

		//////////////////////////////////////////////////////////////////////////
		char guestFilePath[] = "E:\\ssss\\1.txt";
		gnvim1__GuestWindowsFileAttributes gfattrib;
		std::string strUrl = fm.InitiateFileTransferToGuest(
			vmById,//VirtualMachine vm, 
			&auth,//gnvim1__GuestAuthentication*  auth, 
			guestFilePath,//char* guestFilePath, 
			&gfattrib,//gnvim1__GuestFileAttributes*  fileAttributes, 
			1024,//__int64 fileSize, 
			true//bool overwrite
			);


		//////////////////////////////////////////////////////////////////////////
		gnvim1__GuestProgramSpec spec;
		spec.programPath = "c:\\windows\\system32\\notepad.exe";
		spec.arguments = "";// "e:\\c.txt";
		spec.workingDirectory = "";//"c:\\windows";
		spec.__sizeenvVariables = 0;
		spec.envVariables = nullptr;

		//
		//auth.interactiveSession = false;
		//gnvim1__GuestProgramSpec spec;
		//spec.programPath = "c:\\windows\\system32\\notepad.exe";
		//spec.arguments = "";// "e:\\c.txt";
		//spec.workingDirectory = "c:\\windows";
		//spec.__sizeenvVariables = 0;
		//spec.envVariables = nullptr;
		__int64 pid = pm.StartProgramInGuest(vmById, &auth, &spec);

		auth.interactiveSession = false;
		pm.TerminateProcessInGuest(vmById, &auth, pid);

		//////////////////////////////////////////////////////////////////////////

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}


int main_4(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		vector<Datacenter> vecDc;
		const char * dcName = "datacenter.pheonix";
		search_datacenter_from_root_folder(vimMgrInstance, vecDc, dcName, sc);

		char path[] = "[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx";
		for (auto & datacenter : vecDc)
		{
			vector<VirtualMachine> vecVM;
			search_vm_by_path(vimMgrInstance, vecVM, datacenter, path, sc);

			for (auto & vmById : vecVM)
			{
				gnvim1__VirtualMachineConfigInfo vmci = vmById.get_config();

				//////////////////////////////////////////////////////////////////////////
				GuestOperationsManager gopm(vimMgrInstance, sc.guestOperationsManager);

				

				//////////////////////////////////////////////////////////////////////////
				GuestProcessManager pm = gopm.get_processManager();

				gnvim1__NamePasswordAuthentication auth;

				//////////////////////////////////////////////////////////////////////////
				//auth.interactiveSession = false;
				//auth.username = "administrator";
				//auth.password = "Master16516336";
				//__int64 sessionID = 0;
				//GuestAuthManager gam = gopm.get_authManager();
				//gnvim1__GuestAuthentication vmga = gam.AcquireCredentialsInGuest(vmById, &auth, &sessionID);
				
				//////////////////////////////////////////////////////////////////////////
				auth.interactiveSession = false;
				auth.username = "administrator";
				auth.password = "Master16516336";
				gnvim1__GuestProgramSpec spec;
				spec.programPath = "c:\\windows\\system32\\notepad.exe";
				spec.arguments = "";// "e:\\c.txt";
				spec.workingDirectory = "";//"c:\\windows";
				spec.__sizeenvVariables = 0;
				spec.envVariables = nullptr;
				__int64 pid = pm.StartProgramInGuest(vmById, &auth, &spec);

				auth.interactiveSession = false;
				auth.username = "administrator";
				auth.password = "Master16516336";
				pm.TerminateProcessInGuest(vmById, &auth, pid);
				VimNoOp();
			}
		}

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}


int main_5(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		gnvim1__ManagedObjectReference mor;
		std::string __item = "vm-5903";
		mor.__item = (char*)__item.c_str();
		std::string type = VirtualMachine::type;
		mor.type = (char*)type.c_str();
		VirtualMachine vm(vimMgrInstance, &mor);

		char installerOptions[] = "/s /v/qn";
		Task taskUT = vm.UpgradeTools_Task(installerOptions);
		gnvim1__TaskInfo ti;
		VimWaitForTask(taskUT, ti, 3000, 3600);
		if (gnvim1__TaskInfoState__success == ti.state)
		{

		}
		else
		{
			const char * fault_messae = ti.error->localizedMessage;
			vm.MountToolsInstaller();
			gnvim1__GuestInfo gi = vm.get_guest();
			vm.UnmountToolsInstaller();
		}


		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}


int main_6(int argc, char* argv[])
{
	try
	{
		using namespace vim_wrapper_ns;
		char* ip = (argc > 2 ? argv[2] : "shuli02-vc60");
		char* user = (argc > 3 ? argv[3] : "administrator");
		char* pwd = (argc > 4 ? argv[4] : "c@123456");
		binding_proxy proxy(ip, true);
		VimManager vimMgrInstance(&proxy);
		ServiceInstance si(vimMgrInstance, vimMgrInstance.get_service_instance());
		gnvim1__ServiceContent sc = si.RetrieveServiceContent();
		SessionManager sm(vimMgrInstance, sc.sessionManager);
		sm.Login(user, pwd, NULL);

		gnvim1__ManagedObjectReference mor;
		std::string __item = "vm-62";
		mor.__item = (char*)__item.c_str();
		std::string type = VirtualMachine::type;
		mor.type = (char*)type.c_str();
		VirtualMachine vm(vimMgrInstance, &mor);

		PrintVMInfo(vm);

#if 0
		{
			gnvim1__VirtualMachineConfigInfo vmci = vm.get_config();
			std::ostringstream oss;
			soap_write_helper(nullptr, &vmci, &oss, soap_write_type_helper(gnvim1__VirtualMachineConfigInfo));
			string str = oss.str();

			gnvim1__VirtualMachineConfigInfo vmci2;
			istrstream in_str(str.c_str());
			soap_read_helper(nullptr, &vmci2, &in_str, soap_read_type_helper(gnvim1__VirtualMachineConfigInfo));


			std::ostringstream oss2;
			soap_write_helper(nullptr, &vmci2, &oss2, soap_write_type_helper(gnvim1__VirtualMachineConfigInfo));
			string str2 = oss2.str();

			if (str == str2)
			{

			}
			else
			{
				DebugBreak();
			}
		}
#else
		{
			struct soap * soapTemp = soap_new_utf_helper();

			gnvim1__VirtualMachineConfigInfo vmci1 = vm.get_config();

			int error = SOAP_OK;
			std::string error_string;
			//ostringstream out1;
			std::ofstream out1("1.xml");
			error = soap_output_helper(gnvim1__VirtualMachineConfigInfo)(soapTemp, &vmci1, out1);
			if (error)
			{
				error_string = soap_error_string_helper(soapTemp);
			}
			out1.close();
			//string str1 = out1.str();

			gnvim1__VirtualMachineConfigInfo vmci2;
			//istrstream in2(str1.c_str());
			std::ifstream in2("1.xml");
			error = soap_input_helper(gnvim1__VirtualMachineConfigInfo)(soapTemp, &vmci2, in2);
			if (error)
			{
				error_string = soap_error_string_helper(soapTemp);
			}
			in2.close();


			//std::ostringstream out2;
			std::ofstream out2("2.xml");
			error = soap_output_helper(gnvim1__VirtualMachineConfigInfo)(soapTemp, &vmci2, out2);
			if (error)
			{
				error_string = soap_error_string_helper(soapTemp);
			}
			//string str2 = out2.str();
			out2.close();

			soap_delete_helper(soapTemp); soapTemp = nullptr;

			//if (str1 == str2)
			//{

			//}
			//else
			//{
			//	DebugBreak();
			//}
		}
#endif
		{
			soap * soap_temp = soap_new_utf_helper();

			int error = SOAP_OK;
			std::string error_string;

			std::ifstream in1("vsci.xml");
			gnvim1__VirtualMachineConfigInfo vmci1;
			error = soap_input_helper(gnvim1__VirtualMachineConfigInfo)(soap_temp, &vmci1, in1);
			if (error)
			{
				error_string = soap_error_string_helper(soap_temp);
			}
			in1.close();


			std::ofstream out2("vsci-2.xml");
			error = soap_output_helper(gnvim1__VirtualMachineConfigInfo)(soap_temp, &vmci1, out2);
			if (error)
			{
				error_string = soap_error_string_helper(soap_temp);
			}
			out2.close();


			soap_delete_helper(soap_temp); soap_temp = nullptr;
		}
		

		sm.Logout();
		return 0;
	}
	catch (const std::string& err)
	{
		std::cout << "exception: " << err;
		return 1;
	}
}





int main(int argc, char* argv[])
{
	int testCase = 0;
	if (argc > 1)
	{
		testCase = strtol(argv[1], NULL, 0);
	}

	int result = 0;
	switch (testCase)
	{
	case 0:
		result = main_0(argc, argv);
		break;
	case 1:
		result = main_1(argc, argv);
		break;
	case 2:
		result = main_2(argc, argv);
		break;
	case 3:
		result = main_3(argc, argv);
		break;
	case 4:
		result = main_4(argc, argv);
		break;
	case 5:
		result = main_5(argc, argv);
		break;
	case 6:
		result = main_6(argc, argv);
		break;
	case 7:
		break;
	default:
		break;
	}
	return result;
}