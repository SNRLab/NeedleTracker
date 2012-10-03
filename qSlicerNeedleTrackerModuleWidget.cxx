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
  : Superclass( _parent ), d_ptr( new qSlicerNeedleTrackerModuleWidgetPrivate())
{
}

//-----------------------------------------------------------------------------
qSlicerNeedleTrackerModuleWidget::~qSlicerNeedleTrackerModuleWidget()
{
  
  this->Thread->Delete();
  this->vtkmutex->Delete();
  this->Thread = NULL;
  this->vtkmutex = NULL;
  
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
  
  // 10/1/2012 ayamada: for viewer
  this->actor = vtkActor::New();
  this->FocalPlaneMapper = vtkPolyDataMapper::New();
  this->viewerPlane = vtkPlaneSource::New();
  this->testvariable = 0;
  
  // 10/2/2012 ayamada: for thread
  this->ThreadID = -1;
  this->Thread = vtkMultiThreader::New();
  this->vtkmutex = vtkMutexLock::New();  
  this->threadFlag = 0;
  
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
  //vtkPlaneSource* plane = vtkPlaneSource::New();
  
  // 9/30/2012 ayamada
  //vtkTexture *atext = vtkTexture::New();
  //vtkImageImport *
  //importer = vtkImageImport::New();
  
  this->viewerPlane->SetPoint1(128,-128,0);// (width,0,0)
  this->viewerPlane->SetPoint2(-128,128,0);// (0,height,0)
  this->viewerPlane->SetCenter(0,0,0);
  this->viewerPlane->SetOrigin(-128,-128,0);
  
  this->viewerPlane->Update();
  
  model->SetAndObservePolyData(this->viewerPlane->GetOutput());
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
  //this->viewerPlane->Delete();  
  
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
  
  cvWaitKey(500);
  
  // VTKthread 
  makeCVThread();
  
  
}

void qSlicerNeedleTrackerModuleWidget::makeCVThread()
{
  
  std::cerr << "\n\nmake cvThread: OK\n\n" << std::endl;
  
  this->ThreadID = this->Thread->SpawnThread((vtkThreadFunctionType) &qSlicerNeedleTrackerModuleWidget::thread_CVThread, this);
  
}

// thread function
void *qSlicerNeedleTrackerModuleWidget::thread_CVThread(void* t)
{
  
  vtkMultiThreader::ThreadInfo* vinfo = static_cast<vtkMultiThreader::ThreadInfo*>(t);
  qSlicerNeedleTrackerModuleWidget* pw = static_cast<qSlicerNeedleTrackerModuleWidget*>(vinfo->UserData);
  
  // example for mutex lock/unlock
  pw->vtkmutex->Lock();
  pw->threadFlag = 1;
  pw->vtkmutex->Unlock();
  
  CvCapture *src;
  IplImage *frame;
  
  // 9/30/2012 ayamada
  unsigned char* idata;
  CvSize imageSize;
  IplImage* captureImage;
  IplImage* RGBImage;
  IplImage* captureImageTmp;
  vtkTexture *atext = vtkTexture::New();
  vtkImageImport *importer = vtkImageImport::New();
  
  // Capture test loop 
  if((src = cvCreateCameraCapture(0)) != NULL)
  { 
    cout << "Capturing is starting...\n";
    cvWaitKey(500);
    frame = cvQueryFrame(src);
    
    while(1)
    {
      
      // 9/30/2012 ayamada: describ the reading part of images       
      frame = cvQueryFrame(src);            
      imageSize = cvSize
      (
       (int)cvGetCaptureProperty(src,CV_CAP_PROP_FRAME_WIDTH ),
       (int)cvGetCaptureProperty(src,CV_CAP_PROP_FRAME_HEIGHT )
       );
      
      captureImage = cvCreateImage(imageSize, IPL_DEPTH_8U,3);
      captureImageTmp = cvCreateImage(imageSize, IPL_DEPTH_8U,3);
      RGBImage = cvCreateImage(imageSize, IPL_DEPTH_8U, 3);
      imageSize = cvGetSize( captureImageTmp );
      cvFlip(captureImageTmp, captureImage, 0);
      cvCvtColor(captureImage, RGBImage, CV_BGR2RGB);
      
      idata = (unsigned char*) RGBImage->imageData;
      importer->SetWholeExtent(0,imageSize.width-1,0,imageSize.height-1,0,0);
      importer->SetDataExtentToWholeExtent();
      importer->SetDataScalarTypeToUnsignedChar();
      importer->SetNumberOfScalarComponents(3);
      importer->SetImportVoidPointer(idata);
      atext->SetInputConnection(importer->GetOutputPort());
      atext->InterpolateOn();
      importer->Update();
      break;
    
    }

  }
  
  pw->threadFlag = 2;
  return NULL;

  
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
  /*  
  // 9/30/2012 ayamada
  unsigned char* idata = NULL;
  this->importer = vtkImageImport::New();
  this->atext = vtkTexture::New();
  this->importer = vtkImageImport::New();
  this->src = NULL;
  
  // 10/1/2012 ayamada
  
  // Capture test loop 
  if((this->src = cvCreateCameraCapture(0)) != NULL)
  { 
    cout << "Capturing is starting...\n";
    cvWaitKey(500);
    this->frame = cvQueryFrame(this->src);
    
    // 10/2/2012 ayamada: create VTK structures
    //wp->planeRatio = VIEW_SIZE_X / VIEW_SIZE_Y;
    //wp->CameraFocusPlane(wp->fileCamera, wp->planeRatio);
    ///wp->FocalPlaneMapper->SetInput(wp->viewerPlane->GetOutput());
    mutex.lock();
    //wp->actor->SetMapper(wp->FocalPlaneMapper);
    //wp->testvariable = 1;
    mutex.unlock();
    //wp->actor->SetUserMatrix(wp->ExtrinsicMatrix);
    //wp->actor->SetTexture(wp->atext);
    
    while (!this->stopped)
    {
            
      // 9/30/2012 ayamada: describ the reading part of images       
      this->frame = cvQueryFrame(this->src);            
      this->imageSize = cvSize
      (
       (int)cvGetCaptureProperty(this->src,CV_CAP_PROP_FRAME_WIDTH ),
       (int)cvGetCaptureProperty(this->src,CV_CAP_PROP_FRAME_HEIGHT )
      );
      
      this->captureImage = cvCreateImage(this->imageSize, IPL_DEPTH_8U,3);
      this->captureImageTmp = cvCreateImage(this->imageSize, IPL_DEPTH_8U,3);
      this->RGBImage = cvCreateImage(this->imageSize, IPL_DEPTH_8U, 3);
      imageSize = cvGetSize( captureImageTmp );
      cvFlip(this->captureImageTmp, this->captureImage, 0);
      cvCvtColor( this->captureImage, this->RGBImage, CV_BGR2RGB);
      
      idata = (unsigned char*) this->RGBImage->imageData;
      this->importer->SetWholeExtent(0,this->imageSize.width-1,0,this->imageSize.height-1,0,0);
      this->importer->SetDataExtentToWholeExtent();
      this->importer->SetDataScalarTypeToUnsignedChar();
      this->importer->SetNumberOfScalarComponents(3);
      this->importer->SetImportVoidPointer(idata);
      this->atext->SetInputConnection(this->importer->GetOutputPort());
      this->atext->InterpolateOn();
      this->importer->Update();
      break;
      
    }
  }else{
    cout << "Capturing is not starting...\n";
  }
  this->stopped = false;
   */
}

void OpenCVThread::stop()
{    
  this->stopped = true;
  cvReleaseCapture(&this->src);
}



