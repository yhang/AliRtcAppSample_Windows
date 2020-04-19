// AliRtcCSharp.h : AliRtcCSharp DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "AliRtcEngine.h"
#include <string>
#include <functional>
#include <atomic>
#include <vector>


#ifdef _EXPORTING 
#define API_DECLSPEC extern "C" _declspec(dllexport) 
#else 
#define API_DECLSPEC  extern "C" _declspec(dllimport) 
#endif


// CAliRtcCSharpApp
// �йش���ʵ�ֵ���Ϣ������� AliRtcCSharp.cpp
//

using namespace std;

class CAliRtcCSharpApp : public CWinApp
{
public:
	CAliRtcCSharpApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/*                                         RTC  �� �� ��                                       */
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
class AliRtcEngineListener :public AliRtcEventListener
{
    virtual void onRemoteUserOnLineNotify(const AliRtc::String &uid) override;                  /// ���֪ͨ(Remote user entry notification)
    virtual void onRemoteUserOffLineNotify(const AliRtc::String &uid) override;                 /// ���֪ͨ(Remote user departure notification)
    virtual void onRemoteTrackAvailableNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) override;                     /// Զ���û�������ֹͣ����(Distant user publishes or stops publishing)
    virtual void onSubscribeChangedNotify(const AliRtc::String &uid,
        AliRtcAudioTrack audioTrack, AliRtcVideoTrack videoTrack) override;                     /// ��������������仯ʱ(When subscriptions change)
    virtual void onBye(int code) override;                                                      /// ���������߳�����Ƶ���ر�ʱ�ص�
    virtual void onOccurWarning(int warn) override;                                             /// warning�ص�
    virtual void onOccurError(int error) override;                                              /// error�ص�
};

// �ص���������
typedef void(*JoinResultCallBack)(int);
typedef void(*RemoteUserOnLineNotifyCallBack)(char*);
typedef void(*RemoteUserOffLineNotifyCallBack)(char*);
typedef void(*RemoteTrackAvailableNotifyCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*SubscribeChangedNotifyCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*OccurErrorCallBack)(int);
typedef void(*OccurWarningCallBack)(int);
typedef void(*ByeCallBack)(int);
typedef void(*SubscribeResultCallBack)(char*, AliRtcAudioTrack, AliRtcVideoTrack);
typedef void(*PublishResultCallBack)(int);

// ���ú���
AliRtc::String CStringToAliString(CString &s);
CString AliStringToCString(AliRtc::String& aliStr);

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
/*                                        ��  ��  ��  ��                                       */
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
extern "C" __declspec(dllexport) bool initAliSdk();                                             /// 1.��ʼ��sdk
extern "C" __declspec(dllexport) void releaseAliSdk();                                          /// 2.�ͷ�sdk
extern "C" __declspec(dllexport) void exitChannel();                                            /// 3.�˳�����
extern "C" __declspec(dllexport) bool joinChannel(char *pChannel, char *pAppId, char *pUserId,
    char *pNonce, char *pToken, char *pGlsb, char *pUserName, long iTimesTamp);                 /// 4.���뷿��
extern "C" __declspec(dllexport) int setLocalViewConfig(HWND hWndControlID);                    /// 5.Ϊ����Ԥ�����ô����Լ����Ʋ���
extern "C" __declspec(dllexport) int startPreview();                                            /// 6.�����������
extern "C" __declspec(dllexport) int stopPreview();                                             /// 7.ֹͣ����Ԥ��
extern "C" __declspec(dllexport) void setLogLevel(AliRtcLogLevel logLevel);                     /// 8.������־�ȼ�
extern "C" __declspec(dllexport) int setAutoPublishSubscribe(bool bAutoPub, bool bAutoSub);     /// 9.�����Ƿ��Զ�publish����Ƶ����subscribe����Ƶ��
extern "C" __declspec(dllexport) void configLocalAudioPublish(bool bEnable);                    /// 10.�����Ƿ���������audio
extern "C" __declspec(dllexport) void configLocalCameraPublish(bool bEnable);                   /// 11.�����Ƿ���������camera track
extern "C" __declspec(dllexport) int configLocalSimulcast(bool bEnabled,AliRtcVideoTrack track);/// 12.�Ƿ��������ʹ�Ҫ��Ƶ��
extern "C" __declspec(dllexport) bool configRemoteAudio(char *pUid, bool bEnable);              /// 13.�����Ƿ�����Ƶ��
extern "C" __declspec(dllexport) bool configRemoteCameraTrack(char *pUid, bool bPreferMaster,
    bool bEnable);                                                                              /// 14.�����Ƿ�����Ƶ��
extern "C" __declspec(dllexport) bool configRemoteScreenTrack(char *pUid, bool bEnable);        /// 15.�����Ƿ��Ĺ�����
extern "C" __declspec(dllexport) bool subscribe(char *pUid);                                    /// 16.�ֶ�������Ƶ����Ƶ��������
extern "C" __declspec(dllexport) bool setRemoteViewConfig(HWND hWndShowID, char *pUid, AliRtcVideoTrack track,
    AliRtcRenderMode renderMode, AliRtcScaleRatio scaleRatio, bool bIsfFlip);                   /// 17.ΪԶ�˵���Ƶ���ô����Լ����Ʋ���
extern "C" __declspec(dllexport) int setAudioOnlyMode(bool bAudioOnly);                         /// 18.������Ƶģʽ��������Ƶģʽ
extern "C" __declspec(dllexport) int muteLocalCamera(bool bMute, AliRtcVideoTrack track);       /// 19.�Ƿ�ֹͣ������Ƶ�ɼ�
extern "C" __declspec(dllexport) void configLocalScreenPublish(bool bEnable);                   /// 20.�Ƿ����ñ�����Ļ����
extern "C" __declspec(dllexport) bool publish();                                                /// 21.�ֶ�������Ƶ����Ƶ��������
extern "C" __declspec(dllexport) void setVideoProfile(AliRtcVideoProfile profile, 
    AliRtcVideoTrack track);                                                                    /// 22.������Ƶtrack��profile
extern "C" __declspec(dllexport) void setH5CompatibleMode(bool bEnable);                        /// 23.����H5����ģʽ
extern "C" __declspec(dllexport) int muteLocalMic(bool bEnable);                                /// 24.������Ƶ�ɼ������ؾ�����
extern "C" __declspec(dllexport) int muteRemoteAudioPlaying(char *pUid, bool bEnable);          /// 25.Զ����Ƶ�ɼ���Զ�˾�����

extern "C" __declspec(dllexport) void getAudioCaptures(char *buf);

// �ӿڻص�
extern "C" __declspec(dllexport) void CALLBACK setJoinResultCallBack(JoinResultCallBack jrcb);                                         /// 1.���ص�
extern "C" __declspec(dllexport) void CALLBACK setRemoteUserOnLineNotifyCallBack(RemoteUserOnLineNotifyCallBack ruoncb);               /// 2.Զ���û����ص�
extern "C" __declspec(dllexport) void CALLBACK setRemoteUserOffLineNotifyCallBack(RemoteUserOffLineNotifyCallBack ruoncb);             /// 3.Զ���û����ص�
extern "C" __declspec(dllexport) void CALLBACK setRemoteTrackAvailableNotifyCallBack(RemoteTrackAvailableNotifyCallBack rtancb);       /// 4.Զ���û�������ֹͣ�����ص�
extern "C" __declspec(dllexport) void CALLBACK setSubscribeChangedNotifyCallBack(SubscribeChangedNotifyCallBack scncb);                /// 5.�������ı�ص�
extern "C" __declspec(dllexport) void CALLBACK setOccurErrorCallBack(OccurErrorCallBack oecb);                                         /// 6.Error�ص�
extern "C" __declspec(dllexport) void CALLBACK setOccurWarningCallBack(OccurWarningCallBack owcb);                                     /// 7.Warning�ص�
extern "C" __declspec(dllexport) void CALLBACK setByeCallBack(ByeCallBack bcb);                                                        /// 8.���������߳�����Ƶ���ر�ʱ�ص�
extern "C" __declspec(dllexport) void CALLBACK setSubscribeResultCallBack(SubscribeResultCallBack srcb);                               /// 9.�ֶ����Ļص�
extern "C" __declspec(dllexport) void CALLBACK setPublishResultCallBack(PublishResultCallBack prcb);                                   /// 10.�ֶ������ص�
