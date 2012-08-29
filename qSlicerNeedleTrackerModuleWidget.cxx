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
  
  // 8/20/2012 ayamada
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(startOrStopOpenCVThread()));
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(displayOpenCVPlane()));  
}


// 8/28/2012 ayamada
void qSlicerNeedleTrackerModuleWidget::displayOpenCVPlanes()
{
  Q_D(qSlicerNeedleTrackerModuleWidget);
  
  // test codes
  
  if(this->planeOnOffSwitch == 0)
  {
    this->planeOnOffSwitch++;

    vtkMRMLLinearTransformNode* cvSagittalPlane = vtkMRMLLinearTransformNode::New();
    vtkMatrix4x4* cvSagittalPlaneMatrix = vtkMatrix4x4::New();
    
    cvSagittalPlaneMatrix->Identity();
  
    cvSagittalPlane->SetAndObserveMatrixTransformToParent(cvSagittalPlaneMatrix);
    cvSagittalPlane->SetName("cvSagittalPlane");
    this->mrmlScene()->AddNode(cvSagittalPlane);
    
    {
      vtkMRMLModelNode* model = vtkMRMLModelNode::New();
    
      vtkPlaneSource* plane = vtkPlaneSource::New();
      plane->SetPoint1(128,0,0);// (width,0,0)
      plane->SetPoint2(0,128,0);// (0,height,0)
      plane->SetCenter(0,0,0);
      plane->Update();
    
      model->SetAndObservePolyData(plane->GetOutput());
      this->displaySagittal->SetPolyData(model->GetPolyData());
    
      this->mrmlScene()->SaveStateForUndo();
      this->mrmlScene()->AddNode(this->displaySagittal);
      this->mrmlScene()->AddNode(model);
      this->displaySagittal->SetScene(this->mrmlScene());
      model->SetScene(this->mrmlScene());
      model->SetAndObserveDisplayNodeID(this->displaySagittal->GetID());
      model->SetHideFromEditors(0);
      model->SetAndObserveTransformNodeID(cvSagittalPlane->GetID());
    
      this->displaySagittal->SetAmbient(70);
      this->displaySagittal->SetDiffuse(100);
    
      model->Delete();
      plane->Delete();
    }   
    
    cvSagittalPlane->Delete();
    cvSagittalPlaneMatrix->Delete();
  
    vtkMRMLLinearTransformNode* cvCoronalPlane = vtkMRMLLinearTransformNode::New();
    vtkMatrix4x4* cvCoronalPlaneMatrix = vtkMatrix4x4::New();
  
    cvCoronalPlaneMatrix->Identity();
    cvCoronalPlaneMatrix->SetElement(0,0,0);
    cvCoronalPlaneMatrix->SetElement(0,2,1);
    cvCoronalPlaneMatrix->SetElement(2,0,-1);
    cvCoronalPlaneMatrix->SetElement(2,2,0);
  
    cvCoronalPlane->SetAndObserveMatrixTransformToParent(cvCoronalPlaneMatrix);
    cvCoronalPlane->SetName("cvCoronalPlane");
    this->mrmlScene()->AddNode(cvCoronalPlane);
    
    {
      vtkMRMLModelNode* model = vtkMRMLModelNode::New();
      
      vtkPlaneSource* plane = vtkPlaneSource::New();
      plane->SetPoint1(128,0,0);// (width,0,0)
      plane->SetPoint2(0,128,0);// (0,height,0)
      plane->SetCenter(0,0,0);
      plane->Update();
      
      model->SetAndObservePolyData(plane->GetOutput());
      this->displayCoronal->SetPolyData(model->GetPolyData());
      
      this->mrmlScene()->SaveStateForUndo();
      this->mrmlScene()->AddNode(this->displayCoronal);
      this->mrmlScene()->AddNode(model);
      this->displayCoronal->SetScene(this->mrmlScene());
      model->SetScene(this->mrmlScene());
      model->SetAndObserveDisplayNodeID(this->displayCoronal->GetID());
      model->SetHideFromEditors(0);
      model->SetAndObserveTransformNodeID(cvCoronalPlane->GetID());
      
      this->displayCoronal->SetAmbient(70);
      this->displayCoronal->SetDiffuse(100);
      
      model->Delete();
      plane->Delete();
    }   
    
    cvCoronalPlane->Delete();
    cvCoronalPlaneMatrix->Delete();
  }else if(this->planeOnOffSwitch == 1){
    this->displaySagittal->SetVisibility(0);
    this->displayCoronal->SetVisibility(0);
    this->planeOnOffSwitch = 2;
  }else if(this->planeOnOffSwitch == 2){
    this->displaySagittal->SetVisibility(1);
    this->displayCoronal->SetVisibility(1);
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
  // Capture test loop 
  if((this->src = cvCreateCameraCapture(0)) != NULL)
  { 
    this->frame = cvQueryFrame(this->src);
    
    while (!this->stopped)
    {
      this->frame = cvQueryFrame(this->src);
    }
  }
  this->stopped = false;
}

void OpenCVThread::stop()
{    
  this->stopped = true;
  cvReleaseCapture(&this->src);  
}



