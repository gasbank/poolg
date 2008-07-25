#include "EmptyProjectPCH.h"
#include "Sound.h"

Sound::Sound(void)
{
}

Sound::~Sound(void)
{
    release();
}

HRESULT Sound::init()
{
	ZeroMemory( &audioState, sizeof( AUDIO_STATE ) );

	hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );  // COINIT_APARTMENTTHREADED will work too
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

    // Initialize & create the XACT runtime 
    XACT_RUNTIME_PARAMETERS xrParams = {0};
    xrParams.lookAheadTime = 250;
    hr = audioState.pEngine->Initialize( &xrParams );
    if( FAILED( hr ) )
        return hr;

    // Create an "in memory" XACT wave bank file using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you 
    // have enough virtual address space for a full map of the file
    hr = E_FAIL; // assume failure
    hFile = CreateFile( L"sounds.xwb", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
            if( hMapFile )
            {
                audioState.pbWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( audioState.pbWaveBank )
                {
                    hr = audioState.pEngine->CreateInMemoryWaveBank( audioState.pbWaveBank, dwFileSize, 0,
                                                                       0, &audioState.pWaveBank );
                }
                CloseHandle( hMapFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

    // Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
       hr = E_FAIL; // assume failure
    hFile = CreateFile( L"sounds.xsb", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
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
    audioState.iZap = audioState.pSoundBank->GetCueIndex( "zap" );

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
                audioState.pSoundBank->Play( g_audioState.iZap, 0, 0, NULL );
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

    if( audioState.pbSoundBank )
        delete[] audioState.pbSoundBank;
    audioState.pbSoundBank = NULL;

    // After pEngine->ShutDown() returns it is safe to release memory mapped files
    if( audioState.pbWaveBank )
        UnmapViewOfFile( audioState.pbWaveBank );
    audioState.pbWaveBank = NULL;

    CoUninitialize();
}