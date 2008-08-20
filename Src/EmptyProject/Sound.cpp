#include "EmptyProjectPCH.h"
#include "Sound.h"
#include "WorldManager.h"
#include "World.h"

IMPLEMENT_SINGLETON( AUDIO_STATE );

void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
AUDIO_STATE audioState;


Sound::Sound(void)
{
}

Sound::~Sound(void)
{
}

HRESULT Sound::init()
{
	ZeroMemory( &audioState, sizeof( AUDIO_STATE ) );
	InitializeCriticalSection( &audioState.cs );

	audioState.nCurSongPlaying = -1;
	audioState.nCurWorld = -1;
	
	audioState.bGlobalPaused = false;
	audioState.bMusicPaused = false;
	audioState.bBGMPaused = false;

	audioState.bBGMStopped = false;
	audioState.bBGMFade = false;

	audioState.fGlobalVolume = 1.0f;
	audioState.fMusicVolume = 1.0f;
	audioState.fBGMVolume = 1.0f;

	hr = CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );  // COINIT_APARTMENTTHREADED will work too
    if( SUCCEEDED( hr ) )
    {
        // Switch to auditioning mode based on command line.  Change if desired
        bool bAuditionMode = DoesCommandLineContainAuditionSwitch();
        bool bDebugMode = false;

        DWORD dwCreationFlags = 0;
        if( bAuditionMode ) dwCreationFlags |= XACT_FLAG_API_AUDITION_MODE;
        if( bDebugMode ) dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;

        hr = XACT3CreateEngine( dwCreationFlags, &audioState.pEngine );
    }
    if( FAILED( hr ) || audioState.pEngine == NULL )
        return E_FAIL;

    // Load the global settings file and pass it into XACTInitialize
    VOID* pGlobalSettingsData = NULL;
    DWORD dwGlobalSettingsFileSize = 0;
    bool bSuccess = false;
    hFile = CreateFile( L"Sounds.xgs", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile )
    {
		dwGlobalSettingsFileSize = GetFileSize( hFile, NULL );
		if( dwGlobalSettingsFileSize != INVALID_FILE_SIZE )
		{
			pGlobalSettingsData = CoTaskMemAlloc( dwGlobalSettingsFileSize );
			if( pGlobalSettingsData )
			{
				if( 0 != ReadFile( hFile, pGlobalSettingsData, dwGlobalSettingsFileSize, &dwBytesRead, NULL ) )
				{
					bSuccess = true;
				}
			}
		}
		CloseHandle( hFile );
	}
   
    if( !bSuccess )
    {
        if( pGlobalSettingsData )
            CoTaskMemFree( pGlobalSettingsData );
        pGlobalSettingsData = NULL;
        dwGlobalSettingsFileSize = 0;
    }

    // Initialize & create the XACT runtime 
    XACT_RUNTIME_PARAMETERS xrParams = {0};
    xrParams.pGlobalSettingsBuffer = pGlobalSettingsData;
    xrParams.globalSettingsBufferSize = dwGlobalSettingsFileSize;
    xrParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA; // this will tell XACT to delete[] the buffer when its unneeded
    xrParams.lookAheadTime = 250;
	xrParams.fnNotificationCallback = XACTNotificationCallback;
    hr = audioState.pEngine->Initialize( &xrParams );
    if( FAILED( hr ) )
        return hr;

	//-----------------------------------------------------------------------------------------
    // Register for XACT notifications
    //-----------------------------------------------------------------------------------------

    // The "wave bank prepared" notification will let the app know when it is save to use
    // play cues that reference streaming wave data.
    XACT_NOTIFICATION_DESCRIPTION desc = {0};
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_WAVEBANKPREPARED;
    audioState.pEngine->RegisterNotification( &desc );

    // The "sound bank destroyed" notification will let the app know when it is save to use
    // play cues that reference streaming wave data.
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED;
    audioState.pEngine->RegisterNotification( &desc );

    // The "cue stop" notification will let the app know when it a song stops so a new one 
    // can be played
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_CUESTOP;
    desc.cueIndex = XACTINDEX_INVALID;
    audioState.pEngine->RegisterNotification( &desc );

    // The "cue prepared" notification will let the app know when it a a cue that uses 
    // streaming data has been prepared so it is ready to be used for zero latency streaming
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_CUEPREPARED;
    desc.cueIndex = XACTINDEX_INVALID;
    audioState.pEngine->RegisterNotification( &desc );

    // Create an "in memory" XACT wave bank file using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you 
    // have enough virtual address space for a full map of the file
    hr = E_FAIL; // assume failure
    hFile = CreateFile( L"InMemory.xwb", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
            if( hMapFile )
            {
                audioState.pbInMemoryWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( audioState.pbInMemoryWaveBank )
                {
                    hr = audioState.pEngine->CreateInMemoryWaveBank( audioState.pbInMemoryWaveBank, dwFileSize, 0,
                                                                       0, &audioState.pInMemoryWaveBank );
                }
                CloseHandle( hMapFile ); // pbInMemoryWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile ); // pbInMemoryWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

	//-----------------------------------------------------------------------------------------
    // Create a streaming XACT wave bank file.  
    // Take note of the following:
    // 1) This wave bank in the XACT project file must marked as a streaming wave bank 
    //    This is set inside the XACT authoring tool)
    // 2) Use FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING flags when opening the file 
    // 3) To use cues that reference this streaming wave bank, you must wait for the 
    //    wave bank to prepared first or the playing the cue will fail
    //-----------------------------------------------------------------------------------------
    hr = E_FAIL; // assume failure
    audioState.hStreamingWaveBankFile = CreateFile( L"Stream.xwb",
                                                      GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                                      FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL );
    if( audioState.hStreamingWaveBankFile != INVALID_HANDLE_VALUE )
    {
        XACT_WAVEBANK_STREAMING_PARAMETERS wsParams;
        ZeroMemory( &wsParams, sizeof( XACT_WAVEBANK_STREAMING_PARAMETERS ) );
        wsParams.file = audioState.hStreamingWaveBankFile;
        wsParams.offset = 0;

        // 64 means to allocate a 64 * 2k buffer for streaming.  
        // This is a good size for DVD streaming and takes good advantage of the read ahead cache
        wsParams.packetSize = 64;

        hr = audioState.pEngine->CreateStreamingWaveBank( &wsParams, &audioState.pStreamingWaveBank );
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

    // Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
    hr = E_FAIL; // assume failure
    hFile = CreateFile( L"Sounds.xsb", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            audioState.pbSoundBank = new BYTE[dwFileSize];
            if( audioState.pbSoundBank )
            {
                if( 0 != ReadFile( hFile, audioState.pbSoundBank, dwFileSize, &dwBytesRead, NULL ) )
                {
                    hr = audioState.pEngine->CreateSoundBank( audioState.pbSoundBank, dwFileSize, 0,
                                                                0, &audioState.pSoundBank );
                }
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

    // Get the sound cue index from the sound bank
    //
    // Note that if the cue does not exist in the sound bank, the index will be XACTINDEX_INVALID
    // however this is ok especially during development.  The Play or Prepare call will just fail.
    audioState.iSE = audioState.pSoundBank->GetCueIndex( "se" );
	for( int i = 0; i < 2; i++ )
    {
		for( int j = 0; j < 2; j++)
		{
	        CHAR sz[256];
	        StringCchPrintfA( sz, 256, "stage%d-%d", i + 1, j + 1 );
		    audioState.iStage[i][j] = audioState.pSoundBank->GetCueIndex( sz );
		}
    }

	for( int i = 0; i < 2; i++ )
	{
		CHAR sz[256];
		StringCchPrintfA( sz, 256, "battle%d", i + 1 );
		audioState.iBattle[i] = audioState.pSoundBank->GetCueIndex( sz );
	}
	audioState.iOpening = audioState.pSoundBank->GetCueIndex( "opening" );
	audioState.iBoss = audioState.pSoundBank->GetCueIndex( "boss" );
	audioState.iCeiling = audioState.pSoundBank->GetCueIndex( "ceiling" );

	audioState.iGlobalCategory = audioState.pEngine->GetCategory( "Global" );
	audioState.iMusicCategory = audioState.pEngine->GetCategory( "Music" );
	audioState.iBGMCategory = audioState.pEngine->GetCategory( "BGM" );

    return S_OK;
}

bool Sound::DoesCommandLineContainAuditionSwitch()
{
    const WCHAR* strAuditioning = L"-audition"; size_t nArgLen; int nNumArgs;
    WCHAR** pstrArgList = CommandLineToArgvW( GetCommandLine(), &nNumArgs );
    for( int iArg = 1; iArg < nNumArgs; iArg++ )
    {
        StringCchLength( pstrArgList[iArg], 256, &nArgLen );
        if( _wcsnicmp( pstrArgList[iArg], strAuditioning, nArgLen ) == 0 && nArgLen == 9 )
            return true;
    }
    return false;
}

LRESULT Sound::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( lParam );

	switch(uMsg)
	{
		case WM_KEYDOWN:
			if( wParam == VK_SPACE )
                audioState.pSoundBank->Play( audioState.iSE, 0, 0, NULL );
			if( wParam == 'M' )
			{
				if(	audioState.bBGMStopped )
				{
					audioState.bBGMStopped = false;
					audioState.pSoundBank->Play( audioState.iStage[audioState.nCurWorld][audioState.nCurSongPlaying], 0, 0, NULL );
				}
				else
				{
					audioState.bBGMStopped = true;
					audioState.pSoundBank->Stop( audioState.iStage[audioState.nCurWorld][audioState.nCurSongPlaying], 0 );
				}
			}
			if( wParam == 'N' )
			{
				audioState.bBGMPaused = !audioState.bBGMPaused;
				audioState.pEngine->Pause( audioState.iBGMCategory, audioState.bBGMPaused );
			}
			if( wParam == 'B' )
			{
				audioState.bBGMFade = !audioState.bBGMFade;
			}
			break;
	}

	return FALSE;
}

void Sound::release()
{
	// Shutdown XACT
    //
    // Note that pEngine->ShutDown is synchronous and will take some time to complete 
    // if there are still playing cues.  Also pEngine->ShutDown() is generally only 
    // called when a game exits and is not the preferred method of changing audio 
    // resources. To know when it is safe to free wave/sound bank data without 
    // shutting down the XACT engine, use the XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED 
    // or XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED notifications 
    if( audioState.pEngine )
    {
        audioState.pEngine->ShutDown();
        audioState.pEngine->Release();
    }

    // After pEngine->ShutDown() returns, it is safe to release audio file memory
    if( audioState.hStreamingWaveBankFile != INVALID_HANDLE_VALUE && audioState.hStreamingWaveBankFile != NULL )
        CloseHandle( audioState.hStreamingWaveBankFile );

    if( audioState.pbSoundBank )
        delete[] audioState.pbSoundBank;
    audioState.pbSoundBank = NULL;

    if( audioState.pbInMemoryWaveBank )
        UnmapViewOfFile( audioState.pbInMemoryWaveBank );
    audioState.pbInMemoryWaveBank = NULL;

    DeleteCriticalSection( &audioState.cs );

    CoUninitialize();
}

//-----------------------------------------------------------------------------------------
// This is the callback for handling XACT notifications.  This callback can be executed on a 
// different thread than the app thread so shared data must be thread safe.  The game 
// also needs to minimize the amount of time spent in this callbacks to avoid glitching, 
// and a limited subset of XACT API can be called from inside the callback so 
// it is sometimes necessary to handle the notification outside of this callback.
//-----------------------------------------------------------------------------------------
void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification )
{
    // Use the critical section properly to make shared data thread safe while avoiding deadlocks.  
    //
    // To do this follow this advice:
    // 1) Use a specific CS only to protect the specific shared data structures between the callback and the app thread.
    // 2) Don’t make any API calls while holding the CS. Use it to access the shared data, make a local copy of the data, release the CS and then make the API call.
    // 3) Spend minimal amount of time in the CS (to prevent the callback thread from waiting too long causing a glitch).   
    // 
    // Instead of using a CS, you can also use a non-blocking queues to keep track of notifications meaning 
    // callback will push never pop only push and the app thread will only pop never push

    // In this simple tutorial, we will respond to a cue stop notification for the song 
    // cues by simply playing another song but its ultimately it's up the application 
    // and sound designer to decide what to do when a notification is received. 
	if( GetWorldManager().getCurWorld()->getWorldName() == "EpRoomWorld" )
		audioState.nCurWorld = 0;
	else if( GetWorldManager().getCurWorld()->getWorldName() == "EpA213World" )
		audioState.nCurWorld = 1;
	else if( GetWorldManager().getCurWorld()->getWorldName() == "EpCeilingWorld" )
		audioState.nCurWorld = 2;
	else
		audioState.nCurWorld = -1;

	if( audioState.bBGMStopped )
		return;

    if( pNotification->type == XACTNOTIFICATIONTYPE_CUESTOP &&
        ( pNotification->cue.cueIndex == audioState.iStage[0][0] ||
          pNotification->cue.cueIndex == audioState.iStage[0][1] ||
		  pNotification->cue.cueIndex == audioState.iStage[1][0] ||
		  pNotification->cue.cueIndex == audioState.iStage[1][1] ) )
    {
        // The previous background song ended, so pick and new song to play it
        EnterCriticalSection( &audioState.cs );
        audioState.nCurSongPlaying++; audioState.nCurSongPlaying %= 2;
        audioState.bHandleSongStopped = true;
        LeaveCriticalSection( &audioState.cs );
    }

    if( pNotification->type == XACTNOTIFICATIONTYPE_WAVEBANKPREPARED &&
        pNotification->waveBank.pWaveBank == audioState.pStreamingWaveBank )
    {
        // Respond to this notification outside of this callback so Prepare() can be called
        EnterCriticalSection( &audioState.cs );
        audioState.bHandleStreamingWaveBankPrepared = true;
        LeaveCriticalSection( &audioState.cs );
    }

    if( pNotification->type == XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED &&
        pNotification->soundBank.pSoundBank == audioState.pSoundBank )
    {
        // Cleanup sound bank memory
        if( audioState.pbSoundBank )
        {
            delete[] audioState.pbSoundBank;
            audioState.pbSoundBank = NULL;
        }
    }
}

//-----------------------------------------------------------------------------------------
// Handle these notifications outside of the callback and call pEngine->DoWork()
//-----------------------------------------------------------------------------------------
void Sound::UpdateAudio()
{
    // Handle these notifications outside of the callback because
    // only a subset of XACT APIs can be called inside the callback.

    // Use the critical section properly to make shared data thread safe while avoiding deadlocks.  
    //
    // To do this follow this advice:
    // 1) Use a specific CS only to protect the specific shared data structures between the callback and the app thread.
    // 2) Don’t make any API calls while holding the CS. Use it to access the shared data, make a local copy of the data, release the CS and then make the API call.
    // 3) Spend minimal amount of time in the CS (to prevent the callback thread from waiting too long causing a glitch).   
    // 
    // Instead of using a CS, you can also use a non-blocking queues to keep track of notifications meaning 
    // callback will push never pop only push and the app thread will only pop never push

    EnterCriticalSection( &audioState.cs );
    bool bHandleStreamingWaveBankPrepared = audioState.bHandleStreamingWaveBankPrepared;
    LeaveCriticalSection( &audioState.cs );

    if( bHandleStreamingWaveBankPrepared )
    {
        EnterCriticalSection( &audioState.cs );
        audioState.bHandleStreamingWaveBankPrepared = false;
        LeaveCriticalSection( &audioState.cs );

        // Starting playing background music after the streaming wave bank 
        // has been prepared but no sooner.  The background music does not need to be 
        // zero-latency so the cues do not need to be prepared first 
        audioState.nCurSongPlaying = 0;
        if ( audioState.nCurWorld != -1 )
        {
        	if ( audioState.nCurWorld == 2 )
				audioState.pSoundBank->Play( audioState.iCeiling, 0, 0, NULL );
			else
				audioState.pSoundBank->Play( audioState.iStage[audioState.nCurWorld][audioState.nCurSongPlaying], 0, 0, NULL );
				
        }
    }

    if( audioState.bHandleSongStopped )
    {
        EnterCriticalSection( &audioState.cs );
        audioState.bHandleSongStopped = false;
        LeaveCriticalSection( &audioState.cs );

		audioState.nCurSongPlaying = 0;
		if ( audioState.nCurWorld != -1 )
		{
			if ( audioState.nCurWorld == 2 )
				audioState.pSoundBank->Play( audioState.iCeiling, 0, 0, NULL );
			else
				audioState.pSoundBank->Play( audioState.iStage[audioState.nCurWorld][audioState.nCurSongPlaying], 0, 0, NULL );
		}
    }


    // It is important to allow XACT to do periodic work by calling pEngine->DoWork().  
    // However this must function be call often enough.  If you call it too infrequently, 
    // streaming will suffer and resources will not be managed promptly.  On the other hand 
    // if you call it too frequently, it will negatively affect performance. Calling it once 
    // per frame is usually a good balance.
    if( audioState.pEngine )
        audioState.pEngine->DoWork();

	if( audioState.bBGMFade )
	{
		if( audioState.fBGMVolume > 0.0f )
			audioState.fBGMVolume -= 0.025f;
		audioState.pEngine->SetVolume( audioState.iBGMCategory, audioState.fBGMVolume );
	}
	if( !audioState.bBGMFade )
	{
		if( audioState.fBGMVolume < 1.0f )
			audioState.fBGMVolume += 0.025f;
		audioState.pEngine->SetVolume( audioState.iBGMCategory, audioState.fBGMVolume );
	}

	if( audioState.bMusicFade )
	{
		if( audioState.fMusicVolume > 0.0f )
			audioState.fMusicVolume -= 0.025f;
		audioState.pEngine->SetVolume( audioState.iMusicCategory, audioState.fMusicVolume );
	}
	if( !audioState.bMusicFade )
	{
		if( audioState.fMusicVolume < 1.0f )
			audioState.fMusicVolume += 0.025f;
		audioState.pEngine->SetVolume( audioState.iMusicCategory, audioState.fMusicVolume );
	}
}

void Sound::Opening()
{
    EnterCriticalSection( &audioState.cs );
    bool bHandleStreamingWaveBankPrepared = audioState.bHandleStreamingWaveBankPrepared;
    LeaveCriticalSection( &audioState.cs );

    if( bHandleStreamingWaveBankPrepared )
    {
        EnterCriticalSection( &audioState.cs );
        audioState.bHandleStreamingWaveBankPrepared = false;
        LeaveCriticalSection( &audioState.cs );

        // Starting playing background music after the streaming wave bank 
        // has been prepared but no sooner.  The background music does not need to be 
        // zero-latency so the cues do not need to be prepared first 
		audioState.pSoundBank->Play( audioState.iOpening, 0, 0, NULL );
    }

    // It is important to allow XACT to do periodic work by calling pEngine->DoWork().  
    // However this must function be call often enough.  If you call it too infrequently, 
    // streaming will suffer and resources will not be managed promptly.  On the other hand 
    // if you call it too frequently, it will negatively affect performance. Calling it once 
    // per frame is usually a good balance.
    if( audioState.pEngine )
        audioState.pEngine->DoWork();
}

void AUDIO_STATE::enterBattle()
{
	audioState.bBGMFade = true;
	audioState.bMusicFade = false;
	audioState.pEngine->Stop( GetAudioState().iMusicCategory, 0 );
	audioState.pSoundBank->Play( GetAudioState().iBattle[GetAudioState().nCurWorld], 0, 0, NULL );
}

void AUDIO_STATE::leaveBattle()
{
	audioState.bBGMFade = false;
	audioState.bMusicFade = true;
}