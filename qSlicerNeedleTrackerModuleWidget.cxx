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

// 8/20/2012 ayamada
#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

//#include "OpenCVThread.h"


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
  this->OpenCVswitch = 0;
  connect(d->OpenCVswitch, SIGNAL(clicked()),
          this, SLOT(onOpenCVstatusChanged()));
  
}

// 8/20/2012 ayamada
void qSlicerNeedleTrackerModuleWidget::onOpenCVstatusChanged()
{

  
  if(this->OpenCVswitch == 0)
  {
    /*
    // 8/20/2012 ayamada
    // Capture test
    CvCapture* src = NULL;
    IplImage *frame;
    cvNamedWindow("TestCV");
    
    //src = cvCaptureFromCAM(0);
    if((src = cvCreateCameraCapture(0)) == NULL)
    {
      cvWaitKey(10);
    };
    
    cvNamedWindow("TestCV", CV_WINDOW_AUTOSIZE);
    frame = cvQueryFrame(src);
    cvShowImage("TestCV", frame);
    
    
    while(1)
    {
      frame = cvQueryFrame(src);
      cvShowImage("TestCV", frame);
    }
    */
    
    this->OpenCVswitch = 1;
    std::cout << "this->OpenCVswitch =" << this->OpenCVswitch << endl;
  }else{

    /*
    // quit
    cvReleaseCapture(&src);
    cvReleaseImage(frame);
    cvDestroyWindow("TestCV");
    */
    
    this->OpenCVswitch = 0;
    std::cout << "this->OpenCVswitch =" << this->OpenCVswitch << endl;
  }
  
  
}



