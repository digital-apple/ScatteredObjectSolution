#include "Hooks.h"
#include "Settings.h"

namespace Addresses
{
	struct DisableForce
	{
		static float thunk(float a_force, RE::bhkRigidBody* a_target, bool a_unk3)
		{
			if (Settings::GetSingleton()->GetToggle() && a_target) {
				if (const auto bhkReference = a_target->referencedObject.get(); bhkReference) {
					if (const auto hkpReference = static_cast<RE::hkpEntity*>(bhkReference); hkpReference) {
						if (const auto targetReference = hkpReference->GetUserData(); targetReference) {
							if (targetReference->GetBaseObject()->GetFormType() == RE::FormType::SoulGem) {
								if (*Settings::pushSoulGems) {
									return func(a_force, a_target, a_unk3);
								} else {
									a_force *= (float)*Settings::forceMultiplier;
								}
							} else {
								a_force *= (float)*Settings::forceMultiplier;
							}
						}
					}
				}
			}
			return func(a_force, a_target, a_unk3);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	void Hook()
	{
		if (*Settings::adjustExplosionForce) {
			REL::Relocation<std::uintptr_t> function{ RELOCATION_ID(42682, 43854), REL::Relocate(0x250, 0x229) };
			stl::write_thunk_call<DisableForce>(function.address());
			logs::info("Hooked Explosions.");
		}

		if (*Settings::adjustShoutForce) {
			REL::Relocation<std::uintptr_t> function{ RELOCATION_ID(42634, 43802), REL::Relocate(0x46B, 0x432) };
			stl::write_thunk_call<DisableForce>(function.address());
			logs::info("Hooked Shouts.");
		}

		if (*Settings::adjustProjectileForce) {
			REL::Relocation<std::uintptr_t> function{ RELOCATION_ID(43024, 44215), 0xC7 };
			stl::write_thunk_call<DisableForce>(function.address());
			logs::info("Hooked Projectiles.");
		}
	}
}