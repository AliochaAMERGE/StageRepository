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

 L0AppScienceProcessorImpl.cpp

 $Rev: 0000 $

 ************************************************************************/

#include <plato/das/l0app/science/L0AppScienceProcessorImpl.hpp>
#include <plato/das/idb/common/enumerations/pusEnumerations/IdbServiceSubtypeScienceObservationEnum.hpp>

ScienceProcessorImpl::ScienceProcessorImpl() {
	//--- reset pointer members
	cameraDataSet = 0;
	cameraConfiguration = 0;
	scienceTmFifo = 0;
	//---
	//--- set 0 to members
	cameraId = cameraIdentifier::NCAM_A; //< 0
	fastCadenceCounter = 0;
	longCadenceCounter = 0;
	fluxOutlierPacketEnable = idbEnabledType::DISABLED;
	fastSmearingPacketEnable = idbEnabledType::DISABLED;
	sciencePacketEnable	= idbEnabledType::DISABLED;
	//---
	dataPool = 0;
}

void ScienceProcessorImpl::initialize(cameraIdentifier::CameraIdentifierEnum cameraId, uint8_t processId) {

	//-- prepare offset components
	offsetPacketBuilder.init(cameraDataSet, scienceTmFifo, processId);
	//--

	//-- prepare smearing components
	smearingCalculator.init(cameraDataSet, cameraId);
	smearingPacketBuilder.init(cameraDataSet, scienceTmFifo, processId);
	//--

	//-- prepare background components
	backgroundCalculator.init(cameraDataSet);
	backgroundPacketBuilder.init(cameraDataSet, scienceTmFifo, processId);
	//--

	//-- prepare flux and COB components
	fluxCalculator.init(cameraDataSet);
	fluxAverager.init(cameraDataSet);
	outlierPacketBuilder.init(cameraDataSet, scienceTmFifo, processId);
	sciencePacketBuilder.init(cameraDataSet, scienceTmFifo, processId);
	//--

	//-- prepare stars positions calculators components
	starsPositionsCalculator.init(cameraDataSet, cameraId, dataPool);
	starsPositionsCalculator.setCameraConfiguration(cameraConfiguration);
}

void ScienceProcessorImpl::prepareScienceProcessing() {

	//-- reset fast and long cadence counters
	fastCadenceCounter = 1;
	longCadenceCounter = 1;
	//--

	for(uint32_t i = 0U; i < ccdFeatures::CCD_COUNT; i++) {
		acquisitionTimestamps[i].reset();
	}

	//-- prepare offset components
	cameraDataSet->getOffsetCalculator(idbCcdSideType::LEFT).reset();
	cameraDataSet->getOffsetCalculator(idbCcdSideType::RIGHT).reset();

	offsetPacketBuilder.setAcquisitionParameters(cameraConfiguration->getConfigurationId());
	//--

	//-- prepare smearing components
	uint32_t smearingRowsNumber = 0U;
	if (cameraConfiguration->getVEnd() >= CCD_VISIBLE_ROW_NUMBER) {
		smearingRowsNumber = (cameraConfiguration->getVEnd() + 1) - CCD_VISIBLE_ROW_NUMBER;
	}
	smearingCalculator.setSmearingRowsNumber(smearingRowsNumber);

	for (uint32_t i = 0U; i < ccdFeatures::CCD_COUNT; i++) {
		// NDPU-SRS-6965 : when the observation service starts, compute the smearing coefficients for each of the 8 half-CCD
		cameraDataSet->getCcdDataSet(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(i)).getSmearingGlobalParameterSet(idbCcdSideType::LEFT).calculateCoefficients();
		cameraDataSet->getCcdDataSet(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(i)).getSmearingGlobalParameterSet(idbCcdSideType::RIGHT).calculateCoefficients();
	}
	//--

	//-- prepare background components
	backgroundPacketBuilder.setAcquisitionParameters(cameraConfiguration->getConfigurationId());
	//--

	//-- prepare flux and COB components
	sciencePacketBuilder.setAcquisitionParameters(cameraConfiguration->getConfigurationId());
	fluxAverager.initializeAveragingStacks();
	//--
}

void ScienceProcessorImpl::executeScienceProcessing(const uint32_t observationCycleCounter, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId,
		const GsbTime& currentTimestamp) {

	//--  update fast and long cadence counters
	if (observationCycleCounter < fastCadenceCounter) {
		// when observation cycle counter is reset, increment long cadence counter
		longCadenceCounter++;
	}
	if (observationCycleCounter != fastCadenceCounter) {
		// when observation cycle counter change, keep fast cadence counter synchronized
		fastCadenceCounter = observationCycleCounter;
	}
	//--

	acquisitionTimestamps[ccdId].pushAcquisitionTime(currentTimestamp);

	//-- call science algorithms and associated TM packet productions
	// NDPU-SRS-7832 : step 4 of the observation service processing
	processOffset(ccdId, currentTimestamp);
	// NDPU-SRS-7832 : step 5 and 6 of the observation service processing
	processSmearing(ccdId, currentTimestamp);
	// NDPU-SRS-7832 : step 7 of the observation service processing
	processBackground(ccdId, currentTimestamp);
	// NDPU-SRS-7832 : step 8 to 13 of the observation service processing
	processFlux(ccdId, currentTimestamp);
	//--
}

void ScienceProcessorImpl::calculateOffset(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const idbCcdSideType::IdbCcdSideTypeEnum ccdSide) {

	if (cameraConfiguration->getCcdSideActivation(ccdId, ccdSide) == true) {
		OffsetDescriptorSet& offsetDescriptorSet = cameraDataSet->getCcdDataSet(ccdId).getOffsetDescriptorSet(ccdSide);
		OffsetCalculator& offsetCalculator = cameraDataSet->getOffsetCalculator(ccdSide);

		// NDPU-SRS-6959 : run offset mean and variance computation algorithm
		offsetCalculator.calculateOffset(offsetDescriptorSet.getOffsetOutlierEnable(), offsetDescriptorSet.getOffsetExcludedNumber());
		// store offset algorithm algorithm
		offsetDescriptorSet.setMean(offsetCalculator.getOffsetMean());
		offsetDescriptorSet.setVariance(offsetCalculator.getOffsetVariance());
	}
}

void ScienceProcessorImpl::processOffset(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp) {

	// NDPU-SRS-7669 : compute offset values of each of the two half-CCDs after the CCD is readout
	calculateOffset(ccdId, idbCcdSideType::LEFT);
	calculateOffset(ccdId, idbCcdSideType::RIGHT);

	if (sciencePacketEnable == idbEnabledType::ENABLED) {
		// NDPU-SRS-7670 : produce TM science offset packet containing offset results
		offsetPacketBuilder.buildOffsetPacket(ccdId, currentTimestamp);
	}
}

void ScienceProcessorImpl::processSmearing(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp) {

	// NDPU-SRS-7050 : compute smearing values of each of the two half-CCDs after the CCD is readout
	// NDPU-SRS-8210 : skip the smearing computation if the treatment of the half-CCD is disabled
	if (cameraConfiguration->getCcdSideActivation(ccdId, idbCcdSideType::LEFT) == true) {
		smearingCalculator.calculateSmearing(ccdId, idbCcdSideType::LEFT, fastCadenceCounter, longCadenceCounter);
	}
	if (cameraConfiguration->getCcdSideActivation(ccdId, idbCcdSideType::RIGHT) == true) {
		smearingCalculator.calculateSmearing(ccdId, idbCcdSideType::RIGHT, fastCadenceCounter, longCadenceCounter);
	}

	if (sciencePacketEnable == idbEnabledType::ENABLED) {
		if (fastSmearingPacketEnable == idbEnabledType::ENABLED) {
			// NDPU-SRS-9065 : produce TM fast smearing debug packet
			smearingPacketBuilder.buildFastSmearingDebugPacket(ccdId, currentTimestamp);
		}

		// NDPU-SRS-7652 : compute and send the average of the last long cadence sample rate smearing data
		if (fastCadenceCounter == cameraConfiguration->getLongCadenceSampleRate()) {
			// NDPU-SRS-7654 : produce TM smearing packet containing smearing results
			smearingPacketBuilder.buildSmearingPacket(ccdId, currentTimestamp, cameraConfiguration->getLongCadenceSampleRate());
		}
	}
}

void ScienceProcessorImpl::processBackground(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp) {

	// NDPU-SRS-6920 : compute background mean and variance computation algorithm
	backgroundCalculator.calculateBackground(ccdId, fastCadenceCounter, longCadenceCounter);

	if (sciencePacketEnable == idbEnabledType::ENABLED) {
		// NDPU-SRS-7673 : produce TM science background packet containing background results
		backgroundPacketBuilder.buildBackgroundPacket(ccdId, currentTimestamp);
	}
}

void ScienceProcessorImpl::processFlux(const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const GsbTime& currentTimestamp) {

	// NDPU-SRS-7827 : perform some activities of the observation service every exposures
	fluxCalculator.calculateFlux(ccdId);

	bool endOfShortCadence = false;
	bool endOfLongCadence = false;

	// NDPU-SRS-7756, NDPU-SRS-7869, NDPU-SRS-7870 : compute short and long cadence flux and COB averaging and outlier detection
	fluxAverager.averageFlux(ccdId, fastCadenceCounter, longCadenceCounter, cameraConfiguration->getLongCadenceSampleRate(), endOfShortCadence, endOfLongCadence);

	if (sciencePacketEnable == idbEnabledType::ENABLED) {
		// NDPU-SRS-7720, NDPU-SRS-8483, NDPU-SRS-8486, NDPU-SRS-7723 : transmit the scientific product of all stars with a fast cadence processing type
		sciencePacketBuilder.buildAllFastSciencePackets(ccdId, currentTimestamp);

		if (endOfShortCadence == true) {
			GsbTime timestamp = acquisitionTimestamps[ccdId].getShortAcquisitionTime(cameraDataSet->getCommonScienceParameterSet().getTargetOutlierPointNumber());
			// NDPU-SRS-7869 : step 12 of the observation service processing, transmit the scientific product of all stars with a short cadence processing type
			sciencePacketBuilder.buildAllShortSciencePackets(ccdId, timestamp);
		}
		if (endOfLongCadence == true) {
			GsbTime timestamp = acquisitionTimestamps[ccdId].getLongAcquisitionTime(cameraConfiguration->getLongCadenceSampleRate(),
					cameraDataSet->getCommonScienceParameterSet().getTargetOutlierPointNumber());
			// NDPU-SRS-7870 : step 13 of the observation service processing, transmit the scientific product of all stars with a long cadence processing type
			sciencePacketBuilder.buildAllLongSciencePackets(ccdId, timestamp);
		}
		if (fluxOutlierPacketEnable == idbEnabledType::ENABLED) {
			// NDPU-SRS-7812 : the DOWNLOAD_OUTL_FX is ENABLED, send the outlier information in packets
			GsbTime timestamp = acquisitionTimestamps[ccdId].getOutlierTime(cameraDataSet->getCommonScienceParameterSet().getTargetOutlierPointNumber());
			outlierPacketBuilder.buildOutlierPackets(ccdId, timestamp);
		}
	}
}

void ScienceProcessorImpl::calculateStarsPositions(){

	// Reset window programming status for all CCDs in camera data set
	cameraDataSet->getCcdDataSet(idbCcdNumberType::CCD1).setWindowingListProgrammingStatus(idbWlistStatType::NON_PROGRAMMED);
	cameraDataSet->getCcdDataSet(idbCcdNumberType::CCD2).setWindowingListProgrammingStatus(idbWlistStatType::NON_PROGRAMMED);
	cameraDataSet->getCcdDataSet(idbCcdNumberType::CCD3).setWindowingListProgrammingStatus(idbWlistStatType::NON_PROGRAMMED);
	cameraDataSet->getCcdDataSet(idbCcdNumberType::CCD4).setWindowingListProgrammingStatus(idbWlistStatType::NON_PROGRAMMED);

	// Calculate the positions of the stars in the CCD
	starsPositionsCalculator.calculateStarsPositions();
}