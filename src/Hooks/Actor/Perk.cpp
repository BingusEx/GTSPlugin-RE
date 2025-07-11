#include "Hooks/Actor/Perk.hpp"
#include "Hooks/Util/HookUtil.hpp"

namespace Hooks {

	struct AddPerk {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::Actor::AddPerk)> func;
		static inline const std::size_t funcIndex = REL::Relocate(0x0FB, 0x0FB, 0x0FD);

		template<int ID>
		static void thunk(RE::Actor* a_this, RE::BGSPerk* a_perk, std::uint32_t a_rank) {

			func<ID>(a_this, a_perk, a_rank);

			AddPerkEvent Event = {
				.actor = a_this,
				.perk = a_perk,
				.rank = a_rank,
			};

			EventDispatcher::DoAddPerk(Event);
		}

	};

	struct RemovePerk {

		template<int ID>
		static inline REL::Relocation<decltype(&RE::Actor::RemovePerk)> func;
		static inline const std::size_t funcIndex = REL::Relocate(0x0FC, 0x0FC, 0x0FE);

		template<int ID>
		static void thunk(RE::Actor* a_this, RE::BGSPerk* a_perk) {

			RemovePerkEvent Event = {
				.actor = a_this,
				.perk = a_perk,
			};

			EventDispatcher::DoRemovePerk(Event);

			func<ID>(a_this, a_perk);
		}

	};


	void Hook_Perk::Install() {

		logger::info("Installing Perk VTABLE MultiHooks...");

		stl::write_vfunc_unique<RE::Actor, AddPerk, 0>();
		stl::write_vfunc_unique<RE::Character, AddPerk, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, AddPerk, 2>();

		stl::write_vfunc_unique<RE::Actor, RemovePerk, 0>();
		stl::write_vfunc_unique<RE::Character, RemovePerk, 1>();
		stl::write_vfunc_unique<RE::PlayerCharacter, RemovePerk, 2>();

	}
}
