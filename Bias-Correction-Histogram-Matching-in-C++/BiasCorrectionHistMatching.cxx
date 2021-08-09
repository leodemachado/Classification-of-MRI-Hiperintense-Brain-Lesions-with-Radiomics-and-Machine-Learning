// Calculating the statistics of the histogram with NORMALIZATION

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <iostream>
#include <fstream>

#include "itkN4BiasFieldCorrectionImageFilter.h"

#include "itkHistogramMatchingImageFilter.h"

int main( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing command line arguments" << std::endl;
    std::cerr << "Usage :  Image  inputImageFileName " << std::endl;
    return EXIT_FAILURE;
    }

  // Defining Image Types
  const unsigned int         Dimension = 3;
  typedef float      PixelType;
  typedef itk::Image<PixelType, Dimension > ImageType;

  // Defining Reader Types
  typedef itk::ImageFileReader< ImageType > ReaderType;

  // Instantiating and filling inputs
  ReaderType::Pointer imageReader1 = ReaderType::New();
  imageReader1->SetFileName( argv[1] );
  ReaderType::Pointer imageReader2 = ReaderType::New();
  imageReader2->SetFileName( argv[2] );

  // updating (reading the GreyScaleImage) in a try/catch block.
  try
    {
    imageReader1->Update();
    imageReader2->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Problem reading image file : " << argv[1] << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  // Grayscale Image.
  ImageType::ConstPointer rawImage = imageReader1->GetOutput();
  ImageType::ConstPointer refImage = imageReader2->GetOutput();

  // catching filename (with extension) to use in saving file routine
  std::string fullname = argv[1];
  std::string fileNameWithExtension = fullname.substr(fullname.find_last_of("\\/")+1);
  std::string dirName = fullname.substr(0, fullname.find_last_of("\\/")+1);
  // Removing extension
  std::string fileName = fileNameWithExtension.substr(0, fileNameWithExtension.find_last_of("."));

  std::cout<<"File Name: "<<fileName<<std::endl;
  std::cout<<"Dir Name: "<<dirName<<std::endl;

  using CorrectorType = itk::N4BiasFieldCorrectionImageFilter< ImageType >;
  CorrectorType::Pointer corrector = CorrectorType::New();

  corrector->SetInput(rawImage);
  corrector->Update();
  std::cout<<"Bias Field Corrected!"<<std::endl;

  using HistoMatchingType = itk::HistogramMatchingImageFilter< ImageType, ImageType >;
  HistoMatchingType::Pointer histoMatcher = HistoMatchingType::New();

  histoMatcher->SetInput(corrector->GetOutput());
  histoMatcher->SetReferenceImage(refImage);
  histoMatcher->Update();
  std::cout<<"Histogram Matched!"<<std::endl;

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  std::string newFileName = dirName + "Prep-" + fileName;
  writer->SetFileName( newFileName );
  writer->SetInput( histoMatcher->GetOutput() );

  try
     {
     writer->Update();
     }
  catch( itk::ExceptionObject & error )
     {
     std::cerr << "Error: " << error << std::endl;
     return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
