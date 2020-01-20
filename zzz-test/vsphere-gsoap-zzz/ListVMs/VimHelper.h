#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include "soapVimBindingProxy.h"
#include "vim_wrapper.h"
using namespace vim_wrapper_ns;
using namespace std;

namespace vim_wrapper_ns
{


VOID VimWaitForTask(Task & task, gnvim1__TaskInfo & ti, ULONG IntervalMillSec, ULONG LoopCount);

void PrintVMInfo(const VirtualMachine & vmById);

VOID VimNoOp();


//////////////////////////////////////////////////////////////////////////
struct soap * soap_new_utf_helper();
void soap_reset_utf_helper(struct soap * soap);
void soap_delete_helper(struct soap * soap);
std::string soap_error_string_helper(struct soap * soap);

#define soap_output_helper(obj_type) \
[](soap * soap_ptr, obj_type * obj_ptr, std::ostream & out_stream) -> int \
{ \
	soap_ptr->os = &out_stream; \
	return soap_write_ ## obj_type(soap_ptr, obj_ptr); \
}

#define soap_input_helper(obj_type) \
[](soap * soap_ptr, obj_type * obj_ptr, std::istream & in_stream) -> int \
{ \
	soap_ptr->is = &in_stream; \
	return soap_read_ ## obj_type(soap_ptr, obj_ptr); \
}

} //namespace vim_wrapper_ns