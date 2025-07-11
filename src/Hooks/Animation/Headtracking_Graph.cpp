#include "Hooks/Animation/HeadTracking_Graph.hpp"
#include "Hooks/Util/HookUtil.hpp"

using namespace GTS;
// This hooks handles behavior-graph that affects headtracking, such as adjusting TDM graph variables

namespace {
        
	int ptrOffset = REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_629) == std::strong_ordering::less ? -0xB8 : -0xC0;
	// Credits to ERSH

	void Headtracking_ManageSpineToggle(Actor* actor) {
		if (actor && actor->Is3DLoaded()) { // Player is handled inside HeadTracking.cpp -> SetGraphVariableBool hook
			std::string taskname = std::format("SpineBool_{}", actor->formID);
			ActorHandle giantHandle = actor->CreateRefHandle();

			double Start = Time::WorldTimeElapsed();

			TaskManager::RunFor(taskname, 1.0f, [=](auto& progressData){
				if (!giantHandle) {
					return false;
				}

				double Finish = Time::WorldTimeElapsed();

				double timepassed = Finish - Start;
				if (timepassed > 0.10) {
					auto giant = giantHandle.get().get();
					bool Disable = !(IsCrawling(giant) || IsProning(giant) || IsHandStomping_L(actor) || IsHandStomping_H(actor));
					giant->SetGraphVariableBool("bHeadTrackSpine", Disable);
					//log::info("Setting {} for {}", Disable, giant->GetDisplayFullName());
					return false;
				}
				return true;
			});
		}
	}
}


namespace Hooks {

	struct SetGraphVariableBool {

		static bool thunk(IAnimationGraphManagerHolder* a_graph, const BSFixedString& a_variableName, bool a_in) {
			//log::info("SetGraph hooked");
			// Disable weird spine rotation during crawl/prone
			if (a_variableName == "bHeadTrackSpine") { 
				// Done through hook since TDM seems to adjust it constantly
				auto actor = skyrim_cast<Actor*>(a_graph);
				if (actor) {
					//log::info("Holder found: {}", actor->GetDisplayFullName());
					bool ShouldDisable = (IsCrawling(actor) || IsProning(actor) || IsHandStomping_L(actor) || IsHandStomping_H(actor));
					if (ShouldDisable) {
						return func(a_graph, a_variableName, false);
					}
				}
			}
			return func(a_graph, a_variableName, a_in);
		}

		static inline decltype(thunk)* func;

	};

	struct AddMovementFlagsSneak {

		static bool thunk(RE::ActorState* a_this, int16_t a_flag) {
			//Adjust actor pointer based on game version, AE > .629 Changed Struct Layouts
			auto actor = SKSE::stl::adjust_pointer<RE::Actor>(a_this, ptrOffset);
			if (actor) {
				// Toggle spine HT on/off based on GTS state
				Headtracking_ManageSpineToggle(actor); 
			}

			return func(a_this, a_flag);
		}

		static inline REL::Relocation<decltype(thunk)> func;

	};

	struct RemoveMovementFlagsSneak {

		static bool thunk(RE::ActorState* a_this, int16_t a_flag) {
			//Adjust actor pointer based on game version, AE > .629 Changed Struct Layouts
			auto actor = SKSE::stl::adjust_pointer<RE::Actor>(a_this, ptrOffset); // Some black magic from Ersh
			if (actor) {
				// Toggle spine HT on/off based on GTS state
				Headtracking_ManageSpineToggle(actor); 
			}
			return func(a_this, a_flag);
		}

		static inline REL::Relocation<decltype(thunk)> func;

	};

	void Hook_HeadTrackingGraph::Install() {

		logger::info("Installing HeadTrackingGraph Hooks...");

		stl::write_detour<SetGraphVariableBool>(REL::RelocationID(32141, 32885, NULL));
		stl::write_call<AddMovementFlagsSneak>(REL::RelocationID(36926, 37951, NULL), REL::VariantOffset(0xE4, 0xA0, NULL));
		stl::write_call<RemoveMovementFlagsSneak>(REL::RelocationID(36926, 37951, NULL), REL::VariantOffset(0xEB, 0xB2, NULL));
	}

}