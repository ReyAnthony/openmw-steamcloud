if (NOT DEFINED OPENMW_RESOURCES_ROOT)
    return()
endif()

# Copy resource files into the build directory
set(SDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DDIRRELATIVE resources/shaders)

set(SHADER_FILES
    groundcover_vertex.glsl
    groundcover_fragment.glsl
    water_vertex.glsl
    water_fragment.glsl
    water_nm.png
    alpha.glsl
    depth.glsl
    objects_vertex.glsl
    objects_fragment.glsl
    openmw_fragment.glsl
    openmw_fragment.h.glsl
    openmw_fragment_multiview.glsl
    openmw_vertex.glsl
    openmw_vertex.h.glsl
    openmw_vertex_multiview.glsl
    terrain_vertex.glsl
    terrain_fragment.glsl
    lighting.glsl
    lighting_util.glsl
    parallax.glsl
    s360_fragment.glsl
    s360_vertex.glsl
    shadows_vertex.glsl
    shadows_fragment.glsl
    shadowcasting_vertex.glsl
    shadowcasting_fragment.glsl
    vertexcolors.glsl
	multiview_resolve_vertex.glsl
	multiview_resolve_fragment.glsl
    nv_default_vertex.glsl
    nv_default_fragment.glsl
    nv_nolighting_vertex.glsl
    nv_nolighting_fragment.glsl
    blended_depth_postpass_vertex.glsl
    blended_depth_postpass_fragment.glsl
    gui_vertex.glsl
    gui_fragment.glsl
    debug_vertex.glsl
    debug_fragment.glsl
    sky_vertex.glsl
    sky_fragment.glsl
    skypasses.glsl
    softparticles.glsl
    hdr_resolve_fragment.glsl
    hdr_luminance_fragment.glsl
    fullscreen_tri_vertex.glsl
    fullscreen_tri_fragment.glsl
    fog.glsl
)

copy_all_resource_files(${CMAKE_CURRENT_SOURCE_DIR} ${OPENMW_RESOURCES_ROOT} ${DDIRRELATIVE} "${SHADER_FILES}")
