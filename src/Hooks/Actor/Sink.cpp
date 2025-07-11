#include "Hooks/Actor/Sink.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {

	struct GetScaledBoundSize {

		static float thunk(TESObjectREFR* a_this) {
			float result = func(a_this);
			if (a_this) {
				Actor* actor = skyrim_cast<Actor*>(a_this);
				if (actor) {
					if (actor->Get3D1(false)) {
						if (actor->Is3DLoaded()) {
							const float scale = get_visual_scale(actor);
							if (scale > 1e-4) {
								result *= scale;
							}
						}
					}
				}
			}
			return result;
		}
		static inline decltype(thunk)* func;
	};

	struct GetSubmergeLevel {

		static float thunk(TESObjectREFR* a_this, float z_pos, TESObjectCELL* a_cell) {

			float result = func(a_this, z_pos, a_cell);
			Actor* actor = skyrim_cast<Actor*>(a_this);

			if (actor) {
				float scale = get_visual_scale(actor);
				if (scale > 10.0f) {
					result = 0.0f;
				}
			}
			return result;
		}
		static inline decltype(thunk)* func;
	};

	void Hook_Sink::Install() {

		logger::info("Installing Sink Detour Hooks...");
		stl::write_detour<GetScaledBoundSize>(REL::RelocationID(36448, 37444, NULL));

		// Enable this hook to prevent swimming after a certain scale
		//stl::write_detour<GetSubmergeLevel>(REL::RelocationID(36452, 37448, NULL));

	}
}
