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

 L0AppTmNcamREvtInfCcdPosDetailedProducer.cpp

 $Rev: 000 $

 ************************************************************************/
#include <plato/das/idb/processes/ncam/parameters/IdbNcamItemIdEnum.hpp>
#include <plato/das/l0app/events/parameterCalculation/ncam/L0AppTmNcamREvtInfCcdPosDetailedProducer.hpp>

template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
void TmNcamREvtInfCcdPosDetailedProducer<cameraId>::processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters) {
	//--- 1 - retrieve the data pool of the packet feeder
	if (this->packetProducer.getDataPool() != 0U) {
		GsbDataPoolTemplate<GsbLongItemMetaData>* dataPool = this->packetProducer.getDataPool();
		//--- 2 - update informations stored in the data pool
		// NDPU-SRS-7768
		// set NC{#cun}_DESCSR_ID to the star descriptor identifier
		dataPool->setItemValue<uint32_t>(idbNcamItemId::NCAM_DESCR_ID, parameters.getDescrId());
		// set NC{#cun}_TIMESTAMP to the time that was used for the star position computation
		dataPool->setItemValue<CucTime>(idbNcamItemId::NCAM_TIMESTAMP, CucTime(parameters.getTimestamp()));
		// set NC{#cun}_GCRS_ASCENSION to the star ascension in GCRS coordinates
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_GCRS_ASCENSION, parameters.getGcrsAscension());
		// set NC{#cun}_GCRS_DECLINATION to the star declination in GCRS coordinates
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_GCRS_DECLINATION, parameters.getGcrsDeclination());
		// set NC{#cun}_CAMERA_X, NC{#cun}_CAMERA_Y, NC{#cun}_CAMERA_Z to the star coordinates in the camera reference frame
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_CAMERA_X, parameters.getCameraX());
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_CAMERA_Y, parameters.getCameraY());
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_CAMERA_Z, parameters.getCameraZ());
		// set NC{#cun}_FOCAL_PLANE_X, NC{#cun}_FOCAL_PLANE_Y to the star position projection on the focal plane
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_FOCAL_PLANE_X, parameters.getFocalPlaneX());
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_FOCAL_PLANE_Y, parameters.getFocalPlaneY());
		// set NC{#cun}_CCD_ID to the identifier of the CCD where the star has been located
		dataPool->setItemValue<idbCcdNumberType::IdbCcdNumberTypeEnum>(idbNcamItemId::NCAM_CCD_ID, parameters.getCcdId());
		// set NC{#cun}_CCD_SIDE to the side of the CCD where the star has been located
		dataPool->setItemValue<idbCcdSideType::IdbCcdSideTypeEnum>(idbNcamItemId::NCAM_CCD_SIDE, parameters.getCcdSide());
		// set NC{#cun}_WINDOW_X and NC{#cun}_WINDOW_Y to the window position
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_WINDOW_X, parameters.getWindowX());
		dataPool->setItemValue<uint16_t>(idbNcamItemId::NCAM_WINDOW_Y, parameters.getWindowY());
	}
	//--- 3 - call the event manager
	this->callEventManager();
}