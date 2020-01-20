#pragma once


#define BEGIN_BLOCK(x)	do { int _block_87fshseh_ ## x ## _;
#define LEAVE_BLOCK(x)	(_block_87fshseh_ ## x ## _); break;
#define END_BLOCK(x)	(_block_87fshseh_ ## x ## _); } while (false);