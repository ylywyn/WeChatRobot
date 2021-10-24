#include "stdafx.h"
#include "CPublic.h"


//************************************************************
// ��������: HookAnyAddress
// ����˵��: Hook�����ַ
// ��    ��: GuiShou
// ʱ    ��: 2019/11/13
// ��    ��: dwHookAddr ��ҪHOOK�ĵ�ַ dwJmpAddress��ת�ĵ�ַ dwBackAddress ���صĵ�ַ
// �� �� ֵ: void 
//************************************************************
void HookAnyAddress(DWORD dwHookAddr, LPVOID dwJmpAddress)
{
	//��װ��ת����
	BYTE jmpCode[5] = { 0 };
	jmpCode[0] = 0xE9;

	//����ƫ��
	*(DWORD*)&jmpCode[1] = (DWORD)dwJmpAddress - dwHookAddr - 5;

	// ������ǰ���������ڻ�ԭ
	DWORD OldProtext = 0;

	// ��ΪҪ�������д�����ݣ�����Ϊ������ǲ���д�ģ�������Ҫ�޸�����
	VirtualProtect((LPVOID)dwHookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);

	//д���Լ��Ĵ���
	memcpy((void*)dwHookAddr, jmpCode, 5);

	// ִ�����˲���֮����Ҫ���л�ԭ
	VirtualProtect((LPVOID)dwHookAddr, 5, OldProtext, &OldProtext);
}


//************************************************************
// ��������: GetWeChatWinBase
// ����˵��: ��ȡWeChatWin��ַ
// ��    ��: GuiShou
// ʱ    ��: 2019/11/13
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
DWORD g_WeChatWinBase = 0;
DWORD GetWeChatWinBase()
{
	if (g_WeChatWinBase == 0) {
		g_WeChatWinBase = (DWORD)GetModuleHandle(TEXT("WeChatWin.dll"));
	}

	return  g_WeChatWinBase;
}



//************************************************************
// ��������: UTF8ToUnicode
// ����˵��: ��UTF8����תΪUnicode(΢��Ĭ�ϱ���ΪUTF8)
// ��    ��: GuiShou
// ʱ    ��: 2019/7/7
// ��    ��: str ��Ҫת�����ַ���
// �� �� ֵ: wchar_t ���ص��ַ��� 
//************************************************************
wchar_t* UTF8ToUnicode(const char* str)
{
	int    textlen = 0;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}




wstring UTF8ToUnicode2(const char* str)
{
	int textlen = 0;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);

	wstring tempstr(result);
	free(result);
	result = NULL;
	return tempstr;
}


void DebugCode(LPVOID pAddress)
{
	char buff[40] = { 0 };
	sprintf_s(buff, "%p", pAddress);
	MessageBoxA(0, buff, "��ַ", 0);
}

void DebugLog(LPCWSTR lpOutputString)
{
	wstring buf(L"WeChatWinDebug==>");
	buf += lpOutputString;
	OutputDebugString(buf.c_str());
}