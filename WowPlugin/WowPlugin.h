#include <windows.h>
#include <timeapi.h>
#include <wingdi.h>
#include <Gdiplus.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace Gdiplus;

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

class WowPlugin
{
public:
	WowPlugin();
	~WowPlugin();

	bool InitGame();
	void GameLoop();
	void ProcessInput();
	void PlayGame();
	HBITMAP ScreenShotByWndPos(HWND hWnd);
	void AddGameIconInfo(std::string kKey, std::string kFileName, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void RefrashIconInfoFast();
	void RefrashIconInfoNormal();
	bool ComPareImageFast(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void ComPareImageNormal(int iBeginX, int iEndX, int iBeginY, int iEndY, HDC hdcGameSnap, std::map<std::string, GameIconInfo>& akGameIconInfo);
	void ReadConfig();
	void SaveConfig();

private:
	HWND m_iTargetHandle;
	std::map<std::string, GameIconInfo> m_akGameIconMidBottomInfo;
	std::map<std::string, GameIconInfo> m_akGameIconLeftTopInfo;
	std::map<std::string, GameIconInfo> m_akGameIconMidInfo;
	DWORD m_aiPoolGameSnap[2000][2000];
	int m_iStartPlayGame;
	std::vector<int> m_aiMessage;
};