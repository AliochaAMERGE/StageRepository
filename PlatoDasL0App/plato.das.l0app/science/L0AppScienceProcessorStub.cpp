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

 L0AppScienceProcessorStub.cpp

 $Rev: 0000 $

 ************************************************************************/

#include <plato/das/l0app/science/L0AppScienceProcessorStub.hpp>

ScienceProcessorStub::ScienceProcessorStub() {
	// set pointer to 0
	scienceProcessor = 0;
}


void ScienceProcessorStub::processMessage(GscTaskMessage* message) {
	//--- if scienceProcessor define, forward it the message to the correct method
	if (scienceProcessor != 0) {
		switch (message->id) {
			case SET_FLUX_OUTLIER_PACKET_ENABLE: //--- retrieve parameters in the message
				scienceProcessor->setFluxOutlierPacketEnable(static_cast<idbEnabledType::IdbEnabledTypeEnum>(message->param[0]));
				break;
			case SET_FAST_SMEARING_PACKET_ENABLE: //--- retrieve parameters in the message
				scienceProcessor->setFastSmearingPacketEnable(static_cast<idbEnabledType::IdbEnabledTypeEnum>(message->param[0]));
				break;
			case SET_SCIENCE_PACKET_ENABLE: //--- retrieve parameters in the message
				scienceProcessor->setSciencePacketEnable(static_cast<idbEnabledType::IdbEnabledTypeEnum>(message->param[0]));
				break;
			case INITIALIZE: //--- retrieve parameters in the message
				scienceProcessor->initialize(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]), message->param[1]);
				break;
			case PREPARE_SCIENCE_PROCESSING: //--- no parameter
				scienceProcessor->prepareScienceProcessing();
				break;
			case EXECUTE_SCIENCE_PROCESSING: //--- retrieve parameters in the message
				scienceProcessor->executeScienceProcessing(message->param[0], static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]),
						GsbTime(message->param[2], message->param[3]));
				break;
			case CALCULATE_STARS_POSITIONS: //--- no parameter
				scienceProcessor->calculateStarsPositions();
				break;
			default: //--- useless default
				break;
		}
	}
}

idbEnabledType::IdbEnabledTypeEnum ScienceProcessorStub::getSciencePacketEnable() const {

	idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable = idbEnabledType::ENABLED;
	if (scienceProcessor != 0) {
		// access directly to the status of the science packet production and transmission
		sciencePacketEnable = scienceProcessor->getSciencePacketEnable();
	}
	return sciencePacketEnable;
}

void ScienceProcessorStub::setFluxOutlierPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fluxOutlierPacketEnable) {
	//--- prepare the message
	GscTaskMessage message;
	message.id = SET_FLUX_OUTLIER_PACKET_ENABLE;
	message.param[0] = static_cast<uint32_t>(fluxOutlierPacketEnable);
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::setFastSmearingPacketEnable(const idbEnabledType::IdbEnabledTypeEnum fastSmearingPacketEnable) {
	//--- prepare the message
	GscTaskMessage message;
	message.id = SET_FAST_SMEARING_PACKET_ENABLE;
	message.param[0] = static_cast<uint32_t>(fastSmearingPacketEnable);
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::setSciencePacketEnable(const idbEnabledType::IdbEnabledTypeEnum sciencePacketEnable) {
	//--- prepare the message
	GscTaskMessage message;
	message.id = SET_SCIENCE_PACKET_ENABLE;
	message.param[0] = static_cast<uint32_t>(sciencePacketEnable);
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::initialize(const cameraIdentifier::CameraIdentifierEnum cameraId, const uint8_t processId) {
	//--- prepare the message
	GscTaskMessage message;
	message.id = INITIALIZE;
	message.param[0] = static_cast<uint32_t>(cameraId);
	message.param[1] = processId;
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::prepareScienceProcessing() {
	//--- prepare the message
	GscTaskMessage message;
	message.id = PREPARE_SCIENCE_PROCESSING;
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::executeScienceProcessing(const uint32_t observationCycleCounter, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId,
		const GsbTime& currentTimestamp) {
	//--- prepare the message
	GscTaskMessage message;
	message.id = EXECUTE_SCIENCE_PROCESSING;
	message.param[0] = observationCycleCounter;
	message.param[1] = static_cast<uint32_t>(ccdId);
	message.param[2] = currentTimestamp.getCoarseTime();
	message.param[3] = currentTimestamp.getFineTime();
	//--- send the message
	sendMessage(&message);
}

void ScienceProcessorStub::calculateStarsPositions(){
	//--- prepare the message
	GscTaskMessage message;
	message.id = CALCULATE_STARS_POSITIONS;
	//--- send the message
	sendMessage(&message);
}