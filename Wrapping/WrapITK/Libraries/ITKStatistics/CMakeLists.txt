WRAP_LIBRARY(ITKStatistics)

set(WRAPPER_LIBRARY_GROUPS
  ITKCostFunctions
  itkSample
  itkHistogram
  itkListSample
  itkMembershipFunctionBase
)

set(WRAPPER_LIBRARY_DEPENDS ITKCommon)

set(WRAPPER_LIBRARY_LINK_LIBRARIES ${ITK_LIBRARIES})

AUTO_INCLUDE_MODULES()
END_WRAP_LIBRARY()
