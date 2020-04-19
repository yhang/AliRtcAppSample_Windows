// AliRtcCSharp.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AliRtcCSharp.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CAliRtcCSharpApp

BEGIN_MESSAGE_MAP(CAliRtcCSharpApp, CWinApp)
END_MESSAGE_MAP()


// CAliRtcCSharpApp ����

CAliRtcCSharpApp::CAliRtcCSharpApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAliRtcCSharpApp ����

CAliRtcCSharpApp theApp;

// ��������
AliRtcEngineListener *g_AliRtcCSharpEventListener = nullptr;
AliRtcEngine     *g_pEngine = nullptr;
static JoinResultCallBack g_JRCB = NULL;
static RemoteUserOnLineNotifyCallBack g_RUOLNCB = NULL;
static RemoteUserOffLineNotifyCallBack g_RUOFLNCB = NULL;
static RemoteTrackAvailableNotifyCallBack g_RTANCB = NULL;
static SubscribeChangedNotifyCallBack g_SCNCB = NULL;
static OccurErrorCallBack g_OECB = NULL;
static OccurWarningCallBack g_OWCB = NULL;
static ByeCallBack g_BCB = NULL;
static SubscribeResultCallBack g_SRCB = NULL;
static PublishResultCallBack g_PRCB = NULL;
// CAliRtcCSharpApp ��ʼ��

BOOL CAliRtcCSharpApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

vector<CString> StringSplit(const string &s, const string &seperator)
{
	vector<CString> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size())
	{
		//�ҵ��ַ������׸������ڷָ�������ĸ��
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			CString strTemp(s.substr(i, j - i).c_str());
			result.push_back(strTemp);
			i = j;
		}
	}

	return result;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                         RTC �ص�����                                        */
/*                                         RTC Callback                                        */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void AliRtcEngineListener::onRemoteUserOnLineNotify(const AliRtc::String & uid)
{
    if (g_RUOLNCB != NULL && g_pEngine != nullptr)
        g_RUOLNCB((char*)uid.c_str());
}

void AliRtcEngineListener::onRemoteUserOffLineNotify(const AliRtc::String &uid)
{
    if (g_RUOFLNCB != NULL)
        g_RUOFLNCB((char*)uid.c_str());
}

void AliRtcEngineListener::onRemoteTrackAvailableNotify(const AliRtc::String &uid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
{
    if (g_RTANCB != NULL)
        g_RTANCB((char*)uid.c_str(), audioTrack, videoTrack);
}

void AliRtcEngineListener::onSubscribeChangedNotify(const AliRtc::String &uid, AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack)
{
    if (g_SCNCB != NULL)
        g_SCNCB((char*)uid.c_str(), audioTrack, videoTrack);
}

void AliRtcEngineListener::onOccurError(int error)
{
    if (g_OECB != NULL)
        g_OECB(error);
}

void AliRtcEngineListener::onOccurWarning(int warn)
{
    if (g_OWCB != NULL)
        g_OWCB(warn);
}

void AliRtcEngineListener::onBye(int code)
{
    if (g_BCB != NULL)
        g_BCB(code);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                         ��װ�ӿں���                                        */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
bool initAliSdk()
{
    if (nullptr == g_AliRtcCSharpEventListener)
        g_AliRtcCSharpEventListener = new AliRtcEngineListener;

    if (nullptr == g_pEngine)
        g_pEngine = AliRtcEngine::sharedInstance(g_AliRtcCSharpEventListener, "");

    if (nullptr == g_pEngine)
        return false;

    return true;
}

void releaseAliSdk()
{
    if (nullptr != g_pEngine)
    {
        g_pEngine->stopPreview();
        g_pEngine->leaveChannel();
        AliRtcEngine::destroy();
        g_pEngine = nullptr;
    }

    if (nullptr != g_AliRtcCSharpEventListener)
    {
        delete g_AliRtcCSharpEventListener;
        g_AliRtcCSharpEventListener = nullptr;
    }
}

bool joinChannel(char *pChannel, char *pAppId, char *pUserId, char *pNonce, char *pToken, char *pGlsb,char *pUserName, long iTimesTamp)
{
    AliRtcAuthInfo auth_info;

    if (strlen(pChannel) <= 0 || strlen(pAppId) <= 0 || strlen(pUserId) <= 0 || strlen(pNonce) <= 0 || strlen(pToken) <= 0 || strlen(pGlsb) <= 0 || strlen(pUserName) <= 0)
        return false;

    CString strChannel(pChannel);
    auth_info.channel = CStringToAliString(strChannel);
    CString strAppid(pAppId);
    auth_info.appid = CStringToAliString(strAppid);
    CString strUserId(pUserId);
    auth_info.user_id = CStringToAliString(strUserId);
    CString strNonce(pNonce);
    auth_info.nonce = CStringToAliString(strNonce);
    auth_info.timestamp = iTimesTamp;
    CString strToken(pToken);
    auth_info.token = CStringToAliString(strToken);

    vector<CString> vecGlsb = StringSplit(pGlsb, "|");

    for (int index = 0; index < vecGlsb.size(); index++)
        auth_info.gslb.AddString(CStringToAliString(vecGlsb[index]));

    CString strUserName(pUserName);
    AliRtc::String username = CStringToAliString(strUserName);

    auto CallBack_JoinResult = [](void *opaque, int errCode) 
    {
        if (opaque)
        {
            if (g_JRCB != NULL)
            {
                g_JRCB(errCode);
            }
        }
    };

    if (nullptr != g_pEngine && nullptr != g_AliRtcCSharpEventListener)
    {
        g_pEngine->joinChannel(auth_info, username, CallBack_JoinResult, g_AliRtcCSharpEventListener);

        return true;
    }

    return false;
}

void exitChannel()
{
    if (g_pEngine == nullptr || g_AliRtcCSharpEventListener == nullptr)
        return;

    g_pEngine->stopPreview();
    g_pEngine->leaveChannel();
}

int setLocalViewConfig(HWND hWndControlID)
{
    if (g_pEngine != nullptr)
    {
        AliVideoCanvas canvas;
        canvas.hWnd = hWndControlID;

        // Config preview window to engine
        return g_pEngine->setLocalViewConfig(canvas, AliRtcVideoTrackCamera);
    }

    return -1;
}

int startPreview()
{
    if (g_pEngine != nullptr)
    {
        return g_pEngine->startPreview();
    }

    return -1;
}

void setLogLevel(AliRtcLogLevel logLevel)
{
    if (g_pEngine == nullptr)
        return;

    g_pEngine->setLogLevel(logLevel);
}

int setAutoPublishSubscribe(bool bAutoPub, bool bAutoSub)
{
    if (g_pEngine == nullptr)
        return -1;

    return g_pEngine->setAutoPublishSubscribe(bAutoPub, bAutoSub);
}

void configLocalAudioPublish(bool bEnable)
{
    if (g_pEngine == nullptr)
        return;

    g_pEngine->configLocalAudioPublish(bEnable);
}

void configLocalCameraPublish(bool bEnable)
{
    if (g_pEngine == nullptr)
        return;

    g_pEngine->configLocalCameraPublish(bEnable);
}

int configLocalSimulcast(bool bEnabled, AliRtcVideoTrack track)
{
    if (g_pEngine == nullptr)
        return -1;

    return g_pEngine->configLocalSimulcast(bEnabled, track);
}

bool configRemoteAudio(char *pUid, bool bEnable)
{
    if (g_pEngine == nullptr || g_AliRtcCSharpEventListener == nullptr || strlen(pUid) <= 0)
        return false;

    CString strUid(pUid);
    g_pEngine->configRemoteAudio(CStringToAliString(strUid), bEnable);

    return true;
}

bool configRemoteCameraTrack(char *pUid, bool bPreferMaster, bool bEnable)
{
    if (g_pEngine == nullptr || g_AliRtcCSharpEventListener == nullptr || strlen(pUid) <= 0)
        return false;

    CString strUid(pUid);
    g_pEngine->configRemoteCameraTrack(CStringToAliString(strUid), bPreferMaster, bEnable);

    return true;
}

bool configRemoteScreenTrack(char *pUid, bool bEnable)
{
    if (g_pEngine == nullptr || g_AliRtcCSharpEventListener == nullptr || strlen(pUid) <= 0)
        return false;

    CString strUid(pUid);
    g_pEngine->configRemoteScreenTrack(CStringToAliString(strUid), bEnable);

    return true;
}

bool subscribe(char *pUid)
{
    if (NULL != g_SRCB && nullptr != g_AliRtcCSharpEventListener && g_pEngine != nullptr && strlen(pUid) > 0)
    {
        CString strUid(pUid);

        auto onSubResult = [](void *opaque, const AliRtc::String &uid, AliRtcVideoTrack vt, AliRtcAudioTrack at)
        {
            if (NULL != g_SRCB)
            {
                g_SRCB((char*)uid.c_str(), at, vt);
            }
        };

        g_pEngine->subscribe(CStringToAliString(strUid), onSubResult, g_AliRtcCSharpEventListener);

        return true;
    }

    return false;
}

bool setRemoteViewConfig(HWND hWndShowID, char *pUid , AliRtcVideoTrack track, AliRtcRenderMode renderMode, AliRtcScaleRatio scaleRatio, bool bIsfFlip)
{
    if (g_pEngine == nullptr || g_AliRtcCSharpEventListener == nullptr || strlen(pUid) <= 0)
        return false;

    AliVideoCanvas canvas;

    canvas.hWnd = hWndShowID;
    canvas.renderMode = renderMode;
    canvas.scaleRatio = scaleRatio;

    CString strUid(pUid);

    g_pEngine->setRemoteViewConfig(&canvas, CStringToAliString(strUid), track);

    return true;
}

int setAudioOnlyMode(bool bAudioOnly)
{
    if (g_pEngine == nullptr)
        return -1;

    return g_pEngine->setAudioOnlyMode(bAudioOnly);
}

int stopPreview()
{
    if (g_pEngine == nullptr)
        return -1;

    return g_pEngine->stopPreview();
}

int muteLocalCamera(bool bMute, AliRtcVideoTrack track)
{
    if (g_pEngine == nullptr)
        return -1;

    return g_pEngine->muteLocalCamera(bMute, track);
}

void configLocalScreenPublish(bool bEnable)
{
    if (g_pEngine == nullptr)
        return;

    g_pEngine->configLocalScreenPublish(bEnable);
}

bool publish()
{
    if (g_pEngine != nullptr && g_AliRtcCSharpEventListener != nullptr && g_PRCB != NULL)
    {
        auto onPubResult = [](void *opaque, int errCode) 
        {
            // publish Success
            if (g_PRCB != NULL)
            {
                g_PRCB(errCode);
            }
        };

        g_pEngine->publish(onPubResult, g_AliRtcCSharpEventListener);

        return true;
    }

    return false;
}

void setVideoProfile(AliRtcVideoProfile profile, AliRtcVideoTrack track)
{
    if (g_pEngine != nullptr)
    {
        g_pEngine->setVideoProfile(profile, track);
    }
}

void setH5CompatibleMode(bool bEnable)
{
    AliRtcEngine::setH5CompatibleMode(bEnable);
}

int muteLocalMic(bool bEnable)
{
    if (g_pEngine != nullptr)
    {
        return g_pEngine->muteLocalMic(bEnable);
    }

    return -1;
}

int muteRemoteAudioPlaying(char *pUid, bool bEnable)
{
    if (g_pEngine != nullptr && strlen(pUid) > 0)
    {
        CString strUid(pUid);
        return g_pEngine->muteRemoteAudioPlaying(CStringToAliString(strUid), bEnable);
    }

    return -1;
}

void getAudioCaptures(char *buf) {
    if (g_pEngine != nullptr)
    {
        AliRtc::StringArray  aliArray = AliRtc::StringArray();
        g_pEngine->getAudioCaptures(aliArray);
        int size = aliArray.size();
        AliRtc::String aliStr = aliArray.at(0);
        //CString cstr = AliStringToCString(aliStr);
        strcpy_s(buf, 100,  aliStr.c_str());
    }
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                          �ӿڻص�����                                       */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
void CALLBACK setJoinResultCallBack(JoinResultCallBack jrcb)
{
    g_JRCB = jrcb;
}

void CALLBACK setRemoteUserOnLineNotifyCallBack(RemoteUserOnLineNotifyCallBack ruoncb)
{
    g_RUOLNCB = ruoncb;
}

void CALLBACK setRemoteUserOffLineNotifyCallBack(RemoteUserOffLineNotifyCallBack ruoncb)
{
    g_RUOFLNCB = ruoncb;
}

void CALLBACK setRemoteTrackAvailableNotifyCallBack(RemoteTrackAvailableNotifyCallBack rtancb)
{
    g_RTANCB = rtancb;
}

void CALLBACK setSubscribeChangedNotifyCallBack(SubscribeChangedNotifyCallBack scncb)
{
    g_SCNCB = scncb;
}

void CALLBACK setOccurErrorCallBack(OccurErrorCallBack oecb)
{
    g_OECB = oecb;
}

void CALLBACK setOccurWarningCallBack(OccurWarningCallBack owcb)
{
    g_OWCB = owcb;
}

void CALLBACK setByeCallBack(ByeCallBack bcb)
{
    g_BCB = bcb;
}

void CALLBACK setSubscribeResultCallBack(SubscribeResultCallBack srcb)
{
    g_SRCB = srcb;
}

void CALLBACK setPublishResultCallBack(PublishResultCallBack prcb)
{
    g_PRCB = prcb;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*                                           ���ú���                                          */
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/// CStringתAliString��CString To AliString��
AliRtc::String CStringToAliString(CString &s)
{
    int n = s.GetLength();

    if (n == 0)
        return AliRtc::String();

    int len = WideCharToMultiByte(CP_UTF8, 0, s, n, NULL, 0, NULL, NULL);
    char *pChar = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, s, n, pChar, len, NULL, NULL);
    pChar[len] = 0;
    AliRtc::String res = pChar;
    delete[] pChar;
    return res;
}

CString AliStringToCString(AliRtc::String& aliStr)
{
    char* strbuf = (char*)aliStr.c_str();
    int bufSize = strlen(strbuf);
    int len = MultiByteToWideChar(CP_UTF8, 0, strbuf, bufSize, NULL, 0);

    wchar_t* wBuf = new wchar_t[len];
    wmemset(wBuf, 0, len);

    MultiByteToWideChar(CP_UTF8, 0, strbuf, bufSize, wBuf, len);

    //delete(wBuf);
    CString s = CString(wBuf);
    return s;
}