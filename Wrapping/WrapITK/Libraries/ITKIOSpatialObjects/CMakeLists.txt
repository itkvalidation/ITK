WRAP_LIBRARY(ITKIOSpatialObjects)

set(WRAPPER_LIBRARY_GROUPS
)

set(WRAPPER_LIBRARY_DEPENDS ITKSpatialObjects ITKIOBase)

set(WRAPPER_LIBRARY_LINK_LIBRARIES ${ITK_LIBRARIES})

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()
