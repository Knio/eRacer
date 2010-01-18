#include "SoundLayer.h"

SoundLayer* SoundLayer::m_pGlobalSoundLayer = NULL;

SoundLayer::SoundLayer()
{
}

SoundLayer::~SoundLayer()
{
	if (NULL != m_pGlobalSoundLayer) {
		delete m_pGlobalSoundLayer;
		m_pGlobalSoundLayer = NULL;
	}
}

int SoundLayer::Init()
{
	if (FSOUND_GetVersion() < FMOD_VERSION)
		return -1; //Outdated DLL

	if (!FSOUND_Init(32000, 64, 0))
    {
		return -1; //Failed to Initialize
        printf("%s\n", FMOD_ErrorString(FSOUND_GetError()));
    }

	return 0;
}

int SoundLayer::SetOrientation3D(Vector3 listenerPos, Vector3 listenerVel, Vector3 atVector, Vector3 upVector)
{
	//To be replaced when new Vector3 class is implemented
	float pos[3] = { listenerPos.x, listenerPos.y, listenerPos.z };
	float vel[3] = { listenerVel.x, listenerVel.y, listenerVel.z };
	FSOUND_3D_Listener_SetAttributes(pos, vel, 
		atVector.x, atVector.y, atVector.z, 
		upVector.x, upVector.y, upVector.z);

	return 0;
}

int SoundLayer::Update()
{
	//Update needs to be called each frame for 3D sound to change pitch.
	FSOUND_Update();
	return 0;
}

int SoundLayer::Shutdown()
{
	FMUSIC_FreeSong(m_fmodule);	 // this crashes the game. fix me
	
	//Clear the Cache
    for( map< const char*, FSOUND_SAMPLE* >::const_iterator it = m_SoundCache2D.begin(); it != m_SoundCache2D.end(); ++it)
		FSOUND_Sample_Free(it->second);
	for( map< const char*, FSOUND_SAMPLE* >::const_iterator it = m_SoundCache3D.begin(); it != m_SoundCache3D.end(); ++it)
		FSOUND_Sample_Free(it->second);

    FSOUND_Close();
	return 0;
}

int SoundLayer::LoopMusic(const char* name)
{
	//Potentially multiple songs can be playing at once
	//But this design only allows for one
	//The game will forseeably need to keep better track of channels being used 
	m_fmodule = FMUSIC_LoadSong(name);
    if (!m_fmodule) //failed to load the song
		return -1;

    int chan = FMUSIC_PlaySong(m_fmodule);  
	return chan;
}

int SoundLayer::StopMusic()
{
	FMUSIC_StopAllSongs();
	return 0;
}

int SoundLayer::PlaySound3D(const char* name, Vector3 pos, Vector3 vel)
{
	float p[3] = { pos.x, pos.y, pos.z };
	float v[3] = { vel.x, vel.y, vel.z };

	//The channel is given a 3D position
	if (m_SoundCache3D.count(name) == 0) { //Cache Miss
		FSOUND_SAMPLE  *soundClip = NULL;
		soundClip = FSOUND_Sample_Load(FSOUND_FREE, name, FSOUND_HW3D, 0, 0);
		if (!soundClip)
			return -1; //Failed to load
		m_SoundCache3D[name] = soundClip;
		FSOUND_Sample_SetMinMaxDistance(soundClip, 4.0f, 10000.0f);
	}

	int chan = FSOUND_PlaySound(FSOUND_FREE, m_SoundCache3D[name]);
	FSOUND_3D_SetAttributes( chan, p, v);

	return chan;
}

int SoundLayer::ChangePitch(int channel, int pitch)
{
	FSOUND_SetFrequency(channel, pitch); //Can alter the pitch of the sound by altering the playback frequency
	return 0;
}

int SoundLayer::PlaySound2D(const char* name)
{
	if (m_SoundCache2D.count(name) == 0) { //Cache Miss
		FSOUND_SAMPLE  *soundClip = NULL;
		soundClip = FSOUND_Sample_Load(FSOUND_FREE, name, FSOUND_HW2D, 0, 0);
		if (!soundClip)
			return -1; //Failed to load
		m_SoundCache2D[name] = soundClip;
	}
	int chan = FSOUND_PlaySound(FSOUND_FREE, m_SoundCache2D[name]);
	return 0;
}
