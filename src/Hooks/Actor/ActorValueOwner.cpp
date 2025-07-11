#include "Hooks/Actor/ActorValueOwner.hpp"
#include "Hooks/Util/HookUtil.hpp"
#include "Managers/Attributes.hpp"
#include "Managers/AI/AIFunctions.hpp"

namespace Hooks {

	struct GetActorValue {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::ActorValueOwner::GetActorValue)> func;
		static constexpr std::size_t funcIndex = 0x01;

		template<int ID>
		static float thunk(ActorValueOwner* a_owner, ActorValue a_akValue) {

			float value = func<ID>(a_owner, a_akValue);
			const auto actor = skyrim_cast<Actor*>(a_owner);
			if (actor) {
				if (a_akValue == ActorValue::kCarryWeight) {
					value = AttributeManager::AlterGetAv(actor, a_akValue, value);
				}
				if (a_akValue == ActorValue::kSpeedMult && actor->formID != 0x14) {
					value = GetNPCSpeedOverride(actor, value);
				}
			}

			return value;
		}

	};

	struct GetPermanentActorValue {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::ActorValueOwner::GetPermanentActorValue)> func;
		static constexpr std::size_t funcIndex = 0x02;

		template<int ID>
		static float thunk(ActorValueOwner* a_owner, ActorValue a_akValue) {

			float value = func<ID>(a_owner, a_akValue);
			const auto actor = skyrim_cast<Actor*>(a_owner);
			if (actor) {
				if (a_akValue == ActorValue::kCarryWeight) {
					value = AttributeManager::AlterGetPermenantAv(actor, a_akValue, value);
				}
			}

			return value;
		}

	};

	struct GetBaseActorValue {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::ActorValueOwner::GetBaseActorValue)> func;
		static constexpr std::size_t funcIndex = 0x03;

		template<int ID>
		static float thunk(ActorValueOwner* a_owner, ActorValue a_akValue) {

			const float value = func<ID>(a_owner, a_akValue);

			float bonus = 0.0f;
			const auto actor = skyrim_cast<Actor*>(a_owner);
			if (actor) {
				if (a_akValue == ActorValue::kCarryWeight) {
					bonus = AttributeManager::AlterGetBaseAv(actor, a_akValue, value);
				}
			}

			return value + bonus;

		}
	};

	struct SetBaseActorValue {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::ActorValueOwner::SetBaseActorValue)> func;
		static constexpr std::size_t funcIndex = 0x04;

		template<int ID>
		static void thunk(ActorValueOwner* a_owner, ActorValue a_akValue, float a_value) {

			const auto actor = skyrim_cast<Actor*>(a_owner);
			if (actor) {
				if (a_akValue == ActorValue::kCarryWeight) {
					a_value = AttributeManager::AlterSetBaseAv(actor, a_akValue, a_value);
				}
			}

			func<ID>(a_owner, a_akValue, a_value);

		}
	};

	void Hook_ActorValueOwner::Install() {

		logger::info("Installing ActorValueOwner VTABLE MultiHooks...");

		stl::write_vfunc_unique<RE::Actor, 5, GetActorValue, 0>();
		stl::write_vfunc_unique<RE::Character, 5, GetActorValue, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, 5, GetActorValue, 2>();

		stl::write_vfunc_unique<RE::Actor, 5, GetPermanentActorValue, 0>();
		stl::write_vfunc_unique<RE::Character, 5, GetPermanentActorValue, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, 5, GetPermanentActorValue, 2>();

		stl::write_vfunc_unique<RE::Actor, 5, GetBaseActorValue, 0>();
		stl::write_vfunc_unique<RE::Character, 5, GetBaseActorValue, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, 5, GetBaseActorValue, 2>();

		stl::write_vfunc_unique<RE::Actor, 5, SetBaseActorValue, 0>();
		stl::write_vfunc_unique<RE::Character, 5, SetBaseActorValue, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, 5, SetBaseActorValue, 2>();

	}
}
