#pragma once

namespace Events
{
	using EventResult = RE::BSEventNotifyControl;

	class Events final : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		static Events* GetSingleton();
		static void Register();
	private:
		EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) override;

		Events() = default;
		Events(const Events&) = delete;
		Events(Events&&) = delete;

		~Events() override = default;

		Events& operator=(const Events&) = delete;
		Events& operator=(Events&&) = delete;
	};
}