#include "Hooks/Hooks.hpp"


#include "Hooks/Actor/Actor.hpp"
#include "Hooks/Actor/ActorValueOwner.hpp"
#include "Hooks/Actor/ActorEquipManager.hpp"
#include "Hooks/Actor/BSAnimationGraph.hpp"
#include "Hooks/Actor/Damage.hpp"
#include "Hooks/Actor/Detection.hpp"
#include "Hooks/Actor/HeadTracking.hpp"
#include "Hooks/Actor/Jump.hpp"
#include "Hooks/Actor/MagicTarget.hpp"
#include "Hooks/Actor/Perk.hpp"
#include "Hooks/Actor/Scale.hpp"
#include "Hooks/Actor/Sink.hpp"

#include "Hooks/Animation/Headtracking_Graph.hpp"
#include "Hooks/Animation/PreventAnimations.hpp"

#include "Hooks/Camera/PlayerCamera.hpp"
#include "Hooks/Camera/TESCamera.hpp"
#include "Hooks/Camera/TESCameraState.hpp"

#include "Hooks/Actor/Damage.hpp"
#include "Hooks/Actor/ActorEquipManager.hpp"

#include "Hooks/Papyrus/PushAway.hpp"
#include "Hooks/Papyrus/VM.hpp"

#include "Hooks/UI/Console.hpp"



#include "Hooks/Skyrim/Controls.hpp"
#include "Hooks/Skyrim/RaceMenu.hpp"

//#include "Hooks/Skyrim/HeadTracking_Graph.hpp"
//#include "Hooks/Skyrim/PreventAnimations.hpp"
//#include "Hooks/Skyrim/hkbBehaviorGraph.hpp"
//#include "Hooks/Skyrim/HeadTracking.hpp"
//#include "Hooks/Skyrim/MagicTarget.hpp"
//#include "Hooks/Skyrim/Projectiles.hpp"
//#include "Hooks/Skyrim/D3DPresent.hpp"
//#include "Hooks/Skyrim/Movement.hpp"
//
//#include "Hooks/Skyrim/Pushback.hpp"
//
//#include "Hooks/Skyrim/Stealth.hpp"
//#include "Hooks/Skyrim/Impact.hpp"
//
//#include "Hooks/Skyrim/Input.hpp"
//#include "Hooks/Skyrim/Havok.hpp"
//#include "Hooks/Skyrim/Main.hpp"
//#include "Hooks/Skyrim/Sink.hpp"
//#include "Hooks/Skyrim/Jump.hpp"


namespace Hooks
{
	void InstallControls() {

		log::info("Applying Control Hooks...");

		Hook_Controls<ActivateHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_ActivateHandler[0]));
		Hook_Controls<JumpHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_JumpHandler[0]));
		Hook_Controls<ShoutHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_ShoutHandler[0]));
		Hook_Controls<SneakHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_SneakHandler[0]));
		Hook_Controls<ToggleRunHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_ToggleRunHandler[0]));

		// Hooked so it's impossible to sheathe/unsheathe during transition anims
		Hook_Controls<AttackBlockHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_AttackBlockHandler[0]));

		//Hook_Controls<AutoMoveHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_AutoMoveHandler[0]));
		//Hook_Controls<MovementHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_MovementHandler[0]));
		//Hook_Controls<ReadyWeaponHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_ReadyWeaponHandler[0]));
		//Hook_Controls<RunHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_RunHandler[0]));
		//Hook_Controls<ThirdPersonState>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_ThirdPersonState[0]));
		//Hook_Controls<SprintHandler>::Hook(REL::Relocation<std::uintptr_t>(RE::VTABLE_SprintHandler[0]));
		
		log::info("Applied Control Hooks");
	}

	void Install(){

		logger::info("Installing Hooks...");
		auto& SKSETrampoline = SKSE::GetTrampoline();
		SKSETrampoline.create(384);

		//Actor
		Hook_Actor::Install();
		Hook_ActorEquipManager::Install();
		Hook_ActorValueOwner::Install();
		Hook_BSAnimationGraph::Install();
		Hook_Damage::Install();
		Hook_Detection::Install();
		Hook_HeadTracking::Install();
		Hook_Jump::Install();
		//Hook_MagicTarget::Install(); //Unused
		Hook_Perk::Install();
		Hook_Scale::Install();
		Hook_Sink::Install();

		//Animation

		//Camera
		//Hook_PlayerCamera::Install(); //Unused
		//Hook_TESCamera::Install(); //Unused
		Hook_TESCameraState::Install();

		//Engine

		//Havok

		//Papyrus

		//Projectile

		//Sound

		//UI

		


		Hook_VM::Install();

		Hook_PushAway::Install();
		Hook_Console::Install();


		//Old Hooks
		//Hook_Projectiles::Hook();      // Experimental stuff with scaling arrows and other projectiles. It works but mostly visually.
		//Hook_BGSImpactManager::Hook();

		//Hook_hkbBehaviorGraph::Hook();

		//Hook_MainUpdate::Hook(SKSETrampoline);
		//Hook_Input::Hook(SKSETrampoline);
		//Hook_Renderer::Hook(SKSETrampoline);
		//Hook_BGSImpactManager::Hook(SKSETrampoline);
		//Hook_Havok::Hook(SKSETrampoline);

		//Hook_Sinking::Hook(SKSETrampoline);
		//Hook_Jumping::Hook(SKSETrampoline);

		//Hook_PushBack::Hook(SKSETrampoline);

		//Hook_Stealth::Hook(SKSETrampoline);
		//Hook_Movement::Hook(SKSETrampoline);
		//Hook_HeadTracking::Hook(SKSETrampoline);
		//Hook_HeadTrackingGraph::Hook(SKSETrampoline);
		//Hook_PreventAnimations::Hook(SKSETrampoline);
		Hook_RaceMenu::Hook(SKSETrampoline);



		InstallControls();
		
		//if (REL::Module::IsSE()) { // Used when something is not RE'd yet for AE

		//}

		//Hook_MagicTarget::Hook();
		//Hook_ActorRotation::Hook(SKSETrampoline);
		//Hook_Experiments::Hook(SKSETrampoline);

		log::info("Finished applying hooks");
		log::info("Default Trampoline Used: {}/{} Bytes", SKSETrampoline.allocated_size(), SKSETrampoline.capacity());
		

	}
}