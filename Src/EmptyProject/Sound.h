﻿#pragma once
//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------


class AUDIO_STATE : public Singleton<AUDIO_STATE>
{
public:
	AUDIO_STATE()
	{
		pbSoundBank = 0;
	}
	~AUDIO_STATE()
	{
		SAFE_DELETE_ARRAY( pbSoundBank );
	}
	XACTINDEX iSE;
    XACTINDEX iStage[2][2];
	XACTINDEX iOpening;
	XACTINDEX iBattle[2];
	XACTINDEX iBoss;
	XACTINDEX iCeiling;
	XACTINDEX iAttack;
	XACTINDEX iBomb;
	XACTINDEX iHit;
	XACTINDEX iPuzzleClear;
	XACTINDEX iWalk;

	XACTCATEGORY iGlobalCategory;
	XACTCATEGORY iMusicCategory;
	XACTCATEGORY iBGMCategory;

#ifdef USE_XACT3
	IXACT3Cue* pZeroLatencyWalkCue;
    IXACT3Engine* pEngine;
	IXACT3WaveBank* pInMemoryWaveBank;
    IXACT3WaveBank* pStreamingWaveBank;
    IXACT3SoundBank* pSoundBank;
#else
	IXACTCue* pZeroLatencyWalkCue;
    IXACTEngine* pEngine;
	IXACTWaveBank* pInMemoryWaveBank;
    IXACTWaveBank* pStreamingWaveBank;
    IXACTSoundBank* pSoundBank;
#endif
    
	HANDLE hStreamingWaveBankFile; // Handles to audio files to be closed upon cleanup
    VOID* pbInMemoryWaveBank; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed

	CRITICAL_SECTION cs;
    bool bHandleStreamingWaveBankPrepared;
    bool bHandleSongStopped;
	bool bBattleSongStopped;
	bool bHandleZeroLatencyWalkCueStop;
	bool bInBattle;

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

/**
@brief 배경 음악 및 효과음을 관리하는 클래스

XACT를 이용해 배경 음악 및 효과음을 재생하게 해 주는 클래스입니다.
각 상황에 따라 음악을 바꿔주거나 캐릭터 이동 시에 발자국 소리를 나게 합니다.
*/
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
