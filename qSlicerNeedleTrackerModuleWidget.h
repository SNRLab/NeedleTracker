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

class qSlicerNeedleTrackerModuleWidgetPrivate;
class vtkMRMLNode;

// 8/21/2012 ayamada
// QThread class to use OpenCV with thread
class OpenCVThread : public QThread
{
  Q_OBJECT
  
public:
  OpenCVThread();
  
  void setMessage(const QString &message);
  void stop();
  
  CvCapture* src;
  IplImage* frame;

  
protected:
  void run();
  
private:
  QString messageStr;
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

  // 8/20/2012 ayamada
  // use of OpenCVThread class
  OpenCVThread OpenCVthread;

public slots:

  // 8/20/2012 ayamada
  void startOrStopOpenCVThread();
  
protected:
  QScopedPointer<qSlicerNeedleTrackerModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerNeedleTrackerModuleWidget);
  Q_DISABLE_COPY(qSlicerNeedleTrackerModuleWidget);
};


#endif
