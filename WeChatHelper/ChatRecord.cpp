
#include "stdafx.h"
#include <string>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#include "ChatRecord.h"
#include "FriendList.h"
#include "CAutoFunction.h"
#include "Function.h"
#include <stdio.h>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
using namespace std;



#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ      
#define PORT           8080         //�������Ķ˿ں�      
#define MSGSIZE        1024         //�շ��������Ĵ�С  

BOOL g_AutoChat = FALSE;	//�Ƿ��Զ�����
BOOL isSendTuLing = FALSE;	//�Ƿ��Ѿ������˻�����
wchar_t tempwxid[50];	//���΢��ID


//���ص�ַ
DWORD RetkReciveMsgAddr = GetWeChatWinBase() + WxReciveMessage + 5;

//�����ǵ�call�ĵ�ַ
DWORD OverReciveMsgCallAddr = GetWeChatWinBase() + WxReciveMessageCall;


//************************************************************
// ��������: HookChatRecord
// ����˵��: HOOK�����¼
// ��    ��: GuiShou
// ʱ    ��: 2019/7/6
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void HookChatRecord()
{
	HookAnyAddress(GetWeChatWinBase() + WxReciveMessage, RecieveWxMesage);
}



//************************************************************
// ��������: RecieveMesage
// ����˵��: ������Ϣ
// ��    ��: GuiShou
// ʱ    ��: 2019/7/6
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
__declspec(naked) void RecieveWxMesage()
{
	//�����ֳ�
	__asm
	{
		pushad;
		push eax;
		call SendWxMessage;
		popad;
		//���ñ����ǵ�call
		call OverReciveMsgCallAddr;
		//��ת�����ص�ַ
		jmp RetkReciveMsgAddr;
	}
}



//************************************************************
// ��������: ReceiveMsgProc
// ����˵��: ������Ϣ���̻߳ص�����
// ��    ��: GuiShou
// ʱ    ��: 2019/11/8
// ��    ��: Context ������
// �� �� ֵ: void 
//************************************************************
void DealWithMsg(LPVOID Context)
{


	try
	{
		ChatMessageData* msg = (ChatMessageData*)Context;


		BOOL isFriendMsg = FALSE;		//�Ƿ��Ǻ�����Ϣ
		BOOL isImageMessage = FALSE;	//�Ƿ���ͼƬ��Ϣ
		BOOL isRadioMessage = FALSE;	//�Ƿ�����Ƶ��Ϣ
		BOOL isVoiceMessage = FALSE;	//�Ƿ���������Ϣ
		BOOL isBusinessCardMessage = FALSE;	//�Ƿ�����Ƭ��Ϣ
		BOOL isExpressionMessage = FALSE;	//�Ƿ�����Ƭ��Ϣ
		BOOL isLocationMessage = FALSE;	//�Ƿ���λ����Ϣ
		BOOL isSystemMessage = FALSE;	//�Ƿ���ϵͳ������Ϣ
		BOOL isPos_File_Money_XmlLink = FALSE;			//�Ƿ�λ�� �ļ� ת�˺�������Ϣ
		BOOL isFriendRequestMessage = FALSE;	//�Ƿ��Ǻ���������Ϣ
		BOOL isOther = FALSE;	//�Ƿ���������Ϣ


		switch (msg->dwtype)
		{
		case 0x01:
			memcpy(msg->sztype, L"����", sizeof(L"����"));
			break;
		case 0x03:
			memcpy(msg->sztype, L"ͼƬ", sizeof(L"ͼƬ"));
			isImageMessage = TRUE;
			break;
		case 0x22:
			memcpy(msg->sztype, L"����", sizeof(L"����"));
			isVoiceMessage = TRUE;
			break;
		case 0x25:
			memcpy(msg->sztype, L"����ȷ��", sizeof(L"����ȷ��"));
			isFriendRequestMessage = TRUE;
			break;
		case 0x28:
			memcpy(msg->sztype, L"POSSIBLEFRIEND_MSG", sizeof(L"POSSIBLEFRIEND_MSG"));
			isOther = TRUE;
			break;
		case 0x2A:
			memcpy(msg->sztype, L"��Ƭ", sizeof(L"��Ƭ"));
			isBusinessCardMessage = TRUE;
			break;
		case 0x2B:
			memcpy(msg->sztype, L"��Ƶ", sizeof(L"��Ƶ"));
			isRadioMessage = TRUE;
			break;
		case 0x2F:
			//ʯͷ������
			memcpy(msg->sztype, L"����", sizeof(L"����"));
			isExpressionMessage = TRUE;
			break;
		case 0x30:
			memcpy(msg->sztype, L"λ��", sizeof(L"λ��"));
			isLocationMessage = TRUE;
			break;
		case 0x31:
			//����ʵʱλ��
			//�ļ�
			//ת��
			//����
			//�տ�
			memcpy(msg->sztype, L"����ʵʱλ�á��ļ���ת�ˡ�����", sizeof(L"����ʵʱλ�á��ļ���ת�ˡ�����"));
			isPos_File_Money_XmlLink = TRUE;
			break;
		case 0x32:
			memcpy(msg->sztype, L"VOIPMSG", sizeof(L"VOIPMSG"));
			isOther = TRUE;
			break;
		case 0x33:
			memcpy(msg->sztype, L"΢�ų�ʼ��", sizeof(L"΢�ų�ʼ��"));
			isOther = TRUE;
			break;
		case 0x34:
			memcpy(msg->sztype, L"VOIPNOTIFY", sizeof(L"VOIPNOTIFY"));
			isOther = TRUE;
			break;
		case 0x35:
			memcpy(msg->sztype, L"VOIPINVITE", sizeof(L"VOIPINVITE"));
			isOther = TRUE;
			break;
		case 0x3E:
			memcpy(msg->sztype, L"С��Ƶ", sizeof(L"С��Ƶ"));
			isRadioMessage = TRUE;
			break;
		case 0x270F:
			memcpy(msg->sztype, L"SYSNOTICE", sizeof(L"SYSNOTICE"));
			isOther = TRUE;
			break;
		case 0x2710:
			//ϵͳ��Ϣ
			//���
			memcpy(msg->sztype, L"ϵͳ��Ϣ", sizeof(L"ϵͳ��Ϣ"));
			isSystemMessage = TRUE;
			break;
		case 0x2712:
			//������Ϣ
			memcpy(msg->sztype, L"������Ϣ", sizeof(L"������Ϣ"));
			break;
		default:
			break;
		}



		wstring fullmessgaedata(msg->content);	//��������Ϣ����
		wchar_t* tempcontent = msg->content;	//��������Ϣ����

		//����΢��ID�ж��Ƿ��Ǻ�����Ϣ
		wstring wxid_wstr = msg->wxid;
		if (wxid_wstr.find(L"@im.chatroom") != wxid_wstr.npos)
		{
			memcpy(msg->source, L"��ҵ΢��Ⱥ��Ϣ", sizeof(L"��ҵ΢��Ⱥ��Ϣ"));
		}
		else if (wxid_wstr.find(L"@chatroom") != wxid_wstr.npos)
		{
			memcpy(msg->source, L"Ⱥ��Ϣ", sizeof(L"Ⱥ��Ϣ"));
		}
		else
		{
			memcpy(msg->source, L"������Ϣ", sizeof(L"������Ϣ"));
			isFriendMsg = TRUE;
			memcpy(msg->sendername, L"NULL", sizeof(L"NULL"));

		}




		//��΢��IDתΪ΢���ǳ�/Ⱥ���� 
		wstring transformwxid(msg->wxid);
		swprintf_s(msg->wxname, L"%s", GetNickNameByWxid(transformwxid).c_str());

		//��Ⱥ������΢��IDת��Ϊ�������ǳ�
		if (isFriendMsg == FALSE)
		{
			//����΢��ID��ȡ��ϸ��Ϣ����������
			swprintf_s(msg->sendername, L"%s", GetNicknameByWxid(msg->sender));
		}


		//��ʾ��Ϣ����  �����޷���ʾ����Ϣ ��ֹ����
		if (StrStrW(msg->wxid, L"gh"))
		{
			isFriendMsg = FALSE;

			//�������������˷�������Ϣ ������Ϣ�Ѿ����͸�������
			if ((StrCmpW(msg->wxid, ChatRobotWxID) == 0) && isSendTuLing)
			{
				SendTextMessage(tempwxid, msg->content);
				isSendTuLing = FALSE;
			}
			//���΢��IDΪgh_3dfda90e39d6 ˵�����տ���Ϣ
			else if ((StrCmpW(msg->wxid, L"gh_3dfda90e39d6") == 0))
			{
				swprintf_s(msg->content, L"%s", L"΢���տ��");
			}
			else
			{
				//���΢��ID�д���gh ˵���ǹ��ں�
				swprintf_s(msg->content, L"%s", L"���ںŷ�������,�����ֻ��ϲ鿴");

			}
		}

		//����ͼƬ��Ϣ 
		else if (isImageMessage == TRUE)
		{
			swprintf_s(msg->content, L"%s", L"�յ�ͼƬ��Ϣ");
		}
		else if (isRadioMessage == TRUE)
		{
			swprintf_s(msg->content, L"%s", L"�յ���Ƶ��Ϣ,�����ֻ��ϲ鿴");
		}
		else if (isVoiceMessage == TRUE)
		{
			swprintf_s(msg->content, L"%s", L"�յ�������Ϣ,�����ֻ��ϲ鿴");
		}
		else if (isBusinessCardMessage == TRUE)
		{
			//�Զ������Ƭ����
			//AutoAddCardUser(fullmessgaedata);
			swprintf_s(msg->content, L"%s", L"�յ���Ƭ��Ϣ,���Զ���Ӻ���");

		}
		else if (isExpressionMessage == TRUE)
		{
			swprintf_s(msg->content, L"%s", L"�յ�������Ϣ,�����ֻ��ϲ鿴");
		}
		else if (isFriendRequestMessage == TRUE)
		{
			//�Զ�ͨ����������
			AutoAgreeUserRequest(fullmessgaedata);
			swprintf_s(msg->content, L"%s", L"�յ���������,���Զ�ͨ��");

		}
		//����XML���º��Զ�����ת����Ϣ
		else if (isPos_File_Money_XmlLink == TRUE)
		{
			//�ж��Ƿ���ת����Ϣ
			//�ж��Ƿ���ת����Ϣ
			if (StrStrW(tempcontent, L"<type>2000</type>"))
			{
				//�Զ��տ�
				memcpy(msg->sztype, L"ת����Ϣ", sizeof(L"ת����Ϣ"));
				AutoCllectMoney(fullmessgaedata, msg->wxid);
				swprintf_s(msg->content, L"%s", L"�յ�ת����Ϣ,���Զ��տ�");
			}
			//<type>5 </type> ����XML���ºͽ�Ⱥ����
			else if (StrStrW(tempcontent, L"<type>5</type>"))
			{
				//������Ⱥ������
				if (fullmessgaedata.find(L"<![CDATA[���������Ⱥ��]]></title>") != wstring::npos && fullmessgaedata.find(L"<url><![CDATA[") != wstring::npos)
				{
					memcpy(msg->sztype, L"Ⱥ����", sizeof(L"Ⱥ����"));
					swprintf_s(msg->content, L"%s", L"�յ�Ⱥ����,�����ֻ��ϲ鿴");
				}
				//������ѡ֪ͨ
				else if (fullmessgaedata.find(L"������ѡ֪ͨ") != wstring::npos)
				{
					memcpy(msg->sztype, L"������ѡ", sizeof(L"������ѡ"));
					swprintf_s(msg->content, L"%s", L"���ں�������ѡ֪ͨ,�����ֻ��ϲ鿴");
				}
				else
				{
					//���� ˵����XML��������
					memcpy(msg->sztype, L"XML������Ϣ", sizeof(L"XML������Ϣ"));
					swprintf_s(msg->content, L"%s", L"�յ�XML������Ϣ,�����ֻ��ϲ鿴");
				}
			}
			//�ļ���Ϣ
			else if (StrStrW(tempcontent, L"<type>6</type>"))
			{
				memcpy(msg->sztype, L"�ļ���Ϣ", sizeof(L"�ļ���Ϣ"));
				swprintf_s(msg->content, L"%s", L"�յ��ļ� �뼰ʱ�鿴");
			}
			//����ʵʱλ����Ϣ
			else if (StrStrW(tempcontent, L"<type>17</type>"))
			{
				memcpy(msg->sztype, L"����ʵʱλ��", sizeof(L"����ʵʱλ��"));
				swprintf_s(msg->content, L"%s", L"�յ�����ʵʱλ�� �����ֻ��ϲ鿴");
			}
			//�ϲ�ת���������¼
			else if (StrStrW(tempcontent, L"�������¼</title>"))
			{
				memcpy(msg->sztype, L"�����¼��Ϣ", sizeof(L"�����¼��Ϣ"));
				swprintf_s(msg->content, L"%s", L"�յ��ϲ�ת���������¼ �����ֻ��ϲ鿴");
			}
		}

		else if (isLocationMessage == TRUE)
		{
			swprintf_s(msg->content, L"%s", L"�յ�λ����Ϣ,�����ֻ��ϲ鿴");
		}
		else if (isSystemMessage == TRUE)
		{
			//�����ﴦ�����Ⱥ����Ϣ
			if ((StrStrW(tempcontent, L"�Ƴ���Ⱥ��") || StrStrW(tempcontent, L"������Ⱥ��")))
			{
				wcscpy_s(msg->content, wcslen(tempcontent) + 1, tempcontent);
			}
			else
			{
				swprintf_s(msg->content, L"%s", L"�յ������ϵͳ��Ϣ,�����ֻ��ϲ鿴");
			}
		}
		//������������Ϣ֮��
		else
		{
			//�ж���Ϣ���� ������ȳ����Ͳ���ʾ
			if (wcslen(tempcontent) > 200)
			{
				swprintf_s(msg->content, L"%s", L"��Ϣ���ݹ��� �Ѿ�����");
			}
		}


		//���ﴦ���Զ�����
		if (isFriendMsg == TRUE && g_AutoChat == TRUE && isSendTuLing == FALSE)
		{

			//����һ��΢��ID
			wcscpy_s(tempwxid, wcslen(msg->wxid) + 1, msg->wxid);
			//�õ���Ϣ���� ����������
			SendTextMessage((wchar_t*)ChatRobotWxID, msg->content);
			isSendTuLing = TRUE;
		}


		//���͵����ƶ�
		HWND hWnd = FindWindow(NULL, TEXT("΢������"));
		if (hWnd == NULL)
		{
			OutputDebugStringA("δ���ҵ�΢�����ִ���");
			return;
		}

		COPYDATASTRUCT chatmsg;
		chatmsg.dwData = WM_ShowChatRecord;//����һ����ֵ, ������������־��
		chatmsg.cbData = sizeof(ChatMessageData);// strlen(szSendBuf);//�����͵����ݵĳ�
		chatmsg.lpData = msg;// szSendBuf;//�����͵����ݵ���ʼ��ַ(����ΪNULL)
		SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&chatmsg);

		delete msg;
	}
	catch (...)
	{
		OutputDebugStringA("�����¼���쳣��");
	}


}



//************************************************************
// ��������: SendMessage
// ����˵��: �����յ�����Ϣ���͸��ͻ���
// ��    ��: GuiShou
// ʱ    ��: 2019/7/6
// ��    ��: void
// �� �� ֵ: void 
//************************************************************
void __stdcall SendWxMessage(DWORD r_eax)
{
	if (IsBadReadPtr((void*)r_eax, 4) || IsBadReadPtr((void*)(r_eax + MsgTypeOffset), 4) || IsBadReadPtr((void*)(r_eax + MsgContentOffset), 4) || IsBadReadPtr((void*)(r_eax + WxidOffset), 4) || IsBadReadPtr((void*)(r_eax + GroupMsgSenderOffset), 4))
	{
		return;
	}
	try
	{
		ChatMessageData* msg = new ChatMessageData;
		//ȡ����Ϣ����
		msg->dwtype = *((DWORD*)(r_eax + MsgTypeOffset));

		//ȡ����Ϣ����
		LPVOID pContent = *((LPVOID*)(r_eax + MsgContentOffset));
		swprintf_s(msg->content, L"%s", (wchar_t*)pContent);


		//ȡ��΢��ID/ȺID
		LPVOID pWxid = *((LPVOID*)(r_eax + WxidOffset));
		swprintf_s(msg->wxid, L"%s", (wchar_t*)pWxid);

		if (StrStrW(msg->wxid, L"gh_"))
		{
			return;
		}



		//ȡ����Ϣ������
		LPVOID pSender = *((LPVOID*)(r_eax + GroupMsgSenderOffset));
		swprintf_s(msg->sender, L"%s", (wchar_t*)pSender);


		//ȡ����Ϣ������
		LPVOID pSource = *((LPVOID*)(r_eax + MsgSourceOffset));
		swprintf_s(msg->source, L"%s", (wchar_t*)pSource);

		//�����̴߳�����Ϣ
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DealWithMsg, msg, 0, NULL);
		CloseHandle(hThread);
	}
	catch (...)
	{
		OutputDebugStringA("������Ϣ�쳣��....");
	}


}


//************************************************************
// ��������: GetMsgByAddress
// ����˵��: �ӵ�ַ�л�ȡ��Ϣ����
// ��    ��: GuiShou
// ʱ    ��: 2019/7/6
// ��    ��: DWORD memAddress  Ŀ���ַ
// �� �� ֵ: LPCWSTR	��Ϣ����
//************************************************************
std::wstring GetMsgByAddress(DWORD memAddress)
{
	wstring tmp;
	DWORD msgLength = *(DWORD*)(memAddress + 4);
	if (msgLength > 0) {
		WCHAR* msg = new WCHAR[msgLength + 1]{ 0 };
		wmemcpy_s(msg, msgLength + 1, (WCHAR*)(*(DWORD*)memAddress), msgLength + 1);
		tmp = msg;
		delete[]msg;
	}
	return  tmp;
}




//Hook΢��Send Call
struct wxMsg
{
	const wchar_t* msg;
	int len;
	int cap;
	char buff[0x34];
};

#define WxSendMsgOffset          0x149EF4	
#define WxRealSendMsgOffset      0x42D200	

DWORD dwDllBase = GetWeChatWinBase();
DWORD dwSendMsgRetAddr = dwDllBase + WxSendMsgOffset + 5;	       //���ص�ַ
DWORD dwSendMsgRealCallAddr = dwDllBase + WxRealSendMsgOffset;	   //���ǵ�call


void MySendMsg(const wchar_t* wxid, const wchar_t* msg)
{
	wxMsg id = { 0 };
	id.msg = wxid;
	id.len = wcslen(wxid);
	id.cap = wcslen(wxid) * 2;
	char* pWxid = (char*)&id.msg;

	wxMsg text = { 0 };
	text.msg = msg;
	text.len = wcslen(msg);
	text.cap = wcslen(msg) * 2;
	char* pWxmsg = (char*)&text.msg;

	char buff[0x81C] = { 0 };

	__asm
	{
		pushad;
		pushfd;

		push 0x1;

		mov eax, 0;
		push eax;

		mov edi, pWxmsg;
		push edi;

		mov edx, pWxid;

		lea ecx, buff;

		call dwSendMsgRealCallAddr;
		add esp, 0xC;

		popfd;
		popad;
	}
}



void __stdcall HookSendMsgEx(DWORD did, DWORD dmsg)
{
	try
	{
		//MySendMsg(L"weixin", L"hello,yangl");
	}
	catch (...)
	{
		DebugLog(L"������Ϣ�쳣��....");
	}
}
//void  __declspec(naked) HookSendMsgFun()
__declspec(naked)  void HookSendMsgFun()
{
	__asm
	{
		pushad;
		pushfd;

		push edi;
		push edx;
		call  HookSendMsgEx;

		popfd;
		popad;

		call dwSendMsgRealCallAddr;
		jmp dwSendMsgRetAddr;
	}
}

void HookSendMsg()
{
	DebugLog(L"Begin...HookSendMsg");
	HookAnyAddress(dwDllBase + WxSendMsgOffset, HookSendMsgFun);
}

//