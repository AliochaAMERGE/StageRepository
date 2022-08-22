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

 L0AppScienceProcessorStub.hpp

 $Rev: 0000 $

 ************************************************************************/

#ifndef L0APPSCIENCEPROCESSORSTUB_HPP
#define L0APPSCIENCEPROCESSORSTUB_HPP

#include <gericos/core/realtime/GscTask.hpp>
#include <plato/das/l0app/science/L0AppScienceProcessor.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				/**
				 *
				 */
				class ScienceProcessorStub : public virtual GscTask, public ScienceProcessor {
				private:
					ScienceProcessorStub(const ScienceProcessorStub* source);		///< Non functional copy constructor
					ScienceProcessorStub(const ScienceProcessorStub& source);		///< Non functional copy constructor
					ScienceProcessorStub& operator=(ScienceProcessorStub& source);	///< Non functional overload of the assignment operator

				protected:
					ScienceProcessor* scienceProcessor;

				public:
					enum MESSAGES {
						SET_FLUX_OUTLIER_PACKET_ENABLE = 1,
						SET_FAST_SMEARING_PACKET_ENABLE = 2,
						SET_SCIENCE_PACKET_ENABLE = 3,
						INITIALIZE = 4,
						PREPARE_SCIENCE_PROCESSING = 5,
						EXECUTE_SCIENCE_PROCESSING = 6,
						CALCULATE_STARS_POSITIONS = 7
					};

					/**
					 * Constructor
					 */
					ScienceProcessorStub();

					/**
					 * Set connection with the threaded task implementation
					 *
					 * @param[in]  scienceProcessor  pointer on an instance of science processor
					 */
					void setScienceProcessor(ScienceProcessor* scienceProcessor) {
						this->scienceProcessor = scienceProcessor;
					}

					/**
					 * Inherited from GscTask
					 */
					virtual void processMessage(GscTaskMessage* message);

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual idbEnabledType::IdbEnabledTypeEnum getSciencePacketEnable() const;

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setFluxOutlierPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fluxOutlierPacketEnable);

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setFastSmearingPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fastSmearingPacketEnable);

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setSciencePacketEnable(const idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable);

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void initialize(const cameraIdentifier::CameraIdentifierEnum cameraId, const uint8_t processId);

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void prepareScienceProcessing();

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void executeScienceProcessing(const uint32_t observationCycleCounter, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId,
							const GsbTime& currentTimestamp);

					/**
					 * @brief Inherited from ScienceProcessor
					 */
					virtual void calculateStarsPositions();
				};
			}
		}
	}
}

using plato::das::l0app::science::ScienceProcessorStub;

#endif /* L0APPSCIENCEPROCESSORSTUB_HPP */
