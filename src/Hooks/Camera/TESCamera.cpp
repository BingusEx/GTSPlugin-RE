#include "Hooks/Camera/TESCamera.hpp"
#include "Hooks/Util/HookUtil.hpp"

using namespace GTS;

namespace Hooks {

	struct TESCameraUpdate {

		static inline REL::Relocation<decltype(&RE::TESCamera::Update)> func;
		static constexpr std::size_t funcIndex = 0x02;

		static void thunk(RE::TESCamera* a_this) {
			func(a_this);
		}
	};


	void Hook_TESCamera::Install() {
		//Currently Unused
		log::info("Installing TESCamera::Update VTABLE Hooks...");
		stl::write_vfunc<RE::PlayerCamera, TESCameraUpdate>();
	}

}
