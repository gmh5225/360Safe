#pragma once
#include <ntifs.h>
#include "Data.h"


//白名单进程个数
#define SAFEMODMMDATALISTNUMBER				 0x7CE
//白名单进程个数				       
#define SAFEMODMMDATALISTNUMBERMAXIMUM       0x7D0

//白名单路径个数
#define SAFEMONPATHNUMBER	                 0x1FE
//白名单路径个数 
#define SAFEMONPATHNUMBERMAXIMUM             0x200

//C:\Program Files\360\360safe\SAFEMON
typedef struct _SAFEMONPATH_DIRECTORY
{
	ULONG ListNumber;											// +0       个数 <=0x1FE   
	WCHAR DosPath[SAFEMONPATHNUMBERMAXIMUM][DOSPATHSIZE / 2];	// +4       Dos路径数组，一共200个，每个最大长度520 / 2
	ULONG ArrayIndex[SAFEMONPATHNUMBERMAXIMUM];					// +0x41004 这个没卵用，保存DosPath在数组第几项，感觉多余的，从1开始算
	KSPIN_LOCK	SpinLock;										// 末尾 自旋锁 
}SAFEMONPATH_DIRECTORY, *PSAFEMONPATH_DIRECTORY;

PSAFEMONPATH_DIRECTORY g_SafeMonPath_List;



//与SafeMod结构类似，每个SAFEMONPATH_DIRECTORY结构对应一个SAFEMONDATA_DIRECTORY结构，两个共同使用
typedef struct _SAFEMONDATA_DIRECTORY {
	ULONG     ListNumber;											  //个数 <=0x7CE 
	HANDLE    SafeMonSectionHandle[SAFEMODMMDATALISTNUMBERMAXIMUM];	  //起始偏移0000*4,保存SafeMod的SectionHandle 
	PVOID     SafeMonSectionObject[SAFEMODMMDATALISTNUMBERMAXIMUM];   //起始偏移2002*4,保存SectionObject
	ULONG	  SafeMonIndex[SAFEMODMMDATALISTNUMBERMAXIMUM];			  //起始偏移4003*4,保存SafeMon,查找该dos路径在列表第几项，ret_arg = 返回数组下标
	PEPROCESS SafeMonProcess[SAFEMODMMDATALISTNUMBERMAXIMUM];		  //起始偏移6001*4,保存SafeMod的Eprocess结构
	KSPIN_LOCK	SpinLock;										      //自旋锁 
}SAFEMONDATA_DIRECTORY, *PSAFEMONDATA_DIRECTORY;
PSAFEMONDATA_DIRECTORY   g_SafeMonData_List;

//
// Section object type.
//
extern POBJECT_TYPE MmSectionObjectType;

/*****************************删除*****************************/
//删除名单进程信息
//成功返回对应的下标，失败返回0
//Fake_ZwCreateProcess删除
ULONG NTAPI Safe_DeleteSafeMonDataList(_In_ HANDLE SafeMonSectionHandle);


/*****************************删除*****************************/

/*****************************查询*****************************/
//查询该DosPath是否在列表中，如果是返回Index
ULONG NTAPI Safe_QuerSafeMonPathList(IN PWCHAR DosPath, OUT ULONG ret_arg);
/*****************************查询*****************************/

/*****************************添加*****************************/
//添加名单进程信息
// 成功返回1，失败返回0
//After_ZwCreateSection_Func添加
BOOLEAN Safe_InsertSafeMonDataList(_In_ HANDLE SafeMonSectionHandle, _In_ ULONG SafeMonIndex);
/*****************************添加*****************************/
