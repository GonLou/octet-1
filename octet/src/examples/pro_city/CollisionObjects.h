////////////////////////////////////////////////////////////////////////////////
///
/// Goncalo Lourenco 2015
///
/// Collision Class
///
/// is where all the collisions stand

namespace octet {
	namespace scene {
		/// Collision Class
		class CollisionObjects {
			int id;
			btRigidBody *body;
			void *objectclass;

		public:
			CollisionObjects(int _id, btRigidBody *_body, void *_objectclass) {
				id = _id;
				body = _body;
				objectclass = _objectclass;
			}


			int getid() {
				return id;
			}

			btRigidBody* getbody() {
				return body;
			}

			void* getobjectclass() {
				return objectclass;
			}

		};
	}
}