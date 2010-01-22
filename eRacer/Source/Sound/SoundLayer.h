//-----------------------------------------------------------------------------
// File: SoundLayer.h
// Jan 9, 2010 
//
// Proof of Sound concept. Provides minimal sound and music playback at the
// moment. More features to be added as needed.
//-----------------------------------------------------------------------------

#ifndef SOUND_H
#define SOUND_H

#pragma comment(lib, "fmodvc.lib")
#pragma warning( disable : 4995 ) // disable deprecated warning 
#include <string>
#include "wincompat.h"
#pragma warning( default : 4995 )
#include <map>
#include <windows.h>
#include "fmod.h"
#include "fmod_errors.h"
#include "..\Core\types.h"

#undef min
#undef max

using namespace std;

namespace Sound {


class SoundLayer
{
private:
	static SoundLayer *m_pGlobalSoundLayer;
	FMUSIC_MODULE *m_fmodule;

	//map<const char*, FMUSIC_MODULE*> m_MusicCache;
	map<const char*, FSOUND_SAMPLE*> m_SoundCache2D;
	map<const char*, FSOUND_SAMPLE*> m_SoundCache3D;

protected:
	SoundLayer();	//Constructor, Singleton 
	SoundLayer(const SoundLayer&);
	SoundLayer& operator= (const SoundLayer); 

public:
	~SoundLayer();	//Destructor

	/**
	 * @brief Call Init to initiliaze the sound layer.
	 * @return Poorly defined error code
	 */
	int Init();

	/**
	 * @brief Update must be called each frame when using 3D sound to dynamically change the sounds
	 * @return Poorly defined error code
	 */
	int Update();

	/**
	 * @brief For 3D Sound. Set the listener parameter and orientation
	 * 
	 * @param listerPos Listener Position. Usually the camera position
	 * @param listenerVel Listener Velocity for doppler effects. In meters per second
	 * @param atVector The direction the listener is facing
	 * @param upVector The up direction for the listener
	 * @return Poorly defined error code
	 */
	int SetOrientation3D(Vector3 listenerPos, Vector3 listenerVel, Vector3 atVector, Vector3 upVector); //For 3D sound

	/**
	 * @brief Play sound in 3D Space
	 * 
	 * @param name Resource file name
	 * @param pos Position to play the sound
	 * @param vel Velocity vector for doppler effects. In meters per second
	 * @return The channel the sound is playing on
	 */
	int PlaySound3D(const char* name, Vector3 pos, Vector3 vel);

	/**
	 * @brief Play sound in 3D Space
	 * 
	 * @param name Resource file name
	 * @return The channel the sound is playing on
	 */
	int PlaySound2D(const char* name);

	/**
	 * @brief Changes the pitch for a sound being played on a given channel
	 * 
	 * @param channel The Channel the sound is playing on. Get the channel from the PlaySound return code.
	 * @param pitch Playback rate for the sound. Normal is around 22000. Higher numbers mean higher pitch
	 * @return Poorly defined error code
	 */
	int ChangePitch(int channel, int pitch);

	/**
	 * @brief Play looped music. Currently a limit of 1 song
	 * 
	 * @param name Resource file name
	 * @return The channel the sound is playing on
	 */
	int LoopMusic(const char* name);

	/**
	 * @brief Stop looped music
	 * 
	 * @param name Resource file name
	 * @return The channel the sound is playing on
	 */
	int StopMusic();

	/**
	 * @brief Call Shutdown to close sound objects and free memory
	 *
	 * @return Poorly defined error code
	 */
	int Shutdown();

	static SoundLayer *GetInstance()
	{
		if (m_pGlobalSoundLayer == NULL)
			m_pGlobalSoundLayer = new SoundLayer;

		return m_pGlobalSoundLayer;
	}
};

inline SoundLayer *Sound()
{
	return SoundLayer::GetInstance();
}

};
#endif
