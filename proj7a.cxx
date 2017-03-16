



/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SpecularSpheres.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This examples demonstrates the effect of specular lighting.
//
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkOpenGLPolyDataMapper.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkPolyDataReader.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkDataSetReader.h>

#include <vtkDataSetMapper.h>

#include <vtkDataSet.h>
#include <vtkContourFilter.h>
#include <vtkScalarsToColors.h>

#include <vtkPlane.h>
#include <vtkCutter.h>

#include <vtkPointSource.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkMaskPoints.h>

#include <vtkStreamTracer.h>
#include <vtkLineSource.h>

/* 
Questions 1:

Multiple render windows, render sizes..

viewpoint and viewport?

Vis1t lecture video.


*/

int main()
{
  
 // Read Data
  vtkDataSetReader *rdr = vtkDataSetReader::New();
  rdr->SetFileName("proj7.vtk");
  rdr->Update();
  cerr << "After update, file has " << rdr->GetOutput()->GetNumberOfCells() << " cells." << endl;

  
// Create Render Window
  vtkSmartPointer<vtkRenderWindow> renWin =
      vtkSmartPointer<vtkRenderWindow>::New();


/*******************************************************
*       Renderer 1 - Iso Surface
*         - vtkContourFilter
*         - vtkPolyDataMapper   
********************************************************
*/

vtkDataSet *hardyglobalDataSet = rdr->GetOutput();
hardyglobalDataSet->GetPointData()->SetActiveVectors("grad");
//vtkDataSet *ds = rdr->GetOutput()->SetActiveVectors("hardyglobal");
//rdr->GetOutput()->GetPointData()->SetActiveVectors("hardyglobal");

//rdr->SetActiveVectors("hardyglobal");
vtkContourFilter *cf = vtkContourFilter::New();
  cf->SetNumberOfContours(2);
  cf->SetValue(0, 2.5);
  cf->SetValue(1, 5.0);
  
  cf->SetInputData(hardyglobalDataSet);
  cf->Update();
  cerr << "After contouring, isosurface has " << cf->GetOutput()->GetNumberOfCells() << " triangles." << endl;

  
// Create and Configure Mappers
  vtkSmartPointer<vtkPolyDataMapper> win1Mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  win1Mapper->SetInputConnection(cf->GetOutputPort());
  win1Mapper->SetScalarRange(cf->GetOutput()->GetScalarRange()); // Sets COLOR

// Create Actors
  vtkSmartPointer<vtkActor> win1Actor =
    vtkSmartPointer<vtkActor>::New();
  win1Actor->SetMapper(win1Mapper);

  vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();
    
  renWin->AddRenderer(ren1);
  
  ren1->AddActor(win1Actor);
  ren1->SetBackground(1, 1, 1); // BLACK BACKGROUND
  ren1->SetViewport(0, 0, 0.5, 0.5);
 
    
  
  /*******************************************************
  *       Renderer 2 - Two Slices
  *         - vtkPlane
  *         - vtkCutter
  *         - vtkPolyDataMapper
  ********************************************************
*/
  cerr << "Start Renderer 2" << endl; 

  cerr << "Create Z-Plane slice; "; 
  // Cut Z-Plane Slice
  vtkPlane *zPlane = vtkPlane::New();
  zPlane->SetOrigin(0, 0, 0 ); 
  zPlane->SetNormal(0, 0, 1 );
      
  vtkCutter *cutter = vtkCutter::New();
  cutter->SetInputConnection(rdr->GetOutputPort());
  cutter->SetCutFunction(zPlane);

  cerr << "Create Y-Plane slice; "; 
  // Cut Y-Plane Slice
  vtkPlane *yPlane = vtkPlane::New();
  yPlane->SetOrigin(0, 0, 0 ); 
  yPlane->SetNormal(0, 1, 0 );

  vtkCutter *cutter2 = vtkCutter::New();
  cutter2->SetInputConnection(rdr->GetOutputPort());
  cutter2->SetCutFunction(yPlane);

  cerr << "Create X-Plane slice; " << endl; 
  // Cut X-Plane Slice
  vtkPlane *xPlane = vtkPlane::New();
  xPlane->SetOrigin(0, 0, 0 ); 
  xPlane->SetNormal(1, 0, 0 );

  vtkCutter *cutter3 = vtkCutter::New();
  cutter3->SetInputConnection(rdr->GetOutputPort());
  cutter3->SetCutFunction(xPlane);

cerr << "Create and Configure Mappers" << endl; 
// Create and Configure Mappers
  vtkSmartPointer<vtkPolyDataMapper> win1Mapper2_1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  win1Mapper2_1->SetInputConnection(cutter->GetOutputPort());
  win1Mapper2_1->SetScalarRange(rdr->GetOutput()->GetScalarRange()); // Sets COLOR

  vtkSmartPointer<vtkPolyDataMapper> win1Mapper2_2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  win1Mapper2_2->SetInputConnection(cutter2->GetOutputPort());
  win1Mapper2_2->SetScalarRange(rdr->GetOutput()->GetScalarRange()); // Sets COLOR

  vtkSmartPointer<vtkPolyDataMapper> win1Mapper2_3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  win1Mapper2_3->SetInputConnection(cutter3->GetOutputPort());
  win1Mapper2_3->SetScalarRange(rdr->GetOutput()->GetScalarRange()); // Sets COLOR

cerr << "Create and Set Actors" << endl; 
  // Create and Set Actors
  vtkSmartPointer<vtkActor> win1Actor2_1 =
    vtkSmartPointer<vtkActor>::New();
  win1Actor2_1->SetMapper(win1Mapper2_1);

   vtkSmartPointer<vtkActor> win1Actor2_2 =
    vtkSmartPointer<vtkActor>::New();
  win1Actor2_2->SetMapper(win1Mapper2_2);

   vtkSmartPointer<vtkActor> win1Actor2_3 =
    vtkSmartPointer<vtkActor>::New();
  win1Actor2_3->SetMapper(win1Mapper2_3);


  cerr << "Populate Renderer 2" << endl; 
  // Set up Renderer
  vtkSmartPointer<vtkRenderer> ren2 =
    vtkSmartPointer<vtkRenderer>::New();

  renWin->AddRenderer(ren2);

    // add actors
  ren2->AddActor(win1Actor2_1);
  ren2->AddActor(win1Actor2_2);
  ren2->AddActor(win1Actor2_3);
    // view settings
  ren2->SetBackground(0, 0, 0); // BLACK BACKGROUND
  ren2->SetViewport(0, 0.5, 0.5, 1);

    
/*******************************************************
  *       Renderer 3 - HedgeHog Glyphs
  *         - vtkArrowSource
  *         - vtkMask
  *         - vtk3DGlyph
  *         - vtkPolyDataMapper   
  ********************************************************
*/

vtkDataSet *gradDataSet = rdr->GetOutput();
gradDataSet->GetPointData()->SetActiveVectors("grad");

  //Create an arrow.
  vtkSmartPointer<vtkArrowSource> arrowSource =
    vtkSmartPointer<vtkArrowSource>::New();
    //arrowSource->SetInputData(gradDataSet);
  arrowSource->SetInputData(gradDataSet);

  arrowSource->SetTipLength(0.2);
  arrowSource->SetTipRadius(0.3);
  arrowSource->SetTipResolution(6);
  arrowSource->SetShaftResolution(6);
  arrowSource->SetShaftRadius(0.1);


  // Create a point set
  cerr << "Create a point set" << endl; 
  vtkSmartPointer<vtkMaskPoints> maskPoints =
      vtkSmartPointer<vtkMaskPoints>::New();
  maskPoints->SetOnRatio(167); //keep every 2nd point (half the number of points)
  maskPoints->SetInputData(gradDataSet);
  maskPoints->Update();

   vtkSmartPointer<vtkPolyData> input =
    vtkSmartPointer<vtkPolyData>::New();
  input->ShallowCopy(maskPoints->GetOutput());

  
  vtkGlyph3D *glyph = vtkGlyph3D::New();
  glyph->SetInputData(input);
  glyph->SetSourceConnection(arrowSource->GetOutputPort());
  
  glyph->SetVectorModeToUseVector();
  glyph->SetColorModeToColorByScalar();
  glyph->SetScaleModeToDataScalingOff();
  glyph->OrientOn();
  glyph->SetScaleModeToScaleByVector();
  glyph->SetScaleFactor(10);
  glyph->Update();

  //rdr->GetOutput()->GetPointData()->SetActiveVectors("grad");

 // Create and Configure Mappers
  vtkSmartPointer<vtkPolyDataMapper> glyphMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  glyphMapper->SetInputConnection(glyph->GetOutputPort());
  glyphMapper->SetScalarRange(rdr->GetOutput()->GetScalarRange()); // Sets COLOR
  glyphMapper->ScalarVisibilityOn();
   

cerr << "Create and Set Actors" << endl; 
  // Create and Set Actors
 vtkSmartPointer<vtkActor> win1Actor3_1 =
    vtkSmartPointer<vtkActor>::New();
  win1Actor3_1->SetMapper(glyphMapper);


cerr << "Populate Renderer 3" << endl; 
  // Set up Renderer
  vtkSmartPointer<vtkRenderer> ren3 =
    vtkSmartPointer<vtkRenderer>::New();

  renWin->AddRenderer(ren3);

    // add actors
  ren3->AddActor(win1Actor3_1);
  
 
    // view settings
  ren3->SetBackground(0, 0, 0); // BLACK BACKGROUND
  ren3->SetViewport(0.5, 0, 1, 0.5);


/*******************************************************
  *       Renderer 4 - Streamlines
  *         - vtkLineSource
  *         - vtkStreamTracer
  *         - vtkPolyDataMapper
  ********************************************************
*/
  vtkLineSource *seeds = vtkLineSource::New();
  seeds->SetPoint1(-9.0, 0.0, 0.0);
  seeds->SetPoint2(+9.0, 0.0, 0.0);
  seeds->SetResolution(19);

  vtkStreamTracer *streamTracer = vtkStreamTracer::New();
  streamTracer->SetIntegratorTypeToRungeKutta4();
  //streamTracer->SetInputConnection(rdr->GetOutputPort());
  streamTracer->SetInputData(gradDataSet);
  streamTracer->SetSourceConnection(seeds->GetOutputPort());
  streamTracer->SetIntegrationDirectionToForward();
  streamTracer->SetMaximumPropagation(10.0);


// Create and Configure Mappers
  vtkSmartPointer<vtkPolyDataMapper> streamTracerMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    streamTracerMapper->SetInputConnection(streamTracer->GetOutputPort());
    streamTracerMapper->SetScalarRange(rdr->GetOutput()->GetScalarRange()); // Sets COLOR


cerr << "Create and Set Actors" << endl; 
  // Create and Set Actors
 vtkSmartPointer<vtkActor> win1Actor4_1 =
    vtkSmartPointer<vtkActor>::New();
  win1Actor4_1->SetMapper(streamTracerMapper);


cerr << "Populate Renderer 4" << endl; 
// Set up Renderer
  vtkSmartPointer<vtkRenderer> ren4 =
    vtkSmartPointer<vtkRenderer>::New();

  renWin->AddRenderer(ren4);

    // add actors
  ren4->AddActor(win1Actor4_1);
 
    // view settings
  ren4->SetBackground(1, 1, 1); // WHITE BACKGROUND
  ren4->SetViewport(0.5, 0.5, 1, 1);



/*******************************************************
*         Load and Start Render Window
*******************************************************
*/

cerr << "Load Start Render Window" << endl; 
vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  renWin->SetSize(1000, 1000);
  ((vtkInteractorStyle *)iren->GetInteractorStyle())->SetAutoAdjustCameraClippingRange(0);
  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}