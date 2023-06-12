#pragma once

class Settings
{
public:
    using bSetting = AutoTOML::bSetting;
    using fSetting = AutoTOML::fSetting;
    using iSetting = AutoTOML::iSetting;
    using sSetting = AutoTOML::sSetting;

    [[nodiscard]] static Settings* GetSingleton()
    {
        static Settings singleton;
        return std::addressof(singleton);
    }

    [[nodiscard]] bool GetToggle() const;
    void SetToggle(bool a_value);

    static inline bSetting adjustExplosionForce{ "Settings", "AdjustExplosionForce", true };
    static inline bSetting adjustProjectileForce{ "Settings", "AdjustProjectileForce", false };
    static inline bSetting adjustShoutForce{ "Settings", "AdjustShoutForce", true };
    static inline bSetting pushSoulGems{ "Settings", "PushSoulGems", true };
    static inline fSetting forceMultiplier{"Settings", "ForceMultiplier", 0.0};
    static inline iSetting toggleHotKey{"Toggle", "ToggleHotKey", 38};
    static inline bSetting showNotifications{"Notifications", "ShowNotifications", true};
    static inline sSetting notificationTextOn{"Notifications", "NotificationTextON", "Scattered Object Solution: ON"};
    static inline sSetting notificationTextOff{"Notifications", "NotificationTextOFF", "Scattered Object Solution: OFF"};

    static bool load_config(const std::string& a_path)
    {
        try {
            const auto table = toml::parse_file(a_path);
            const auto& settings = ISetting::get_settings();

            for (const auto& setting : settings) {
                try {
                    setting->load(table);
                } catch (const std::exception& e) {
                    logs::warn(fmt::runtime(e.what()));
                }
            }
        } catch (const toml::parse_error& e) {
            std::ostringstream ss;
            ss
                << "Error parsing file \'" << *e.source().path
                << "\':\n"
                << e.description()
                << "\n  (" << e.source().begin << ")\n";
            logs::error(fmt::runtime(ss.str()));
            return false;
        }
        return true;
    }
private:
    using ISetting = AutoTOML::ISetting;

    Settings();
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;

    ~Settings() = default;

    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

    std::atomic_bool toggle;
};