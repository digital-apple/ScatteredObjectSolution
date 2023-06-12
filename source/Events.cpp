#include "Events.h"
#include "Settings.h"

namespace Events
{
	Events* Events::GetSingleton()
	{
		static Events singleton;
		return std::addressof(singleton);
	}

	void Events::Register()
	{
		if (const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton()) {
			inputDeviceManager->AddEventSink(GetSingleton());

			logs::info("Registered for hotkey events.");
		}
	}

	EventResult Events::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*)
	{
		if (!a_event) {
			return EventResult::kContinue;
		}

		const auto player = RE::PlayerCharacter::GetSingleton();

		if (!player || !player->Is3DLoaded()) {
			return EventResult::kContinue;
		}

		const auto UI = RE::UI::GetSingleton();

		if (!UI || UI->IsMenuOpen(RE::Console::MENU_NAME) || UI->GameIsPaused()) {
			return EventResult::kContinue;
		}

		const auto settings = Settings::GetSingleton();
		const auto hotKey = static_cast<std::uint32_t>(*Settings::toggleHotKey);

		for (auto event = *a_event; event; event = event->next) {
			if (const auto button = event->AsButtonEvent(); button) {
				const auto device = event->GetDevice();

				auto key = button->GetIDCode();

				switch (device) {
					case RE::INPUT_DEVICE::kMouse:
						key += SKSE::InputMap::kMacro_MouseButtonOffset;
						break;
					case RE::INPUT_DEVICE::kGamepad:
						key = SKSE::InputMap::GamepadMaskToKeycode(key);
						break;
					default:
						break;
				}

				if (key == hotKey && !button->IsRepeating()) {
					if (settings->GetToggle()) {
						settings->SetToggle(false);
						if (*Settings::showNotifications) {
							RE::DebugNotification((*Settings::notificationTextOff).c_str(), nullptr, true);
						}
					} else {
						settings->SetToggle(true);
						if (*Settings::showNotifications) {
							RE::DebugNotification((*Settings::notificationTextOn).c_str(), nullptr, true);
						}
					}
				}
			}
		}
		return EventResult::kContinue;
	}
}