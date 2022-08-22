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

 L0AppTmNcamEvtWarParameterCalculationProducer.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/idb/processes/ncam/parameters/IdbNcamItemIdEnum.hpp>
#include <plato/das/l0app/events/parameterCalculation/ncam/L0AppTmNcamEvtWarParameterCalculationProducer.hpp>

template <typename cameraIdentifier::CameraIdentifierEnum cameraId>
void TmNcamEvtWarParameterCalculationProducer<cameraId>::processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY) {
	//--- 1 - retrieve the data pool of the packet feeder
	if (this->packetProducer.getDataPool() != 0U) {
		GsbDataPoolTemplate<GsbLongItemMetaData>* dataPool = this->packetProducer.getDataPool();
		//--- 2 - update informations stored in the data pool
		dataPool->setItemValue<idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum>(idbNcamItemId::NCAM_PCALC_WAR_CODE, pCalcWarCode);
		// NDPU-SRS-7605 - increment HK_NCAM{#cun}_PARAM_CALC_WAR_CNT
		uint32_t warningCount = dataPool->getItemValue<uint16_t>(idbNcamItemId::NCAM_PARAM_CALC_WAR_CNT) + 1U;
		dataPool->setItemValue<uint32_t>(idbNcamItemId::NCAM_PARAM_CALC_WAR_CNT, warningCount);
		// update the information related to the parameter calculation warning
		dataPool->setItemValue<uint32_t>(idbNcamItemId::NCAM_DESCR_ID_EV, descrId);
		dataPool->setItemValue<idbCcdNumberType::IdbCcdNumberTypeEnum>(idbNcamItemId::NCAM_CCD_ID, ccdId);
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_STAR_CCD_X, starCcdX);
		dataPool->setItemValue<double>(idbNcamItemId::NCAM_STAR_CCD_Y, starCcdY);
	}
	//--- 3 - call the event manager
	this->callEventManager();
}