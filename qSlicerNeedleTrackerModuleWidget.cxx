/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerNeedleTrackerModuleWidget.h"
#include "ui_qSlicerNeedleTrackerModule.h"

// 8/28/2012 ayamada
#include "Logic/vtkSlicerNeedleTrackerLogic.h"

// CTK includes
#include "ctkCollapsibleButton.h"

#include "qSlicerMouseModeToolBar.h"
#include "qMRMLSceneDisplayableModel.h"

// VTK includes
#include <vtkCommand.h>
#include <vtkCollection.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

// MRML includes
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLSelectionNode.h"

// 8/28/2012 ayamada test
#include <qMRMLNodeFactory.h>
#include <vtkMRMLScene.h>
#include <vtkMatrix4x4.h>
#include <vtkSphereSource.h>
#include <vtkPlaneSource.h>
#include <vtkCollection.h>
#include <vtkNew.h>

#include <QtCore>


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerNeedleTrackerModuleWidgetPrivate: public Ui_qSlicerNeedleTrackerModule
{
public:
  qSlicerNeedleTrackerModuleWidgetPrivate();  
};

//-----------------------------------------------------------------------------
// qSlicerNeedleTrackerModuleWidgetPrivate methods
//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModuleWidgetPrivate::qSlicerNeedleTrackerModuleWidgetPrivate()
{
}
 
//-----------------------------------------------------------------------------
// qSlicerNeedleTrackerModuleWidget methods
//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModuleWidget::qSlicerNeedleTrackerModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerNeedleTrackerModuleWidgetPrivate() )
{
}

//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModuleWidget::~qSlicerNeedleTrackerModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerNeedleTrackerModuleWidget::setup()
{
  Q_D(qSlicerNeedleTrackerModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
  
  this->planeOnOffSwitch = 0;
  this->displaySagittal = vtkMRMLModelDisplayNode::New();
  this->displayCoronal = vtkMRMLModelDisplayNode::New();
  this->displayAxial = vtkMRMLModelDisplayNode::New();
  
  // 8/20/2012 ayamada
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(startOrStopOpenCVThread()));
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(displayOpenCVPlane()));  
}

void qSlicerNeedleTrackerModuleWidget::CreateTransformationMatrix(vtkMatrix4x4* transformation, int r, int a, int s)
{
  transformation->Identity();
  
  transformation->SetElement(r,r,s);
  transformation->SetElement(2,2,s);
  transformation->SetElement(r,2,a);
  transformation->SetElement(2,r,-a);
}

void qSlicerNeedleTrackerModuleWidget::CreateModel
(vtkMRMLModelDisplayNode *display, vtkMRMLLinearTransformNode* transform)
{

  vtkMRMLModelNode* model = vtkMRMLModelNode::New();
  vtkPlaneSource* plane = vtkPlaneSource::New();
  
  // 9/30/2012 ayamada
  //vtkTexture *atext = vtkTexture::New();
  //vtkImageImport *
  //importer = vtkImageImport::New();
  
  plane->SetPoint1(128,0,0);// (width,0,0)
  plane->SetPoint2(0,128,0);// (0,height,0)
  plane->SetCenter(0,0,0);
  plane->Update();
  
  model->SetAndObservePolyData(plane->GetOutput());
  display->SetPolyData(model->GetPolyData());
  
  this->mrmlScene()->SaveStateForUndo();
  this->mrmlScene()->AddNode(display);
  this->mrmlScene()->AddNode(model);
  display->SetScene(this->mrmlScene());
  model->SetScene(this->mrmlScene());
  model->SetAndObserveDisplayNodeID(display->GetID());
  model->SetHideFromEditors(0);
  model->SetAndObserveTransformNodeID(transform->GetID());
  
  display->SetAmbient(70);
  display->SetDiffuse(100);
  
  model->Delete();
  plane->Delete();  
  
}

// 8/28/2012 ayamada
void qSlicerNeedleTrackerModuleWidget::displayOpenCVPlanes()
{
  Q_D(qSlicerNeedleTrackerModuleWidget);
  
  // test codes
  
  if(this->planeOnOffSwitch == 0)
  {

    //S-I plane setting
    vtkMRMLLinearTransformNode* cvSagittalPlane = vtkMRMLLinearTransformNode::New();
    vtkMatrix4x4* cvSagittalPlaneMatrix = vtkMatrix4x4::New();
    
    this->CreateTransformationMatrix(cvSagittalPlaneMatrix, 1,1,0);
    cvSagittalPlane->SetAndObserveMatrixTransformToParent(cvSagittalPlaneMatrix);
    cvSagittalPlane->SetName("cvSagittalPlane");
    this->mrmlScene()->AddNode(cvSagittalPlane);
    this->CreateModel(this->displaySagittal, cvSagittalPlane);    

    cvSagittalPlane->Delete();
    cvSagittalPlaneMatrix->Delete();
  
    //R-L plane setting
    vtkMRMLLinearTransformNode* cvCoronalPlane = vtkMRMLLinearTransformNode::New();
    vtkMatrix4x4* cvCoronalPlaneMatrix = vtkMatrix4x4::New();
  
    this->CreateTransformationMatrix(cvCoronalPlaneMatrix, 0,1,0);
    cvCoronalPlane->SetAndObserveMatrixTransformToParent(cvCoronalPlaneMatrix);
    cvCoronalPlane->SetName("cvCoronalPlane");
    this->mrmlScene()->AddNode(cvCoronalPlane);
    this->CreateModel(this->displayCoronal, cvCoronalPlane);

    cvCoronalPlane->Delete();
    cvCoronalPlaneMatrix->Delete();

    //A-L plane setting
    vtkMRMLLinearTransformNode* cvAxialPlane = vtkMRMLLinearTransformNode::New();
    vtkMatrix4x4* cvAxialPlaneMatrix = vtkMatrix4x4::New();
    
    this->CreateTransformationMatrix(cvAxialPlaneMatrix, 0,0,1);
    cvAxialPlane->SetAndObserveMatrixTransformToParent(cvAxialPlaneMatrix);
    cvAxialPlane->SetName("cvAxialPlane");
    this->mrmlScene()->AddNode(cvAxialPlane);
    this->CreateModel(this->displayAxial, cvAxialPlane);
    
    cvAxialPlane->Delete();
    cvAxialPlaneMatrix->Delete();
    
    // display planes
    this->planeOnOffSwitch = 1;
    this->displaySagittal->SetVisibility(1);
    this->displayCoronal->SetVisibility(1);
    this->displayAxial->SetVisibility(1);
    
  }else if(this->planeOnOffSwitch == 1){

    // hide planes
    this->displaySagittal->SetVisibility(0);
    this->displayCoronal->SetVisibility(0);
    this->displayAxial->SetVisibility(0);
    this->planeOnOffSwitch = 2;

  }else if(this->planeOnOffSwitch == 2){

    // display planes
    this->displaySagittal->SetVisibility(1);
    this->displayCoronal->SetVisibility(1);
    this->displayAxial->SetVisibility(1);
    this->planeOnOffSwitch = 1;

  }
  
}

// 8/20/2012 ayamada
// SLOT function
void qSlicerNeedleTrackerModuleWidget::startOrStopOpenCVThread()
{
  Q_D(qSlicerNeedleTrackerModuleWidget);

  if(OpenCVthread.isRunning())
  {
    OpenCVthread.stop();
    OpenCVthread.wait();
    d->OpenCVswitch->setText(tr("OpenCV ON"));
    std::cout << "OpenCV thread was stopped." << endl;
  }else{
    OpenCVthread.start();
    d->OpenCVswitch->setText(tr("OpenCV OFF"));
    std::cout << "OpenCV thread was started." << endl;
  }
  
  this->displayOpenCVPlanes();
  
}

OpenCVThread::OpenCVThread()
{
  this->stopped = false;
  // initialization of cvCapture
  this->src = NULL;
}

// 8/21/2012 ayamada: thread for capturing
void OpenCVThread::run()
{
  
  // 9/30/2012 ayamada
  unsigned char* idata = NULL;
  this->frame = NULL;
  this->importer = NULL;
    
  // Capture test loop 
  if((this->src = cvCreateCameraCapture(0)) != NULL)
  { 
    cout << "Capturing is starting...\n";
    
    this->frame = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U,3);//cvQueryFrame(this->src);
    
    while (!this->stopped)
    {
            
      //this->frame = cvQueryFrame(this->src);
      
      // 9/30/2012 ayamada: describ the reading part of images 
      this->imageSize = cvGetSize( this->frame );
      
      
      this->captureImage = cvCreateImage(this->imageSize, IPL_DEPTH_8U,3);
      this->captureImageTmp = cvCreateImage(this->imageSize, IPL_DEPTH_8U,3);
      this->RGBImage = cvCreateImage(this->imageSize, IPL_DEPTH_8U, 3);
      imageSize = cvGetSize( captureImageTmp );
      cvFlip(this->captureImageTmp, this->captureImage, 0);
      cvCvtColor( this->captureImage, this->RGBImage, CV_BGR2RGB);
      
      idata = (unsigned char*) this->RGBImage->imageData;
      this->importer->SetWholeExtent(0,this->imageSize.width-1,0,this->imageSize.height-1,0,0);
      
      /*
      this->importer->SetDataExtentToWholeExtent();
      this->importer->SetDataScalarTypeToUnsignedChar();
      this->importer->SetNumberOfScalarComponents(3);
      this->importer->SetImportVoidPointer(idata);
      this->atext->SetInputConnection(this->importer->GetOutputPort());
      this->atext->InterpolateOn();
      this->importer->Update();
      */
       //break;
      
      
    }
  }else{
    cout << "Capturing is not starting...\n";
  }
  this->stopped = false;
}

void OpenCVThread::stop()
{    
  this->stopped = true;
  
  cvReleaseCapture(&this->src);  
}



