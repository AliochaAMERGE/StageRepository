/*************************************************************************

 Copyright (c) 2019 by CNRS/LESIA

 This software is copyrighted by and is the sole property of CNRS/LESIA.
 All rights, title, ownership, or other interests in the software remain
 the property of CNRS/LESIA.
 This software may only be used in accordance with the corresponding
 license agreement. Any unauthorized use, duplication, transmission,
 distribution, or disclosure of this software is expressly forbidden.

 This Copyright notice may not be removed or modified without prior
 written consent of CNRS/LESIA.


 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 PlatoDasL0App Component

 L0AppScienceProcessor.hpp

 $Rev: 0000 $

 ************************************************************************/

#ifndef L0APPSCIENCEPROCESSOR_HPP
#define L0APPSCIENCEPROCESSOR_HPP

#include <gericos/blocks/time/GsbTime.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdNumberTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbEnabledTypeEnum.hpp>
#include <plato/das/idb/util/identifier/IdbCameraIdentifierEnum.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				/**
				 * Science computing task of the system.
				 */
				class ScienceProcessor {
				private:
					ScienceProcessor(ScienceProcessor const* source);		///< Non functional copy constructor
					ScienceProcessor(ScienceProcessor const& source);		///< Non functional copy constructor
					ScienceProcessor& operator=(ScienceProcessor& source);	///< Non functional overload of the assignment operator

				public:
					/**
					 * Empty default constructor.
					 */
					ScienceProcessor() {}

					/**
					 * Get the status of the science packet production and transmission
					 * @return status of the science packet production and transmission
					 */
					virtual idbEnabledType::IdbEnabledTypeEnum getSciencePacketEnable() const = 0;

					/**
					 * Set the status of the flux outlier packet production and transmission
					 * @param fluxOutlierPacketEnable status of the flux outlier packet production and transmission
					 */
					virtual void setFluxOutlierPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fluxOutlierPacketEnable) = 0;

					/**
					 * Set the status of the fast smearing packet production and transmission
					 * @param fastSmearingPacketEnable status of the fast smearing packet production and transmission
					 */
					virtual void setFastSmearingPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fastSmearingPacketEnable) = 0;

					/**
					 * Set the status of the science packet production and transmission
					 * @param sciencePacketEnable status of the science packet production and transmission
					 */
					virtual void setSciencePacketEnable(const idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable) = 0;

					/**
					 * Initialize the task
					 * @param cameraId identifier of the camera
					 * @param processId process id of the camera
					 */
					virtual void initialize(const cameraIdentifier::CameraIdentifierEnum cameraId, const uint8_t processId) = 0;

					/**
					 * Prepare task for the incoming observation service
					 */
					virtual void prepareScienceProcessing() = 0;

					/**
					 * Process all the configured science parameters
					 */
					virtual void executeScienceProcessing(const uint32_t observationCycleCounter, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId,
							const GsbTime& currentTimestamp) = 0;


					/**
					 * @brief Calculate the Target star CCD position and initial window position calculation
					 */
					virtual void calculateStarsPositions() = 0;
				};
			}
		}
	}
}

using plato::das::l0app::science::ScienceProcessor;

#endif /* L0APPSCIENCEPROCESSOR_HPP */
