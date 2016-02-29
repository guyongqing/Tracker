#pragma once

// GUID
#include <initguid.h>
// MZFC ͷ�ļ�
#include <mzfc_inc.h>
// MZ ��Ȩ�ļ�
#include <MyStoreLib.h>
// Shell ͷ�ļ�
#include <ShellNotifyMsg.h>
// ������Ӧ
#include <acc_api.h>

// ������
#include <IMixer.h>
#include <IMixer_GUID.h>

// USB �¼�
#include <UsbNotifyApi.h>
// �����¼�
#include <CallNotifyApi.h>

// ������
#include <hssData.h>
#include <hssSpeaker.h>

// FPS ����
#include "TrackerFPS.h"
// ͼ������
#include "TrackerImage.h"

// ����
#define APP_MUTEX_NAME			L"TrackerMutex"
#define APP_WINDOW_TEXT			L"��ӰѰ��"
#define APP_GUID				L"{9EB77B8F-6AD7-4c05-8591-AC8B4895F045}"

// ��Ϸ FPS
#define FPS						25			// ��ϷFPS

// ��Ļ����
#define	SCREEN_WIDTH			480			// ��Ļ���
#define	SCREEN_HEIGHT			720			// ��Ļ�߶�
#define SCREEN_COLOR			16			// 16 λɫ

// ��������
#define DOT_SIZE				48			// ÿһ���С
#define DOT_COUNT				150			// �ܵ���
#define DOT_WIDTH				10			// ���
#define DOT_HEIGHT				15			// �߶�
#define DOT_STYLE				5			// ��ʽ

// ͼ������
#define IMG_LOADING_WIDTH		12			// ���϶�ȡ�п��
#define IMG_LOADING_HEIGHT		60			// ���϶�ȡ�и߶�
#define IMG_LOADING_SIZE		720			// ���϶�ȡ�д�С

#define IMG_LOADING_X			250			// ���϶�ȡ�� X ����
#define IMG_LOADING_Y			330			// ���϶�ȡ�� Y ����

#define IMG_BACKGROUND_WIDTH	480			// �������
#define IMG_BACKGROUND_HEIGHT	720			// �����߶�
#define IMG_BACKGROUND_SIZE		691200		// ������С
#define IMG_BACKGROUND_FILE		L"15149001.dat"

#define IMG_FOREGROUND_WIDTH	480			// ǰ�����
#define IMG_FOREGROUND_HEIGHT	720			// ǰ���߶�
#define IMG_FOREGROUND_SIZE		691200		// ǰ����С
#define IMG_FOREGROUND_FILE		L"15149002.dat"

// ����������
#define SPEAKER_FREQMIX			22050		// Ƶ��
#define SPEAKER_BPS				16			// λ��
#define SPEAKER_STEREO			true		// ������

// ��������
#define MUSIC_FILE_SIZE			50000		// �������ֻ���
#define MUSIC_FILE_COUNT		2			// ͬʱ����2������

#define	MUSIC_VOLUME_DEFAULT	16			// Ĭ������
#define MUSIC_VOLUME_MAX		64			// �������
#define MUSIC_VOLUME_MIN		0			// ��С����
#define MUSIC_VOLUME_STEP		10			// ��������

// ��������
#define SOUND_FILE_SIZE			400000		// ��������
#define SOUND_FILE_COUNT		11			// ͬʱ����11������
#define SOUND_SAMPLE_COUNT		5			// ��ɫͳ��
#define SOUND_BUTTON_FILE		L"15149003.dat"

#define	SOUND_VOLUME_DEFAULT	16			// Ĭ������
#define SOUND_VOLUME_TOP		100			// �������
#define SOUND_VOLUME_MAX		64			// �������
#define SOUND_VOLUME_MIN		0			// ��С����
#define SOUND_VOLUME_STEP		8			// ��������

// ��Ϸ״̬
#define GAME_STATUS_START		0			// ��ʼ
#define GAME_STATUS_LOGO		1			// ��ʾ��־
#define GAME_STATUS_RUN			2			// ��ʾ��Ȩ
#define GAME_STATUS_MENU		3			// ��ʾ�˵�

// ������ʾ
#define GAME_ERROR				L"�������"
#define GAME_ERROR_INIT			L"�����ʼ������"
#define GAME_ERROR_FILE			L"���϶�ȡ����"
#define GAME_ERROR_COPY			L"��Ȩ�ļ�У��ʧ�ܣ����������ذ�װ��"
#define GAME_ERROR_FREE			L"����ǰʹ�õ������ð汾��"

// ������ɫ
#define COLOR_BLACK		0					// ��ɫ
#define COLOR_WHITE		0xFFFF				// ��ɫ

// ������Ӧ
#define ACC_MAX		56

//************************************************************************************************
//*************************** �ļ����� ***********************************************************
//************************************************************************************************

// ��ȡ����ǰĿ¼
void FileGetPath(void);
// ��ȡ�ļ�·��
void FileGetName(LPTSTR);
// ��ȡ�ļ�����
bool FileGetData(LPTSTR, char*, int);

//************************************************************************************************
//*************************** ������Ӧ ***********************************************************
//************************************************************************************************

void AccRun(void);

//************************************************************************************************
//*************************** �������� ***********************************************************
//************************************************************************************************

// ��ʼ��������
bool SpeakerInit(void);
// �ر��������豸
void SpeakerEnd(void);
// ��������ͣ
void SpeakerPause(void);
// ����������
void SpeakerContinue(void);
// ��������
void SpeakerPlaySound(int i);
// ��ȡ�����ļ�
void SpeakerLoadingMusic(LPTSTR, int, int);
// ��ȡ�����ļ�
void SpeakerLoadingSound(LPTSTR, int, int);
// ��������
void SpeakerSoundVolumeDown(int);
// ��������
void SpeakerSoundVolumeUp(int);
// ��ʼ��������
bool MixerInit(void);
// ��������������
void MixerVolumeDown(int);
// ��������������
void MixerVolumeUp(int);
// ����״̬����
void MixerModeSetting(void);

//************************************************************************************************
//*************************** DirectDraw *********************************************************
//************************************************************************************************

#define DDRAW_INIT_STRUCT(ddsd) {memset(&ddsd,0,sizeof(ddsd));ddsd.dwSize=sizeof(ddsd);}

// ��ʼ��
bool DirectDrawInit(void);
// ����
void DirectDrawEnd(void);
// װ������
void DirectDrawLoading(void);
// ��������
bool DirectDrawCreateSurface(LPDIRECTDRAWSURFACE*, int, int);

//************************************************************************************************
//*************************** �� Ȩ �� �� ********************************************************
//************************************************************************************************

// �շ�����Ϸ�����֤
bool LicenseVerify(void);

//************************************************************************************************
//*************************** �� �� �� ȡ ********************************************************
//************************************************************************************************

// ��ȡ�����ļ�
void LoadingSoundFile(int,int);
// ��ȡͼ���ļ�
void LoadingImageFile(LPTSTR, int, LPDIRECTDRAWSURFACE, int);

//************************************************************************************************
//*************************** �� Ϸ �� �� ********************************************************
//************************************************************************************************

// ��Ϸ��ʼ��
bool GameInit(void);
// ��Ϸ��ʼ
void GameRun(void);
// ��Ϸ����
void GameEnd(void);
// ������Ϸ
void GameLive(void);
// ֹͣ��Ϸ
void GameDead(void);
// ��ͣ��Ϸ
void GamePause(void);
// ������Ϸ
void GameContinue(void);
// ��Ϸ����
void GameError(LPTSTR);
// ��������
void GamePlaySound(void);
// ���þ���
void GameRectInit(void);
// ������Ϸ״̬
void GameStatus(int);

//************************************************************************************************
//*************************** ������� ***********************************************************
//************************************************************************************************

// ��Ϸ����
void Draw(void);
// ��Ϸ����ʱ
void DrawRun(void);
// ����
void DrawDot(int);
// ������ͣ����
void DrawPause(void);
// ����������
void DrawPrimary(void);
// ����
void DrawCls(int);
// ���ƾ���
void DrawRect(LPDIRECTDRAWSURFACE,int,int,int,int,int,int,int,int);
// ���Ʊ�־
void DrawLogo(void);
// �������϶�ȡ�л���
void DrawLoading(int);
// ���Ʋ˵�
void DrawMenu(void);

//************************************************************************************************
//*************************** ��괦�� ***********************************************************
//************************************************************************************************

// ��������¼�
void MouseUp(int, int);
// ��Ϸ����ʱ������
void MouseUpRun(int, int);
// ���˫��ʱ
void MouseClick(int, int);
// �������λ��
bool MouseArea(int, int, int, int, int, int);

//************************************************************************************************
//*************************** �� �� �� ***********************************************************
//************************************************************************************************

// �����ڶ���
class MyWindow : public CMzWndEx
{

	// MZFC�����༰������Ĵ�������������
	MZ_DECLARE_DYNAMIC(MyWindow);

	// ���г�Ա
	public:

		// ���캯��
		MyWindow(void);
		// ��������
		~MyWindow(void);

		// ��������
		bool CreateWindowNow(void);

	// ������Ա
	protected:

		// ��ʼ��
		virtual BOOL OnInitDialog();
		// ��HOME����ʱ
		virtual int OnShellHomeKey(UINT, WPARAM, LPARAM);
		// ��Ϣ����
		virtual LRESULT MzDefWndProc(UINT, WPARAM, LPARAM);
		// ���˫��
		virtual void OnLButtonDblClk(UINT, int, int);
		// ����ƶ�ʱ
		virtual void OnMouseMove(UINT, int, int);		
		// ���̧��ʱ
		virtual void OnLButtonUp(UINT, int, int);
		// ��갴��ʱ
		virtual void  OnLButtonDown(UINT, int, int);

	// ˽�г�Ա
	private:

		



};

//************************************************************************************************
//*************************** �� �� �� ***********************************************************
//************************************************************************************************


// �� CMzApp ������Ӧ�ó�����
class MyApp : public CMzApp
{

	// ���г�Ա
	public:

		// ���캯��
		MyApp(void);
		// ��������
		~MyApp(void);

	// ������Ա
	protected:

		// ��ʼ��
		virtual BOOL Init(void);
		// ����
		virtual int Run(void);

	// ˽�г�Ա
	private:

		// �����ж�
		bool IsAppRunning(void);
		// ��Ϣ����
		void MessageRegister(void);

};

