#pragma once

// GUID
#include <initguid.h>
// MZFC 头文件
#include <mzfc_inc.h>
// MZ 授权文件
#include <MyStoreLib.h>
// Shell 头文件
#include <ShellNotifyMsg.h>
// 重力感应
#include <acc_api.h>

// 混音器
#include <IMixer.h>
#include <IMixer_GUID.h>

// USB 事件
#include <UsbNotifyApi.h>
// 来电事件
#include <CallNotifyApi.h>

// 声音库
#include <hssData.h>
#include <hssSpeaker.h>

// FPS 控制
#include "TrackerFPS.h"
// 图像数据
#include "TrackerImage.h"

// 程序
#define APP_MUTEX_NAME			L"TrackerMutex"
#define APP_WINDOW_TEXT			L"魅影寻声"
#define APP_GUID				L"{9EB77B8F-6AD7-4c05-8591-AC8B4895F045}"

// 游戏 FPS
#define FPS						25			// 游戏FPS

// 屏幕属性
#define	SCREEN_WIDTH			480			// 屏幕宽度
#define	SCREEN_HEIGHT			720			// 屏幕高度
#define SCREEN_COLOR			16			// 16 位色

// 点阵属性
#define DOT_SIZE				48			// 每一点大小
#define DOT_COUNT				150			// 总点数
#define DOT_WIDTH				10			// 宽度
#define DOT_HEIGHT				15			// 高度
#define DOT_STYLE				5			// 样式

// 图像数据
#define IMG_LOADING_WIDTH		12			// 资料读取中宽度
#define IMG_LOADING_HEIGHT		60			// 资料读取中高度
#define IMG_LOADING_SIZE		720			// 资料读取中大小

#define IMG_LOADING_X			250			// 资料读取中 X 坐标
#define IMG_LOADING_Y			330			// 资料读取中 Y 坐标

#define IMG_BACKGROUND_WIDTH	480			// 背景宽度
#define IMG_BACKGROUND_HEIGHT	720			// 背景高度
#define IMG_BACKGROUND_SIZE		691200		// 背景大小
#define IMG_BACKGROUND_FILE		L"15149001.dat"

#define IMG_FOREGROUND_WIDTH	480			// 前景宽度
#define IMG_FOREGROUND_HEIGHT	720			// 前景高度
#define IMG_FOREGROUND_SIZE		691200		// 前景大小
#define IMG_FOREGROUND_FILE		L"15149002.dat"

// 扬声器数据
#define SPEAKER_FREQMIX			22050		// 频率
#define SPEAKER_BPS				16			// 位数
#define SPEAKER_STEREO			true		// 立体声

// 音乐数据
#define MUSIC_FILE_SIZE			50000		// 背景音乐缓冲
#define MUSIC_FILE_COUNT		2			// 同时播放2种音乐

#define	MUSIC_VOLUME_DEFAULT	16			// 默认音量
#define MUSIC_VOLUME_MAX		64			// 最大声音
#define MUSIC_VOLUME_MIN		0			// 最小声音
#define MUSIC_VOLUME_STEP		10			// 音量步伐

// 声音数据
#define SOUND_FILE_SIZE			400000		// 声音缓冲
#define SOUND_FILE_COUNT		11			// 同时播放11种声音
#define SOUND_SAMPLE_COUNT		5			// 音色统计
#define SOUND_BUTTON_FILE		L"15149003.dat"

#define	SOUND_VOLUME_DEFAULT	16			// 默认音量
#define SOUND_VOLUME_TOP		100			// 最大声音
#define SOUND_VOLUME_MAX		64			// 最大声音
#define SOUND_VOLUME_MIN		0			// 最小声音
#define SOUND_VOLUME_STEP		8			// 音量步伐

// 游戏状态
#define GAME_STATUS_START		0			// 开始
#define GAME_STATUS_LOGO		1			// 显示标志
#define GAME_STATUS_RUN			2			// 显示版权
#define GAME_STATUS_MENU		3			// 显示菜单

// 错误提示
#define GAME_ERROR				L"程序错误！"
#define GAME_ERROR_INIT			L"程序初始化错误！"
#define GAME_ERROR_FILE			L"资料读取错误！"
#define GAME_ERROR_COPY			L"授权文件校验失败，请重新下载安装！"
#define GAME_ERROR_FREE			L"您当前使用的是试用版本！"

// 常用颜色
#define COLOR_BLACK		0					// 黑色
#define COLOR_WHITE		0xFFFF				// 白色

// 重力感应
#define ACC_MAX		56

//************************************************************************************************
//*************************** 文件操作 ***********************************************************
//************************************************************************************************

// 获取程序当前目录
void FileGetPath(void);
// 获取文件路径
void FileGetName(LPTSTR);
// 读取文件数据
bool FileGetData(LPTSTR, char*, int);

//************************************************************************************************
//*************************** 重力感应 ***********************************************************
//************************************************************************************************

void AccRun(void);

//************************************************************************************************
//*************************** 声音处理 ***********************************************************
//************************************************************************************************

// 初始化扬声器
bool SpeakerInit(void);
// 关闭扬声器设备
void SpeakerEnd(void);
// 扬声器暂停
void SpeakerPause(void);
// 扬声器继续
void SpeakerContinue(void);
// 播放声音
void SpeakerPlaySound(int i);
// 读取音乐文件
void SpeakerLoadingMusic(LPTSTR, int, int);
// 读取声音文件
void SpeakerLoadingSound(LPTSTR, int, int);
// 降低音量
void SpeakerSoundVolumeDown(int);
// 降低升高
void SpeakerSoundVolumeUp(int);
// 初始化混音器
bool MixerInit(void);
// 混音器音量降低
void MixerVolumeDown(int);
// 混音器音量升高
void MixerVolumeUp(int);
// 喇叭状态设置
void MixerModeSetting(void);

//************************************************************************************************
//*************************** DirectDraw *********************************************************
//************************************************************************************************

#define DDRAW_INIT_STRUCT(ddsd) {memset(&ddsd,0,sizeof(ddsd));ddsd.dwSize=sizeof(ddsd);}

// 初始化
bool DirectDrawInit(void);
// 销毁
void DirectDrawEnd(void);
// 装载数据
void DirectDrawLoading(void);
// 创建表面
bool DirectDrawCreateSurface(LPDIRECTDRAWSURFACE*, int, int);

//************************************************************************************************
//*************************** 授 权 控 制 ********************************************************
//************************************************************************************************

// 收费软件合法性验证
bool LicenseVerify(void);

//************************************************************************************************
//*************************** 素 材 读 取 ********************************************************
//************************************************************************************************

// 读取声音文件
void LoadingSoundFile(int,int);
// 读取图像文件
void LoadingImageFile(LPTSTR, int, LPDIRECTDRAWSURFACE, int);

//************************************************************************************************
//*************************** 游 戏 控 制 ********************************************************
//************************************************************************************************

// 游戏初始化
bool GameInit(void);
// 游戏开始
void GameRun(void);
// 游戏结束
void GameEnd(void);
// 激活游戏
void GameLive(void);
// 停止游戏
void GameDead(void);
// 暂停游戏
void GamePause(void);
// 继续游戏
void GameContinue(void);
// 游戏错误
void GameError(LPTSTR);
// 播放音乐
void GamePlaySound(void);
// 设置矩阵
void GameRectInit(void);
// 设置游戏状态
void GameStatus(int);

//************************************************************************************************
//*************************** 画面绘制 ***********************************************************
//************************************************************************************************

// 游戏绘制
void Draw(void);
// 游戏运行时
void DrawRun(void);
// 画点
void DrawDot(int);
// 绘制暂停画面
void DrawPause(void);
// 绘制主表面
void DrawPrimary(void);
// 清屏
void DrawCls(int);
// 绘制矩形
void DrawRect(LPDIRECTDRAWSURFACE,int,int,int,int,int,int,int,int);
// 绘制标志
void DrawLogo(void);
// 绘制资料读取中画面
void DrawLoading(int);
// 绘制菜单
void DrawMenu(void);

//************************************************************************************************
//*************************** 鼠标处理 ***********************************************************
//************************************************************************************************

// 处理鼠标事件
void MouseUp(int, int);
// 游戏运行时点击鼠标
void MouseUpRun(int, int);
// 鼠标双击时
void MouseClick(int, int);
// 处理鼠标位置
bool MouseArea(int, int, int, int, int, int);

//************************************************************************************************
//*************************** 主 窗 口 ***********************************************************
//************************************************************************************************

// 主窗口对象
class MyWindow : public CMzWndEx
{

	// MZFC窗口类及派生类的窗口类名声名宏
	MZ_DECLARE_DYNAMIC(MyWindow);

	// 公有成员
	public:

		// 构造函数
		MyWindow(void);
		// 析构函数
		~MyWindow(void);

		// 创建窗口
		bool CreateWindowNow(void);

	// 保护成员
	protected:

		// 初始化
		virtual BOOL OnInitDialog();
		// 当HOME按下时
		virtual int OnShellHomeKey(UINT, WPARAM, LPARAM);
		// 消息处理
		virtual LRESULT MzDefWndProc(UINT, WPARAM, LPARAM);
		// 鼠标双击
		virtual void OnLButtonDblClk(UINT, int, int);
		// 鼠标移动时
		virtual void OnMouseMove(UINT, int, int);		
		// 鼠标抬起时
		virtual void OnLButtonUp(UINT, int, int);
		// 鼠标按下时
		virtual void  OnLButtonDown(UINT, int, int);

	// 私有成员
	private:

		



};

//************************************************************************************************
//*************************** 主 程 序 ***********************************************************
//************************************************************************************************


// 从 CMzApp 派生的应用程序类
class MyApp : public CMzApp
{

	// 公有成员
	public:

		// 构造函数
		MyApp(void);
		// 析构函数
		~MyApp(void);

	// 保护成员
	protected:

		// 初始化
		virtual BOOL Init(void);
		// 运行
		virtual int Run(void);

	// 私有成员
	private:

		// 单例判断
		bool IsAppRunning(void);
		// 消息处理
		void MessageRegister(void);

};

