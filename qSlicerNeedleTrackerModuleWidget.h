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

#ifndef __qSlicerNeedleTrackerModuleWidget_h
#define __qSlicerNeedleTrackerModuleWidget_h

// 8/28/2012 ayamada
// CTK includes
#include <ctkVTKObject.h>

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include "qSlicerNeedleTrackerModuleExport.h"

// 8/21/2012 ayamada
#include <QThread.h>
#include <QMutex.h>
#include <QReadWriteLock.h>
#include <QSemaphore.h>
#include <QWaitCondition.h>


#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

/*
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
*/
#include <vtkMRMLScene.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLLinearTransformNode.h>

#include <vtkImageImport.h>
#include <vtkTexture.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlaneSource.h>


class qSlicerNeedleTrackerModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLScalarVolumeNode;
class vtkMRMLViewNode;
class vtkMRMLInteractionNode;
class qSlicerNeedleTrackerModuleWidget;

// 8/21/2012 ayamada
// QThread class to use OpenCV with thread
class OpenCVThread : public QThread
{
  Q_OBJECT
  
public:
  //OpenCVThread(qSlicerNeedleTrackerModuleWidget* wp);
  OpenCVThread();
  
  void setMessage(const QString &message);
  void stop();
  
  CvCapture *src;
  IplImage *frame;
  
  qSlicerNeedleTrackerModuleWidget* wp;
  
  // 9/30/2012 ayamada
  CvSize imageSize;
  IplImage* captureImage;
  IplImage* RGBImage;
  IplImage* captureImageTmp;
  vtkTexture *atext;
  vtkImageImport *importer;
  
protected:
  void run();
  
private:
  QString messageStr;
  QMutex mutex;
  volatile bool stopped;
};

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_NEEDLETRACKER_EXPORT qSlicerNeedleTrackerModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerNeedleTrackerModuleWidget(QWidget *parent=0);
  virtual ~qSlicerNeedleTrackerModuleWidget();

  int planeOnOffSwitch;
  vtkMRMLModelDisplayNode *displaySagittal;
  vtkMRMLModelDisplayNode *displayCoronal;
  vtkMRMLModelDisplayNode *displayAxial;
  
  void CreateTransformationMatrix(vtkMatrix4x4*, int, int, int);
  void CreateModel(vtkMRMLModelDisplayNode *, vtkMRMLLinearTransformNode*);
  
  // 10/1/2012 ayamada: viewer
  vtkActor *actor;
  vtkPolyDataMapper *FocalPlaneMapper;
  vtkPlaneSource* viewerPlane;
  
  int testvariable;
  
  // Use OpenCVThread class
  OpenCVThread OpenCVthread;
    
public slots:

  void startOrStopOpenCVThread();
  void displayOpenCVPlanes();  
  
protected:
  QScopedPointer<qSlicerNeedleTrackerModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerNeedleTrackerModuleWidget);
  Q_DISABLE_COPY(qSlicerNeedleTrackerModuleWidget);
};




#endif
