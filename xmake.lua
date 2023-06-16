-- set minimum xmake version
set_xmakever("2.7.8")

-- set project
set_project("scattered-object-solution")
set_version("1.1.1")
set_license("MIT")
set_languages("c++23")
set_optimize("faster")
set_warnings("allextra", "error")

-- set allowed
set_allowedarchs("windows|x64")
set_allowedmodes("debug", "releasedbg")

-- set defaults
set_defaultarchs("windows|x64")
set_defaultmode("releasedbg")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)

-- require packages
add_requires("commonlibsse-ng", { configs = { skyrim_vr = true } }, "toml++")

-- targets
target("scattered-object-solution")
    -- add packages to target
    add_packages("fmt", "spdlog", "commonlibsse-ng", "toml++")

    -- add commonlibsse-ng plugin
    add_rules("@commonlibsse-ng/plugin", {
        name = "scattered-object-solution",
        author = "digital-apple"
    })

    -- add src files
    add_files("source/**.cpp")
    add_headerfiles("source/**.h", "include/**.hpp")
    add_includedirs("source", "include")
    set_pcxxheader("source/PCH.h")

    -- copy build files to MODS or SKYRIM paths
    after_build(function(target)
        local copy = function(env, ext)
            for _, env in pairs(env:split(";")) do
                if os.exists(env) then
                    local plugins = path.join(env, ext, "SKSE/Plugins")
                    os.mkdir(plugins)
                    os.trycp(target:targetfile(), plugins)
                    os.trycp(target:symbolfile(), plugins)
                end
            end
        end
        if os.getenv("SKYRIM_MODS_PATH") then
            copy(os.getenv("SKYRIM_MODS_PATH"), target:name())
        elseif os.getenv("SKYRIM_PATH") then
            copy(os.getenv("SKYRIM_PATH"), "Data")
        end
    end)
