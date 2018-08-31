#include "WowPlugin.h"

#define SKILL_1 "skill_1"
#define SKILL_2 "skill_2"
#define SKILL_3 "skill_3"
#define SKILL_4 "skill_4"
#define SKILL_5 "skill_5"
#define SKILL_R "skill_R"
#define SKILL_F "skill_F"
#define SKILL_T "skill_T"
#define SKILL_G "skill_G"
#define BUFF_DING1_1 "buff_1"
#define BUFF_DING "buff_2"
#define BUFF_POWERBAR "buff_3"
#define BUFF_SHUNPI_ONE_SECOND "buff_4"
#define BUFF_SHUNPI	"buff_5"

WowPlugin::WowPlugin()
{
	m_iTargetHandle = 0;
	m_akGameIconMidBottomInfo.clear();
	m_akGameIconLeftTopInfo.clear();
	m_akGameIconMidInfo.clear();
	m_iStartPlayGame = 0;
	m_aiMessage.clear();
}

WowPlugin::~WowPlugin()
{
	m_akGameIconMidBottomInfo.clear();
	m_akGameIconLeftTopInfo.clear();
	m_akGameIconMidInfo.clear();
}

bool WowPlugin::InitGame()
{
	m_iTargetHandle = FindWindowA(NULL, "魔兽世界");

	// 添加技能图标
	AddGameIconInfo(SKILL_1, "skill_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_2, "skill_2.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_3, "skill_3.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_4, "skill_4.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_5, "skill_5.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_R, "skill_6.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_F, "skill_7.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_T, "skill_8.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_G, "skill_9.bmp", m_akGameIconMidBottomInfo);

	// 添加中间BUFF图标
	AddGameIconInfo(BUFF_DING1_1, "buff_1.bmp", m_akGameIconMidInfo);
	AddGameIconInfo(BUFF_DING, "buff_2.bmp", m_akGameIconMidInfo);
	AddGameIconInfo(BUFF_SHUNPI_ONE_SECOND, "buff_4.bmp", m_akGameIconMidInfo);
	AddGameIconInfo(BUFF_SHUNPI, "buff_5.bmp", m_akGameIconMidInfo);

	// 添加右上角血条图标
	AddGameIconInfo(BUFF_POWERBAR, "buff_3.bmp", m_akGameIconLeftTopInfo);

	return true;
}

void WowPlugin::GameLoop()
{
	ProcessInput();

	// 100是检测模式
	if (m_iStartPlayGame == 100)
	{
		static int iLastTimeRefrashNormal = timeGetTime();
		if (timeGetTime() - iLastTimeRefrashNormal > 500)
		{
			iLastTimeRefrashNormal = timeGetTime();

			RefrashIconInfoNormal();

			// 检测下是不是所有的图标都通过了
			bool bAllPass = true;

			std::map<std::string, GameIconInfo>::iterator itr = m_akGameIconMidBottomInfo.begin();
			for (; itr != m_akGameIconMidBottomInfo.end(); itr++)
			{
				GameIconInfo& kGameIconInfo = itr->second;
				if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
				{
					bAllPass = false;
				}
			}
			itr = m_akGameIconLeftTopInfo.begin();
			for (; itr != m_akGameIconLeftTopInfo.end(); itr++)
			{
				GameIconInfo& kGameIconInfo = itr->second;
				if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
				{
					bAllPass = false;
				}
			}
			itr = m_akGameIconMidInfo.begin();
			for (; itr != m_akGameIconMidInfo.end(); itr++)
			{
				GameIconInfo& kGameIconInfo = itr->second;
				if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
				{
					bAllPass = false;
				}
			}

			// 所有的都通过，写入文件
			if (bAllPass)
			{
				SaveConfig();
				m_iStartPlayGame = 0;
			}
			
		}
	}
	else if (m_iStartPlayGame)
	{
		static int iLastTimeRefrashFast = timeGetTime();
		
		static int iLastTimePlay = timeGetTime();

		if (timeGetTime() - iLastTimeRefrashFast > 200)
		{
			iLastTimeRefrashFast = timeGetTime();

			RefrashIconInfoFast();
			iLastTimePlay = 0;
		}
		
		if (timeGetTime() - iLastTimePlay > 200)
		{
			iLastTimePlay = timeGetTime();
			PlayGame();
		
		}
	}
}

void WowPlugin::PlayGame()
{
	static int iLastTime = timeGetTime();
	int iDifTime = timeGetTime() - iLastTime;
	iLastTime = timeGetTime();
	char str[100];
	sprintf(str, "PlayGameDifTime:%d\n", iDifTime);
	OutputDebugStringA(str);

	GameIconInfo& kBuffDingOneSecond = m_akGameIconMidInfo[BUFF_DING1_1];
	GameIconInfo& kBuffDing = m_akGameIconMidInfo[BUFF_DING];
	GameIconInfo& kBuffPowerBar = m_akGameIconLeftTopInfo[BUFF_POWERBAR];
	GameIconInfo& kBuffShunPiOneSecond = m_akGameIconMidInfo[BUFF_SHUNPI_ONE_SECOND];
	GameIconInfo& kBuffShunPi = m_akGameIconMidInfo[BUFF_SHUNPI];


	// 当钉刺BUFF只剩一秒，或者钉刺BUFF没有的状态下，使用钉刺射击
	if (kBuffDingOneSecond.fComPareRate > 0.9f || kBuffDingOneSecond.fComPareRate < 0.1f && kBuffDing.fComPareRate < 0.1f)
	{
		m_aiMessage.push_back(VK_NUMPAD7);
		OutputDebugStringA("kSkillF\n");
	}

	// 夺命黑鸦，好了就用
	GameIconInfo& kSkill1 = m_akGameIconMidBottomInfo[SKILL_1];
	if (kSkill1.fComPareRate > 0.9f)
	{
		m_aiMessage.push_back(VK_NUMPAD1);
		OutputDebugStringA("kSkill1\n");
	}

	// 野性守护，好了就用 加暴击回集中值
	GameIconInfo& kSkill2 = m_akGameIconMidBottomInfo[SKILL_2];
	if (kSkill2.fComPareRate > 0.9f)
	{
		m_aiMessage.push_back(VK_NUMPAD2);
		OutputDebugStringA("kSkill2\n");
	}

	// 狂野怒火，好了就用
	GameIconInfo& kSkill3 = m_akGameIconMidBottomInfo[SKILL_3];
	if (kSkill3.fComPareRate > 0.9f)
	{
		m_aiMessage.push_back(VK_NUMPAD3);
		OutputDebugStringA("kSkill3\n");
	}

	// 杀戮指令,好了就用
	GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_4];
	if (kSkill4.fComPareRate > 0.9f)
	{
		m_aiMessage.push_back(VK_NUMPAD4);
		OutputDebugStringA("kSkill4\n");
	}

	// 多目标状态状态 没有顺劈BUFF，或者顺劈BUFF小于1秒 就使用多重射击
	if (m_iStartPlayGame == 2)
	{
		GameIconInfo& kSkillT = m_akGameIconMidBottomInfo[SKILL_T];
		if (kSkillT.fComPareRate > 0.9f && (kBuffShunPi.fComPareRate < 0.1f || kBuffDingOneSecond.fComPareRate > 0.9f))
		{
			m_aiMessage.push_back(VK_NUMPAD8);
			OutputDebugStringA("kSkillT\n");
		}

		// 弹幕射击，好了就用
		GameIconInfo& kSkillG = m_akGameIconMidBottomInfo[SKILL_G];
		if (kSkillG.fComPareRate > 0.9f)
		{
			m_aiMessage.push_back(VK_NUMPAD9);
			OutputDebugStringA("kSkillG\n");
		}
	}


	// 奇美拉射击，好了就用
	GameIconInfo& kSkillR = m_akGameIconMidBottomInfo[SKILL_R];
	if (kSkillR.fComPareRate > 0.9f)
	{
		m_aiMessage.push_back(VK_NUMPAD6);
		OutputDebugStringA("kSkillR\n");
	}

	if (m_iStartPlayGame == 1)
	{
		// 单目标 眼镜蛇射击， 集中值溢出，同时杀戮指令进入冷却
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_5];
		if (kSkill5.fComPareRate > 0.9f && kBuffPowerBar.fComPareRate > 0.65f && kSkill4.fComPareRate < 0.9)
		{
			m_aiMessage.push_back(VK_NUMPAD5);
			OutputDebugStringA("kSkill5\n");
		}
	}
	else if (m_iStartPlayGame == 2)
	{
		// 多目标 集中值溢出， 并且有顺劈BUFF 并且顺劈BUFF大于1秒
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_5];
		if (kSkill5.fComPareRate > 0.9f && kBuffPowerBar.fComPareRate > 0.76f && kBuffShunPi.fComPareRate > 0.9f && kBuffShunPiOneSecond.fComPareRate < 0.1f)
		{
			m_aiMessage.push_back(VK_NUMPAD5);
			
			OutputDebugStringA("kSkill5\n");
		}
	}

	for (int i = m_aiMessage.size() - 1; i >= 0; --i)
	{
		SendMessage(m_iTargetHandle, WM_KEYDOWN, m_aiMessage[i], 0);
		SendMessage(m_iTargetHandle, WM_KEYUP, m_aiMessage[i], 0);
	}
	m_aiMessage.clear();
}

void WowPlugin::ProcessInput()
{
	static int iLastUpdateTime = timeGetTime();
	if (timeGetTime() - iLastUpdateTime > 10)
	{
		iLastUpdateTime = timeGetTime();

		if (GetKeyState(VK_F9) & 0x800/* && GetKeyState(VK_MENU) & 0x800*/)
		{
			m_iStartPlayGame = 1;
		}
		if (GetKeyState(VK_F10) & 0x800 /*&& GetKeyState(VK_MENU) & 0x800*/)
		{
			m_iStartPlayGame = 2;
		}
		if (GetKeyState(VK_F11) & 0x800/* && GetKeyState(VK_MENU) & 0x800*/)
		{
			m_iStartPlayGame = 0;
		}
		if (GetKeyState(VK_F12) & 0x800 && GetKeyState(VK_MENU) & 0x800)
		{
			m_iStartPlayGame = 100;
		}
		if (GetKeyState(VK_F12) & 0x800 && GetKeyState(VK_CONTROL) & 0x800)
		{
			ReadConfig();
		}
		if (GetKeyState('F') & 0x800)
		{

		}
	}
}

HBITMAP WowPlugin::ScreenShotByWndPos(HWND hWnd)
{
	HDC       hScrDC, hMemDC;// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;  // 位图句柄
	int       nX, nY;      	// 选定区域坐标
	int       nWidth, nHeight;

	RECT* pRtWnd = NULL;
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &windowInfo);
	pRtWnd = &windowInfo.rcWindow;

	// 获得选定区域坐标
	nX = pRtWnd->left;
	nY = pRtWnd->top;

	nWidth = pRtWnd->right - pRtWnd->left;
	nHeight = pRtWnd->bottom - pRtWnd->top;

	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL); //为屏幕创建设备描述表	
	hMemDC = CreateCompatibleDC(hScrDC);             //为屏幕设备描述表创建兼容的内存设备描述表

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); // 创建一个与屏幕设备描述表兼容的位图	
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	// 把新位图选到内存设备描述表中

	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	return hBitmap;
}

void WowPlugin::AddGameIconInfo(std::string kKey, std::string kFileName, std::map<std::string, GameIconInfo>& akGameIconInfo)
{
	HBITMAP hBitMapIcon = (HBITMAP)::LoadImageA(NULL, kFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitMapIcon != 0)
	{
		HDC hdcIcon = CreateCompatibleDC(NULL);
		BITMAP  bitMapIcon;
		GetObject(hBitMapIcon, sizeof(BITMAP), &bitMapIcon);
		SelectObject(hdcIcon, hBitMapIcon);

		GameIconInfo& kGameIconInfo = akGameIconInfo[kKey];
		kGameIconInfo.hGameIconBitMap = hBitMapIcon;
		kGameIconInfo.iPixelWidth = bitMapIcon.bmWidth;
		kGameIconInfo.iPixelHeight = bitMapIcon.bmHeight;
		kGameIconInfo.aiPixelGameIcon = new COLORREF[bitMapIcon.bmWidth * bitMapIcon.bmHeight];
		for (int i = 0; i < bitMapIcon.bmWidth; ++i)
		{
			for (int j = 0; j < bitMapIcon.bmHeight; ++j)
			{
				kGameIconInfo.aiPixelGameIcon[i * bitMapIcon.bmHeight + j] = GetPixel(hdcIcon, i, j);
			}
		}
		DeleteDC(hdcIcon);
		::DeleteObject(hBitMapIcon);
	}
	else
	{
		std::string kStrWaring = "加载ICON图片失败:";
		kStrWaring = kStrWaring + kFileName;
		MessageBoxA(NULL, kStrWaring.c_str(), "Warning", MB_OK);
	}
}

#define MID_RIGHT	1830,	1880,	335,	845
#define LEFT_TOP	50,		280,	20,		150
#define MID_MID		890,	1050,	705,	765

void WowPlugin::RefrashIconInfoFast()
{
	OutputDebugStringA("kSkillF\n");
	HBITMAP hBitMapGameSnap = ScreenShotByWndPos(m_iTargetHandle);
	if (hBitMapGameSnap != 0)
	{
		// 获取屏幕截图数据
		BITMAP  bitMapGamSnap;
		GetObject(hBitMapGameSnap, sizeof(BITMAP), &bitMapGamSnap);
		HDC hdcGameSnap = CreateCompatibleDC(NULL);
		SelectObject(hdcGameSnap, hBitMapGameSnap);

		// 中右方技能区域
		ComPareImageFast(MID_RIGHT, hdcGameSnap, m_akGameIconMidBottomInfo);
		// 左上角血条区域
		ComPareImageFast(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		// 中间BUFF区域
		ComPareImageFast(MID_MID, hdcGameSnap, m_akGameIconMidInfo);

		//OutputDebugStringA("m_akGameIconMidBottomInfo\n");
		//std::map<std::string, GameIconInfo>::iterator itr = m_akGameIconMidBottomInfo.begin();
		//for (; itr != m_akGameIconMidBottomInfo.end(); itr++)
		//{
		//	GameIconInfo& kGameIconInfo = itr->second;
		//	char str[20];
		//	itoa((int)(kGameIconInfo.fComPareRate * 10), str, 10);
		//	OutputDebugStringA(str);
		//	OutputDebugStringA("\n");
		//}
		//OutputDebugStringA("m_akGameIconLeftTopInfo\n");
		//itr = m_akGameIconLeftTopInfo.begin();
		//for (; itr != m_akGameIconLeftTopInfo.end(); itr++)
		//{
		//	GameIconInfo& kGameIconInfo = itr->second;
		//	char str[20];
		//	itoa((int)(kGameIconInfo.fComPareRate * 10), str, 10);
		//	OutputDebugStringA(str);
		//	OutputDebugStringA("\n");
		//}
		//OutputDebugStringA("m_akGameIconMidInfo\n");
		//itr = m_akGameIconMidInfo.begin();
		//for (; itr != m_akGameIconMidInfo.end(); itr++)
		//{
		//	GameIconInfo& kGameIconInfo = itr->second;
		//	char str[20];
		//	itoa((int)(kGameIconInfo.fComPareRate * 10), str, 10);
		//	OutputDebugStringA(str);
		//	OutputDebugStringA("\n");
		//}
		::DeleteDC(hdcGameSnap);
		::DeleteObject(hBitMapGameSnap);
		
	}
}

void WowPlugin::RefrashIconInfoNormal()
{
	HBITMAP hBitMapGameSnap = ScreenShotByWndPos(m_iTargetHandle);
	if (hBitMapGameSnap != 0)
	{
		// 获取屏幕截图数据
		BITMAP  bitMapGamSnap;
		GetObject(hBitMapGameSnap, sizeof(BITMAP), &bitMapGamSnap);
		HDC hdcGameSnap = CreateCompatibleDC(NULL);
		SelectObject(hdcGameSnap, hBitMapGameSnap);

		// 中右方技能区域
		ComPareImageNormal(MID_RIGHT, hdcGameSnap, m_akGameIconMidBottomInfo);
		// 左上角血条区域
		ComPareImageNormal(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		// 中间BUFF区域
		ComPareImageNormal(MID_MID, hdcGameSnap, m_akGameIconMidInfo);

		::DeleteDC(hdcGameSnap);
		::DeleteObject(hBitMapGameSnap);
	}
}


// 快速检测，前提是已经normal检测通过一次，有起始坐标
bool WowPlugin::ComPareImageFast(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo)
{
	bool bHasNormalIcon = false;
	// 先先进行快速检测
	std::map<std::string, GameIconInfo>::iterator itr = akGameIconInfo.begin();
	for (; itr != akGameIconInfo.end(); itr++)
	{
		// 获取ICON数据
		GameIconInfo& kGameIconInfo = itr->second;
		
		// 第二次快速查找
		if (kGameIconInfo.iComPareBeginX > 0 && kGameIconInfo.iComPareBeginY > 0)
		{
			kGameIconInfo.fComPareRate = 0;
			int iPixelPass = 0;
			int iGameSnapX, iGameSnapY;
			for (int k = 0; k < kGameIconInfo.iPixelWidth; ++k)
			{
				for (int l = 0; l < kGameIconInfo.iPixelHeight; ++l)
				{
					iGameSnapX = kGameIconInfo.iComPareBeginX + k;
					iGameSnapY = kGameIconInfo.iComPareBeginY + l;
					m_aiPoolGameSnap[iGameSnapX][iGameSnapY] = GetPixel(hdcGameSnap, iGameSnapX, iGameSnapY);
					if (kGameIconInfo.aiPixelGameIcon[k * kGameIconInfo.iPixelHeight + l] == m_aiPoolGameSnap[iGameSnapX][iGameSnapY])
					{
						iPixelPass++;
					}
				}
			}

			float fComPareRate = iPixelPass * 1.0f / kGameIconInfo.iPixelWidth / kGameIconInfo.iPixelHeight;
			if (fComPareRate > kGameIconInfo.fComPareRate)
			{
				kGameIconInfo.fComPareRate = fComPareRate;
			}
		}
		else
		{
			bHasNormalIcon = true;
		}
	}
	return bHasNormalIcon;
}

void WowPlugin::ComPareImageNormal(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo)
{
	// 填充像素
	for (int i = iBeginX; i < iEndX; ++i)
	{
		for (int j = iBeginY; j < iEndY; ++j)
		{
			m_aiPoolGameSnap[i][j] = GetPixel(hdcGameSnap, i, j);
		}
	}

	// 匹配像素
	std::map<std::string, GameIconInfo>::iterator itr = akGameIconInfo.begin();
	for (; itr != akGameIconInfo.end(); itr++)
	{
		// 获取ICON数据
		GameIconInfo& kGameIconInfo = itr->second;
		if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
		{
			kGameIconInfo.fComPareRate = 0;

			// 第一次匹配下位置
			for (int i = iBeginX; i < iEndX; ++i)
			{
				bool iComPareSucess = false;
				for (int j = iBeginY; j < iEndY; ++j)
				{
					int iPixelPass = 0;
					for (int k = 0; k < kGameIconInfo.iPixelWidth; ++k)
					{
						for (int l = 0; l < kGameIconInfo.iPixelHeight; ++l)
						{
							if (kGameIconInfo.aiPixelGameIcon[k * kGameIconInfo.iPixelHeight + l] == m_aiPoolGameSnap[i + k][j + l])
							{
								iPixelPass++;
							}
						}
					}

					float fComPareRate = iPixelPass * 1.0f / kGameIconInfo.iPixelWidth / kGameIconInfo.iPixelHeight;
					if (fComPareRate > kGameIconInfo.fComPareRate)
					{
						kGameIconInfo.fComPareRate = fComPareRate;
						if (fComPareRate > 0.9f)
						{
							kGameIconInfo.iComPareBeginX = i;
							kGameIconInfo.iComPareBeginY = j;
							iComPareSucess = true;
							break;
						}
					}
				}
				if (iComPareSucess) break;
			}
		}
	}
}

void WowPlugin::ReadConfig()
{
	FILE* pkFile = fopen("WowPluginConfig.txt", "r");
	if (pkFile)
	{
		while (feof(pkFile) == 0)
		{
			char str[200];
			fgets(str, 200, pkFile);
			std::string kName = str;
			kName = kName.substr(0, kName.length() - 1);
			fgets(str, 200, pkFile);
			int iBeginX = atoi(str);
			fgets(str, 200, pkFile);
			int iBeginY = atoi(str);
			std::map<std::string, GameIconInfo>::iterator itr = m_akGameIconMidBottomInfo.find(kName);
			if (itr != m_akGameIconMidBottomInfo.end())
			{
				itr->second.iComPareBeginX = iBeginX;
				itr->second.iComPareBeginY = iBeginY;
			}
			itr = m_akGameIconLeftTopInfo.find(kName);
			if (itr != m_akGameIconLeftTopInfo.end())
			{
				itr->second.iComPareBeginX = iBeginX;
				itr->second.iComPareBeginY = iBeginY;
			}
			itr = m_akGameIconMidInfo.find(kName);
			if (itr != m_akGameIconMidInfo.end())
			{
				itr->second.iComPareBeginX = iBeginX;
				itr->second.iComPareBeginY = iBeginY;
			}
		}
	}
	fclose(pkFile);
}

void WowPlugin::SaveConfig()
{
	FILE* pkFile = fopen("WowPluginConfig.txt", "w+");
	if (pkFile)
	{
		std::map<std::string, GameIconInfo>::iterator itr = m_akGameIconMidBottomInfo.begin();
		for (; itr != m_akGameIconMidBottomInfo.end(); itr++)
		{
			GameIconInfo& kGameIconInfo = itr->second;
			fputs(itr->first.c_str(), pkFile);
			fputs("\n", pkFile);
			char str[20];
			itoa(kGameIconInfo.iComPareBeginX, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
			itoa(kGameIconInfo.iComPareBeginY, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
		}
		itr = m_akGameIconLeftTopInfo.begin();
		for (; itr != m_akGameIconLeftTopInfo.end(); itr++)
		{
			GameIconInfo& kGameIconInfo = itr->second;
			fputs(itr->first.c_str(), pkFile);
			fputs("\n", pkFile);
			char str[20];
			itoa(kGameIconInfo.iComPareBeginX, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
			itoa(kGameIconInfo.iComPareBeginY, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
		}
		itr = m_akGameIconMidInfo.begin();
		for (; itr != m_akGameIconMidInfo.end(); itr++)
		{
			GameIconInfo& kGameIconInfo = itr->second;
			fputs(itr->first.c_str(), pkFile);
			fputs("\n", pkFile);
			char str[20];
			itoa(kGameIconInfo.iComPareBeginX, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
			itoa(kGameIconInfo.iComPareBeginY, str, 10);
			fputs(str, pkFile);
			fputs("\n", pkFile);
		}
	}
	fclose(pkFile);
}