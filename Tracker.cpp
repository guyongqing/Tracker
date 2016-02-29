
// 预编译头
#include "stdafx.h"
// 引用文件
#include "Tracker.h"

// 全局应用程序对象
MyApp TheApp;
// 主窗口
MyWindow TheWindow;

// FPS全局变量 
TrackerFps*	TheFPS;


// 窗口全局变量
HWND	TheHwnd;					// 命令栏句柄

// 路径全局变量
TCHAR	ThePath[MAX_PATH];					// 路径
TCHAR	TheFileName[MAX_PATH];				// 文件名

// DirectDraw 全局变量

DDSURFACEDESC			TheDDrawSurface;		// DirectDraw 表面结构
DDBLTFX					TheDDrawBltFx;			// DirectDraw BltFx结构
DDPIXELFORMAT			TheDDrawPixelFormat;	// DirectDraw 像素格式结构

LPDIRECTDRAW			TheDDraw;				// DirectDraw 界面
LPDIRECTDRAWCLIPPER		TheDDrawClipper;		// DirectDraw 裁剪器
LPDIRECTDRAWSURFACE		TheDDrawPrimary;		// DirectDraw 主表面
LPDIRECTDRAWSURFACE		TheDDrawBackBuffer;		// DirectDraw 后备表面
LPDIRECTDRAWSURFACE		TheDDrawLodingBuffer;	// DirectDraw 资料读取中表面
LPDIRECTDRAWSURFACE		TheDDrawBackground;		// DirectDraw 背景表面
LPDIRECTDRAWSURFACE		TheDDrawForeground;		// DirectDraw 前景表面

// 混音器
IMixer*		TheMixer;

// 声音全局变量
hssSpeaker	TheSpeaker;												// 扬声器

// 音乐对象
hssMusic	TheMusic[MUSIC_FILE_COUNT];								// 音乐数量
hssChannel*	TheMusicChannel[MUSIC_FILE_COUNT];						// 音乐通道

// 声音对象
hssSound	TheSound[SOUND_FILE_COUNT];								// 声音数量
hssChannel*	TheSoundChannel[SOUND_FILE_COUNT];						// 声音通道

int			TheSoundVolume = SOUND_VOLUME_DEFAULT;					// 声音音量
int			TheMusicVolume = MUSIC_VOLUME_DEFAULT;					// 背景音乐音量

char		TheMusicBuffer[MUSIC_FILE_SIZE];						// 背景音乐缓冲
char		TheSoundBuffer[SOUND_FILE_COUNT][SOUND_FILE_SIZE];		// 声音缓冲

// 默认声音文件
LPTSTR TheWaveFile[SOUND_FILE_COUNT];

// 重力感应
signed char	TheAxisX;  
signed char	TheAxisY;  
signed char TheAxisZ; 

// USB 处理
int TheUsbNotify;
// 来电处理
UINT TheCallNotify;
// 短信处理
UINT TheSmsNotify;
// 彩信处理
UINT TheMmsNotify;

// 来电消息
DWORD TheCallMessage;
// 短信消息
DWORD TheSmsMessage;
// 彩信消息
DWORD TheMmsMessage;

// 是否有系统消息
bool TheSystemMessage = false;

// 屏幕矩阵
RECT TheRectScreen;				// 全屏矩阵
RECT TheRectTarget;				// 目标矩阵
RECT TheRectSource;				// 来源矩阵
RECT TheRectDot[DOT_STYLE];		// 效果矩阵

// 游戏全局变量
bool TheGameLive = false;		// 游戏活动状态
bool TheGamePause = false;		// 游戏暂停状态

// 当前游戏状态
int TheGameStatus = GAME_STATUS_START;
// 当前游戏步伐
int TheGameStep = 0;
// 当前游戏顺序
int TheGameIndex = 0;
// 当前游戏跨度
int TheGameFoot = 5;

// 当前采样
int TheSamplesIndex = 0;

// 收费验证
bool TheLicenseVerify = true;

// 灯泡开关数组
bool TheDotStatus[DOT_COUNT];

// 灯泡状态数组
int TheDotStep[DOT_COUNT];

//************************************************************************************************
//*************************** 文件操作 ***********************************************************
//************************************************************************************************

// 获取程序当前目录
void FileGetPath(void)
{

	// 获取
	GetModuleFileName(NULL,ThePath,MAX_PATH);
	
	// 去掉文件名
	for(int i=MAX_PATH;i>0;i--){
		if(ThePath[i]=='\\'){
			break;
		}else{
			ThePath[i]='\0';
		}
	}

}

// 获取文件路径
void FileGetName(LPTSTR fileName)
{

	int j = 0;

	// 复制路径到文件名
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

// 读取文件数据
bool FileGetData(LPTSTR fileName, char* fileBuffer, int fileSize)
{

	// 获取文件名
	FileGetName(fileName);

	// 文件句柄
	HANDLE hFile = CreateFile(TheFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	// 读取字节数
	DWORD dSize = 0;

	// 无法打开文件
	if(hFile==INVALID_HANDLE_VALUE){
		return false;
	}

	// 读取文件
	if(!ReadFile(hFile,fileBuffer,fileSize,&dSize,NULL)){
		return false;
	}

	// 关闭文件句柄
	CloseHandle(hFile);

	return true;

}

//************************************************************************************************
//*************************** 扬声器处理 *********************************************************
//************************************************************************************************

// 初始化扬声器
bool SpeakerInit(void)
{

	// 初始化扬声器
	if(TheSpeaker.open(SPEAKER_FREQMIX,SPEAKER_BPS,SPEAKER_STEREO,MUSIC_FILE_COUNT,SOUND_FILE_COUNT)!=HSS_OK){
		return false;
	}
	
	// 设置音乐音量
	TheSpeaker.volumeMusics(TheMusicVolume);
	// 设置声音音量
	TheSpeaker.volumeSounds(TheSoundVolume);

	return true;

}

// 降低音量
void SpeakerSoundVolumeDown(int i)
{

	// 降低音量
	TheSoundVolume--;

	// 如果小于最低音量
	if(TheSoundVolume<SOUND_VOLUME_MIN){
		// 设置成最低音量
		TheSoundVolume = SOUND_VOLUME_MIN;
	}

	// 设置音量
	TheSpeaker.volumeSounds(TheSoundVolume);	
	// 播放按钮声音
	TheSpeaker.playSound(&TheSound[SOUND_FILE_COUNT-1],SOUND_FILE_COUNT-1);

}

// 升高音量
void SpeakerSoundVolumeUp(int i)
{

	// 升高音量
	TheSoundVolume++;

	// 如果大于最高音量
	if(TheSoundVolume>SOUND_VOLUME_MAX){
		// 设置成最高音量
		TheSoundVolume = SOUND_VOLUME_MAX;
	}

	// 设置音量
	TheSpeaker.volumeSounds(TheSoundVolume);	
	// 播放按钮声音
	TheSpeaker.playSound(&TheSound[SOUND_FILE_COUNT-1],SOUND_FILE_COUNT-1);

}


// 关闭扬声器设备
void SpeakerEnd(void)
{

	// 关闭扬声器对象
	TheSpeaker.close();

}

// 扬声器暂停
void SpeakerPause(void)
{

	// 暂停音乐
	TheSpeaker.pauseMusics();
	// 暂停声音
	TheSpeaker.pauseSounds();

}

// 播放声音
void SpeakerPlaySound(int i)
{
	
	// 通道编号
	int channel = -1;

	// 停止当前通道的播放
	if(TheSoundChannel[i]){						
		TheSoundChannel[i]->stop();		
	}

	// 获取新的通道编号
	channel = TheSpeaker.playSound(&TheSound[i],i);

	// 如果播放成功
	if(channel>=0){
		// 获取新的通道指针
		TheSoundChannel[i] = TheSpeaker.channel(channel);
	}

}

// 扬声器继续
void SpeakerContinue(void)
{

	// 继续音乐
	TheSpeaker.unpauseMusics();
	// 继续声音
	TheSpeaker.unpauseSounds();	

}

// 读取声音文件
void SpeakerLoadingSound(LPTSTR soundFile, int loadingStep, int soundIndex)
{

	// 任务完成进行下一步
	TheGameStep = loadingStep;

	// 读取声音文件
	if(!FileGetData(soundFile,TheSoundBuffer[soundIndex],SOUND_FILE_SIZE)){		
		return;
	}

	// 装载音乐
	if(TheSound[soundIndex].load(TheSoundBuffer[soundIndex],SOUND_FILE_SIZE,SOUND_FILE_COUNT)!=HSS_OK){		
		return;
	}

}

// 读取音乐文件
void SpeakerLoadingMusic(LPTSTR musicFile, int loadingStep, int musicIndex)
{

	// 读取音乐文件
	if(!FileGetData(musicFile,TheMusicBuffer,MUSIC_FILE_SIZE)){
		return;
	}

	// 装载音乐
	if(TheMusic[musicIndex].load(TheMusicBuffer,MUSIC_FILE_SIZE,HSS_LOAD_DEFAULT)!=HSS_OK){		
		return;
	}

	// 任务完成进行下一步
	TheGameStep = loadingStep;

}

// 混音器音量降低
void MixerVolumeDown(int i)
{

	int v = 0;		// 音量
	int h = -1;		// 设备

	// 检测耳机状态
	h = TheMixer->HeadphoneDetect();

	// 如果耳机插入
	if(h==TURNON_HEADPHONE){

		// 获取耳机音量并减低音量
		v = TheMixer->GetHeadphoneVolume() - i;

	}else{

		// 获取喇叭音量并减低音量
		v = TheMixer->GetSpeakerVolume() - i;

	}
	
	// 如果音量小于最小值
	if(v<SOUND_VOLUME_MIN){
		v = SOUND_VOLUME_MIN;
	}

	// 如果耳机插入
	if(h==TURNON_HEADPHONE){

		// 设置耳机音量
		TheMixer->SetHeadphoneVolume(v);

	}else{

		// 设置喇叭音量
		TheMixer->SetSpeakerVolume(v);

	}

}

// 混音器音量升高
void MixerVolumeUp(int i)
{
	
	int v = 0;		// 音量
	int h = -1;		// 设备

	// 检测耳机状态
	h = TheMixer->HeadphoneDetect();

	// 如果耳机插入
	if(h==TURNON_HEADPHONE){

		// 获取耳机音量并升高音量
		v = TheMixer->GetHeadphoneVolume() + i;

	}else{

		// 获取喇叭音量并升高音量
		v = TheMixer->GetSpeakerVolume() + i;

	}
	
	// 如果音量大于最大值
	if(v>SOUND_VOLUME_TOP){
		v = SOUND_VOLUME_TOP;
	}

	// 如果耳机插入
	if(h==TURNON_HEADPHONE){

		// 设置耳机音量
		TheMixer->SetHeadphoneVolume(v);

	}else{

		// 设置喇叭音量
		TheMixer->SetSpeakerVolume(v);

	}

}

bool MixerInit(void)
{
	
	// 注册混音器COM组件，用来控制音量
	if(FAILED(CoCreateInstance(CLSID_Mixer,NULL,CLSCTX_INPROC_SERVER,IID_MZ_Mixer,(void **)&TheMixer))){
		return false;
    }

	// 打开混音设备
	if(!TheMixer->OpenMixerDevice(TheHwnd)){
		return false;
	}

	return true;

}

// 设置混音器状态
void MixerModeSetting(void)
{

	// 有系统消息
	if(TheSystemMessage){
		
		TheMixer->SetHeadphoneMode(HEADPHONE_ON);
		TheMixer->SetSpeakerMode(SPEAKER_AUTO);

		return;

	}

	// 如果耳机插入
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


// 初始化
bool DirectDrawInit(void)
{

	// 创建DirectDraw对象
	if(DirectDrawCreate(NULL,&TheDDraw,NULL)!=DD_OK){
		return false;
	}

	// 设置协作方式
	if(TheDDraw->SetCooperativeLevel(TheHwnd,DDSCL_NORMAL)!=DD_OK){
		return false;
	}

	/********************************主表面****************************************/

	// 初始化表面结构
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	TheDDrawSurface.dwFlags = DDSD_CAPS;
	TheDDrawSurface.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	// 创建主表面
	if(TheDDraw->CreateSurface(&TheDDrawSurface,&TheDDrawPrimary,NULL)!=DD_OK){
		return false;
	}

	// 设置像素格式	
	DDRAW_INIT_STRUCT(TheDDrawPixelFormat);
	// 获取像素格式
	TheDDrawPrimary->GetPixelFormat(&TheDDrawPixelFormat);

	// 如果不是16位色模式
	if(TheDDrawPixelFormat.dwRGBBitCount!=SCREEN_COLOR){
		return false;
	}

	// 创建裁剪器
	if(TheDDraw->CreateClipper(0,&TheDDrawClipper,NULL)!=DD_OK){
		return false;
	}
	
	// 设置裁剪器窗口句柄
	if(TheDDrawClipper->SetHWnd(0,TheHwnd)!=DD_OK){
		return false;
	}

	// 关联裁剪器
	if(TheDDrawPrimary->SetClipper(TheDDrawClipper)!=DD_OK){
		return false;
	}

	// 创建后备表面
	if(!DirectDrawCreateSurface(&TheDDrawBackBuffer,SCREEN_WIDTH,SCREEN_HEIGHT)){
		return false;
	}

	// 资料读取中表面
	if(!DirectDrawCreateSurface(&TheDDrawLodingBuffer,IMG_LOADING_WIDTH,IMG_LOADING_HEIGHT)){
		return false;
	}

	// 创建背景表面
	if(!DirectDrawCreateSurface(&TheDDrawBackground,IMG_BACKGROUND_WIDTH,IMG_BACKGROUND_HEIGHT)){
		return false;
	}

	// 创建前景表面
	if(!DirectDrawCreateSurface(&TheDDrawForeground,IMG_FOREGROUND_WIDTH,IMG_FOREGROUND_HEIGHT)){
		return false;
	}

	// 装入数据
	DirectDrawLoading();

	return true;

}

// 创建表面
bool DirectDrawCreateSurface(LPDIRECTDRAWSURFACE* s, int w, int h)
{

	// 初始化表面结构
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// 设置资料读取中表面参数
	TheDDrawSurface.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	TheDDrawSurface.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
	TheDDrawSurface.dwWidth = w;
	TheDDrawSurface.dwHeight = h;

	// 创建资料读取中表面
	if(TheDDraw->CreateSurface(&TheDDrawSurface,s,NULL)!=DD_OK){
		return false;
	}

	return true;

}

// 装载数据
void DirectDrawLoading(void)
{

	// 如果资料读取中表面没有创建
	if(!TheDDrawLodingBuffer){
		return;
	}

	// 初始化表面结构
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// 锁定表面
	TheDDrawLodingBuffer->Lock(NULL,&TheDDrawSurface,DDLOCK_WAITNOTBUSY,NULL);

	USHORT *bb = (USHORT *)TheDDrawSurface.lpSurface;

	for(int i=0;i<IMG_LOADING_SIZE;i++){
		bb[i]=TheImageLoading[i]?COLOR_WHITE:0;
		
	}

	// 解锁表面
	TheDDrawLodingBuffer->Unlock(NULL);

}

// 销毁
void DirectDrawEnd(void)
{

	// 释放前景表面
	if(TheDDrawForeground){
		TheDDrawForeground->Release();
		TheDDrawForeground = NULL;
	}

	// 释放背景表面
	if(TheDDrawBackground){
		TheDDrawBackground->Release();
		TheDDrawBackground = NULL;
	}

	// 释放资料读取中表面
	if(TheDDrawLodingBuffer){
		TheDDrawLodingBuffer->Release();
		TheDDrawLodingBuffer = NULL;
	}

	// 释放后备表面
	if(TheDDrawBackBuffer){
		TheDDrawBackBuffer->Release();
		TheDDrawBackBuffer = NULL;
	}

	// 释放主表面
	if(TheDDrawPrimary){
		TheDDrawPrimary->Release();
		TheDDrawPrimary = NULL;
	}

	// 销毁剪切版
	if(TheDDrawClipper){
		TheDDrawClipper->Release();
		TheDDrawClipper = NULL;
	}

	// 释放内部分配资源
	if(TheDDraw){
		TheDDraw->Release();
		TheDDraw = NULL;
	}

}

//************************************************************************************************
//*************************** 授 权 控 制 ********************************************************
//************************************************************************************************

// 收费软件合法性验证
bool LicenseVerify(void)
{

	TCHAR pszFileName[MAX_PATH] = {0};
    GetModuleFileName(0, pszFileName, MAX_PATH);            // 当前进程EXE的文件名
    MYSTORE_VERIFY_CONTEXT mystore = {0};
    DWORD dwRet = MyStoreVerify(pszFileName, &mystore);     // 验证此文件是否合法

	// 验证合法
	if(dwRet==0){

		// 过期
		if(mystore.Expired){

			return false;
			
		}else{

			// 免费
			if((mystore.Reserved[0]&0x40)==0x40){

				return false;
			
			// 收费
			}else{

				return true;
			
			}
		
		}

	}

	return false;

}

//************************************************************************************************
//*************************** 素 材 读 取 ********************************************************
//************************************************************************************************

// 读取声音文件
void LoadingSoundFile(int soundIndex, int loadingStep)
{

	// 读取声音文件
	SpeakerLoadingSound(TheWaveFile[soundIndex],loadingStep,soundIndex);

}

// 读取图像文件
void LoadingImageFile(LPTSTR imgFile, int imgSize, LPDIRECTDRAWSURFACE imgSurface, int loadingStep)
{

	bool flag = false;

	// 初始化表面结构
	DDRAW_INIT_STRUCT(TheDDrawSurface);

	// 锁定主角表面
	imgSurface->Lock(NULL,&TheDDrawSurface,DDLOCK_WAITNOTBUSY,NULL);

	// 缓冲数据
	CHAR *bb = (CHAR *)TheDDrawSurface.lpSurface;

	// 读取文件
	flag = FileGetData(imgFile,bb,imgSize);

	// 解锁主角表面
	imgSurface->Unlock(NULL);

	// 任务完成进行下一步
	TheGameStep = loadingStep;

	// 读取失败
	if(!flag){
		GameError(GAME_ERROR_FILE);
	}

}

//************************************************************************************************
//*************************** 画面绘制 ***********************************************************
//************************************************************************************************

// 清屏幕
void DrawCls(int color)
{
	// 绘制进度条
	DrawRect(TheDDrawLodingBuffer,5,0,6,1,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

}

// 绘制主表面
void DrawPrimary(void)
{

	// 将后备表面复制到主表面
	TheDDrawPrimary->Blt(&TheRectScreen,TheDDrawBackBuffer,&TheRectScreen,DDBLT_WAITNOTBUSY,NULL);

}

// 绘制标志
void DrawLogo(void)
{

	// 读取标志图片
	switch(TheGameStep){
		
		case 0:
			// 读取背景图像文件
			LoadingImageFile(IMG_BACKGROUND_FILE, IMG_BACKGROUND_SIZE, TheDDrawBackground, 1);
			break;
		
		case 1:
			// 读取前景图像文件
			LoadingImageFile(IMG_FOREGROUND_FILE, IMG_FOREGROUND_SIZE, TheDDrawForeground, 2);
			break;

		case 2:			
			// 读取声音文件
			LoadingSoundFile(0,3);
			break;

		case 3:
			// 读取声音文件
			LoadingSoundFile(1,4);
			break;

		case 4:
			// 读取声音文件
			LoadingSoundFile(2,5);
			break;

		case 5:
			// 读取声音文件
			LoadingSoundFile(3,6);
			break;

		case 6:
			// 读取声音文件
			LoadingSoundFile(4,7);
			break;

		case 7:
			// 读取声音文件
			LoadingSoundFile(5,8);
			break;

		case 8:
			// 读取声音文件
			LoadingSoundFile(6,9);
			break;

		case 9:
			// 读取声音文件
			LoadingSoundFile(7,10);
			break;

		case 10:
			// 读取声音文件
			LoadingSoundFile(8,11);
			break;

		case 11:
			// 读取声音文件
			LoadingSoundFile(9,12);	
			break;

		case 12:
			// 读取声音文件
			SpeakerLoadingSound(SOUND_BUTTON_FILE,13,SOUND_FILE_COUNT-1);
			break;

	}

	// 处理LOADING状态
	if(TheGameStep<13){

		// 绘制资料读取中画面
		DrawLoading(TheGameStep<<3);

	}else{

		// 如果通过收费认证
		if(TheLicenseVerify){

			// 进程结束，进入下一步
			GameStatus(GAME_STATUS_RUN);
		
		}else{

			// 进程结束，进入下一步
			GameStatus(GAME_STATUS_MENU);
		
		}
		

	}

}

// 绘制矩形
void DrawRect(LPDIRECTDRAWSURFACE ddSurface, int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	
	// 源画面
	SetRect(&TheRectSource,x1,y1,x2,y2);
	// 目标画面
	SetRect(&TheRectTarget,x3,y3,x4,y4);
	// 复制源画面数据至后备画面
	TheDDrawBackBuffer->Blt(&TheRectTarget,ddSurface,&TheRectSource,DDBLT_WAITNOTBUSY,NULL);

}

// 绘制资料读取中画面
void DrawLoading(int flag)
{

	// 绘制文字
	DrawRect(TheDDrawLodingBuffer,0,0,IMG_LOADING_WIDTH,IMG_LOADING_HEIGHT,IMG_LOADING_X,IMG_LOADING_Y,IMG_LOADING_WIDTH + IMG_LOADING_X,IMG_LOADING_HEIGHT + IMG_LOADING_Y);
	// 绘制外框
	DrawRect(TheDDrawLodingBuffer,0,0,1,1,IMG_LOADING_X - 20,150,IMG_LOADING_X - 10,SCREEN_HEIGHT - 150);
	// 绘制内框
	DrawRect(TheDDrawLodingBuffer,1,1,2,2,IMG_LOADING_X - 19,151,IMG_LOADING_X - 11,SCREEN_HEIGHT - 151);
	// 绘制进度条
	DrawRect(TheDDrawLodingBuffer,0,0,1,1,IMG_LOADING_X - 18,152,IMG_LOADING_X - 12,152 + (flag<<2));

}

// 游戏运行时
void DrawRun(void)
{
	// 复制源画面数据至后备画面
	TheDDrawBackBuffer->Blt(&TheRectScreen,TheDDrawBackground,&TheRectScreen,DDBLT_WAITNOTBUSY,NULL);

	for(int i=0;i<DOT_COUNT;i++){

		// 如果灯量
		if(TheDotStatus[i]){
			DrawDot(i);
		}

	}

	// 播放音乐
	GamePlaySound();

	// 重力感应处理
	AccRun();

}

// 画点
void DrawDot(int i)
{

	// 计算坐标
	int x = i % DOT_WIDTH;
	int y = i / DOT_WIDTH;
	int xx = (x << 5) + (x << 4);
	int yy = (y << 5) + (y << 4);

	// 目标画面
	SetRect(&TheRectTarget,xx,yy,xx+DOT_SIZE,yy+DOT_SIZE);
	// 复制源画面数据至后备画面
	TheDDrawBackBuffer->Blt(&TheRectTarget,TheDDrawForeground,&TheRectDot[TheDotStep[i]],DDBLT_WAITNOTBUSY,NULL);

	// 如果不是最终画面
	if(TheDotStep[i]>0){
		TheDotStep[i]--;
	}

}

// 游戏绘制
void Draw(void)
{

	// 清屏
	DrawCls(COLOR_BLACK);

	// 判断游戏状态
	switch(TheGameStatus)
	{

		// 如果是显示标志状态
		case GAME_STATUS_LOGO:

			// 绘制标志
			DrawLogo();
			break;

		// 游戏运行状态
		case GAME_STATUS_RUN:
			// 绘制游戏运行时
			DrawRun();
			break;

		// 显示菜单时
		case GAME_STATUS_MENU:
			DrawMenu();
			break;
	
	}

	// 绘制暂停画面
	if(TheGamePause){
		DrawPause();
	}

	// 显示主表面
	DrawPrimary();

}

// 绘制菜单
void DrawMenu(void)
{
	
	// 绘制提示信息
	DrawRect(TheDDrawForeground,0,60,90,370,180,200,270,510);
	

}

// 绘制暂停画面
void DrawPause(void)
{


}

//************************************************************************************************
//*************************** 游 戏 控 制 ********************************************************
//************************************************************************************************

// 播放音乐
void GamePlaySound(void)
{
	
	// 序号
	int index = 0;

	// 间隔时间到
	if(TheGameStep==0){
	
		// 播放当前行
		for(int i=0;i<DOT_WIDTH;i++){
			
			// 获得当前序号
			index = i + (TheGameIndex << 3) + (TheGameIndex << 1);

			// 如果当前序号灯亮
			if(TheDotStatus[index]){

				// 设置当前灯为最亮
				TheDotStep[index] = DOT_STYLE - 1;
				// 播放声音
				SpeakerPlaySound(i);

			}

		}

		// 跳到下一行
		TheGameIndex++;

		// 如果超出最多行数
		if(TheGameIndex>=DOT_HEIGHT){

			// 回到第一行
			TheGameIndex = 0;

		}

	}

	// 步伐前进
	TheGameStep++;

	// 如果超出最多步数
	if(TheGameStep>TheGameFoot){

		// 回到第一步
		TheGameStep = 0;

	}

}

// 游戏错误
void GameError(LPTSTR msg)
{

	// 游戏暂停
	GamePause();

	// 游戏结束
	GameEnd();

	// 显示错误信息
	MzMessageBox(0,msg,GAME_ERROR,MB_OK,0,false);

	// 退出程序
	PostQuitMessage(0);

}

// 设置矩阵
void GameRectInit(void)
{

	// 设定屏幕矩阵
	SetRect(&TheRectScreen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);	

	// 设置矩阵
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

// 游戏初始化
bool GameInit(void)
{

	// 窗口没有正确创建
	if(!TheHwnd){
		return false;
	}

	// 设置矩阵
	GameRectInit();

	// 初始化 DirectDraw 失败
	if(!DirectDrawInit()){
		return false;
	}

	
	// 开启扬声器
	SpeakerInit();
	
	// 开启acc功能		
	MzAccOpen();

	// 初始化成功，进入显示标志过程
	GameStatus(GAME_STATUS_LOGO);

	// 激活游戏
	TheGameLive = true;

	return true;

}

// 游戏开始
void GameRun(void)
{

	
	// 绘制画面
	Draw();

}

// 继续游戏
void GameContinue(void)
{

	// 暂停游戏
	TheGamePause = false;
	// 继续扬声器
	SpeakerContinue();

}		

// 暂停游戏
void GamePause(void)
{

	// 暂停游戏
	TheGamePause = true;	
	// 暂停扬声器
	SpeakerPause();

}

// 停止游戏
void GameDead(void)
{

	// 关闭游戏状态
	TheGameLive = false;	
	// 暂停游戏
	GamePause();
	// 解除SHELL捕获音量控制键
	UnHoldShellUsingSomeKeyFunction(TheHwnd, MZ_HARDKEY_VOLUME_UP | MZ_HARDKEY_VOLUME_DOWN);			
	// 显示顶栏
	ShowMzTopBar();
	// 混音器状态设置
	MixerModeSetting();

}

// 游戏结束
void GameEnd(void)
{

	// 停止游戏
	GameDead();	
	// 销毁扬声器对象
	SpeakerEnd();
	// 关闭acc功能
	MzAccClose();
	// 销毁 DDraw 对象
	DirectDrawEnd();	

	// 释放混音器对象
	if(TheMixer!=NULL){
		TheMixer->Release();
	}

}

// 游戏激活
void GameLive(void)
{

	// 窗口句柄
	if(TheHwnd){

		// 没有系统消息
		TheSystemMessage = false;
		// 打开游戏状态
		TheGameLive = true;				
		// 阻止SHELL捕获音量控制键
		HoldShellUsingSomeKeyFunction(TheHwnd, MZ_HARDKEY_VOLUME_UP | MZ_HARDKEY_VOLUME_DOWN);
		// 隐藏顶栏
		HideMzTopBar();
		// 混音器状态设置
		MixerModeSetting();
		
	}

}

// 设置游戏状态
void GameStatus(int s)
{

	// 设置游戏状态
	TheGameStatus = s;
	TheGameStep = 0;
	TheGameIndex = 0;

}

//************************************************************************************************
//*************************** 重力感应 ***********************************************************
//************************************************************************************************

// 重力感应处理
void AccRun(void)
{

	// 获取重力感应加速度
	MzAccGetXYZ(&TheAxisX,&TheAxisY,&TheAxisZ);

	// 计数
	if(TheAxisX>ACC_MAX||TheAxisY>ACC_MAX){
		memset(TheDotStatus,0,sizeof(TheDotStatus));
	}

}


//************************************************************************************************
//*************************** 鼠标处理 ***********************************************************
//************************************************************************************************

// 鼠标抬起时
void MouseUp(int x, int y)
{

	// 游戏状态
	switch(TheGameStatus){

		// 运行状态
		case GAME_STATUS_RUN:
			MouseUpRun(x,y);
			break;

		// 菜单状态
		case GAME_STATUS_MENU:
			// 进程结束，进入下一步
			GameStatus(GAME_STATUS_RUN);
			break;

	}

}

// 鼠标双击时
void MouseClick(int x, int y)
{

	/*

	// 收费验证
	if(!TheLicenseVerify){
		return;
	}

	// 游戏运行时
	if(TheGameStatus==GAME_STATUS_RUN){

		if(x<241){

			// 速度减慢
			if(MouseArea(x,y,0,0,240,240)){
				TheGameFoot++;
			}

			// 恢复初始速度
			if(MouseArea(x,y,0,240,240,480)){
				TheGameFoot = 5;
			}

			// 加快速度
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

			// 采样向前
			if(MouseArea(x,y,240,0,480,240)){
				TheSamplesIndex--;				
			}

			// 恢复默认采样
			if(MouseArea(x,y,240,240,480,480)){
				TheSamplesIndex = 0;
			}

			// 加快速度
			if(MouseArea(x,y,240,480,480,720)){
				TheSamplesIndex++;				
			}

			if(TheSamplesIndex<0){
				TheSamplesIndex = SOUND_SAMPLE_COUNT - 1;
			}

			if(TheSamplesIndex>=SOUND_SAMPLE_COUNT){
				TheSamplesIndex = 0;
			}

			// 停止声音播放
			TheSpeaker.stopSounds();

			// 重新加载声音
			for(int i=0;i<10;i++){
				LoadingSoundFile(i,0,TheSamplesIndex);
			}
		
		}

	}

	*/

}

// 处理鼠标位置
bool MouseArea(int x, int y, int x1, int y1, int x2, int y2)
{

	if((x>x1)&&(x<x2)){

		if((y>y1)&&(y<y2)){

			return true;

		}
	
	}


	return false;

}

// 游戏运行时点击鼠标
void MouseUpRun(int x, int y)
{

	// 算出第几行
	int dotX = x / DOT_SIZE;
	// 算出第几列
	int dotY = y / DOT_SIZE;
	// 算出顺序号
	int dot = dotX + (dotY << 3) + (dotY << 1);
	
	// 播放第几行的声音
	if(TheDotStatus[dot]){
		TheDotStatus[dot] = false;
	}else{
		TheDotStatus[dot] = true;		
	}

	// 设置状态
	TheDotStep[dot] = DOT_STYLE - 1;

}

//************************************************************************************************
//*************************** 主 窗 口 ***********************************************************
//************************************************************************************************

// MZFC窗口类及派生类的窗口类名定义宏
MZ_IMPLEMENT_DYNAMIC(MyWindow)

// 构造函数
MyWindow::MyWindow(void)
{
}

// 析构函数
MyWindow::~MyWindow(void)
{
}

// 初始化
BOOL MyWindow::OnInitDialog()
{
	
	// 必须先执行父对象的初始化方法
	if (!CMzWndEx::OnInitDialog())
	{
		return false;
	}

	// 成功
	return true;

}

// 当HOME键被按下时
int MyWindow::OnShellHomeKey(UINT message, WPARAM wParam, LPARAM lParam)
{

	// 不退出程序，显示桌面
	// return SHK_RET_APPNOEXIT_SHELLTOP;

	// 默认退出
	return SHK_RET_DEFAULT;

}

// 鼠标双击
void MyWindow::OnLButtonDblClk(UINT fwKeys, int xPos, int yPos)
{

	// 处理鼠标事件
	MouseClick(xPos,yPos);

}

// 鼠标抬起时
void MyWindow::OnLButtonUp(UINT fwKeys, int xPos, int yPos) 
{

	// 处理鼠标事件
	MouseUp(xPos,yPos);

}

// 鼠标移动时
void MyWindow::OnMouseMove(UINT fwKeys, int xPos, int yPos) 
{

}

// 鼠标按下时
void  MyWindow::OnLButtonDown(UINT fwKeys, int xPos, int yPos) 
{

}

// 消息处理
LRESULT MyWindow::MzDefWndProc(UINT message, WPARAM  wParam, LPARAM  lParam) 
{

	// 某些状态
	int id = int(lParam);

	// 处理消息
	switch(message){

		// 混音控制事件
		case MM_MIXM_CONTROL_CHANGE:
			// 耳机状态有变化
			if((id==HEADPHONE_MODE_CHANGE_CONTROL_ID)||(id==CHANGE_HEADPHONE_TYPE_CONTROL_ID)){		
				// 设置混音器状态
				MixerModeSetting();
			}
			break;

		// 处理键盘事件
		case WM_KEYUP:

			// 降低音量		
			if(wParam==VK_VOLUME_DOWN){
				// 降低音量				
				SpeakerSoundVolumeDown(SOUND_VOLUME_STEP);
				// 混音器音量降低
				MixerVolumeDown(SOUND_VOLUME_STEP);
			}

			// 音量升高
			if(wParam==VK_VOLUME_UP){
				// 升高音量
				SpeakerSoundVolumeUp(SOUND_VOLUME_STEP);
				// 混音器音量升高
				MixerVolumeUp(SOUND_VOLUME_STEP);
				break;

			}
			break;

		// 得到焦点
		case WM_SETFOCUS:
			
			// 激活游戏
			GameLive();
			break;

		// 失去焦点	
		case WM_KILLFOCUS:

			// 停止游戏
			GameDead();
			break;

		// 任务管理器消息
		case MZ_WM_TASKMAN_ONOFF:

			// 显示任务管理器消息
			if(lParam==TASKMAN_SHOW_WND){
				// 停止游戏
				GameDead();
			}

			// 从任务栏点击程序
			if(lParam==TASKMAN_EXEC_PROG){
				// 激活游戏
				GameLive();
			}

			// 隐藏任务管理器消息
			if(lParam==TASKMAN_QUIT_WND){
				if (GetForegroundWindow()==m_hWnd){
					// 激活游戏
					GameLive();
				}
			}

			break;


	}

	// U盘事件
	if(message==TheUsbNotify){

		// U盘接入
		if(wParam==USB_MASSSTORAGE_ATTACH){
			PostQuitMessage(0);
		}
	
	}

	// 来电事件,短消息事件,彩信事件
	if((message==TheCallNotify)||(message==TheSmsNotify)||(message==TheMmsNotify)){
	
		// 有系统消息
		TheSystemMessage = true;

	}

	// 来电消息,短消息消息,彩信消息
	if((message==TheCallMessage)||(message==TheSmsMessage)||(message==TheMmsMessage)){
	
		// 有系统消息
		TheSystemMessage = true;

	}

	// 返回系统默认消息处理结果
	return CMzWndEx::MzDefWndProc(message,wParam,lParam);

}

// 创建窗口
bool MyWindow::CreateWindowNow(void)
{

	// 创建主窗口
	Create(0,0,SCREEN_WIDTH,SCREEN_HEIGHT, 0, 0, 0);
	SetWindowText(APP_WINDOW_TEXT);
	SetBgColor(COLOR_BLACK);

	// 窗口动画效果
	if(MzGetParam(MZGP_APP_START_ANIMATION)){
		AnimateWindow(MZ_ANIMTYPE_ZOOM_IN,true);
	}

	// 显示主窗口
	Show();

	// 窗口句柄
	TheHwnd = m_hWnd;

	if(TheHwnd<=0){
		return false;
	}

	return true;

}

//************************************************************************************************
//*************************** 主 程 序 ***********************************************************
//************************************************************************************************


// 构造函数
MyApp::MyApp(void)
{
}

// 析构函数
MyApp::~MyApp(void)
{
}

// 消息处理
void MyApp::MessageRegister(void)
{

	// 注册USB消息
	TheUsbNotify = RegisterUsbNotifyMsg();

	// 来电事件
	TheCallNotify = RegisterWindowMessage(WM_CALL_MESSAGE);
	// 短消息事件
	TheSmsNotify = RegisterWindowMessage(WM_SMS_MESSAGE);
	// 彩信事件
	TheMmsNotify = RegisterWindowMessage(WM_MMS_MESSAGE);

	// 来电消息
	TheCallMessage = GetCallRegisterMessage();
	// 短信消息
	TheSmsMessage = GetSmsRegisterMessage();
	// 彩信消息
	TheMmsMessage = GetMmsRegisterMessage();

}

// 单例判断
bool MyApp::IsAppRunning(void)
{

	//检测是否已经有另外的实例在运行中:
    bool bFound = false;

	// 获取句柄
    HANDLE hMutexOneInstantance = CreateMutex(NULL, true, APP_MUTEX_NAME);

	// 找到实例
    if (GetLastError()==ERROR_ALREADY_EXISTS){
		bFound = true;
    }

	// 释放句柄
    if(hMutexOneInstantance){
		ReleaseMutex(hMutexOneInstantance);
    }

	// 如果找到实例
	if(bFound){
		
		//激活已经在运行的实例的窗口   
		HWND pwndFirst = FindWindow((LPCTSTR)MyWindow::m_pwszMyWindowClassName, (LPCTSTR)NULL);

		// 已经找到窗口实例
		if(pwndFirst){

			// 如果窗口被隐藏
            if(!IsWindowVisible(pwndFirst)){
				// 显示窗口
				ShowWindow(pwndFirst, SW_SHOW);
            }

			// 激活窗口
			SetForegroundWindow(pwndFirst);

        }

	}

	return bFound;

}

// 初始化
BOOL MyApp::Init()
{

	// 如果已经运行
	if(IsAppRunning()){
		return false;
	}

	// 隐藏顶栏
	HideMzTopBar();

	// 注册COM
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// 创建混音设备
	if(!MixerInit()){
		return false;
	}
	
	// 窗口创建失败
	if(!TheWindow.CreateWindowNow()){
		return false;
	}

	// 注册各种消息
	MessageRegister();

	// 获取当前程序运行目录
	FileGetPath();

	// 成功
	return true;

}

// 运行
int MyApp::Run()
{

	MSG msg;

	// 初始化游戏
	if(!GameInit()){
		GameError(GAME_ERROR_INIT);
		return false;
	}

	// 激活游戏
	GameLive();

	// 软件授权检验
	TheLicenseVerify = true;// LicenseVerify();

	// 创建FPS对象
	TheFPS = new TrackerFps();

	// 开始FPS计时
	TheFPS->Start(FPS);

	// 主消息循环
	while(true){

		// 处理消息
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){

			// 退出
			if(msg.message==WM_QUIT){
				break;
			}

			// 正常处理消息
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
		}

		// 游戏激活状态
		if(TheGameLive){

			// 运行游戏
			GameRun();

			// 等待下一帧
			TheFPS->Wait();

		}

		// 让出进程
		Sleep(0);
	
	}

	// 游戏结束
	GameEnd();

	return true;

}