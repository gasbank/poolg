#pragma once
//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
	XACTINDEX iSE;
    XACTINDEX iSong[4];

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

    int nCurSongPlaying;
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
	void XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
	void UpdateAudio();
private:
	AUDIO_STATE audioState;
	HRESULT hr;
    HANDLE hFile;
    DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;
};
