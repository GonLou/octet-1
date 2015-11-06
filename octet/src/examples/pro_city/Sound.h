////////////////////////////////////////////////////////////////////////////////
//
//  Goncalo Lourenco 2015
//
// Sound class management

namespace octet {
	namespace scene {
	/// contains all sounds database
	class Sound : public resource {
		private:
			// sounds
			ALuint Start;
			ALuint Collision;
			ALuint Effort;
			ALuint End;
			unsigned current_source;    	// current sound source
			unsigned int sount_stop;
			ALuint num_sound_sources = 32;
			ALuint sources[32];    		// 32 sound sources

			// returns new sound source
			ALuint get_sound_source() {
			return sources[current_source++ % num_sound_sources];
			}

			public:
			/// sound Constructor
			Sound() {
			}

			/// sound Destructor
			~Sound(){
			}

			/// initialise sounds
			void init_sound() {
				current_source = 0;
				Start = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/pro_city/start.wav");
				Collision = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/pro_city/collision.wav");
				Effort = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/pro_city/effort.wav");
				End = resource_dict::get_sound_handle(AL_FORMAT_MONO16, "assets/pro_city/end.wav");

				alGenSources(num_sound_sources, sources);
				sount_stop = 0;
			}

			/// play start sound
			void playSoundStart() {
				ALuint source = get_sound_source();
				alSourcei(source, AL_BUFFER, Start);
				alSourcePlay(source);
			}

			/// play collision sound
			void playSoundCollision() {
				ALuint source = get_sound_source();
				alSourcei(source, AL_BUFFER, Collision);
				alSourcePlay(source);
			}

			/// play effort sound 
			void playSoundEffort() {
				ALuint source = get_sound_source();
				alSourcei(source, AL_BUFFER, Effort);
				alSourcePlay(source);
			}

			/// play end sound
			void playSoundEnd() {
				ALuint source = get_sound_source();
				alSourcei(source, AL_BUFFER, End);
				alSourcePlay(source);
			}

		};
	}
}