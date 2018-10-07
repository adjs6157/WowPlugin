#include <windows.h>
//#include <timeapi.h>
#include <wingdi.h>
#include <Gdiplus.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace Gdiplus;

enum AutoFishStep
{
	AutoFishStep_Start = 0,
	AutoFishStep_Glint,
	AutoFishStep_FindFish,
	AutoFishStep_WaitFish,
	AutoFishStep_GetFish,
	AutoFishStep_Count,
};

struct GameIconInfo
{
	GameIconInfo()
	{
		hGameIconBitMap = 0;
		iComPareBeginX = -1;
		iComPareBeginY = -1;
		fComPareRate = 0;
		iPixelWidth = 0;
		iPixelHeight = 0;
		aiPixelGameIcon = NULL;
	}
	~GameIconInfo()
	{
		if (aiPixelGameIcon != NULL)
		{
			delete aiPixelGameIcon;
		}
	}

	HBITMAP hGameIconBitMap;
	int iComPareBeginX;
	int iComPareBeginY;
	float fComPareRate;
	int iPixelWidth;
	int iPixelHeight;
	DWORD* aiPixelGameIcon;
};

struct WowMessange
{
	WowMessange()
	{
		iType = 0;
		iMssange1 = 0;
		iMessage2 = 0;
		iDownTime = 0;
		iUpTime = 0;
	}

	WowMessange(int _iType, unsigned int _iMssange1, unsigned int _iMessage2, int _iDownTime, int _iUpTime)
	{
		iType = _iType;
		iMssange1 = _iMssange1;
		iMessage2 = _iMessage2;
		iDownTime = _iDownTime;
		iUpTime = _iUpTime;
	}
	unsigned int iType;
	unsigned int iMssange1;
	unsigned int iMessage2;
	int iDownTime;
	int iUpTime;
};

class WowPlugin
{
public:
	WowPlugin();
	~WowPlugin();

	bool InitGame();
	void GameLoop();

	void SetIsCurePet(bool bValue) { m_bIsCurePet = bValue; }
	void SetIsAltStopAttack(bool bValue) { m_bIsAltStopAttack = bValue; }
	void SetIsAutoFish(bool bValue) { m_bAutoFish = bValue; m_iFishFuzzySumX = m_iFishFuzzySumY = m_iFishFuzzySumCount = 0; }

protected:
	void ProcessInput();
	void PlayGame();
	void AutoFish();
	void MouseMove();
	void AddMouseMove(int iPosX, int iPosY);
	void SendKeyUP();
	void AddMessage(unsigned int iType, int iMessage, int iMessageEx, char* acDebugStr);
	HBITMAP ScreenShotByWndPos(HWND hWnd);
	void AddGameIconInfo(std::string kKey, std::string kFileName, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void RefrashIconInfoFast();
	void RefrashIconInfoNormal();
	void RefrashFishIcon();
	bool ComPareImageFast(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void ComPareImageNormal(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void ComPareImageNormalFuzzy(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void ReadConfig();
	void SaveConfig();

private:
	HWND m_iTargetHandle;
	std::map<std::string, GameIconInfo> m_akGameIconMidBottomInfo;
	std::map<std::string, GameIconInfo> m_kGameIconMidFish;
	std::map<std::string, GameIconInfo> m_kGameIconMidFishCursor;
	//std::map<std::string, GameIconInfo> m_akGameIconLeftTopInfo;
	//std::map<std::string, GameIconInfo> m_akGameIconMidInfo;
	DWORD m_aiPoolGameSnap[2000][2000];
	int m_iStartPlayGame;
	std::vector<WowMessange> m_akMessage;
	bool m_bIsCurePet;
	bool m_bIsAltStopAttack;
	bool m_bAutoFish;
	int m_iLastAltTime;
	AutoFishStep m_eAutoFishStep;
	int m_iAutoFishStepStartTime;
	int m_iMouseTargetPosX;
	int m_iMouseTargetPosY;
	int m_iMouseSrcPosX;
	int m_iMouseSrcPosY;
	int m_iStartMouseMoveTime;
	int m_iFishFuzzySumX;
	int m_iFishFuzzySumY;
	int m_iFishFuzzySumCount;
};