# local files
set(GAME
    main.cpp
    engine.cpp
    options.cpp

    ${CMAKE_SOURCE_DIR}/files/windows/openmw.rc
    ${CMAKE_SOURCE_DIR}/files/windows/openmw.exe.manifest
)

if (ANDROID)
    set(GAME ${GAME} android_main.cpp)
endif()

set(GAME_HEADER
    engine.hpp
)

source_group(game FILES ${GAME} ${GAME_HEADER})

add_openmw_dir (mwrender
    actors objects renderingmanager animation rotatecontroller sky skyutil npcanimation vismask
    creatureanimation effectmanager util renderinginterface pathgrid rendermode weaponanimation screenshotmanager
    bulletdebugdraw globalmap characterpreview camera localmap water terrainstorage ripplesimulation
    renderbin actoranimation landmanager navmesh actorspaths recastmesh fogmanager objectpaging groundcover
    postprocessor pingpongcull luminancecalculator pingpongcanvas transparentpass navmeshmode
    )

add_openmw_dir (mwinput
    actions actionmanager bindingsmanager controllermanager controlswitch
    inputmanagerimp mousemanager keyboardmanager sensormanager gyromanager
    )

add_openmw_dir (mwgui
    layout textinput widgets race class birth review windowmanagerimp console dialogue
    windowbase statswindow messagebox journalwindow charactercreation
    mapwindow windowpinnablebase tooltips scrollwindow bookwindow resourceskin
    formatting inventorywindow container hud countdialog tradewindow settingswindow
    confirmationdialog alchemywindow referenceinterface spellwindow mainmenu quickkeysmenu
    itemselection spellbuyingwindow loadingscreen levelupdialog waitdialog spellcreationdialog
    enchantingdialog trainingwindow travelwindow exposedwindow cursor spellicons
    merchantrepair repair soulgemdialog companionwindow bookpage journalviewmodel journalbooks
    itemmodel containeritemmodel inventoryitemmodel sortfilteritemmodel itemview
    tradeitemmodel companionitemmodel pickpocketitemmodel controllers savegamedialog
    recharge mode videowidget backgroundimage itemwidget screenfader debugwindow spellmodel spellview
    draganddrop timeadvancer jailscreen itemchargeview keyboardnavigation textcolours statswatcher
    postprocessorhud
    )

add_openmw_dir (mwdialogue
    dialoguemanagerimp journalimp journalentry quest topic filter selectwrapper hypertextparser keywordsearch scripttest
    )

add_openmw_dir (mwscript
    locals scriptmanagerimp compilercontext interpretercontext cellextensions miscextensions
    guiextensions soundextensions skyextensions statsextensions containerextensions
    aiextensions controlextensions extensions globalscripts ref dialogueextensions
    animationextensions transformationextensions consoleextensions userextensions
    )

add_openmw_dir (mwlua
    luamanagerimp object worldview userdataserializer eventqueue
    luabindings localscripts playerscripts objectbindings cellbindings
    camerabindings uibindings inputbindings nearbybindings postprocessingbindings stats debugbindings
    types/types types/door types/actor types/container types/weapon types/npc types/creature types/activator types/book types/lockpick types/probe types/apparatus types/potion types/ingredient types/misc types/repair
    )

add_openmw_dir (mwsound
    soundmanagerimp openal_output ffmpeg_decoder sound sound_buffer sound_decoder sound_output
    loudness movieaudiofactory alext efx efx-presets regionsoundselector watersoundupdater volumesettings
    )

add_openmw_dir (mwworld
    refdata worldimp scene globals class action nullaction actionteleport
    containerstore actiontalk actiontake manualref player cellvisitors failedaction
    cells localscripts customdata inventorystore ptr actionopen actionread actionharvest
    actionequip timestamp actionalchemy cellstore actionapply actioneat
    store esmstore fallback actionrepair actionsoulgem livecellref actiondoor
    contentloader esmloader actiontrap cellreflist cellref weather projectilemanager
    cellpreloader datetimemanager groundcoverstore magiceffects
    )

add_openmw_dir (mwphysics
    physicssystem trace collisiontype actor convert object heightfield closestnotmerayresultcallback
    contacttestresultcallback deepestnotmecontacttestresultcallback stepper movementsolver projectile
    actorconvexcallback raycasting mtphysics contacttestwrapper projectileconvexcallback
    )

add_openmw_dir (mwclass
    classes activator creature npc weapon armor potion apparatus book clothing container door
    ingredient creaturelevlist itemlevlist light lockpick misc probe repair static actor bodypart
    )

add_openmw_dir (mwmechanics
    mechanicsmanagerimp stat creaturestats magiceffects movement actorutil spelllist
    drawstate spells activespells npcstats aipackage aisequence aipursue alchemy aiwander aitravel aifollow aiavoiddoor aibreathe
    aicast aiescort aiface aiactivate aicombat recharge repair enchanting pathfinding pathgrid security spellcasting spellresistance
    disease pickpocket levelledlist combat steering obstacle autocalcspell difficultyscaling aicombataction summoning
    character actors objects aistate trading weaponpriority spellpriority weapontype spellutil
    spelleffects
    )

add_openmw_dir (mwstate
    statemanagerimp charactermanager character quicksavemanager
    )

add_openmw_dir (mwbase
    environment world scriptmanager dialoguemanager journal soundmanager mechanicsmanager
    inputmanager windowmanager statemanager
    )

# Main executable

if (NOT ANDROID)
    openmw_add_executable(openmw
        ${OPENMW_FILES}
        ${GAME} ${GAME_HEADER}
        ${APPLE_BUNDLE_RESOURCES}
    )
else ()
    add_library(openmw
        SHARED
        ${OPENMW_FILES}
        ${GAME} ${GAME_HEADER}
    )
endif ()

# Sound stuff - here so CMake doesn't stupidly recompile EVERYTHING
# when we change the backend.
include_directories(
    ${FFmpeg_INCLUDE_DIRS}
)

target_link_libraries(openmw
    # CMake's built-in OSG finder does not use pkgconfig, so we have to
    # manually ensure the order is correct for inter-library dependencies.
    # This only makes a difference with `-DOPENMW_USE_SYSTEM_OSG=ON -DOSG_STATIC=ON`.
    # https://gitlab.kitware.com/cmake/cmake/-/issues/21701
    ${OSGPARTICLE_LIBRARIES}
    ${OSGVIEWER_LIBRARIES}
    ${OSGGA_LIBRARIES}
    ${OSGSHADOW_LIBRARIES}
    ${OSGDB_LIBRARIES}
    ${OSGUTIL_LIBRARIES}
    ${OSG_LIBRARIES}

    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${OPENAL_LIBRARY}
    ${FFmpeg_LIBRARIES}
    ${MyGUI_LIBRARIES}
    ${SDL2_LIBRARY}
    ${RecastNavigation_LIBRARIES}
    "osg-ffmpeg-videoplayer"
    "oics"
    components
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.16 AND MSVC)
    target_precompile_headers(openmw PRIVATE
        <boost/filesystem.hpp>
        <boost/program_options/options_description.hpp>

        <sol/sol.hpp>

        <osg/StateSet>
        <osg/Node>
        <osg/Drawable>
        <osg/Camera>

        <osgViewer/Viewer>

        <MyGUI_Widget.h>

        <algorithm>
        <filesystem>
        <fstream>
        <functional>
        <memory>
        <string>
        <string_view>
        <vector>
    )
endif()

if (ANDROID)
    target_link_libraries(openmw EGL android log z)
endif (ANDROID)

if (USE_SYSTEM_TINYXML)
    target_link_libraries(openmw ${TinyXML_LIBRARIES})
endif()

if (NOT UNIX)
target_link_libraries(openmw ${SDL2MAIN_LIBRARY})
endif()

# Fix for not visible pthreads functions for linker with glibc 2.15
if (UNIX AND NOT APPLE)
target_link_libraries(openmw ${CMAKE_THREAD_LIBS_INIT})
endif()

if(APPLE)
    set(BUNDLE_RESOURCES_DIR "${APP_BUNDLE_DIR}/Contents/Resources")

    set(OPENMW_RESOURCES_ROOT ${BUNDLE_RESOURCES_DIR})

    add_subdirectory(../../files/ ${CMAKE_CURRENT_BINARY_DIR}/files)

    configure_file("${OpenMW_BINARY_DIR}/defaults.bin" ${BUNDLE_RESOURCES_DIR} COPYONLY)
    configure_file("${OpenMW_BINARY_DIR}/openmw.cfg" ${BUNDLE_RESOURCES_DIR} COPYONLY)
    configure_file("${OpenMW_BINARY_DIR}/gamecontrollerdb.txt" ${BUNDLE_RESOURCES_DIR} COPYONLY)

    add_custom_command(TARGET openmw
        POST_BUILD
        COMMAND cp "${OpenMW_BINARY_DIR}/resources/version" "${BUNDLE_RESOURCES_DIR}/resources")

    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    target_link_libraries(openmw ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK})

    if (FFmpeg_FOUND)
        target_link_libraries(openmw z)
        target_link_options(openmw PRIVATE "LINKER:SHELL:-framework CoreVideo"
                                           "LINKER:SHELL:-framework CoreMedia"
                                           "LINKER:SHELL:-framework VideoToolbox"
                                           "LINKER:SHELL:-framework AudioToolbox"
                                           "LINKER:SHELL:-framework VideoDecodeAcceleration")
    endif()
endif(APPLE)

if (BUILD_WITH_CODE_COVERAGE)
  add_definitions (--coverage)
  target_link_libraries(openmw gcov)
endif()

if (WIN32)
    INSTALL(TARGETS openmw RUNTIME DESTINATION ".")
endif (WIN32)
