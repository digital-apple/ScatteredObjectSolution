#include "Settings.h"

Settings::Settings() : toggle(true) {}

bool Settings::GetToggle() const
{
	return toggle;
}

void Settings::SetToggle(bool a_value)
{
	toggle = a_value;
}
