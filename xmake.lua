-- include subprojects
includes("lib/commonlibsse")

-- set project constants
set_project("Spellcrafting")
set_version("0.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

add_requires("imgui", {configs = {dx11 = true, win32 = true}})

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- define targets
target("Spellcrafting")
    add_rules("commonlibsse.plugin", {
        name = "Spellcrafting",
        author = "MorbidWhale",
        description = "The College of Winterhold finally learned how to spellcraft again !"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
    add_packages("imgui")
