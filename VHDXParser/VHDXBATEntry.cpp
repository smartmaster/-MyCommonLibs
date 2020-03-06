#include "stdafx.h"
#include "VHDXBATEntry.h"


LPCTSTR VHDX_BAT_ENTRY::StateToString( INT State )
{
	switch (State)
	{
	case PAYLOAD_BLOCK_NOT_PRESENT: //SB_BLOCK_NOT_PRESENT
		return L"PAYLOAD_BLOCK_NOT_PRESENT or SB_BLOCK_NOT_PRESENT";
	case PAYLOAD_BLOCK_UNDEFINED:
		return L"PAYLOAD_BLOCK_UNDEFINED";
	case PAYLOAD_BLOCK_ZERO:
		return L"PAYLOAD_BLOCK_ZERO";
	case PAYLOAD_BLOCK_UNMAPPED:
		return L"PAYLOAD_BLOCK_UNMAPPED";
	case PAYLOAD_BLOCK_FULLY_PRESENT: //SB_BLOCK_PRESENT
		return L"PAYLOAD_BLOCK_FULLY_PRESENT or SB_BLOCK_PRESENT";
	case PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		return L"PAYLOAD_BLOCK_PARTIALLY_PRESENT";
	}

	return L"Not a valid state!";
}

VOID VHDX_BAT_ENTRY::Print(LONG Index)
{
	//D_INFO(0, TEXT("VHDX_BAT_ENTRY[%d]+++++++++++++++++START"), Index);
	PRINT_FIELD((INT)m_State);
	PRINT_FIELD((INT)m_Reserved);
	PRINT_FIELD((UINT64)m_FileOffsetMB);
	D_INFO(0, TEXT("State=%s"), StateToString(m_State));
	//D_INFO(0, TEXT("VHDX_BAT_ENTRY[%d]+++++++++++++++++END\r\n\r\n\r\n"), Index);
}

//<sonmi01>2014-1-9 ###???
BOOL VHDX_BAT_ENTRY::CurentDiskDataZeroedOrUndefined( CONST VHDX_BAT_ENTRY & BatEntry )
{
	return (PAYLOAD_BLOCK_UNDEFINED == BatEntry.m_State) ||
		(PAYLOAD_BLOCK_ZERO == BatEntry.m_State) ||
		(PAYLOAD_BLOCK_UNMAPPED == BatEntry.m_State);
}
