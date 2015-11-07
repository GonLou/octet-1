////////////////////////////////////////////////////////////////////////////////
///
/// Goncalo Lourenco 2014
///
/// Engine Class
///
/// is where all the main game variables stand

namespace octet {
	namespace scene {
		/// Engine Class
		/// main game states
		class Engine : public resource {
		public:
			enum State {
				INIT,
				GAME_START,
				ROLL,
				GAME_END,
				HALL_OF_FAME
			};
			int score;

			/// Default Constructor
			Engine() { }

			/// Destructor where the game variables are destroyed
			~Engine(void) {	}

			Engine(int _score, State _state) {
				this->score = _score;
				this->state = _state;
			}

			int GetScore() {
				return this->score;
			}

			Engine::State GetState() {
				return this->state;
			}

			void SetScore(int _score) {
				this->score = _score;
			}

			void SetState(Engine::State _state) {
				this->state = _state;
			}

		private:

			Engine::State state;


		};
	}
}