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

typedef map<string, FSOUND_SAMPLE*> FontCache;

struct SoundFx 
{
public:
	FSOUND_SAMPLE* soundsample;
	int channel;
	int pitch;
	bool isPaused;
	bool isLooping;

	//3D positional info
	bool is3D;
	Point3 position;
	Vector3 velocity;
	float maxDist;
	float minDist;

   SoundFx() 
   {
      channel = -1;
	  pitch = 44100;
	  isPaused = false;
	  is3D = false;
	  isLooping = true;
	  position = Point3(0,0,0);
	  velocity = Vector3(0,0,0);
	  maxDist = 1000000000.0f;
	  minDist = 50.0f;
   }
   ~SoundFx()
   {
		if (NULL!= soundsample)
			FSOUND_Sample_Free(soundsample); 

   }

};


class SoundLayer
{
private:
	static SoundLayer *m_pGlobalSoundLayer;

	FSOUND_SAMPLE* m_musicSample;

	//map<const char*, FMUSIC_MODULE*> m_MusicCache;
	FontCache m_SoundCache2D;
	FontCache m_SoundCache3D;

protected:
	SoundLayer();	//Constructor, Singleton 
	SoundLayer(const SoundLayer&);
	SoundLayer& operator= (const SoundLayer); 

public:
	~SoundLayer();	//Destructor

	int Init();
	int Update();
	int SetOrientation3D(const Point3& listenerPos, const Vector3& listenerVel, const Vector3& atVector, const Vector3& upVector); //For 3D sound

	void LoadSoundFx(const string& filename, SoundFx* samp);
	void UpdateSoundFx(SoundFx* samp);

	int LoopMusic(const string& name);
	int StopMusic();

	int PlaySound3D(const string& name, const Point3& pos, const Vector3& vel);
	int PlaySound2D(const string& name);
	int ChangePitch(int channel, int pitch);

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
