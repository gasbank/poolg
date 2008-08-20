#pragma once
//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
class AUDIO_STATE : public Singleton<AUDIO_STATE>
{
public:
	XACTINDEX iSE;
    XACTINDEX iStage[2][2];
	XACTINDEX iOpening;
	XACTINDEX iBattle[2];
	XACTINDEX iBoss;
	XACTINDEX iCeiling;

	XACTCATEGORY iGlobalCategory;
	XACTCATEGORY iMusicCategory;
	XACTCATEGORY iBGMCategory;

    IXACT3Engine* pEngine;
	IXACT3WaveBank* pInMemoryWaveBank;
    IXACT3WaveBank* pStreamingWaveBank;
    IXACT3SoundBank* pSoundBank;
    
	HANDLE hStreamingWaveBankFile; // Handles to audio files to be closed upon cleanup
    VOID* pbInMemoryWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed

	CRITICAL_SECTION cs;
    bool bHandleStreamingWaveBankPrepared;
    bool bHandleSongStopped;

	bool bGlobalPaused;
	float fGlobalVolume;
	bool bMusicPaused;
	float fMusicVolume;
	bool bMusicFade;
	bool bBGMPaused;
	float fBGMVolume;
	bool bBGMFade;

	bool bBGMStopped;

    int nCurSongPlaying;
	int nCurWorld;

	void enterBattle();
	void leaveBattle();
};

inline AUDIO_STATE& GetAudioState() { return AUDIO_STATE::getSingleton(); }

class Sound
{
public:
	Sound(void);
	~Sound(void);

	HRESULT init();
	bool DoesCommandLineContainAuditionSwitch();
	LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void release();
	void UpdateAudio();
	void Opening();

private:
	HRESULT hr;
    HANDLE hFile;
    DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;
};
