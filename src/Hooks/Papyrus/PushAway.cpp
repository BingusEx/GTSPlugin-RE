#include "Hooks/Papyrus/PushAway.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {

    struct ObjectRefPushAway {

        // 996340 = 55682 (SE)
		// 9BF370 = 56213 (AE)
		// param_3 = Actor*
		// param_4 = Actor*

	    static void thunk(uintptr_t* a_unk1, uintptr_t a_unk2, Actor* actor_1, Actor* actor_2, uintptr_t a_unk3) {
            if (actor_1 && actor_2) {
                //log::trace<>("Param 3: {}", actor_1->GetDisplayFullName());
                //log::trace("Param 4: {}", actor_2->GetDisplayFullName());
                const float size_difference = GetSizeDifference(actor_2, actor_1, SizeType::GiantessScale, false, false);
                //log::trace("Size difference: {}", size_difference);

                if (size_difference > 1.75f) {
                    //log::trace("> 1.75f, can't be pushed away");
                    func(a_unk1, a_unk2, nullptr, nullptr, a_unk3);
                    return;
                }
                if (size_difference >= 1.25f) {
                    StaggerActor_Directional(actor_1, 0.25f, actor_2);
                    //log::trace("> 1.25f, can't be pushed away");
                    func(a_unk1, a_unk2, nullptr, nullptr, a_unk3);
                    return;
                }
            }

            func(a_unk1, a_unk2, actor_1, actor_2, a_unk3);
	    }

        static inline decltype(thunk)* func;

    };

    void Hook_PushAway::Install() {
        log::info("Installing Papyrus ObjectRefPushAway Detour Hook...");
        stl::write_detour<ObjectRefPushAway>(REL::RelocationID(55682, 56213, NULL));
    }

}
