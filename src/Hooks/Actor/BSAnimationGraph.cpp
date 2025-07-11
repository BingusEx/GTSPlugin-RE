
#include "Hooks/Actor/BSAnimationGraph.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {

	struct BSAnimationGraphEventHandle {

		static constexpr std::size_t funcIndex = 0x01;

		template<int ID>
		static void thunk(RE::BSTEventSink<BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<BSAnimationGraphEvent>* a_src) {

			if (a_event.tag != NULL && a_event.holder != NULL) {
				Actor* actor = static_cast<Actor*>(a_this);
				if (actor) {
					EventDispatcher::DoActorAnimEvent(actor, a_event.tag, a_event.payload);
				}
			}
			return func<ID>(a_this, a_event, a_src);
		}

		template<int ID>
		static inline constinit REL::Relocation<decltype(&thunk<ID>)> func;

	};

	void Hook_BSAnimationGraph::Install() {

		logger::info("Installing BSAnimationGraph VTABLE MultiHooks...");

		stl::write_vfunc_unique<RE::Actor, 2, BSAnimationGraphEventHandle, 0>();
		stl::write_vfunc_unique<RE::Character, 2, BSAnimationGraphEventHandle, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, 2, BSAnimationGraphEventHandle, 2>();
	}
}