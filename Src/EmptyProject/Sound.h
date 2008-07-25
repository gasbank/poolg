#pragma once
//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
    IXACT3Engine* pEngine;
    IXACT3WaveBank* pWaveBank;
    IXACT3SoundBank* pSoundBank;
    XACTINDEX iZap;

    VOID* pbWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed
};

class Sound
{
public:
	Sound(void);
	~Sound(void);

	HRESULT init();
	bool DoesCommandLineContainAuditionSwitch();
	LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void release();
private:
	AUDIO_STATE audioState;
	HRESULT hr;
    HANDLE hFile;
    DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;
};
