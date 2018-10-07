#include "WowPlugin.h"

#define SKILL_WUYA "������ѻ"
#define SKILL_LVYE "��Ҷ"
#define SKILL_HONGREN "����"
#define SKILL_SHALU "ɱ¾ָ��"
#define SKILL_YANJING "�۾������"
#define SKILL_QIMEILA "���������"
#define SKILL_DING "��������"
#define SKILL_DING_1 "����һ��"
#define SKILL_DUOCHONG "�������"
#define SKILL_DANMU "��Ļ���"
#define SKILL_CURE_PET "���Ƴ���"

#define BUFF_DING "����BUFF"
#define BUFF_SHUNPI	"˳��BUFF"
#define BUFF_SELF_POWERBAR "�Լ�����ֵ"
#define BUFF_PET_HP "��������ֵ"
#define BUFF_ENEMY_COUNT "���˸���"
#define BUFF_ENEMY_HP "������Ѫ��"
#define BUFF_DING_OUT "����BUFF�ϵ�"

#define FISH_ICON "������Ư"
#define FISH_CURSOR "�������"
#define FISH_CURSOR_NORMAL "��ͨ���"

WowPlugin::WowPlugin()
{
	m_iTargetHandle = 0;
	m_akGameIconMidBottomInfo.clear();
	//m_akGameIconLeftTopInfo.clear();
	//m_akGameIconMidInfo.clear();
	m_kGameIconMidFish.clear();
	m_iStartPlayGame = 0;
	m_akMessage.clear();
	m_bIsCurePet = false;
	m_iLastAltTime = false;
	m_bAutoFish = false;
	m_eAutoFishStep = AutoFishStep_Count;
	m_iAutoFishStepStartTime = 0;
	m_iStartMouseMoveTime = -1;
	m_iFishFuzzySumX = m_iFishFuzzySumY = m_iFishFuzzySumCount = 0;
}

WowPlugin::~WowPlugin()
{
	m_akGameIconMidBottomInfo.clear();
	//m_akGameIconLeftTopInfo.clear();
	//m_akGameIconMidInfo.clear();
	m_kGameIconMidFish.clear();
	m_kGameIconMidFishCursor.clear();
}

bool WowPlugin::InitGame()
{
	m_iTargetHandle = FindWindowA(NULL, "ħ������");

	// ��Ӽ���ͼ��
	AddGameIconInfo(SKILL_WUYA, "skill_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_LVYE, "skill_2.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_HONGREN, "skill_3.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_SHALU, "skill_4.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_YANJING, "skill_5.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_QIMEILA, "skill_6.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DING, "skill_7.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DING_1, "skill_7_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_DUOCHONG, "skill_8.bmp", m_akGameIconMidBottomInfo);
	//AddGameIconInfo(SKILL_DANMU, "skill_9.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(SKILL_CURE_PET, "skill_10.bmp", m_akGameIconMidBottomInfo);

	// ���BUFFͼ��
	AddGameIconInfo(BUFF_DING, "buff_1.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_SHUNPI, "buff_2.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_SELF_POWERBAR, "buff_3.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_PET_HP, "buff_4.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_ENEMY_COUNT, "buff_5.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_ENEMY_HP, "buff_6.bmp", m_akGameIconMidBottomInfo);
	AddGameIconInfo(BUFF_DING_OUT, "buff_7.bmp", m_akGameIconMidBottomInfo);

	//// ����м�BUFFͼ��
	//AddGameIconInfo(BUFF_DING1_1, "buff_1.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING, "buff_2.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_SHUNPI_ONE_SECOND, "buff_4.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_SHUNPI, "buff_5.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING_SKILL_1, "buff_6.bmp", m_akGameIconMidInfo);
	//AddGameIconInfo(BUFF_DING_SKILL_2, "buff_7.bmp", m_akGameIconMidInfo);

	//// ������Ͻ�Ѫ��ͼ��
	//AddGameIconInfo(BUFF_POWERBAR, "buff_3.bmp", m_akGameIconLeftTopInfo);

	// ����м����ͼ��
	AddGameIconInfo(FISH_ICON, "fish.bmp", m_kGameIconMidFish);

	return true;
}

void WowPlugin::GameLoop()
{
	if (m_iTargetHandle == 0)
	{
		m_iTargetHandle = FindWindowA(NULL, "ħ������");
	}
	ProcessInput();

	SendKeyUP();

	MouseMove();

	//CURSORINFO hCursor;
	//ZeroMemory(&hCursor, sizeof(hCursor));
	//hCursor.cbSize = sizeof(hCursor);
	//if (GetCursorInfo(&hCursor))
	//{
	//	ICONINFO iconInfo;
	//	if (GetIconInfo(hCursor.hCursor, &iconInfo))
	//	{
	//		char str[100];
	//		sprintf(str, "hcursor: %d, %d, %d, %d, %d\n", iconInfo.fIcon, iconInfo.hbmColor, iconInfo.hbmMask, iconInfo.xHotspot, iconInfo.yHotspot);
	//		//OutputDebugStringA(str);

	//		OutputDebugStringA("hcursor:");
	//		if (iconInfo.hbmColor != 0)
	//		{
	//			HDC hdcIcon = CreateCompatibleDC(NULL);
	//			BITMAP  bitMapIcon;
	//			GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitMapIcon);
	//			SelectObject(hdcIcon, iconInfo.hbmColor);

	//			for (int i = 0; i < bitMapIcon.bmWidth; ++i)
	//			{
	//				for (int j = 0; j < bitMapIcon.bmHeight; ++j)
	//				{
	//					char str[100];
	//					sprintf(str, "%d ", GetPixel(hdcIcon, i, j));
	//					OutputDebugStringA(str);
	//				}
	//				OutputDebugStringA("\n");
	//			}
	//			DeleteDC(hdcIcon);
	//		}
	//		OutputDebugStringA("\n");
	//	}
	//}

	// �Զ�����
	if (m_bAutoFish)
	{
		static int iLastTimeRefrashFish = timeGetTime();
		if (timeGetTime() - iLastTimeRefrashFish > 200)
		{
			iLastTimeRefrashFish = timeGetTime();
			AutoFish();
			return;
		}
	}

	// 100�Ǽ��ģʽ
	if (m_iStartPlayGame == 100)
	{
		static int iLastTimeRefrashNormal = timeGetTime();
		if (timeGetTime() - iLastTimeRefrashNormal > 500)
		{
			iLastTimeRefrashNormal = timeGetTime();

			RefrashIconInfoNormal();

			// ������ǲ������е�ͼ�궼ͨ����
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

			// ���еĶ�ͨ����д���ļ�
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

		if (timeGetTime() - iLastTimePlay > 100)
		{
			iLastTimePlay = timeGetTime();
			PlayGame();
		
		}
	}
}

void WowPlugin::MouseMove()
{
	if (m_iStartMouseMoveTime != -1)
	{
		int iLastTime = timeGetTime() - m_iStartMouseMoveTime;
		if (iLastTime > 400)
		{
			m_iStartMouseMoveTime = -1;
			SetCursorPos(m_iMouseTargetPosX, m_iMouseTargetPosY);
		}
		else
		{
			float fCurPosX = m_iMouseSrcPosX + (m_iMouseTargetPosX - m_iMouseSrcPosX) * iLastTime / 500.0f + rand() % 20 - 10;
			float fCurPosY = m_iMouseSrcPosY + (m_iMouseTargetPosY - m_iMouseSrcPosY) * iLastTime / 500.0f + rand() % 20 - 10;
			SetCursorPos(fCurPosX, fCurPosY);
		}
	}
}

void WowPlugin::AddMouseMove(int iPosX, int iPosY)
{
	POINT kPoint;
	if (GetCursorPos(&kPoint))
	{
		m_iMouseTargetPosX = iPosX;
		m_iMouseTargetPosY = iPosY;
		m_iMouseSrcPosX = kPoint.x;
		m_iMouseSrcPosY = kPoint.y;
		m_iStartMouseMoveTime = timeGetTime();
	}
}

void WowPlugin::SendKeyUP()
{
	// ������Ͱ���������Ϣ
	for (int i = 0; i < m_akMessage.size(); ++i)
	{
		if (timeGetTime() >= m_akMessage[i].iUpTime)
		{
			if (m_akMessage[i].iType == WM_KEYDOWN)
			{
				SendMessage(m_iTargetHandle, WM_KEYUP, m_akMessage[i].iMssange1, m_akMessage[i].iMessage2);
			}
			else if (m_akMessage[i].iType == WM_LBUTTONDOWN)
			{
				SendMessage(m_iTargetHandle, WM_LBUTTONUP, m_akMessage[i].iMssange1, m_akMessage[i].iMessage2);
			}
			
			char str[100];
			sprintf(str, "UP:%d\n", timeGetTime());
			OutputDebugStringA(str);

			m_akMessage.erase(m_akMessage.begin() + i);;
			i--;
		}
	}
}

void WowPlugin::AddMessage(unsigned int iType, int iMessage, int iMessageEx, char* acDebugStr)
{
	SendMessage(m_iTargetHandle, iType, iMessage, iMessageEx);

	int iRandTime = rand() % 20 + 20;
	if (iType == WM_LBUTTONDOWN)
	{
		iRandTime = 0;
	}
	m_akMessage.push_back(WowMessange(iType, iMessage, iMessageEx, timeGetTime(), timeGetTime() + iRandTime));

	char str[100];
	if (acDebugStr != nullptr)
	{
		sprintf(str, "DOWN_%s: RandTime:%d  StartTime:%d\n", acDebugStr, iRandTime, timeGetTime());
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
	GameIconInfo& kBuffDingOut = m_akGameIconMidBottomInfo[BUFF_DING_OUT];
	GameIconInfo& kDing_Skill_1 = m_akGameIconMidBottomInfo[SKILL_DING_1];
	GameIconInfo& kDing_Skill_2 = m_akGameIconMidBottomInfo[SKILL_DING];
	GameIconInfo& kBuffSelfPower = m_akGameIconMidBottomInfo[BUFF_SELF_POWERBAR];
	GameIconInfo& kBuffShunPi = m_akGameIconMidBottomInfo[BUFF_SHUNPI];
	GameIconInfo& kBuffPetHP = m_akGameIconMidBottomInfo[BUFF_PET_HP];
	GameIconInfo& kEnemyCount = m_akGameIconMidBottomInfo[BUFF_ENEMY_COUNT];
	GameIconInfo& kEnemyHP = m_akGameIconMidBottomInfo[BUFF_ENEMY_HP];
	GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_YANJING];

	if (kEnemyCount.fComPareRate <= 0.1f) return;


	// ��������
	if (m_bIsCurePet)
	{
		GameIconInfo& kSkillCurePet = m_akGameIconMidBottomInfo[SKILL_CURE_PET];
		if (kSkillCurePet.fComPareRate > 0.9f && kBuffPetHP.fComPareRate < 0.8f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD0, 0, "SKILL_CURE_PET");
			return;
		}
	}

	// ���̶ϵ��˾͵������ڼ�����
	if (kBuffDingOut.fComPareRate < 0.7f && kDing_Skill_2.fComPareRate > 0.9f || kBuffDingOut.fComPareRate > 0.9f)
	{
		// ������BUFFֻʣһ�룬���߶���BUFFû�е�״̬�£�ʹ�ö������, ����������ü��CD�������۾�������Ĺ���CD����
		if (kSkill5.fComPareRate > 0.9f && kBuffDing.fComPareRate < 0.7f && (kDing_Skill_1.fComPareRate > 0.9f || kDing_Skill_2.fComPareRate > 0.9f))
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD7, 0, "SKILL_DING ");
			return;
		}
	}

	// ����Ŀ��������Ҫȫ�̱���˳��
	if (kEnemyCount.fComPareRate > 0.7f)
	{
		GameIconInfo& kSkillT = m_akGameIconMidBottomInfo[SKILL_DUOCHONG];
		if (kSkillT.fComPareRate > 0.9f && kBuffShunPi.fComPareRate < 0.7f && kBuffSelfPower.fComPareRate > 0.5f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD8, 0, "SKILL_DUOCHONG ");
			return;
		}
	}

	if (kEnemyCount.fComPareRate > 0.3f)
	{
		// ��������������˾���
		GameIconInfo& kSkillR = m_akGameIconMidBottomInfo[SKILL_QIMEILA];
		if (kSkillR.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate < 0.88f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD6, 0, "SKILL_QIMEILA ");
			return;
		}

		// ɱ¾ָ��,���˾���
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		if (kSkill4.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD4, 0, "SKILL_SHALU ");
			return;
		}
	}
	else
	{
		// ɱ¾ָ��,���˾���
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		if (kSkill4.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD4, 0, "SKILL_SHALU ");
			return;
		}

		// ��������������˾���
		GameIconInfo& kSkillR = m_akGameIconMidBottomInfo[SKILL_QIMEILA];
		if (kSkillR.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate < 0.88f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD6, 0, "SKILL_QIMEILA ");
			return;
		}
	}
	

	// ������ѻ�����˾���
	GameIconInfo& kSkill1 = m_akGameIconMidBottomInfo[SKILL_WUYA];
	if (kSkill1.fComPareRate > 0.9f)
	{
		AddMessage(WM_KEYDOWN, VK_NUMPAD1, 0, "SKILL_WUYA ");
		return;
	}

	// ��Ұŭ�𣬺��˾���
	GameIconInfo& kSkill3 = m_akGameIconMidBottomInfo[SKILL_HONGREN];
	if (kSkill3.fComPareRate > 0.9f)
	{
		if (m_iStartPlayGame == 2 && kEnemyHP.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD3, 0, "SKILL_HONGREN ");
			return;
		}
		else if (m_iStartPlayGame == 1)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD3, 0, "SKILL_HONGREN ");
			return;
		}
			
		
	}

	// Ұ���ػ������˾��� �ӱ����ؼ���ֵ
	GameIconInfo& kSkill2 = m_akGameIconMidBottomInfo[SKILL_LVYE];
	if (kSkill2.fComPareRate > 0.9f)
	{
		if (m_iStartPlayGame == 2 && kEnemyHP.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD2, 0, "SKILL_LVYE ");
			return;
		}
		else if (m_iStartPlayGame == 1)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD2, 0, "SKILL_LVYE ");
			return;
		}
	}
	

	// ��Ŀ��״̬״̬ û��˳��BUFF������˳��BUFFС��1�� ��ʹ�ö������
	if (kEnemyCount.fComPareRate >= 0.3f)
	{
		GameIconInfo& kSkillT = m_akGameIconMidBottomInfo[SKILL_DUOCHONG];
		if (kSkillT.fComPareRate > 0.9f && kBuffShunPi.fComPareRate < 0.7f && kBuffSelfPower.fComPareRate > 0.5f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD8, 0, "SKILL_DUOCHONG ");
			return;
		}

		// ��Ļ��������˾���
		/*GameIconInfo& kSkillG = m_akGameIconMidBottomInfo[SKILL_DANMU];
		if (kSkillG.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD9, 0, "SKILL_DANMU ");
			return;
		}*/
	}

	if (kEnemyCount.fComPareRate < 0.3f)
	{
		// ��Ŀ�� �۾�������� ����ֵ�����ͬʱɱ¾ָ�������ȴ
		GameIconInfo& kSkill4 = m_akGameIconMidBottomInfo[SKILL_SHALU];
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_YANJING];
		if (kSkill5.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate > 0.5f && kSkill4.fComPareRate < 0.5f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD5, 0, "SKILL_YANJING ");
			return;
		}
	}
	else
	{
		// ��Ŀ�� ����ֵ����� ������˳��BUFF ����˳��BUFF����1��
		GameIconInfo& kSkill5 = m_akGameIconMidBottomInfo[SKILL_YANJING];
		if (kSkill5.fComPareRate > 0.9f && kBuffSelfPower.fComPareRate > 0.7f && kBuffShunPi.fComPareRate > 0.9f)
		{
			AddMessage(WM_KEYDOWN, VK_NUMPAD5, 0, "SKILL_YANJING ");
			return;
		}
	}
}



void WowPlugin::AutoFish()
{
	static int iLastFishPosY = -1;
	static int iLastFishPosX = -1;
	static int iMaxFishPosY = -1;
	static int iMinFishPosY = -1;
	static bool bFindFishDif = false;

	switch (m_eAutoFishStep)
	{
	case AutoFishStep_Start:
	{
							   if (timeGetTime() - m_iAutoFishStepStartTime > 1500)
							   {
								   m_iAutoFishStepStartTime = timeGetTime();
								   AddMessage(WM_KEYDOWN, VK_DELETE, 0, "AUTO_FISH");

								   //OutputDebugStringA("AutoFishStep_Start");
								   char str[100];
								   sprintf(str, "MinPosY: %d, MaxPosY: %d\n", iMinFishPosY, iMaxFishPosY);
								   OutputDebugStringA(str);

								   iLastFishPosY = iLastFishPosX = -1;
								   iMaxFishPosY = iMinFishPosY = -1;
								   bFindFishDif = false;
								   m_eAutoFishStep = AutoFishStep_Glint;
								   AddMouseMove(960 + rand() % 800 - 400, 780 - rand() % 300);

								   
							   }
							   
							   break;
	}
		
	case AutoFishStep_Glint:
	{
							   if (timeGetTime() - m_iAutoFishStepStartTime > 1000)
							   {
								   m_iAutoFishStepStartTime = timeGetTime();
								   m_eAutoFishStep = AutoFishStep_FindFish;
								   //OutputDebugStringA("AutoFishStep_Glint");
							   }
							   break;
	}
	case AutoFishStep_FindFish:
	{
								  if (timeGetTime() - m_iAutoFishStepStartTime > 16000)
								  {
									  m_eAutoFishStep = AutoFishStep_Start;
									  m_iAutoFishStepStartTime = timeGetTime();
								  }
								  else
								  {
										RefrashFishIcon();
										GameIconInfo& kFish = m_kGameIconMidFish[FISH_ICON];
										if (kFish.fComPareRate > 0)
										{
											iLastFishPosY = kFish.iComPareBeginY;
											iLastFishPosX = kFish.iComPareBeginX;
											AddMouseMove(iLastFishPosX + 10, iLastFishPosY - 32);
											m_iAutoFishStepStartTime = timeGetTime();
											m_eAutoFishStep = AutoFishStep_WaitFish;

											char str[100];
											sprintf(str, "fishPosX: %d, fishPosY: %d, %d\n", kFish.iComPareBeginX, kFish.iComPareBeginY, timeGetTime());
											OutputDebugStringA(str);
										}
								  }

								  
								  break;
	}
	case AutoFishStep_WaitFish:
	{
								  if (timeGetTime() - m_iAutoFishStepStartTime > 16000)
								  {
									  m_eAutoFishStep = AutoFishStep_Start;
									  m_iAutoFishStepStartTime = timeGetTime();
								  }
								  else if (timeGetTime() - m_iAutoFishStepStartTime > 700)
								  {
									  CURSORINFO hCursor;
									  ZeroMemory(&hCursor, sizeof(hCursor));
									  hCursor.cbSize = sizeof(hCursor);
									  if (GetCursorInfo(&hCursor))
									  {
									  	ICONINFO iconInfo;
									  	if (GetIconInfo(hCursor.hCursor, &iconInfo))
									  	{
									  		if (iconInfo.hbmColor != 0)
									  		{
									  			HDC hdcIcon = CreateCompatibleDC(NULL);
									  			BITMAP  bitMapIcon;
									  			GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitMapIcon);
									  			SelectObject(hdcIcon, iconInfo.hbmColor);

												if (bFindFishDif)
												{
													if (GetPixel(hdcIcon, 1, 1) == 10262924)
													{
														m_eAutoFishStep = AutoFishStep_GetFish;
														m_iAutoFishStepStartTime = timeGetTime();
													}
												}
												else
												{
													if (GetPixel(hdcIcon, 1, 1) == 10262924)
													{
														SetCursorPos(hCursor.ptScreenPos.x, hCursor.ptScreenPos.y + 1);
														bFindFishDif = true;
													}
													else if (GetPixel(hdcIcon, 1, 1) == 9620713)
													{
														SetCursorPos(hCursor.ptScreenPos.x, hCursor.ptScreenPos.y - 1);
													}
												}
												
									  			DeleteDC(hdcIcon);
									  		}
									  	}
									  }
								  }
								  break;
	}
		
	case AutoFishStep_GetFish:
	{
								 if (timeGetTime() - m_iAutoFishStepStartTime > 10000)
								 {
									 m_eAutoFishStep = AutoFishStep_Start;
									 m_iAutoFishStepStartTime = timeGetTime();
								 }
								 else if (timeGetTime() - m_iAutoFishStepStartTime > 100)
								 {
									 CURSORINFO hCursor;
									 ZeroMemory(&hCursor, sizeof(hCursor));
									 hCursor.cbSize = sizeof(hCursor);
									 if (GetCursorInfo(&hCursor))
									 {
										 ICONINFO iconInfo;
										 if (GetIconInfo(hCursor.hCursor, &iconInfo))
										 {
											 if (iconInfo.hbmColor != 0)
											 {
												 HDC hdcIcon = CreateCompatibleDC(NULL);
												 BITMAP  bitMapIcon;
												 GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitMapIcon);
												 SelectObject(hdcIcon, iconInfo.hbmColor);

												 if (GetPixel(hdcIcon, 1, 1) == 9620713)
												 {
													 POINT kPoint;
													 if (GetCursorPos(&kPoint))
													 {
														 AddMessage(WM_LBUTTONDOWN, 0, MAKELONG(kPoint.x, kPoint.y), "GET_FISH");
													 }

													 m_eAutoFishStep = AutoFishStep_Start;
													 m_iAutoFishStepStartTime = timeGetTime();
												 }
												 DeleteDC(hdcIcon);
											 }
										 }
									 }

									 
								 }
								 
								 //OutputDebugStringA("AutoFishStep_GetFish");
								 break;
	}	
	case AutoFishStep_Count:
	{
							   m_eAutoFishStep = AutoFishStep_Start;
							   m_iAutoFishStepStartTime = timeGetTime();
							   break;
	}
	default:
		break;
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
	HDC       hScrDC, hMemDC;// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;  // λͼ���
	int       nX, nY;      	// ѡ����������
	int       nWidth, nHeight;

	RECT* pRtWnd = NULL;
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &windowInfo);
	pRtWnd = &windowInfo.rcWindow;

	// ���ѡ����������
	nX = pRtWnd->left;
	nY = pRtWnd->top;

	nWidth = pRtWnd->right - pRtWnd->left;
	nHeight = pRtWnd->bottom - pRtWnd->top;

	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL); //Ϊ��Ļ�����豸������	
	hMemDC = CreateCompatibleDC(hScrDC);             //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); // ����һ������Ļ�豸��������ݵ�λͼ	
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	// ����λͼѡ���ڴ��豸��������

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
		int iMaxRed = 0, iMinRed = 255, iMaxGreen = 0, iMinGreen = 255, iMaxBlue = 0, iMinBlue = 255;
		for (int i = 0; i < bitMapIcon.bmWidth; ++i)
		{
			for (int j = 0; j < bitMapIcon.bmHeight; ++j)
			{
				kGameIconInfo.aiPixelGameIcon[i * bitMapIcon.bmHeight + j] = GetPixel(hdcIcon, i, j);
				int iRed = GetRValue(kGameIconInfo.aiPixelGameIcon[i * bitMapIcon.bmHeight + j]);
				int iGreen = GetGValue(kGameIconInfo.aiPixelGameIcon[i * bitMapIcon.bmHeight + j]);
				int iBlue = GetBValue(kGameIconInfo.aiPixelGameIcon[i * bitMapIcon.bmHeight + j]);
				if (iMaxRed < iRed) iMaxRed = iRed;
				if (iMinRed > iRed) iMinRed = iRed;
				if (iMaxGreen < iGreen) iMaxGreen = iGreen;
				if (iMinGreen > iGreen) iMinGreen = iGreen;
				if (iMaxBlue < iBlue) iMaxBlue = iBlue;
				if (iMinBlue > iBlue) iMinBlue = iBlue;
			}
		}
		kGameIconInfo.iMaxColor = RGB(iMaxRed, iMaxGreen, iMaxBlue);
		kGameIconInfo.iMinColor = RGB(iMinRed, iMinGreen, iMinBlue);
		DeleteDC(hdcIcon);
		::DeleteObject(hBitMapIcon);
	}
	else
	{
		std::string kStrWaring = "����ICONͼƬʧ��:";
		kStrWaring = kStrWaring + kFileName;
		MessageBoxA(NULL, kStrWaring.c_str(), "Warning", MB_OK);
	}
}

#define MID_BOTTOM	5,		500,	915,	1080
//#define LEFT_TOP	50,		280,	20,		150
//#define MID_MID		5,		500,	915,	1080

#define MID_FISH	680,	1240,	400,	890

void WowPlugin::RefrashIconInfoFast()
{
	HBITMAP hBitMapGameSnap = ScreenShotByWndPos(m_iTargetHandle);
	if (hBitMapGameSnap != 0)
	{
		// ��ȡ��Ļ��ͼ����
		BITMAP  bitMapGamSnap;
		GetObject(hBitMapGameSnap, sizeof(BITMAP), &bitMapGamSnap);
		HDC hdcGameSnap = CreateCompatibleDC(NULL);
		SelectObject(hdcGameSnap, hBitMapGameSnap);

		// ���ҷ���������
		ComPareImageFast(MID_BOTTOM, hdcGameSnap, m_akGameIconMidBottomInfo);
		//// ���Ͻ�Ѫ������
		//ComPareImageFast(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		//// �м�BUFF����
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
		// ��ȡ��Ļ��ͼ����
		BITMAP  bitMapGamSnap;
		GetObject(hBitMapGameSnap, sizeof(BITMAP), &bitMapGamSnap);
		HDC hdcGameSnap = CreateCompatibleDC(NULL);
		SelectObject(hdcGameSnap, hBitMapGameSnap);

		// ���ҷ���������
		ComPareImageNormal(MID_BOTTOM, hdcGameSnap, m_akGameIconMidBottomInfo);
		//// ���Ͻ�Ѫ������
		//ComPareImageNormal(LEFT_TOP, hdcGameSnap, m_akGameIconLeftTopInfo);
		//// �м�BUFF����
		//ComPareImageNormal(MID_MID, hdcGameSnap, m_akGameIconMidInfo);

		::DeleteDC(hdcGameSnap);
		::DeleteObject(hBitMapGameSnap);
	}
}

void WowPlugin::RefrashFishIcon()
{
	HBITMAP hBitMapGameSnap = ScreenShotByWndPos(m_iTargetHandle);
	if (hBitMapGameSnap != 0)
	{
		// ��ȡ��Ļ��ͼ����
		BITMAP  bitMapGamSnap;
		GetObject(hBitMapGameSnap, sizeof(BITMAP), &bitMapGamSnap);
		HDC hdcGameSnap = CreateCompatibleDC(NULL);
		SelectObject(hdcGameSnap, hBitMapGameSnap);

		// �����Ư
		ComPareImageNormalFuzzy(MID_FISH, hdcGameSnap, m_kGameIconMidFish);

		::DeleteDC(hdcGameSnap);
		::DeleteObject(hBitMapGameSnap);
	}
}

// ���ټ�⣬ǰ�����Ѿ�normal���ͨ��һ�Σ�����ʼ����
bool WowPlugin::ComPareImageFast(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo)
{
	bool bHasNormalIcon = false;
	// ���Ƚ��п��ټ��
	std::map<std::string, GameIconInfo>::iterator itr = akGameIconInfo.begin();
	for (; itr != akGameIconInfo.end(); itr++)
	{
		// ��ȡICON����
		GameIconInfo& kGameIconInfo = itr->second;
		
		// �ڶ��ο��ٲ���
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
	// �������
	for (int i = iBeginX; i < iEndX; ++i)
	{
		for (int j = iBeginY; j < iEndY; ++j)
		{
			m_aiPoolGameSnap[i][j] = GetPixel(hdcGameSnap, i, j);
		}
	}

	// ƥ������
	std::map<std::string, GameIconInfo>::iterator itr = akGameIconInfo.begin();
	for (; itr != akGameIconInfo.end(); itr++)
	{
		// ��ȡICON����
		GameIconInfo& kGameIconInfo = itr->second;
		if (kGameIconInfo.iComPareBeginX < 0 || kGameIconInfo.iComPareBeginY < 0)
		{
			kGameIconInfo.fComPareRate = 0;

			// ��һ��ƥ����λ��
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
						if (fComPareRate > 0.99f)
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

void WowPlugin::ComPareImageNormalFuzzy(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo)
{
	// ƥ������
	std::map<std::string, GameIconInfo>::iterator itr = akGameIconInfo.begin();
	for (; itr != akGameIconInfo.end(); itr++)
	{
		// ��ȡICON����
		GameIconInfo& kGameIconInfo = itr->second;
		kGameIconInfo.fComPareRate = 0;
		kGameIconInfo.iComPareBeginX = 0;
		kGameIconInfo.iComPareBeginY = 0;

		// �Ѿ�ƥ�����˾�������Χ
		if (m_iFishFuzzySumX != 0)
		{
			iBeginX = m_iFishFuzzySumX * 1.0f / m_iFishFuzzySumCount - 200;
			iEndX = m_iFishFuzzySumX * 1.0f / m_iFishFuzzySumCount + 200;
			iBeginY = m_iFishFuzzySumY * 1.0f / m_iFishFuzzySumCount - 150;
			iEndY = m_iFishFuzzySumY * 1.0f / m_iFishFuzzySumCount + 150;
		}

		// �������
		for (int i = iBeginX; i < iEndX; ++i)
		{
			for (int j = iBeginY; j < iEndY; ++j)
			{
				m_aiPoolGameSnap[i][j] = GetPixel(hdcGameSnap, i, j);
			}
		}

		// ��ȡ���ط�Χ
		int iMaxRed = GetRValue(kGameIconInfo.iMaxColor);
		int iMaxGreen = GetGValue(kGameIconInfo.iMaxColor);
		int iMaxBlue = GetBValue(kGameIconInfo.iMaxColor);
		int iMinRed = GetRValue(kGameIconInfo.iMinColor);
		int iMinGreen = GetGValue(kGameIconInfo.iMinColor);
		int iMinBlue = GetBValue(kGameIconInfo.iMinColor);
		int iCompareCount = 0;
		// ƥ������
		//for (int k = 0; k < kGameIconInfo.iPixelWidth; ++k)
		//{
			//for (int l = 0; l < kGameIconInfo.iPixelHeight; ++l)
			//{
				for (int i = iBeginX; i < iEndX; ++i)
				{
					for (int j = iBeginY; j < iEndY; ++j)
					{
						int iRed = GetRValue(m_aiPoolGameSnap[i][j]);
						int iGreen = GetGValue(m_aiPoolGameSnap[i][j]);
						int iBlue = GetBValue(m_aiPoolGameSnap[i][j]);

						//if (kGameIconInfo.aiPixelGameIcon[k * kGameIconInfo.iPixelHeight + l] == m_aiPoolGameSnap[i][j])
						if(iRed <= iMaxRed && iRed >= iMinRed && iGreen <= iMaxGreen && iGreen >= iMinGreen && iBlue <= iMaxBlue && iBlue >= iMinBlue)
						{
							iCompareCount++;
							kGameIconInfo.fComPareRate = 2;
							kGameIconInfo.iComPareBeginX += i;
							kGameIconInfo.iComPareBeginY += j;
						}
					}
				}
			//}
		//}

		// ƥ��ʧ�����÷�Χ
		if (kGameIconInfo.fComPareRate < 1.0f)
		{
			kGameIconInfo.iComPareBeginX = -1;
			kGameIconInfo.iComPareBeginY = -1;
		}
		else
		{
			kGameIconInfo.iComPareBeginX = kGameIconInfo.iComPareBeginX * 1.0f / iCompareCount;
			kGameIconInfo.iComPareBeginY = kGameIconInfo.iComPareBeginY * 1.0f / iCompareCount;
			if (m_iFishFuzzySumCount < 20)
			{
				m_iFishFuzzySumX += kGameIconInfo.iComPareBeginX;
				m_iFishFuzzySumY += kGameIconInfo.iComPareBeginY;
				m_iFishFuzzySumCount++;
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