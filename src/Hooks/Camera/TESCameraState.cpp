#include "Hooks/Camera/TESCameraState.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {

	struct TESCameraStateUpdate {
		template<int ID>
		static inline REL::Relocation<decltype(&RE::TESCameraState::Update)> func;
		static constexpr std::size_t funcIndex = 0x03;

		template<int ID>
		static void thunk(TESCameraState* a_this, BSTSmartPointer<TESCameraState>& a_nextState) {
			func<ID>(a_this, a_nextState);
			GTS::EventDispatcher::DoCameraUpdate();
		}
	};

	void Hook_TESCameraState::Install() {

		logger::info("Installing TESCameraState::Update VTABLE Hooks...");

		stl::write_vfunc_unique<RE::TESCameraState,              TESCameraStateUpdate, 0>();
		stl::write_vfunc_unique<RE::ThirdPersonState,            TESCameraStateUpdate, 1>();
		stl::write_vfunc_unique<RE::DragonCameraState,           TESCameraStateUpdate, 2>();
		stl::write_vfunc_unique<RE::HorseCameraState,            TESCameraStateUpdate, 3>();
		stl::write_vfunc_unique<RE::FirstPersonState,            TESCameraStateUpdate, 4>();
		stl::write_vfunc_unique<RE::FreeCameraState,             TESCameraStateUpdate, 5>();
		stl::write_vfunc_unique<RE::PlayerCameraTransitionState, TESCameraStateUpdate, 6>();
		stl::write_vfunc_unique<RE::BleedoutCameraState,         TESCameraStateUpdate, 7>();
		stl::write_vfunc_unique<RE::VATSCameraState,             TESCameraStateUpdate, 8>();
		stl::write_vfunc_unique<RE::FurnitureCameraState,        TESCameraStateUpdate, 9>();
		stl::write_vfunc_unique<RE::AutoVanityState,             TESCameraStateUpdate, 10>();

		//IronSightsState has no RE'd Class member so just pass its vtable REL ID
		stl::write_vfunc_unique<TESCameraStateUpdate, 11>(VTABLE_IronSightsState[0]);

		//Disabled, Causes single frame camera missplacement when exiting the tween menu
		//stl::write_vfunc_unique<RE::TweenMenuCameraState, TESCameraStateUpdate, 11>();
	}

}
