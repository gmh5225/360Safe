#pragma once
#include <ntifs.h>
#include "MemCheck.h"
#include "Regedit.h"
//������ע���ֵ��
NTSTATUS NTAPI Fake_ZwRenameKey(IN ULONG CallIndex, IN PVOID ArgArray, IN PULONG ret_func, IN PULONG ret_arg);