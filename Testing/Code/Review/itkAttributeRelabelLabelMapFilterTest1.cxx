/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkAttributeRelabelLabelMapFilterTest1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkAttributeLabelObject.h"
#include "itkLabelMap.h"

#include "itkLabelImageToLabelMapFilter.h"
#include "itkAttributeRelabelLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"

#include "itkTestingMacros.h"

int itkAttributeRelabelLabelMapFilterTest1(int argc, char * argv[])
{
  if( argc != 4 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " input output";
    std::cerr << " reverseOrdering(0/1)";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 3;

  typedef unsigned char PixelType;

  typedef itk::Image< PixelType, dim > ImageType;

  typedef itk::AttributeLabelObject< PixelType, dim, int >      LabelObjectType;
  typedef itk::LabelMap< LabelObjectType >                      LabelMapType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::LabelImageToLabelMapFilter< ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );


  // The next step is made outside the pipeline model, so we call Update() now.
  i2l->Update();

  // Now we will valuate the attributes. The attribute will be the object position
  // in the label map
  LabelMapType::Pointer labelMap = i2l->GetOutput();

  // Lets begin by declaring the iterator for the objects in the image.
  LabelMapType::LabelObjectContainerType::const_iterator it;
  // And get the object container to reuse it later
  const LabelMapType::LabelObjectContainerType & labelObjectContainer = labelMap->GetLabelObjectContainer();
  int pos = 0;
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    LabelObjectType * labelObject = it->second;
    labelObject->SetAttribute( pos++ );
    }


  typedef itk::AttributeRelabelLabelMapFilter< LabelMapType > LabelRelabelType;
  LabelRelabelType::Pointer relabel = LabelRelabelType::New();

  //testing get and set macros for ReverseOrdering
  //testing boolean macro for ReverseOrdering
  relabel->ReverseOrderingOn();
  TEST_SET_GET_VALUE( true, relabel->GetReverseOrdering() );

  relabel->ReverseOrderingOff();
  TEST_SET_GET_VALUE( false, relabel->GetReverseOrdering() );

  bool reverseOrdering = atoi( argv[3] );
  relabel->SetReverseOrdering( reverseOrdering );
  TEST_SET_GET_VALUE( reverseOrdering , relabel->GetReverseOrdering() );

  relabel->SetInput( labelMap );

  itk::SimpleFilterWatcher watcher(relabel, "filter");

  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( relabel->GetOutput() );

  typedef itk::ImageFileWriter< ImageType > WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
