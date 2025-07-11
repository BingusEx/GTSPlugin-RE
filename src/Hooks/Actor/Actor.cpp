#include "Hooks/Actor/Actor.hpp"
#include "Hooks/Util/HookUtil.hpp"
#include "Managers/Attributes.hpp"
#include "Managers/Damage/SizeHitEffects.hpp"

namespace Hooks {

	struct HandleHealthDamage {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::Actor::HandleHealthDamage)> func;
		static inline const std::size_t funcIndex = REL::Relocate(0x104, 0x104, 0x106);

		template<int ID>
		static void thunk(RE::Actor* a_this, RE::Actor* a_attacker, float a_damage) {

			if (a_attacker) {

				SizeHitEffects::GetSingleton().ApplyEverything(a_attacker, a_this, a_damage); // Apply bonus damage, overkill, stagger resistance

				if (Runtime::HasPerkTeam(a_this, "GTSPerkSizeReserveAug1")) { // Size Reserve Augmentation
					auto Cache = Persistent::GetSingleton().GetData(a_this);
					if (Cache) {
						Cache->SizeReserve += -a_damage / 3000;
					}
				}
			}

			func<ID>(a_this, a_attacker, a_damage);  // Just reports the value, can't override it.

		}
	};

	struct Move {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::Actor::Move)> func;
		static inline const std::size_t funcIndex = REL::Relocate(0x0C8, 0x0C8, 0x0CA);

		// Override Movement Speed
		template<int ID>
		static bhkCharacterController* thunk(RE::Actor* a_this, float a_arg2, const RE::NiPoint3& a_position) {

			if (a_this && a_this->Get3D1(false) && !a_this->IsInKillMove()) {
				const float bonus = AttributeManager::AlterMovementSpeed(a_this, a_position);
				return func<ID>(a_this, a_arg2, a_position * bonus);
			}

			return func<ID>(a_this, a_arg2, a_position);

		}

	};

	void Hook_Actor::Install() {

		logger::info("Installing Actor VTABLE MultiHooks...");

		stl::write_vfunc_unique<RE::Actor, HandleHealthDamage, 0>();
		stl::write_vfunc_unique<RE::Character, HandleHealthDamage, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, HandleHealthDamage, 2>();

		stl::write_vfunc_unique<RE::Actor, Move, 0>();
		stl::write_vfunc_unique<RE::Character, Move, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, Move, 2>();

	}
}
