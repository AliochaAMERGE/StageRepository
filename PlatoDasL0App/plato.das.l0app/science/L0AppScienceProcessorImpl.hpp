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

 L0AppScienceProcessorImpl.hpp

 $Rev: 0000 $

 ************************************************************************/

#ifndef L0APPSCIENCEPROCESSORIMPL_HPP
#define L0APPSCIENCEPROCESSORIMPL_HPP

#include <plato/das/l0app/science/L0AppScienceProcessor.hpp>
#include <plato/das/l0app/camera/engine/L0AppCameraConfiguration.hpp>
#include <plato/das/l0app/science/calculator/L0AppBackgroundCalculator.hpp>
#include <plato/das/l0app/science/calculator/L0AppFluxAverager.hpp>
#include <plato/das/l0app/science/calculator/L0AppFluxCalculator.hpp>
#include <plato/das/l0app/science/calculator/L0AppSmearingCalculator.hpp>
#include <plato/das/l0app/science/calculator/L0AppStarsPositionsCalculator.hpp>
#include <plato/das/l0app/science/camera/L0AppCameraDataSet.hpp>
#include <plato/das/l0app/science/ccd/L0AppAcquisitionTimestampStack.hpp>
#include <plato/das/l0app/science/packet/L0AppTmBackgroundPacketBuilder.hpp>
#include <plato/das/l0app/science/packet/L0AppTmOffsetPacketBuilder.hpp>
#include <plato/das/l0app/science/packet/L0AppTmOutlierPacketBuilder.hpp>
#include <plato/das/l0app/science/packet/L0AppTmSciencePacketBuilder.hpp>
#include <plato/das/l0app/science/packet/L0AppTmSmearingPacketBuilder.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				/**
				 *
				 */
				class ScienceProcessorImpl : public plato::das::l0app::science::ScienceProcessor {
				private:
					ScienceProcessorImpl(ScienceProcessorImpl const* source);		///< Non functional copy constructor
					ScienceProcessorImpl(ScienceProcessorImpl const& source);		///< Non functional copy constructor
					ScienceProcessorImpl& operator=(ScienceProcessorImpl& source);	///< Non functional overload of the assignment operator

					/// camera identifier
					cameraIdentifier::CameraIdentifierEnum cameraId;
					/// science data set
					CameraDataSet* cameraDataSet;
					/// Data pool
					IdbNcamDataPool* dataPool;
					/// configurations retrieved from the data pool
					CameraConfiguration* cameraConfiguration;
					/// science output fifo
					GsbPacketFifo* scienceTmFifo;

					/// component which build science offset TM packets
					TmOffsetPacketBuilder offsetPacketBuilder;

					/// component which perform smearing computations
					SmearingCalculator smearingCalculator;
					/// component which build science smearing TM packets
					TmSmearingPacketBuilder smearingPacketBuilder;

					/// component which perform flux and COB computations
					FluxCalculator fluxCalculator;
					/// component which perform flux and COB outlier detection and averaging
					FluxAverager fluxAverager;
					/// component which build science flux and COB TM packets
					TmSciencePacketBuilder sciencePacketBuilder;
					/// component which build science outlier flux TM packets
					TmOutlierPacketBuilder outlierPacketBuilder;

					/// component which perform background computations
					BackgroundCalculator backgroundCalculator;
					/// component which build science background TM packets
					TmBackgroundPacketBuilder backgroundPacketBuilder;

					/// component which perform the target star CCD position and initial window position calculation
					StarsPositionsCalculator starsPositionsCalculator;

					/// number of fast cadence cycles (always equal to observation cycle counter, so reset at the end of a long cadence period)
					uint32_t fastCadenceCounter;
					/// number of long cadence cycles that have occurred since the beginning of the observation
					uint32_t longCadenceCounter;
					/// storage of acquisition timestamps used to retrieve previous acquisition timestamp during the observation service
					AcquisitionTimestampStack acquisitionTimestamps[ccdFeatures::CCD_COUNT];
					/// status of the flux outlier packet production and transmission
					idbEnabledType::IdbEnabledTypeEnum fluxOutlierPacketEnable;
					/// status of the fast smearing packet production and transmission
					idbEnabledType::IdbEnabledTypeEnum fastSmearingPacketEnable;
					/// status of the science packet production and transmission
					idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable;

					void calculateOffset(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const idbCcdSideType::IdbCcdSideTypeEnum ccdSide);

					void processOffset(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp);

					void processSmearing(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp);

					void processBackground(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp);

					void processFlux(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp);

					void calculateStarsPositions();

				public:

					//{{RELAX<Rule_0470_V4> false positive : not a variable
					/**
					 * Default constructor
					 */
					explicit ScienceProcessorImpl();
					//}}RELAX<Rule_0470_V4>

					/**
					 * Set connection with the camera data set which store science data
					 * @param cameraDataSet pointer on the camera data set shared resource.
					 */
					void setCameraDataSet(CameraDataSet* cameraDataSet) {
						this->cameraDataSet = cameraDataSet;
					}

					/**
					 * @brief Set a connection with the data pool
					 * @param dataPool pointer on the data pool shared resource.
					 */
					void setDataPool(IdbNcamDataPool* dataPool) {
						this->dataPool = dataPool;
					}

					/**
					 * Set connection with the camera configuration which store setup of N-FEE
					 * @param cameraConfiguration pointer on the instance of the camera configuration.
					 */
					void setCameraConfiguration(CameraConfiguration* cameraConfiguration) {
						this->cameraConfiguration = cameraConfiguration;
					}

					/**
					 * Set connection with the output FIFO where to build the science TM packets
					 * @param scienceTmFifo pointer on the instance of the output FIFO.
					 */
					void setScienceTmFifo(GsbPacketFifo* scienceTmFifo) {
						this->scienceTmFifo = scienceTmFifo;
					}

					/**
					 * Set connection with the smearing warning handler (the eventProducer task of PlatoDasApp project)
					 * @param smearingWarningHandler pointer on the instance of the smearing warning handler
					 */
					void setSmearingWarningHandler(SmearingWarningHandler* smearingWarningHandler) {
						smearingCalculator.setSmearingWarningHandler(smearingWarningHandler);
					}

					/**
					 * @brief Set a connection with the parameter calculation warning handler (the eventProducer task of PlatoDasApp project)
					 * @param parameterCalculationWarningHandler pointer on the instance of the parameter calculation warning handler
					 */
					void setParameterCalculationWarningHandler(ParameterCalculationWarningHandler* parameterCalculationWarningHandler) {
						starsPositionsCalculator.setParameterCalculationWarningHandler(parameterCalculationWarningHandler);
					}

					/**
					 * @brief Set a connection with the informative event CcdPosSummary handler (the eventProducer task of PlatoDasApp project)
					 * @param ccdPositionSummaryHandler pointer on the instance of the informative event CcdPosSummary handler
					 */
					void setCcdPositionSummaryHandler(CcdPositionSummaryHandler* ccdPositionSummaryHandler) {
						starsPositionsCalculator.setCcdPositionSummaryHandler(ccdPositionSummaryHandler);
					}

					/**
					 * @brief Set a connection with the informative event CcdPosDetailed handler (the eventProducer task of PlatoDasApp project)
					 * @param ccdPositionDetailedHandler pointer on the instance of the informative event CcdPosDetailed handler
					 */
					void setCcdPositionDetailedHandler(CcdPositionDetailedHandler* ccdPositionDetailedHandler) {
						starsPositionsCalculator.setCcdPositionDetailedHandler(ccdPositionDetailedHandler);
					}

					/**
					 * @brief Set a connection with the star catalogue
					 * @param starCatalogue the star catalogue
					 */
					void setStarCatalogue(StarCatalogue* starCatalogue) {
						starsPositionsCalculator.setStarCatalogue(starCatalogue);
					}

					/**
					 * @brief Set a connection with the time provider
					 * @param timeProvider the time provider
					 */
					void setTimeProvider(GsbTimeProvider* timeProvider) {
						starsPositionsCalculator.setGsbTimeProvider(timeProvider);
					}

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual idbEnabledType::IdbEnabledTypeEnum getSciencePacketEnable() const {
						return sciencePacketEnable;
					}

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setFluxOutlierPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fluxOutlierPacketEnable) {
						this->fluxOutlierPacketEnable = fluxOutlierPacketEnable;
					}

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setFastSmearingPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fastSmearingPacketEnable) {
						this->fastSmearingPacketEnable = fastSmearingPacketEnable;
					}

					/**
					 * Inherited from ScienceProcessor
					 */
					virtual void setSciencePacketEnable(const idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable) {
						this->sciencePacketEnable = sciencePacketEnable;
					}

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
				};
			}
		}
	}
}

using plato::das::l0app::science::ScienceProcessorImpl;

#endif /* L0APPSCIENCEPROCESSORIMPL_HPP */
