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

// .NAME vtkSlicerNeedleTrackerLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerNeedleTrackerLogic_h
#define __vtkSlicerNeedleTrackerLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicerNeedleTrackerModuleLogicExport.h"


// 8/27/2012 ayamada
#include "vtkMRMLModelNode.h"
#include "vtkMRMLModelDisplayNode.h"

// 8/27/2012 ayamada
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkGeometryFilter.h>
#include "vtkTransformPolyDataFilter.h"
#include "vtkAppendPolyData.h"

#include "vtkCubeSource.h"
#include "vtkPlaneSource.h"
#include "vtkActor.h"
#include "vtkImageImport.h"
#include "vtkMath.h" 
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_NEEDLETRACKER_MODULE_LOGIC_EXPORT vtkSlicerNeedleTrackerLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerNeedleTrackerLogic *New();
  vtkTypeMacro(vtkSlicerNeedleTrackerLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  // 8/28/2012 ayamada
  vtkMRMLModelNode           *planeModel;
  vtkMRMLModelDisplayNode    *planeDisp;
  
  // 8/28/2012 ayamada  
  vtkPlaneSource* FocalPlaneSource; 
  vtkPolyDataMapper* FocalPlaneMapper;
  vtkMatrix4x4* ExtrinsicMatrix;
  vtkTexture* atext; 
  vtkActor* actor;                 
  vtkImageImport* importer;  
  double planeRatio;
  vtkCamera* fileCamera;
   

protected:
  vtkSlicerNeedleTrackerLogic();
  virtual ~vtkSlicerNeedleTrackerLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);
  
  // 8/28/2012 ayamada
  virtual void ProcessMRMLNodesEvents(vtkObject *caller,
                                      unsigned long event,
                                      void *callData );
  
  
private:

  vtkSlicerNeedleTrackerLogic(const vtkSlicerNeedleTrackerLogic&); // Not implemented
  void operator=(const vtkSlicerNeedleTrackerLogic&);               // Not implemented
};

#endif
