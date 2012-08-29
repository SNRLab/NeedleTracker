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

// NeedleTracker Logic includes
#include "vtkSlicerNeedleTrackerLogic.h"

// MRML includes

// VTK includes
#include <vtkNew.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerNeedleTrackerLogic);

//----------------------------------------------------------------------------
vtkSlicerNeedleTrackerLogic::vtkSlicerNeedleTrackerLogic()
{
  
  // 8/28/2012 ayamada
  // setup to display vtk object to Slicer viewer
  
  this->planeDisp = vtkMRMLModelDisplayNode::New();
  this->planeModel = vtkMRMLModelNode::New();
  
  this->FocalPlaneSource = vtkPlaneSource::New();
  this->FocalPlaneMapper = vtkPolyDataMapper::New();
  this->ExtrinsicMatrix = vtkMatrix4x4::New();
  this->actor = vtkActor::New();

  this->FocalPlaneMapper->SetInput(this->FocalPlaneSource->GetOutput());
  this->actor->SetMapper(this->FocalPlaneMapper);
  this->actor->SetUserMatrix(this->ExtrinsicMatrix);
  //this->actor->SetTexture(this->atext);
  
  //this->GetMRMLScene()->AddNode(this->planeDisp);
  //this->GetMRMLScene()->AddNode(this->planeModel);

  
}

//----------------------------------------------------------------------------
vtkSlicerNeedleTrackerLogic::~vtkSlicerNeedleTrackerLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}


//-----------------------------------------------------------------------------
//
//
// MRML event handling: Based on vtkSlicerAnnotationModuleLogic.cxx
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void vtkSlicerNeedleTrackerLogic::ProcessMRMLNodesEvents(vtkObject *caller,
                                                            unsigned long event,
                                                            void *callData)
{
  vtkDebugMacro("ProcessMRMLNodesEvents: Event "<< event);  
  vtkMRMLNode* node = reinterpret_cast<vtkMRMLNode*> (callData);
  
  vtkMRMLModelNode* PlaneNode = vtkMRMLModelNode::SafeDownCast(node);
  
  this->GetMRMLScene()->AddNode(this->planeDisp);
  this->GetMRMLScene()->AddNode(this->planeModel);
  
  this->planeDisp->SetScene(GetMRMLScene());
  
  this->planeModel->SetName("test");
  this->planeModel->SetScene(GetMRMLScene());
  //this->planeDisp->SetAndObserveTextureImageData(this->importer->GetOutput());  
  this->planeModel->SetAndObservePolyData(this->FocalPlaneSource->GetOutput());
  
  this->planeModel->SetAndObserveDisplayNodeID(planeDisp->GetID());
  this->planeModel->SetHideFromEditors(0);
  
  
  
}

