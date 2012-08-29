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

#ifndef __qSlicerNeedleTrackerModule_h
#define __qSlicerNeedleTrackerModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerNeedleTrackerModuleExport.h"

/*
// 8/27/2012 ayamada
#include "vtkCubeSource.h"
#include "vtkPlaneSource.h"
#include "vtkActor.h"
#include "vtkImageImport.h"
#include "vtkMath.h" 
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"
*/


class qSlicerNeedleTrackerModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_NEEDLETRACKER_EXPORT qSlicerNeedleTrackerModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerNeedleTrackerModule(QObject *parent=0);
  virtual ~qSlicerNeedleTrackerModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  /// Return a custom icon for the module
  virtual QIcon icon()const;

  virtual QStringList categories()const;
  virtual QStringList dependencies() const;
  
  /*
  // 8/27/2012 ayamada
  vtkPlaneSource* FocalPlaneSource; 
  vtkPolyDataMapper* FocalPlaneMapper;
  vtkMatrix4x4* ExtrinsicMatrix;
  vtkTexture* atext; 
  vtkActor* actor;                 
  vtkImageImport* importer;  
  double planeRatio;
  vtkCamera* fileCamera;
  */
  

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerNeedleTrackerModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerNeedleTrackerModule);
  Q_DISABLE_COPY(qSlicerNeedleTrackerModule);

};

#endif
