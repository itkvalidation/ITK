set(ITKTransform_SRC
  itkHDF5TransformIO.cxx
  itkHDF5TransformIOFactory.cxx
  itkTxtTransformIO.cxx
  itkTxtTransformIOFactory.cxx
  itkTransformIOBase.cxx
  itkMatlabTransformIO.cxx
  itkTransformFactoryBase.cxx
  itkTransformFileWriter.cxx
  itkTransformFileReader.cxx
  itkMatlabTransformIOFactory.cxx
  itkTransformIOFactory.cxx
  )

add_library(ITKTransform ${ITKTransform_SRC})
target_link_libraries(ITKTransform  ${ITKStatistics_LIBRARIES} itkhdf5 itkhdf5_cpp)
itk_module_target(ITKTransform)
