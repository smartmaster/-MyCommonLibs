//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: HBKSTUB_CBT_SERVICE_NAME
//
// MessageText:
//
// ^AU_ProductName_HBVB^ CBT 服务
//
#define HBKSTUB_CBT_SERVICE_NAME         ((NTSTATUS)0x00000001L)

//
// MessageId: HBKSTUB_CBT_SERVICE_DESCRIPTION
//
// MessageText:
//
// 此服务帮助 ^AU_ProductName_HBVB^ 收集更改的块跟踪数据。
//
#define HBKSTUB_CBT_SERVICE_DESCRIPTION  ((NTSTATUS)0x00000002L)

//
// MessageId: HBKSTUB_CBT_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// ^AU_ProductName_HBVB^ CBT 服务
//
#define HBKSTUB_CBT_UNDEPLOY_ENTRY_NAME  ((NTSTATUS)0x00000003L)

//
// MessageId: HBKSTUB_CBT_UNDEPLOY_ENTRY_HELP_LINK
//
// MessageText:
//
// http://www.arcserve.com/support
//
#define HBKSTUB_CBT_UNDEPLOY_ENTRY_HELP_LINK ((NTSTATUS)0x00000004L)

//
// MessageId: HBKVM_IC_SERVICE_NAME
//
// MessageText:
//
// ^AU_ProductName_HBVB^ 虚拟机集成服务
//
#define HBKVM_IC_SERVICE_NAME            ((NTSTATUS)0x00000005L)

//
// MessageId: HBKVM_IC_SERVICE_DESCRIPTION
//
// MessageText:
//
// 此服务帮助 ^AU_ProductName_HBVB^ 收集应用程序元数据。
//
#define HBKVM_IC_SERVICE_DESCRIPTION     ((NTSTATUS)0x00000006L)

//
// MessageId: HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_NAME
//
// MessageText:
//
// ^AU_ProductName_HBVB^ 虚拟机集成服务
//
#define HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_NAME ((NTSTATUS)0x00000007L)

//
// MessageId: HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_HELP_LINK
//
// MessageText:
//
// http://www.arcserve.com/support
//
#define HBKVM_IC_SERVICE_UNDEPLOY_ENTRY_HELP_LINK ((NTSTATUS)0x00000008L)

//
// MessageId: HBKVM_IC_SERVICE_HELP_STARTING
//
// MessageText:
//
// ^AU_ProductName_HBVB^ 虚拟机集成服务安装程序
//
#define HBKVM_IC_SERVICE_HELP_STARTING   ((NTSTATUS)0x00000009L)

//
// MessageId: HBKVM_IC_SERVICE_HELP_GUIDING
//
// MessageText:
//
// --- 支持的命令 ---
//
#define HBKVM_IC_SERVICE_HELP_GUIDING    ((NTSTATUS)0x0000000AL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_INSTALL
//
// MessageText:
//
// 将 ^AU_ProductName_HBVB^ 虚拟机集成服务安装到本地计算机
//
#define HBKVM_IC_SERVICE_HELP_INSTALL    ((NTSTATUS)0x0000000BL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_UNINSTALL
//
// MessageText:
//
// 从本地计算机卸载 ^AU_ProductName_HBVB^ 虚拟机集成服务
//
#define HBKVM_IC_SERVICE_HELP_UNINSTALL  ((NTSTATUS)0x0000000CL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED
//
// MessageText:
//
// 已成功安装 ^AU_ProductName_HBVB^ VM 集成服务
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED ((NTSTATUS)0x0000000DL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED
//
// MessageText:
//
// 无法安装 ^AU_ProductName_HBVB^ 虚拟机集成服务
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED ((NTSTATUS)0x0000000EL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL
//
// MessageText:
//
// 已成功卸载 ^AU_ProductName_HBVB^ VM 集成服务
//
#define HBKVM_IC_SERVICE_HELP_RESULT_SUCCEEDED_INSTALL ((NTSTATUS)0x0000000FL)

//
// MessageId: HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL
//
// MessageText:
//
// 无法卸载 ^AU_ProductName_HBVB^ 虚拟机集成服务
//
#define HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL ((NTSTATUS)0x00000010L)

//