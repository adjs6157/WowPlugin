#include "WowPlugin.h"

#define SKILL_WUYA "夺命乌鸦"
#define SKILL_LVYE "绿叶"
#define SKILL_HONGREN "红人"
#define SKILL_SHALU "杀戮指令"
#define SKILL_YANJING "眼镜蛇射击"
#define SKILL_QIMEILA "奇美拉射击"
#define SKILL_DING "钉刺两层"
#define SKILL_DING_1 "钉刺一层"
#define SKILL_DUOCHONG "多重射击"
#define SKILL_DANMU "弹幕射击"
#define SKILL_CURE_PET "治疗宠物"

#define BUFF_DING "钉刺BUFF"
#define BUFF_SHUNPI	"顺劈BUFF"
#define BUFF_SELF_POWERBAR "自己集中值"
#define BUFF_PET_HP "宠物生命值"
#define BUFF_ENEMY_COUNT "敌人个数"
#define BUFF_ENEMY_HP "敌人总血量"

WowPlugin::WowPlugin()
{
	m_iTargetHandle = 0;
	m_akGameIconMidBottomInfo.clear();
	//m_akGameIconLeftTopInfo.clear();
	//m_akGameIconMidInfo.clear();
	m_iStartPlayGame = 0;
	m_aiMessage.clear();
	m_aiMessageUpTime.clear();
	m_aiMessangeDownTime.clear();
	m_bIsCurePet = false;
	m_iLastAltTime = false;
}

WowPlugin::~WowPlugin()
{
	m_akGameIconMidBottomInfo.clear();
	//m_akGameIconLeftTopInfo.clear();
	//m_akGameIconMidInfo.clear();
}

bool WowPlugin::InitGame()
{
	m_iTargetHandle = FindWindowA(NULL, "魔兽世界");

	// 添加技能图标
	AddGameIconInfo(SKILL_WUYA, "skill_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_LVYE, "skill_2.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_HONGREN, "skill_3.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_SHALU, "skill_4.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_YANJING, "skill_5.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_QIMEILA, "skill_6.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DING, "skill_7.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DING_1, "skill_7_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DUOCHONG, "skill_8.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DANMU, "skill_9.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_CURE_PET, "skill_10.bmp", m_akGameIconMidBottomInfo);

	// 添加BUFF图标
	AddGameIconInfo(BUFF_DING, "buff_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_SHUNPI, "buff_2.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_SELF_POWERBAR, "buff_3.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_PET_HP, "buff_4.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_ENEMY_COUNT, "buff_5.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_ENEMY_HP, "buff_6.bmp", m_akGameIconMidBottomInfo);

	//// 添加中间BUFF图标
	//AddGameIconInfo(BUFF_DING1_1, "buff_1.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING, "buff_2.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_SHUNPI_ONE_SECOND, "buff_4.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_SHUNPI, "buff_5.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING_SKILL_1, "buff_6.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING_SKILL_2, "buff_7.bmp", m_akGameIconMidInfo);

	//// 添加右上角血条图标
	//AddGameIconInfo(BUFF_POWERBAR, "buff_3.bmp", m_akGameIconLeftTopInfo);

	return true;
}

void WowPlugin::GameLoop()
{
	if (m_iTargetHandle == 0)
	{
		m_iTargetHandle = FindWindowA(NULL, "魔兽世界");
	}
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
			//itr = m_akGameIconLeftTopInfo.begin();
			//for (; itr != m_akGameIconLeftTopInfo.end(); itr++)
			//{
			//	GameIconInfo& kGameIconInfo = itr->second;
			//	if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
			//	{
			//		bAllPass = false;
			//	}
			//}
			//itr = m_akGameIconMidInfo.begin();
			//for (; itr != m_akGameIconMidInfo.end(); itr++)
			//{
			//	GameIconInfo& kGameIconInfo = itr->second;
			//	if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
			//	{
			//		bAllPass = false;
			//	}
			//}

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

		for (int i = 0; i < m_aiMessage.size(); ++i)
		{
			if (timeGetTime() >= m_aiMessageUpTime[i])
			{
				SendMessage(m_iTargetHandle, WM_KEYUP, m_aiMessage[i], 0);
				char str[100];
				sprintf(str, "%d\n", timeGetTime() - m_aiMessangeDownTime[i]);
				OutputDebugStringA(str);
				m_aiMessage.erase(m_aiMessage.begin() + i);
				m_aiMessageUpTime.erase(m_aiMessageUpTime.begin() + i);
				m_aiMessangeDownTime.erase(m_aiMessangeDownTime.begin() + i);
				i--;
			}
		}

		if (timeGetTime() - iLastTimePlay > 100)
		{
			iLastTimePlay = timeGetTime();
			PlayGame();
		
		}
	}
}

void WowPlugin::AddMessage(int iMessage, char* acDebugStr)
{
	SendMessage(m_iTargetHandle, WM_KEYDOWN, iMessage, 0);

	int iRandTime = rand() % 20 + 20;
	m_aiMessageUpTime.push_back(timeGetTime() + iRandTime);
	m_aiMessage.push_back(iMessage);
	m_aiMessangeDownTime.push_back(timeGetTime());

	char str[100];
	if (acDebugStr != nullptr)
	{
		sprintf(str, "%s: %d\n", acDebugStr, iRandTime);
		OutputDebugStringA(str);
	}
}

void WowPlugin::PlayGame()
{
	if (m_bIsAltStopAttack)
	{
		if (timeGetTime() - m_iLastAltTime < 1000)
		{
			return;
		}
	}

	static int iLastTime = timeGetTime();
	int iDifTime = timeGetTime() - iLastTime;
	iLastTime = timeGetTime();
	char str[100];
	sprintf(str, "PlayGameDifTime:%d\n", iDifTime);
	OutputDebugStringA(str);

	GameIconInfo& kBuffDing = m_akGameIconMidBottomInfo[BUFF_DING];
	GameIconInfo& kDing_Skill_1 = m_akGameIconMidBottomInfo[SKILL_DING_1];
	GameIconInfo& kDing_Skill_2 = m_akGameIconMidBottomInfo[SKILL_DING];
	GameIconInfo& kBuffSelfPower = m_akGameIconMidBottomInfo[BUFF_SELF_POWERBAR];
	GameIconInfo& kBuffShunPi = m_akGameIconMidBottomInfo[BUFF_SHUNPI];
	GameIconInfo& kBuffPetHP = m_akGameIconMidBottomInfo[BUFF_PET_HP];
	GameIconInfo& kEnemyCount = m_akGameIconMidBottomInfo[BUFF_ENEMY_COUNT];
	GameIconInfo& kEnemyHP = m_akGameIconMidBottomInfo[BUFF_ENEMY_HP];


	// 宠物治疗
	if (m_bIsCurePet)
	{
		GameIconInfo& kSkillCurePet = m_akGameIconMidBottomInfo[SKILL_CURE_PET];
		if (kSkillCurePet.fComPareRate > 0.9f && kBuffPetHP.fComPareRate < 0.8f)
		{
			AddMessage(VK_NUMPAD0, "SKILL_CURE_PET");
			return;
		}
	}

	// 当钉刺BUFF只剩一秒，或者钉刺BUFF没有的状态下，使用钉刺射击
	if (kDing_Skill_2.fComPareRate > 0.9f || kBuffDing.fComPareRate < 0.7f && (kDing_Skill_1.fComPareRate > 0.9f || kDing_Skill_2.fComPareRate > 0.9f))
	{
		AddMessage(VK_NUMPAD7, "SKILL_DING ");
		return;
	}

	// 三个目标以上需要全程保持顺劈
	if (kEnemyCount.fComPareRate > 0.7f)
	{
		GameIconInfo& kSkillT = m_akGameIconMidBottomInfo[SKILL_DUOCHONG];
		if (kSkillT.fComPareRate > 0.9f && kBuffShunPi.fComPareRate < 0.7f && kBuffSelfPower.fComPareRate > 0.5f)
		{
			AddMessage(VK_NUMPAD8, "SKILL_DUOCHONG ");
			return;
		}
	}

	if (kEnemyCount.fComPareRate > 0.3f)
	{
		// 奇美拉射击，好了就用
		GameIconInfo& kSkillR = m_akGameIconMidBottomInfo[SKILL_QIMEILA];
		if (kSkillR.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate < 0.88f)
		{
			AddMessage(VK_NUMPAD6, "SKILL_QIMEILA ");
			return;
		}

		// 杀戮指令,好了就用
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		if (kSkill4.fComPareRate > 0.9f)
		{
			AddMessage(VK_NUMPAD4, "SKILL_SHALU ");
			return;
		}
	}
	else
	{
		// 杀戮指令,好了就用
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		if (kSkill4.fComPareRate > 0.9f)
		{
			AddMessage(VK_NUMPAD4, "SKILL_SHALU ");
			return;
		}

		// 奇美拉射击，好了就用
		GameIconInfo& kSkillR = m_akGameIconMidBottomInfo[SKILL_QIMEILA];
		if (kSkillR.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate < 0.88f)
		{
			AddMessage(VK_NUMPAD6, "SKILL_QIMEILA ");
			return;
		}
	}
	

	// 夺命黑鸦，好了就用
	GameIconInfo& kSkill1 = m_akGameIconMidBottomInfo[SKILL_WUYA];
	if (kSkill1.fComPareRate > 0.9f)
	{
		AddMessage(VK_NUMPAD1, "SKILL_WUYA ");
		return;
	}

	// 狂野怒火，好了就用
	GameIconInfo& kSkill3 = m_akGameIconMidBottomInfo[SKILL_HONGREN];
	if (kSkill3.fComPareRate > 0.9f && kEnemyHP.fComPareRate > 0.9f)
	{
		AddMessage(VK_NUMPAD3, "SKILL_HONGREN ");
		return;
	}

	// 野性守护，好了就用 加暴击回集中值
	GameIconInfo& kSkill2 = m_akGameIconMidBottomInfo[SKILL_LVYE];
	if (kSkill2.fComPareRate > 0.9f && kEnemyHP.fComPareRate > 0.9f)
	{
		AddMessage(VK_NUMPAD2, "SKILL_LVYE ");
		return;
	}
	

	// 多目标状态状态 没有顺劈BUFF，或者顺劈BUFF小于1秒 就使用多重射击
	if (kEnemyCount.fComPareRate >= 0.3f)
	{
		GameIconInfo& kSkillT = m_akGameIconMidBottomInfo[SKILL_DUOCHONG];
		if (kSkillT.fComPareRate > 0.9f && kBuffShunPi.fComPareRate < 0.7f && kBuffSelfPower.fComPareRate > 0.5f)
		{
			AddMessage(VK_NUMPAD8, "SKILL_DUOCHONG ");
			return;
		}

		// 弹幕射击，好了就用
		GameIconInfo& kSkillG = m_akGameIconMidBottomInfo[SKILL_DANMU];
		if (kSkillG.fComPareRate > 0.9f)
		{
			AddMessage(VK_NUMPAD9, "SKILL_DANMU ");
			return;
		}
	}

	if (kEnemyCount.fComPareRate < 0.3f)
	{
		// 单目标 眼镜蛇射击， 集中值溢出，同时杀戮指令进入冷却
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_YANJING];
		if (kSkill5.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate > 0.5f && kSkill4.fComPareRate < 0.5f)
		{
			AddMessage(VK_NUMPAD5, "SKILL_YANJING ");
			return;
		}
	}
	else
	{
		// 多目标 集中值溢出， 并且有顺劈BUFF 并且顺劈BUFF大于1秒
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_YANJING];
		if (kSkill5.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate > 0.7f && kBuffShunPi.fComPareRate > 0.9f)
		{
			AddMessage(VK_NUMPAD5, "SKILL_YANJING ");
			return;
		}
	}
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
		if (m_bIsAltStopAttack)
		{
			if (GetKeyState(VK_MENU) & 0x800)
			{
				m_iLastAltTime = timeGetTime();
			}
		}
		
		/*static int itime = 0;
		if (GetKeyState('F') & 0x800)
		{
			itime = timeGetTime();
		}
		else if (itime != 0)
		{
			int t = timeGetTime();
			int t1 = t - itime;
			char str[100];
			sprintf(str, "%d\n", t1);
			OutputDebugStringA(str);
			itime = 0;
		}*/
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

#define MID_BOTTOM	5,		500,	915,	1080
//#define LEFT_TOP	50,		280,	20,		150
//#define MID_MID		5,		500,	915,	1080

void WowPlugin::RefrashIconInfoFast()
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
		ComPareImageFast(MID_BOTTOM, hdcGameSnap, m_akGameIconMidBottomInfo);
		//// 左上角血条区域
		//ComPareImageFast(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		//// 中间BUFF区域
		//ComPareImageFast(MID_MID, hdcGameSnap, m_akGameIconMidInfo);

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
		ComPareImageNormal(MID_BOTTOM, hdcGameSnap, m_akGameIconMidBottomInfo);
		//// 左上角血条区域
		//ComPareImageNormal(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		//// 中间BUFF区域
		//ComPareImageNormal(MID_MID, hdcGameSnap, m_akGameIconMidInfo);

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
			//itr = m_akGameIconLeftTopInfo.find(kName);
			//if (itr != m_akGameIconLeftTopInfo.end())
			//{
			//	itr->second.iComPareBeginX = iBeginX;
			//	itr->second.iComPareBeginY = iBeginY;
			//}
			//itr = m_akGameIconMidInfo.find(kName);
			//if (itr != m_akGameIconMidInfo.end())
			//{
			//	itr->second.iComPareBeginX = iBeginX;
			//	itr->second.iComPareBeginY = iBeginY;
			//}
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
		/*itr = m_akGameIconLeftTopInfo.begin();
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
		}*/
	}
	fclose(pkFile);
}