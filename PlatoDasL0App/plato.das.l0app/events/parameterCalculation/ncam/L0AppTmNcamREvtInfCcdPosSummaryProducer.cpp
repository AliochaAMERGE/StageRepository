/*************************************************************************

 Copyright (c) 2022 by CNRS/LESIA

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

 L0AppTmNcamREvtInfCcdPosSummaryProducer.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/idb/processes/ncam/parameters/IdbNcamItemIdEnum.hpp>
#include <plato/das/l0app/events/parameterCalculation/ncam/L0AppTmNcamREvtInfCcdPosSummaryProducer.hpp>

template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
void TmNcamREvtInfCcdPosSummaryProducer<cameraId>::processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters) {
	//--- 1 - retrieve the data pool of the packet feeder
	if (this->packetProducer.getDataPool() != 0U) {
		GsbDataPoolTemplate<GsbLongItemMetaData>* dataPool = this->packetProducer.getDataPool();
		//--- 2 - update informations stored in the data pool
		// NDPU-SRS-7766
		// set NC{#cun}_TIMESTAMP to the time that was used for the star position computation
		dataPool->setItemValue<CucTime>(idbNcamItemId::NCAM_TIMESTAMP, CucTime(parameters.getTimestamp()));
		// set NC{#cun}_NUM_STAR_POS_COMP to the number of processed stars
		dataPool->setItemValue<uint32_t>(idbNcamItemId::NCAM_NUM_STAR_POS_COMP, parameters.getNumStarPosComp());
		// set NC{#cun}_STAR_NUMBER_CCD{#ccd}_{#side} to the number of stars that the service has positioned on the side {#side} of CCD number {#ccd}
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD1_RIGHT, parameters.getStarNumCcd1Right());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD1_LEFT, parameters.getStarNumCcd1Left());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD2_RIGHT, parameters.getStarNumCcd2Right());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD2_LEFT, parameters.getStarNumCcd2Left());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD3_RIGHT, parameters.getStarNumCcd3Right());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD3_LEFT, parameters.getStarNumCcd3Left());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD4_RIGHT, parameters.getStarNumCcd4Right());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_STAR_NUM_CCD4_LEFT, parameters.getStarNumCcd4Left());
	}
	//--- 3 - call the event manager
	this->callEventManager();
}