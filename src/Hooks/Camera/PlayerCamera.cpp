#include "Hooks/Camera/PlayerCamera.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {


	struct PlayerCameraUpdate {

		static inline REL::Relocation<decltype(&RE::PlayerCamera::Update)> func;
		static constexpr std::size_t funcIndex = 0x02;

		static void thunk(RE::PlayerCamera* a_this) {
			func(a_this);
		}
	};

	void Hook_PlayerCamera::Install() {
		//Currently Unused
		log::info("Installing PlayerCamera::Update VTABLE Hooks...");
		stl::write_vfunc<RE::PlayerCamera, PlayerCameraUpdate>();
	}

}
