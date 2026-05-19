set(JUMAKE_TEMPLATE "AudioPlugin")
juce_add_plugin(${PROJECT_NAME}
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "Roses System"
    BUNDLE_ID "art.rosemade.s4plugin"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Rose
    PLUGIN_CODE S4pl
    FORMATS AU VST3 Standalone
    PRODUCT_NAME ${PROJECT_NAME})

# juce_generate_juce_header(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    vendor/mgba
)

set(LIBMGBA_ONLY ON CACHE BOOL "libmgba only")
add_subdirectory(vendor/mgba)

juce_add_binary_data(BinaryData SOURCES
    data/rom.gba
)

target_sources(${PROJECT_NAME}
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        ${GbaResources}
    )


target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        ENABLE_VFS
        ENABLE_VFS_FD
        ENABLE_DIRECTORIES
        M_CORE_GBA
        USE_ZLIB
        ENABLE_GBA
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        mgba
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

set_target_properties(${PROJECT_NAME}
          PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON)