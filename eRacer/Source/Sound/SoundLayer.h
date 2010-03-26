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
#define NOMINMAX
#include <windows.h>
#include "fmod.h"
#include "fmod_errors.h"
#include "Core\types.h"
#include <vector>

#undef min
#undef max

using namespace std;

namespace Sound {

typedef map<string, FSOUND_SAMPLE*> FilenameCache;
typedef pair<string, int> SoundDescription;

class SoundLayer;

class SoundFx 
{
public:
	FSOUND_SAMPLE* soundsample;

	int channel;
	int pitch;
	unsigned char volume;
	Point3 position;
	Vector3 velocity;
	float maxDist;

	/**
	 * @brief minimal distance before the sound is attenuated
	 */
	float minDist;
	
	bool isPaused;
	bool isLooping;
	bool is3D;


   SoundFx() 
   {
	  soundsample = NULL;
      channel = -1;
	  pitch = 44100;
	  volume = 128;
	  isPaused = false;
	  is3D = false;
	  isLooping = true;
	  position = Point3(0,0,0);
	  velocity = Vector3(0,0,0);
	  maxDist = 1000000000.0f;
	  minDist = 50.0f;
   }
   ~SoundFx();
   

};


class SoundLayer
{
private:
	static SoundLayer *m_pGlobalSoundLayer;

	FilenameCache m_SoundCache2D;
	map<SoundDescription, FSOUND_SAMPLE*> m_SoundCacheSimple;


	//TODO: GLOBAL_VOLUME

protected:
	SoundLayer();	//Constructor, Singleton 
	SoundLayer(const SoundLayer&);
	SoundLayer& operator= (const SoundLayer); 

public:
	static const string SOUND_FOLDER;
	bool m_isShutdown;

	~SoundLayer();	//Destructor

	int Init();
	int Update();
	int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound

	void LoadSoundFx(const string& filename, SoundFx* samp);
	void UpdateSoundFx(SoundFx* samp);
	void PlaySoundFx(SoundFx* samp);

	int PlaySound2D(const string& name);

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
