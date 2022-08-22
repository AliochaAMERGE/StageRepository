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

 TcNcamSReqSciAttiDumpParser.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/L0app/telecommand/parser/ncamParameterCalculation/L0AppTcNcamSReqSciAttiDumpParser.hpp>
#include <plato/das/idb/common/enumerations/pusEnumerations/IdbSourceIdTypeEnum.hpp>
#include <plato/das/idb/util/identifier/IdbIdentifierProvider.hpp>

TcNcamSReqSciAttiDumpParser::TcNcamSReqSciAttiDumpParser() {
	// initialize the pointers to 0
	ncamaCameraDataSet = 0;
	ncambCameraDataSet = 0;

	if (tmAttiPacket.getPacketData() != 0) {
		// configure SpaceWire header of the packet
		tmAttiPacket.getPacketData()->getHeader().setDla(idbDla::ICU_NA_HP);
		tmAttiPacket.getPacketData()->getHeader().setProtocolIdentifier(gsbSpacewireProtocol::CCSDS_ID);

		// set pointer on CCSDS stage of packet
		ccsdsTmPacket = tmAttiPacket.getPacketData()->getPacketData();
		// initialize CCSDS stage of the packet (GsbCcsdsPacket)
		ccsdsTmPacket->init();
		// set packet category on CCSDS header of the packet
		ccsdsTmPacket->getCcsdsPacketHeader().setPacketCategory(idbPacketCategoryType::SD);
		// set service type on data field header of the packet
		ccsdsTmPacket->getDataFieldHeader().setServiceType(idbServiceType::PARAMETER_CALC);
		// set service subtype on data field header of the packet
		ccsdsTmPacket->getDataFieldHeader().setServiceSubType(idbServiceSubtypeParameterCalc::SCIENCE_ATTITUDE_DUMP_REPORT);
	}
}

void TcNcamSReqSciAttiDumpParser::finalizeAndDumpPacket(uint32_t dataFieldSize, gsbAccessError::GsbAccessErrorEnum& accessError) {
	// update size used in the user data field
	ccsdsTmPacket->getUserDataField().setCurrentSize(dataFieldSize);
	// when copy of window descriptors has worked
	uint16_t sequenceCount = 0U;
	// finalize TM then forward it
	//- update time in CCSDS header (GsbTmDataFieldHeader)
	ccsdsTmPacket->getDataFieldHeader().setTime(tmWriter.getTimeProvider()->getTime());
	//- finalize CCSDS stage of the packet (GsbCcsdsPacket)
	ccsdsTmPacket->finalizeCcsds(sequenceCount);
	//- finalize packet (GsbPacket)
	tmAttiPacket.finalize();
	//- forward the pointer of the completed packet
	accessError = tmWriter.getPacketWriter()->write(&tmAttiPacket);
	//- reset data in packet
	GsbMemory::fill(&ccsdsTmPacket->getUserDataField(), 0, ccsdsTmPacket->getUserDataField().getSize());
}

uint32_t TcNcamSReqSciAttiDumpParser::buildPacketDataField(CameraDataSet* cameraDataSet, gsbAccessError::GsbAccessErrorEnum& accessError) {
	uint8_t* userDataFieldPtr = reinterpret_cast<uint8_t*>(&ccsdsTmPacket->getUserDataField());

	// Serialize the attitude parameters
	uint32_t attiParamSize = cameraDataSet->getAttitudeMatrix().serialize(userDataFieldPtr, ccsdsTmPacket->getUserDataField().getMaxDataSize(), accessError);

	return attiParamSize;
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSReqSciAttiDumpParser::processEndOfPacket(GsbAuxiliaryParameters& executionStatus) {
	gsbManagerErrorCode::GsbManagerErrorCodeEnum managerError = gsbManagerErrorCode::ERROR_DETECTED;
	// NDPU-SRS-8093
	CameraDataSet* cameraDataSet = 0;

	// Get the TC packet's process ID
	GsbParameterCheckErrorCode::GsbParameterCheckErrorCodeEnum paramError;
	uint8_t processId = packet->getCcsdsPacketHeader()->getProcessId(paramError);
	// Use it to select the correct camera data set
	if (paramError == GsbParameterCheckErrorCode::OK) {
		if (processId == IdentifierProvider::getNcamaPid()) {
			cameraDataSet = ncamaCameraDataSet;
		}
		else if (processId == IdentifierProvider::getNcambPid()) {
			cameraDataSet = ncambCameraDataSet;
		}
	}

	// Verify that all the useful elements are connected to the parser
	if ((cameraDataSet != 0) && (ccsdsTmPacket != 0) && (tmWriter.getPacketWriter() != 0) && (tmWriter.getTimeProvider() != 0)) {
		gsbAccessError::GsbAccessErrorEnum accessError = gsbAccessError::ACCESS_SUCCESSFUL;

		// Set process ID on CCSDS header of the packet
		ccsdsTmPacket->getCcsdsPacketHeader().setProcessId(processId);

		// Set destination ID on data field header of the packet
		// set destination ID using source ID of the received TC
		uint16_t sourceId = packet->getDataFieldHeader()->getSourceId(paramError);
		if (paramError == GsbParameterCheckErrorCode::OK) {
			ccsdsTmPacket->getDataFieldHeader().setDestinationId(sourceId);
		}
		else {
			// if an error occurs during the source ID read, write GROUND value as default destination ID
			ccsdsTmPacket->getDataFieldHeader().setDestinationId(idbSourceIdType::GROUND);
		}

		// Build the user data field
		uint32_t dataFieldSize = buildPacketDataField(cameraDataSet, accessError);

		// If the packets is correctly built, finalize and forward it
		if (accessError == gsbAccessError::ACCESS_SUCCESSFUL) {
			finalizeAndDumpPacket(dataFieldSize, accessError);
		}

		// If all the TM have been successfully sent return success
		if (accessError == gsbAccessError::ACCESS_SUCCESSFUL) {
			managerError = gsbManagerErrorCode::SUCCESS;
		}
	}
	return managerError;
}
