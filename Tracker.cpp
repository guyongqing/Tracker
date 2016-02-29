
// Ԥ����ͷ
#include "stdafx.h"
// �����ļ�
#include "Tracker.h"

// ȫ��Ӧ�ó������
MyApp TheApp;
// ������
MyWindow TheWindow;

// FPSȫ�ֱ��� 
TrackerFps*	TheFPS;


// ����ȫ�ֱ���
HWND	TheHwnd;					// ���������

// ·��ȫ�ֱ���
TCHAR	ThePath[MAX_PATH];					// ·��
TCHAR	TheFileName[MAX_PATH];				// �ļ���

// DirectDraw ȫ�ֱ���

DDSURFACEDESC			TheDDrawSurface;		// DirectDraw ����ṹ
DDBLTFX					TheDDrawBltFx;			// DirectDraw BltFx�ṹ
DDPIXELFORMAT			TheDDrawPixelFormat;	// DirectDraw ���ظ�ʽ�ṹ

LPDIRECTDRAW			TheDDraw;				// DirectDraw ����
LPDIRECTDRAWCLIPPER		TheDDrawClipper;		// DirectDraw �ü���
LPDIRECTDRAWSURFACE		TheDDrawPrimary;		// DirectDraw ������
LPDIRECTDRAWSURFACE		TheDDrawBackBuffer;		// DirectDraw �󱸱���
LPDIRECTDRAWSURFACE		TheDDrawLodingBuffer;	// DirectDraw ���϶�ȡ�б���
LPDIRECTDRAWSURFACE		TheDDrawBackground;		// DirectDraw ��������
LPDIRECTDRAWSURFACE		TheDDrawForeground;		// DirectDraw ǰ������

// ������
IMixer*		TheMixer;

// ����ȫ�ֱ���
hssSpeaker	TheSpeaker;												// ������

// ���ֶ���
hssMusic	TheMusic[MUSIC_FILE_COUNT];								// ��������
hssChannel*	TheMusicChannel[MUSIC_FILE_COUNT];						// ����ͨ��

// ��������
hssSound	TheSound[SOUND_FILE_COUNT];								// ��������
hssChannel*	TheSoundChannel[SOUND_FILE_COUNT];						// ����ͨ��

int			TheSoundVolume = SOUND_VOLUME_DEFAULT;					// ��������
int			TheMusicVolume = MUSIC_VOLUME_DEFAULT;					// ������������

char		TheMusicBuffer[MUSIC_FILE_SIZE];						// �������ֻ���
char		TheSoundBuffer[SOUND_FILE_COUNT][SOUND_FILE_SIZE];		// ��������

// Ĭ�������ļ�
LPTSTR TheWaveFile[SOUND_FILE_COUNT];

// ������Ӧ
signed char	TheAxisX;  
signed char	TheAxisY;  
signed char TheAxisZ; 

// USB ����
int TheUsbNotify;
// ���紦��
UINT TheCallNotify;
// ���Ŵ���
UINT TheSmsNotify;
// ���Ŵ���
UINT TheMmsNotify;

// ������Ϣ
DWORD TheCallMessage;
// ������Ϣ
DWORD TheSmsMessage;
// ������Ϣ
DWORD TheMmsMessage;

// �Ƿ���ϵͳ��Ϣ
bool TheSystemMessage = false;

// ��Ļ����
RECT TheRectScreen;				// ȫ������
RECT TheRectTarget;				// Ŀ�����
RECT TheRectSource;				// ��Դ����
RECT TheRectDot[DOT_STYLE];		// Ч������

// ��Ϸȫ�ֱ���
bool TheGameLive = false;		// ��Ϸ�״̬
bool TheGamePause = false;		// ��Ϸ��ͣ״̬

// ��ǰ��Ϸ״̬
int TheGameStatus = GAME_STATUS_START;
// ��ǰ��Ϸ����
int TheGameStep = 0;
// ��ǰ��Ϸ˳��
int TheGameIndex = 0;
// ��ǰ��Ϸ���
int TheGameFoot = 5;

// ��ǰ����
int TheSamplesIndex = 0;

// �շ���֤
bool TheLicenseVerify = true;

// ���ݿ�������
bool TheDotStatus[DOT_COUNT];

// ����״̬����
int TheDotStep[DOT_COUNT];

//************************************************************************************************
//*************************** �ļ����� ***********************************************************
//************************************************************************************************

// ��ȡ����ǰĿ¼
void FileGetPath(void)
{

	// ��ȡ
	GetModuleFileName(NULL,ThePath,MAX_PATH);
	
	// ȥ���ļ���
	for(int i=MAX_PATH;i>0;i--){
		if(ThePath[i]=='\\'){
			break;
		}else{
			ThePath[i]='\0';
		}
	}

}

// ��ȡ�ļ�·��
void FileGetName(LPTSTR fileName)
{

	int j = 0;

	// ����·�����ļ���
	for(int i=0;i<MAX_PATH;i++){

		if(ThePath[i]=='\0'){

			TheFileName[i] = fileName[j];
			if(fileName[j]=='\0'){break;}	
			j++;
		
		}else{

			TheFileName[i] = ThePath[i];

		}

	}

}

// ��ȡ�ļ�����
bool FileGetData(LPTSTR fileName, char* fileBuffer, int fileSize)
{

	// ��ȡ�ļ���
	FileGetName(fileName);

	// �ļ����
	HANDLE hFile = CreateFile(TheFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	// ��ȡ�ֽ���
	DWORD dSize = 0;

	// �޷����ļ�
	if(hFile==INVALID_HANDLE_VALUE){
		return false;
	}

	// ��ȡ�ļ�
	if(!ReadFile(hFile,fileBuffer,fileSize,&dSize,NULL)){
		return false;
	}

	// �ر��ļ����
	CloseHandle(hFile);

	return true;

}

//************************************************************************************************
//*************************** ���������� *********************************************************
//************************************************************************************************

// ��ʼ��������
bool SpeakerInit(void)
{

	// ��ʼ��������
	if(TheSpeaker.open(SPEAKER_FREQMIX,SPEAKER_BPS,SPEAKER_STEREO,MUSIC_FILE_COUNT,SOUND_FILE_COUNT)!=HSS_OK){
		return false;
	}
	
	// ������������
	TheSpeaker.volumeMusics(TheMusicVolume);
	// ������������
	TheSpeaker.volumeSounds(TheSoundVolume);

	return true;

}

// ��������
void SpeakerSoundVolumeDown(int i)
{

	// ��������
	TheSoundVolume--;

	// ���С���������
	if(TheSoundVolume<SOUND_VOLUME_MIN){
		// ���ó��������
		TheSoundVolume = SOUND_VOLUME_MIN;
	}

	// ��������
	TheSpeaker.volumeSounds(TheSoundVolume);	
	// ���Ű�ť����
	TheSpeaker.playSound(&TheSound[SOUND_FILE_COUNT-1],SOUND_FILE_COUNT-1);

}

// ��������
void SpeakerSoundVolumeUp(int i)
{

	// ��������
	TheSoundVolume++;

	// ��������������
	if(TheSoundVolume>SOUND_VOLUME_MAX){
		// ���ó��������
		TheSoundVolume = SOUND_VOLUME_MAX;
	}

	// ��������
	TheSpeaker.volumeSounds(TheSoundVolume);	
	// ���Ű�ť����
	TheSpeaker.playSound(&TheSound[SOUND_FILE_COUNT-1],SOUND_FILE_COUNT-1);

}


// �ر��������豸
void SpeakerEnd(void)
{

	// �ر�����������
	TheSpeaker.close();

}

// ��������ͣ
void SpeakerPause(void)
{

	// ��ͣ����
	TheSpeaker.pauseMusics();
	// ��ͣ����
	TheSpeaker.pauseSounds();

}

// ��������
void SpeakerPlaySound(int i)
{
	
	// ͨ�����
	int channel = -1;

	// ֹͣ��ǰͨ���Ĳ���
	if(TheSoundChannel[i]){						
		TheSoundChannel[i]->stop();		
	}

	// ��ȡ�µ�ͨ�����
	channel = TheSpeaker.playSound(&TheSound[i],i);

	// ������ųɹ�
	if(channel>=0){
		// ��ȡ�µ�ͨ��ָ��
		TheSoundChannel[i] = TheSpeaker.channel(channel);
	}

}

// ����������
void SpeakerContinue(void)
{

	// ��������
	TheSpeaker.unpauseMusics();
	// ��������
	TheSpeaker.unpauseSounds();	

}

// ��ȡ�����ļ�
void SpeakerLoadingSound(LPTSTR soundFile, int loadingStep, int soundIndex)
{

	// ������ɽ�����һ��
	TheGameStep = loadingStep;

	// ��ȡ�����ļ�
	if(!FileGetData(soundFile,TheSoundBuffer[soundIndex],SOUND_FILE_SIZE)){		
		return;
	}

	// װ������
	if(TheSound[soundIndex].load(TheSoundBuffer[soundIndex],SOUND_FILE_SIZE,SOUND_FILE_COUNT)!=HSS_OK){		
		return;
	}

}

// ��ȡ�����ļ�
void SpeakerLoadingMusic(LPTSTR musicFile, int loadingStep, int musicIndex)
{

	// ��ȡ�����ļ�
	if(!FileGetData(musicFile,TheMusicBuffer,MUSIC_FILE_SIZE)){
		return;
	}

	// װ������
	if(TheMusic[musicIndex].load(TheMusicBuffer,MUSIC_FILE_SIZE,HSS_LOAD_DEFAULT)!=HSS_OK){		
		return;
	}

	// ������ɽ�����һ��
	TheGameStep = loadingStep;

}

// ��������������
void MixerVolumeDown(int i)
{

	int v = 0;		// ����
	int h = -1;		// �豸

	// ������״̬
	h = TheMixer->HeadphoneDetect();

	// �����������
	if(h==TURNON_HEADPHONE){

		// ��ȡ������������������
		v = TheMixer->GetHeadphoneVolume() - i;

	}else{

		// ��ȡ������������������
		v = TheMixer->GetSpeakerVolume() - i;

	}
	
	// �������С����Сֵ
	if(v<SOUND_VOLUME_MIN){
		v = SOUND_VOLUME_MIN;
	}

	// �����������
	if(h==TURNON_HEADPHONE){

		// ���ö�������
		TheMixer->SetHeadphoneVolume(v);

	}else{

		// ������������
		TheMixer->SetSpeakerVolume(v);

	}

}

// ��������������
void MixerVolumeUp(int i)
{
	
	int v = 0;		// ����
	int h = -1;		// �豸

	// ������״̬
	h = TheMixer->HeadphoneDetect();

	// �����������
	if(h==TURNON_HEADPHONE){

		// ��ȡ������������������
		v = TheMixer->GetHeadphoneVolume() + i;

	}else{

		// ��ȡ������������������
		v = TheMixer->GetSpeakerVolume() + i;

	}
	
	// ��������������ֵ
	if(v>SOUND_VOLUME_TOP){
		v = SOUND_VOLUME_TOP;
	}

	// �����������
	if(h==TURNON_HEADPHONE){

		// ���ö�������
		TheMixer->SetHeadphoneVolume(v);

	}else{

		// ������������
		TheMixer->SetSpeakerVolume(v);

	}

}

bool MixerInit(void)
{
	
	// ע�������COM�����������������
	if(FAILED(CoCreateInstance(CLSID_Mixer,NULL,CLSCTX_INPROC_SERVER,IID_MZ_Mixer,(void **)&TheMixer))){
		return false;
    }

	// �򿪻����豸
	if(!TheMixer->OpenMixerDevice(TheHwnd)){
		return false;
	}

	return true;

}

// ���û�����״̬
void MixerModeSetting(void)
{

	// ��ϵͳ��Ϣ
	if(TheSystemMessage){
		
		TheMixer->SetHeadphoneMode(HEADPHONE_ON);
		TheMixer->SetSpeakerMode(SPEAKER_AUTO);

		return;

	}

	// �����������
	if(TheMixer->HeadphoneDetect()==TURNON_HEADPHONE){

		TheMixer->SetHeadphoneMode(HEADPHONE_ON);
		TheMixer->SetSpeakerMode(SPEAKER_OFF);

	}else{

		TheMixer->SetHeadphoneMode(HEADPHONE_OFF);
		TheMixer->SetSpeakerMode(SPEAKER_AUTO);

	}

}

//************************************************************************************************
//*************************** DirectDraw *********************************************************
//************************************************************************************************


// ��ʼ��
bool DirectDrawInit(void)
{

	// ����DirectDraw����
	if(DirectDrawCreate(NULL,&TheDDraw,NULL)!=DD_OK){
		return false;
	}

	// ����Э����ʽ
	if(TheDDraw->SetCooperativeLevel(TheHwnd,DDSCL_NORMAL)!=DD_OK){
		return false;
	}

	/********************************������****************************************/

	// ��ʼ������ṹ
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	TheDDrawSurface.dwFlags = DDSD_CAPS;
	TheDDrawSurface.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	// ����������
	if(TheDDraw->CreateSurface(&TheDDrawSurface,&TheDDrawPrimary,NULL)!=DD_OK){
		return false;
	}

	// �������ظ�ʽ	
	DDRAW_INIT_STRUCT(TheDDrawPixelFormat);
	// ��ȡ���ظ�ʽ
	TheDDrawPrimary->GetPixelFormat(&TheDDrawPixelFormat);

	// �������16λɫģʽ
	if(TheDDrawPixelFormat.dwRGBBitCount!=SCREEN_COLOR){
		return false;
	}

	// �����ü���
	if(TheDDraw->CreateClipper(0,&TheDDrawClipper,NULL)!=DD_OK){
		return false;
	}
	
	// ���òü������ھ��
	if(TheDDrawClipper->SetHWnd(0,TheHwnd)!=DD_OK){
		return false;
	}

	// �����ü���
	if(TheDDrawPrimary->SetClipper(TheDDrawClipper)!=DD_OK){
		return false;
	}

	// �����󱸱���
	if(!DirectDrawCreateSurface(&TheDDrawBackBuffer,SCREEN_WIDTH,SCREEN_HEIGHT)){
		return false;
	}

	// ���϶�ȡ�б���
	if(!DirectDrawCreateSurface(&TheDDrawLodingBuffer,IMG_LOADING_WIDTH,IMG_LOADING_HEIGHT)){
		return false;
	}

	// ������������
	if(!DirectDrawCreateSurface(&TheDDrawBackground,IMG_BACKGROUND_WIDTH,IMG_BACKGROUND_HEIGHT)){
		return false;
	}

	// ����ǰ������
	if(!DirectDrawCreateSurface(&TheDDrawForeground,IMG_FOREGROUND_WIDTH,IMG_FOREGROUND_HEIGHT)){
		return false;
	}

	// װ������
	DirectDrawLoading();

	return true;

}

// ��������
bool DirectDrawCreateSurface(LPDIRECTDRAWSURFACE* s, int w, int h)
{

	// ��ʼ������ṹ
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// �������϶�ȡ�б������
	TheDDrawSurface.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	TheDDrawSurface.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
	TheDDrawSurface.dwWidth = w;
	TheDDrawSurface.dwHeight = h;

	// �������϶�ȡ�б���
	if(TheDDraw->CreateSurface(&TheDDrawSurface,s,NULL)!=DD_OK){
		return false;
	}

	return true;

}

// װ������
void DirectDrawLoading(void)
{

	// ������϶�ȡ�б���û�д���
	if(!TheDDrawLodingBuffer){
		return;
	}

	// ��ʼ������ṹ
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// ��������
	TheDDrawLodingBuffer->Lock(NULL,&TheDDrawSurface,DDLOCK_WAITNOTBUSY,NULL);

	USHORT *bb = (USHORT *)TheDDrawSurface.lpSurface;

	for(int i=0;i<IMG_LOADING_SIZE;i++){
		bb[i]=TheImageLoading[i]?COLOR_WHITE:0;
		
	}

	// ��������
	TheDDrawLodingBuffer->Unlock(NULL);

}

// ����
void DirectDrawEnd(void)
{

	// �ͷ�ǰ������
	if(TheDDrawForeground){
		TheDDrawForeground->Release();
		TheDDrawForeground = NULL;
	}

	// �ͷű�������
	if(TheDDrawBackground){
		TheDDrawBackground->Release();
		TheDDrawBackground = NULL;
	}

	// �ͷ����϶�ȡ�б���
	if(TheDDrawLodingBuffer){
		TheDDrawLodingBuffer->Release();
		TheDDrawLodingBuffer = NULL;
	}

	// �ͷź󱸱���
	if(TheDDrawBackBuffer){
		TheDDrawBackBuffer->Release();
		TheDDrawBackBuffer = NULL;
	}

	// �ͷ�������
	if(TheDDrawPrimary){
		TheDDrawPrimary->Release();
		TheDDrawPrimary = NULL;
	}

	// ���ټ��а�
	if(TheDDrawClipper){
		TheDDrawClipper->Release();
		TheDDrawClipper = NULL;
	}

	// �ͷ��ڲ�������Դ
	if(TheDDraw){
		TheDDraw->Release();
		TheDDraw = NULL;
	}

}

//************************************************************************************************
//*************************** �� Ȩ �� �� ********************************************************
//************************************************************************************************

// �շ�����Ϸ�����֤
bool LicenseVerify(void)
{

	TCHAR pszFileName[MAX_PATH] = {0};
    GetModuleFileName(0, pszFileName, MAX_PATH);            // ��ǰ����EXE���ļ���
    MYSTORE_VERIFY_CONTEXT mystore = {0};
    DWORD dwRet = MyStoreVerify(pszFileName, &mystore);     // ��֤���ļ��Ƿ�Ϸ�

	// ��֤�Ϸ�
	if(dwRet==0){

		// ����
		if(mystore.Expired){

			return false;
			
		}else{

			// ���
			if((mystore.Reserved[0]&0x40)==0x40){

				return false;
			
			// �շ�
			}else{

				return true;
			
			}
		
		}

	}

	return false;

}

//************************************************************************************************
//*************************** �� �� �� ȡ ********************************************************
//************************************************************************************************

// ��ȡ�����ļ�
void LoadingSoundFile(int soundIndex, int loadingStep)
{

	// ��ȡ�����ļ�
	SpeakerLoadingSound(TheWaveFile[soundIndex],loadingStep,soundIndex);

}

// ��ȡͼ���ļ�
void LoadingImageFile(LPTSTR imgFile, int imgSize, LPDIRECTDRAWSURFACE imgSurface, int loadingStep)
{

	bool flag = false;

	// ��ʼ������ṹ
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// �������Ǳ���
	imgSurface->Lock(NULL,&TheDDrawSurface,DDLOCK_WAITNOTBUSY,NULL);

	// ��������
	CHAR *bb = (CHAR *)TheDDrawSurface.lpSurface;

	// ��ȡ�ļ�
	flag = FileGetData(imgFile,bb,imgSize);

	// �������Ǳ���
	imgSurface->Unlock(NULL);

	// ������ɽ�����һ��
	TheGameStep = loadingStep;

	// ��ȡʧ��
	if(!flag){
		GameError(GAME_ERROR_FILE);
	}

}

//************************************************************************************************
//*************************** ������� ***********************************************************
//************************************************************************************************

// ����Ļ
void DrawCls(int color)
{
	// ���ƽ�����
	DrawRect(TheDDrawLodingBuffer,5,0,6,1,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

}

// ����������
void DrawPrimary(void)
{

	// ���󱸱��渴�Ƶ�������
	TheDDrawPrimary->Blt(&TheRectScreen,TheDDrawBackBuffer,&TheRectScreen,DDBLT_WAITNOTBUSY,NULL);

}

// ���Ʊ�־
void DrawLogo(void)
{

	// ��ȡ��־ͼƬ
	switch(TheGameStep){
		
		case 0:
			// ��ȡ����ͼ���ļ�
			LoadingImageFile(IMG_BACKGROUND_FILE, IMG_BACKGROUND_SIZE, TheDDrawBackground, 1);
			break;
		
		case 1:
			// ��ȡǰ��ͼ���ļ�
			LoadingImageFile(IMG_FOREGROUND_FILE, IMG_FOREGROUND_SIZE, TheDDrawForeground, 2);
			break;

		case 2:			
			// ��ȡ�����ļ�
			LoadingSoundFile(0,3);
			break;

		case 3:
			// ��ȡ�����ļ�
			LoadingSoundFile(1,4);
			break;

		case 4:
			// ��ȡ�����ļ�
			LoadingSoundFile(2,5);
			break;

		case 5:
			// ��ȡ�����ļ�
			LoadingSoundFile(3,6);
			break;

		case 6:
			// ��ȡ�����ļ�
			LoadingSoundFile(4,7);
			break;

		case 7:
			// ��ȡ�����ļ�
			LoadingSoundFile(5,8);
			break;

		case 8:
			// ��ȡ�����ļ�
			LoadingSoundFile(6,9);
			break;

		case 9:
			// ��ȡ�����ļ�
			LoadingSoundFile(7,10);
			break;

		case 10:
			// ��ȡ�����ļ�
			LoadingSoundFile(8,11);
			break;

		case 11:
			// ��ȡ�����ļ�
			LoadingSoundFile(9,12);	
			break;

		case 12:
			// ��ȡ�����ļ�
			SpeakerLoadingSound(SOUND_BUTTON_FILE,13,SOUND_FILE_COUNT-1);
			break;

	}

	// ����LOADING״̬
	if(TheGameStep<13){

		// �������϶�ȡ�л���
		DrawLoading(TheGameStep<<3);

	}else{

		// ���ͨ���շ���֤
		if(TheLicenseVerify){

			// ���̽�����������һ��
			GameStatus(GAME_STATUS_RUN);
		
		}else{

			// ���̽�����������һ��
			GameStatus(GAME_STATUS_MENU);
		
		}
		

	}

}

// ���ƾ���
void DrawRect(LPDIRECTDRAWSURFACE ddSurface, int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	
	// Դ����
	SetRect(&TheRectSource,x1,y1,x2,y2);
	// Ŀ�껭��
	SetRect(&TheRectTarget,x3,y3,x4,y4);
	// ����Դ�����������󱸻���
	TheDDrawBackBuffer->Blt(&TheRectTarget,ddSurface,&TheRectSource,DDBLT_WAITNOTBUSY,NULL);

}

// �������϶�ȡ�л���
void DrawLoading(int flag)
{

	// ��������
	DrawRect(TheDDrawLodingBuffer,0,0,IMG_LOADING_WIDTH,IMG_LOADING_HEIGHT,IMG_LOADING_X,IMG_LOADING_Y,IMG_LOADING_WIDTH + IMG_LOADING_X,IMG_LOADING_HEIGHT + IMG_LOADING_Y);
	// �������
	DrawRect(TheDDrawLodingBuffer,0,0,1,1,IMG_LOADING_X - 20,150,IMG_LOADING_X - 10,SCREEN_HEIGHT - 150);
	// �����ڿ�
	DrawRect(TheDDrawLodingBuffer,1,1,2,2,IMG_LOADING_X - 19,151,IMG_LOADING_X - 11,SCREEN_HEIGHT - 151);
	// ���ƽ�����
	DrawRect(TheDDrawLodingBuffer,0,0,1,1,IMG_LOADING_X - 18,152,IMG_LOADING_X - 12,152 + (flag<<2));

}

// ��Ϸ����ʱ
void DrawRun(void)
{
	// ����Դ�����������󱸻���
	TheDDrawBackBuffer->Blt(&TheRectScreen,TheDDrawBackground,&TheRectScreen,DDBLT_WAITNOTBUSY,NULL);

	for(int i=0;i<DOT_COUNT;i++){

		// �������
		if(TheDotStatus[i]){
			DrawDot(i);
		}

	}

	// ��������
	GamePlaySound();

	// ������Ӧ����
	AccRun();

}

// ����
void DrawDot(int i)
{

	// ��������
	int x = i % DOT_WIDTH;
	int y = i / DOT_WIDTH;
	int xx = (x << 5) + (x << 4);
	int yy = (y << 5) + (y << 4);

	// Ŀ�껭��
	SetRect(&TheRectTarget,xx,yy,xx+DOT_SIZE,yy+DOT_SIZE);
	// ����Դ�����������󱸻���
	TheDDrawBackBuffer->Blt(&TheRectTarget,TheDDrawForeground,&TheRectDot[TheDotStep[i]],DDBLT_WAITNOTBUSY,NULL);

	// ����������ջ���
	if(TheDotStep[i]>0){
		TheDotStep[i]--;
	}

}

// ��Ϸ����
void Draw(void)
{

	// ����
	DrawCls(COLOR_BLACK);

	// �ж���Ϸ״̬
	switch(TheGameStatus)
	{

		// �������ʾ��־״̬
		case GAME_STATUS_LOGO:

			// ���Ʊ�־
			DrawLogo();
			break;

		// ��Ϸ����״̬
		case GAME_STATUS_RUN:
			// ������Ϸ����ʱ
			DrawRun();
			break;

		// ��ʾ�˵�ʱ
		case GAME_STATUS_MENU:
			DrawMenu();
			break;
	
	}

	// ������ͣ����
	if(TheGamePause){
		DrawPause();
	}

	// ��ʾ������
	DrawPrimary();

}

// ���Ʋ˵�
void DrawMenu(void)
{
	
	// ������ʾ��Ϣ
	DrawRect(TheDDrawForeground,0,60,90,370,180,200,270,510);
	

}

// ������ͣ����
void DrawPause(void)
{


}

//************************************************************************************************
//*************************** �� Ϸ �� �� ********************************************************
//************************************************************************************************

// ��������
void GamePlaySound(void)
{
	
	// ���
	int index = 0;

	// ���ʱ�䵽
	if(TheGameStep==0){
	
		// ���ŵ�ǰ��
		for(int i=0;i<DOT_WIDTH;i++){
			
			// ��õ�ǰ���
			index = i + (TheGameIndex << 3) + (TheGameIndex << 1);

			// �����ǰ��ŵ���
			if(TheDotStatus[index]){

				// ���õ�ǰ��Ϊ����
				TheDotStep[index] = DOT_STYLE - 1;
				// ��������
				SpeakerPlaySound(i);

			}

		}

		// ������һ��
		TheGameIndex++;

		// ��������������
		if(TheGameIndex>=DOT_HEIGHT){

			// �ص���һ��
			TheGameIndex = 0;

		}

	}

	// ����ǰ��
	TheGameStep++;

	// ���������ಽ��
	if(TheGameStep>TheGameFoot){

		// �ص���һ��
		TheGameStep = 0;

	}

}

// ��Ϸ����
void GameError(LPTSTR msg)
{

	// ��Ϸ��ͣ
	GamePause();

	// ��Ϸ����
	GameEnd();

	// ��ʾ������Ϣ
	MzMessageBox(0,msg,GAME_ERROR,MB_OK,0,false);

	// �˳�����
	PostQuitMessage(0);

}

// ���þ���
void GameRectInit(void)
{

	// �趨��Ļ����
	SetRect(&TheRectScreen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);	

	// ���þ���
	SetRect(&TheRectDot[0],0,0,48,48);
	SetRect(&TheRectDot[1],48,0,96,48);
	SetRect(&TheRectDot[2],96,0,144,48);
	SetRect(&TheRectDot[3],144,0,192,48);
	SetRect(&TheRectDot[4],192,0,240,48);

	TheWaveFile[0] = L"25118000.dat";
	TheWaveFile[1] = L"25118001.dat";
	TheWaveFile[2] = L"25118002.dat";
	TheWaveFile[3] = L"25118003.dat";
	TheWaveFile[4] = L"25118004.dat";
	TheWaveFile[5] = L"25118005.dat";
	TheWaveFile[6] = L"25118006.dat";
	TheWaveFile[7] = L"25118007.dat";
	TheWaveFile[8] = L"25118008.dat";
	TheWaveFile[9] = L"25118009.dat";

	memset(TheDotStatus,0,sizeof(TheDotStatus));
	memset(TheDotStep,0,sizeof(TheDotStep));

}

// ��Ϸ��ʼ��
bool GameInit(void)
{

	// ����û����ȷ����
	if(!TheHwnd){
		return false;
	}

	// ���þ���
	GameRectInit();

	// ��ʼ�� DirectDraw ʧ��
	if(!DirectDrawInit()){
		return false;
	}

	
	// ����������
	SpeakerInit();
	
	// ����acc����		
	MzAccOpen();

	// ��ʼ���ɹ���������ʾ��־����
	GameStatus(GAME_STATUS_LOGO);

	// ������Ϸ
	TheGameLive = true;

	return true;

}

// ��Ϸ��ʼ
void GameRun(void)
{

	
	// ���ƻ���
	Draw();

}

// ������Ϸ
void GameContinue(void)
{

	// ��ͣ��Ϸ
	TheGamePause = false;
	// ����������
	SpeakerContinue();

}		

// ��ͣ��Ϸ
void GamePause(void)
{

	// ��ͣ��Ϸ
	TheGamePause = true;	
	// ��ͣ������
	SpeakerPause();

}

// ֹͣ��Ϸ
void GameDead(void)
{

	// �ر���Ϸ״̬
	TheGameLive = false;	
	// ��ͣ��Ϸ
	GamePause();
	// ���SHELL�����������Ƽ�
	UnHoldShellUsingSomeKeyFunction(TheHwnd, MZ_HARDKEY_VOLUME_UP | MZ_HARDKEY_VOLUME_DOWN);			
	// ��ʾ����
	ShowMzTopBar();
	// ������״̬����
	MixerModeSetting();

}

// ��Ϸ����
void GameEnd(void)
{

	// ֹͣ��Ϸ
	GameDead();	
	// ��������������
	SpeakerEnd();
	// �ر�acc����
	MzAccClose();
	// ���� DDraw ����
	DirectDrawEnd();	

	// �ͷŻ���������
	if(TheMixer!=NULL){
		TheMixer->Release();
	}

}

// ��Ϸ����
void GameLive(void)
{

	// ���ھ��
	if(TheHwnd){

		// û��ϵͳ��Ϣ
		TheSystemMessage = false;
		// ����Ϸ״̬
		TheGameLive = true;				
		// ��ֹSHELL�����������Ƽ�
		HoldShellUsingSomeKeyFunction(TheHwnd, MZ_HARDKEY_VOLUME_UP | MZ_HARDKEY_VOLUME_DOWN);
		// ���ض���
		HideMzTopBar();
		// ������״̬����
		MixerModeSetting();
		
	}

}

// ������Ϸ״̬
void GameStatus(int s)
{

	// ������Ϸ״̬
	TheGameStatus = s;
	TheGameStep = 0;
	TheGameIndex = 0;

}

//************************************************************************************************
//*************************** ������Ӧ ***********************************************************
//************************************************************************************************

// ������Ӧ����
void AccRun(void)
{

	// ��ȡ������Ӧ���ٶ�
	MzAccGetXYZ(&TheAxisX,&TheAxisY,&TheAxisZ);

	// ����
	if(TheAxisX>ACC_MAX||TheAxisY>ACC_MAX){
		memset(TheDotStatus,0,sizeof(TheDotStatus));
	}

}


//************************************************************************************************
//*************************** ��괦�� ***********************************************************
//************************************************************************************************

// ���̧��ʱ
void MouseUp(int x, int y)
{

	// ��Ϸ״̬
	switch(TheGameStatus){

		// ����״̬
		case GAME_STATUS_RUN:
			MouseUpRun(x,y);
			break;

		// �˵�״̬
		case GAME_STATUS_MENU:
			// ���̽�����������һ��
			GameStatus(GAME_STATUS_RUN);
			break;

	}

}

// ���˫��ʱ
void MouseClick(int x, int y)
{

	/*

	// �շ���֤
	if(!TheLicenseVerify){
		return;
	}

	// ��Ϸ����ʱ
	if(TheGameStatus==GAME_STATUS_RUN){

		if(x<241){

			// �ٶȼ���
			if(MouseArea(x,y,0,0,240,240)){
				TheGameFoot++;
			}

			// �ָ���ʼ�ٶ�
			if(MouseArea(x,y,0,240,240,480)){
				TheGameFoot = 5;
			}

			// �ӿ��ٶ�
			if(MouseArea(x,y,0,480,240,720)){
				TheGameFoot--;
			}

			if(TheGameFoot<0){
				TheGameFoot = 0;
			}

			if(TheGameFoot>15){
				TheGameFoot = 15;
			}

		}else{

			// ������ǰ
			if(MouseArea(x,y,240,0,480,240)){
				TheSamplesIndex--;				
			}

			// �ָ�Ĭ�ϲ���
			if(MouseArea(x,y,240,240,480,480)){
				TheSamplesIndex = 0;
			}

			// �ӿ��ٶ�
			if(MouseArea(x,y,240,480,480,720)){
				TheSamplesIndex++;				
			}

			if(TheSamplesIndex<0){
				TheSamplesIndex = SOUND_SAMPLE_COUNT - 1;
			}

			if(TheSamplesIndex>=SOUND_SAMPLE_COUNT){
				TheSamplesIndex = 0;
			}

			// ֹͣ��������
			TheSpeaker.stopSounds();

			// ���¼�������
			for(int i=0;i<10;i++){
				LoadingSoundFile(i,0,TheSamplesIndex);
			}
		
		}

	}

	*/

}

// �������λ��
bool MouseArea(int x, int y, int x1, int y1, int x2, int y2)
{

	if((x>x1)&&(x<x2)){

		if((y>y1)&&(y<y2)){

			return true;

		}
	
	}


	return false;

}

// ��Ϸ����ʱ������
void MouseUpRun(int x, int y)
{

	// ����ڼ���
	int dotX = x / DOT_SIZE;
	// ����ڼ���
	int dotY = y / DOT_SIZE;
	// ���˳���
	int dot = dotX + (dotY << 3) + (dotY << 1);
	
	// ���ŵڼ��е�����
	if(TheDotStatus[dot]){
		TheDotStatus[dot] = false;
	}else{
		TheDotStatus[dot] = true;		
	}

	// ����״̬
	TheDotStep[dot] = DOT_STYLE - 1;

}

//************************************************************************************************
//*************************** �� �� �� ***********************************************************
//************************************************************************************************

// MZFC�����༰������Ĵ������������
MZ_IMPLEMENT_DYNAMIC(MyWindow)

// ���캯��
MyWindow::MyWindow(void)
{
}

// ��������
MyWindow::~MyWindow(void)
{
}

// ��ʼ��
BOOL MyWindow::OnInitDialog()
{
	
	// ������ִ�и�����ĳ�ʼ������
	if (!CMzWndEx::OnInitDialog())
	{
		return false;
	}

	// �ɹ�
	return true;

}

// ��HOME��������ʱ
int MyWindow::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{

	// ���˳�������ʾ����
	// return SHK_RET_APPNOEXIT_SHELLTOP;

	// Ĭ���˳�
	return SHK_RET_DEFAULT;

}

// ���˫��
void MyWindow::OnLButtonDblClk(UINT fwKeys, int xPos, int yPos)
{

	// ��������¼�
	MouseClick(xPos,yPos);

}

// ���̧��ʱ
void MyWindow::OnLButtonUp(UINT fwKeys, int xPos, int yPos) 
{

	// ��������¼�
	MouseUp(xPos,yPos);

}

// ����ƶ�ʱ
void MyWindow::OnMouseMove(UINT fwKeys, int xPos, int yPos) 
{

}

// ��갴��ʱ
void  MyWindow::OnLButtonDown(UINT fwKeys, int xPos, int yPos) 
{

}

// ��Ϣ����
LRESULT MyWindow::MzDefWndProc(UINT message, WPARAM  wParam, LPARAM  lParam) 
{

	// ĳЩ״̬
	int id = int(lParam);

	// ������Ϣ
	switch(message){

		// ���������¼�
		case MM_MIXM_CONTROL_CHANGE:
			// ����״̬�б仯
			if((id==HEADPHONE_MODE_CHANGE_CONTROL_ID)||(id==CHANGE_HEADPHONE_TYPE_CONTROL_ID)){		
				// ���û�����״̬
				MixerModeSetting();
			}
			break;

		// ��������¼�
		case WM_KEYUP:

			// ��������		
			if(wParam==VK_VOLUME_DOWN){
				// ��������				
				SpeakerSoundVolumeDown(SOUND_VOLUME_STEP);
				// ��������������
				MixerVolumeDown(SOUND_VOLUME_STEP);
			}

			// ��������
			if(wParam==VK_VOLUME_UP){
				// ��������
				SpeakerSoundVolumeUp(SOUND_VOLUME_STEP);
				// ��������������
				MixerVolumeUp(SOUND_VOLUME_STEP);
				break;

			}
			break;

		// �õ�����
		case WM_SETFOCUS:
			
			// ������Ϸ
			GameLive();
			break;

		// ʧȥ����	
		case WM_KILLFOCUS:

			// ֹͣ��Ϸ
			GameDead();
			break;

		// �����������Ϣ
		case MZ_WM_TASKMAN_ONOFF:

			// ��ʾ�����������Ϣ
			if(lParam==TASKMAN_SHOW_WND){
				// ֹͣ��Ϸ
				GameDead();
			}

			// ���������������
			if(lParam==TASKMAN_EXEC_PROG){
				// ������Ϸ
				GameLive();
			}

			// ���������������Ϣ
			if(lParam==TASKMAN_QUIT_WND){
				if (GetForegroundWindow()==m_hWnd){
					// ������Ϸ
					GameLive();
				}
			}

			break;


	}

	// U���¼�
	if(message==TheUsbNotify){

		// U�̽���
		if(wParam==USB_MASSSTORAGE_ATTACH){
			PostQuitMessage(0);
		}
	
	}

	// �����¼�,����Ϣ�¼�,�����¼�
	if((message==TheCallNotify)||(message==TheSmsNotify)||(message==TheMmsNotify)){
	
		// ��ϵͳ��Ϣ
		TheSystemMessage = true;

	}

	// ������Ϣ,����Ϣ��Ϣ,������Ϣ
	if((message==TheCallMessage)||(message==TheSmsMessage)||(message==TheMmsMessage)){
	
		// ��ϵͳ��Ϣ
		TheSystemMessage = true;

	}

	// ����ϵͳĬ����Ϣ������
	return CMzWndEx::MzDefWndProc(message,wParam,lParam);

}

// ��������
bool MyWindow::CreateWindowNow(void)
{

	// ����������
	Create(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, 0, 0, 0);
	SetWindowText(APP_WINDOW_TEXT);
	SetBgColor(COLOR_BLACK);

	// ���ڶ���Ч��
	if(MzGetParam(MZGP_APP_START_ANIMATION)){
		AnimateWindow(MZ_ANIMTYPE_ZOOM_IN,true);
	}

	// ��ʾ������
	Show();

	// ���ھ��
	TheHwnd = m_hWnd;

	if(TheHwnd<=0){
		return false;
	}

	return true;

}

//************************************************************************************************
//*************************** �� �� �� ***********************************************************
//************************************************************************************************


// ���캯��
MyApp::MyApp(void)
{
}

// ��������
MyApp::~MyApp(void)
{
}

// ��Ϣ����
void MyApp::MessageRegister(void)
{

	// ע��USB��Ϣ
	TheUsbNotify = RegisterUsbNotifyMsg();

	// �����¼�
	TheCallNotify = RegisterWindowMessage(WM_CALL_MESSAGE);
	// ����Ϣ�¼�
	TheSmsNotify = RegisterWindowMessage(WM_SMS_MESSAGE);
	// �����¼�
	TheMmsNotify = RegisterWindowMessage(WM_MMS_MESSAGE);

	// ������Ϣ
	TheCallMessage = GetCallRegisterMessage();
	// ������Ϣ
	TheSmsMessage = GetSmsRegisterMessage();
	// ������Ϣ
	TheMmsMessage = GetMmsRegisterMessage();

}

// �����ж�
bool MyApp::IsAppRunning(void)
{

	//����Ƿ��Ѿ��������ʵ����������:
    bool bFound = false;

	// ��ȡ���
    HANDLE hMutexOneInstantance = CreateMutex(NULL, true, APP_MUTEX_NAME);

	// �ҵ�ʵ��
    if (GetLastError()==ERROR_ALREADY_EXISTS){
		bFound = true;
    }

	// �ͷž��
    if(hMutexOneInstantance){
		ReleaseMutex(hMutexOneInstantance);
    }

	// ����ҵ�ʵ��
	if(bFound){
		
		//�����Ѿ������е�ʵ���Ĵ���   
		HWND pwndFirst = FindWindow((LPCTSTR)MyWindow::m_pwszMyWindowClassName, (LPCTSTR)NULL);

		// �Ѿ��ҵ�����ʵ��
		if(pwndFirst){

			// ������ڱ�����
            if(!IsWindowVisible(pwndFirst)){
				// ��ʾ����
				ShowWindow(pwndFirst, SW_SHOW);
            }

			// �����
			SetForegroundWindow(pwndFirst);

        }

	}

	return bFound;

}

// ��ʼ��
BOOL MyApp::Init()
{

	// ����Ѿ�����
	if(IsAppRunning()){
		return false;
	}

	// ���ض���
	HideMzTopBar();

	// ע��COM
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// ���������豸
	if(!MixerInit()){
		return false;
	}
	
	// ���ڴ���ʧ��
	if(!TheWindow.CreateWindowNow()){
		return false;
	}

	// ע�������Ϣ
	MessageRegister();

	// ��ȡ��ǰ��������Ŀ¼
	FileGetPath();

	// �ɹ�
	return true;

}

// ����
int MyApp::Run()
{

	MSG msg;

	// ��ʼ����Ϸ
	if(!GameInit()){
		GameError(GAME_ERROR_INIT);
		return false;
	}

	// ������Ϸ
	GameLive();

	// �����Ȩ����
	TheLicenseVerify = true;// LicenseVerify();

	// ����FPS����
	TheFPS = new TrackerFps();

	// ��ʼFPS��ʱ
	TheFPS->Start(FPS);

	// ����Ϣѭ��
	while(true){

		// ������Ϣ
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){

			// �˳�
			if(msg.message==WM_QUIT){
				break;
			}

			// ����������Ϣ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
		}

		// ��Ϸ����״̬
		if(TheGameLive){

			// ������Ϸ
			GameRun();

			// �ȴ���һ֡
			TheFPS->Wait();

		}

		// �ó�����
		Sleep(0);
	
	}

	// ��Ϸ����
	GameEnd();

	return true;

}