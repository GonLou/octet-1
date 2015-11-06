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
				GAME_START,
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

			void SetState(Engine::State _state) {
				state = _state;
			}

			Engine::State GetState() {
				return this->state;
			}

		private:

			Engine::State state;


		};
	}
}