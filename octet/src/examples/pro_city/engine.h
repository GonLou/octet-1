////////////////////////////////////////////////////////////////////////////////
///
/// Goncalo Lourenco 2015
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
			/// diferent game states
			enum State {
				INIT,
				GAME_START,
				ROLL,
				GAME_END,
				HALL_OF_FAME
			};
			/// score value
			int score;

			/// Default Constructor
			Engine() { }

			/// Destructor where the game variables are destroyed
			~Engine(void) {	}

			Engine(int _score, State _state) {
				this->score = _score;
				this->state = _state;
			}

			/// gets score value
			int GetScore() {
				return this->score;
			}

			/// get machine state
			Engine::State GetState() {
				return this->state;
			}

			/// set score value
			void SetScore(int _score) {
				this->score = _score;
			}

			/// set machine state
			void SetState(Engine::State _state) {
				this->state = _state;
			}

		private:

			Engine::State state;

		};
	}
}