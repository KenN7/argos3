/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_engine.h"
#include "pointmass3d_model.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPointMass3DEngine::CPointMass3DEngine() {
   }

   /****************************************/
   /****************************************/

   CPointMass3DEngine::~CPointMass3DEngine() {
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CPhysicsEngine::Init(t_tree);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Reset() {
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Destroy() {
      /* Empty the physics entity map */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Update() {
      DEBUG_FUNCTION_ENTER;
      /* Update the physics state from the entities */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      for(size_t i = 0; i < GetIterations(); ++i) {
         /* Perform the step */
         for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
             it != m_tPhysicsModels.end(); ++it) {
            it->second->Step();
         }
      }
      /* Update the simulated space */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   UInt32 CPointMass3DEngine::GetNumPhysicsModels() {
      DEBUG_FUNCTION_ENTER;
      DEBUG_FUNCTION_EXIT;
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::AddEntity(CEntity& c_entity) {
      CallEntityOperation<CPointMass3DOperationAddEntity, CPointMass3DEngine, void>(*this, c_entity);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::RemoveEntity(CEntity& c_entity) {
      CallEntityOperation<CPointMass3DOperationRemoveEntity, CPointMass3DEngine, void>(*this, c_entity);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::IsPointContained(const CVector3& c_point) {
      return true;
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::IsEntityTransferNeeded() const {
      return false;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::TransferEntities() {
   }

   /****************************************/
   /****************************************/

   CEmbodiedEntity* CPointMass3DEngine::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                                 const CRay3& c_ray) const {
      return NULL;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::AddPhysicsModel(const std::string& str_id,
                                            CPointMass3DModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::RemovePhysicsModel(const std::string& str_id) {
      CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("PointMass3D model id \"" << str_id << "\" not found in point-mass 3D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CPointMass3DEngine,
                           "pointmass3d",
                           "Carlo Pinciroli [ilpincy@gmail.com]",
                           "1.0",
                           "A 3D point-mass physics engine.",
                           "This physics engine is a 3D point-mass engine.\n\n"
                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <pointmass3d id=\"pm3d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "If two engines share the same id, initialization aborts.\n\n"
                           "OPTIONAL XML CONFIGURATION\n\n"
                           "None for the time being.\n\n"
                           ,
                           "Under development"
      );

}
