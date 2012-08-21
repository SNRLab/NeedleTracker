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
  , d_ptr( new qSlicerNeedleTrackerModuleWidgetPrivate )
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
  
  // 8/20/2012 ayamada
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(startOrStopOpenCVThread()));
  
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
      //cerr << qPrintable(messageStr);    
    }
  }
  
  this->stopped = false;
  //cerr << endl;
}

void OpenCVThread::stop()
{
    
  this->stopped = true;
  
  cvReleaseCapture(&this->src);  
  
}



