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

 L0AppTcNcamSReqIgmDumpParser.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/L0App/telecommand/parser/ncamScienceConfiguration/L0AppTcNcamSReqIgmDumpParser.hpp>
#include <plato/das/idb/common/enumerations/pusEnumerations/IdbSourceIdTypeEnum.hpp>
#include <plato/das/idb/util/identifier/IdbIdentifierProvider.hpp>

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSReqIgmDumpParser::processEndOfPacket(GsbAuxiliaryParameters& executionStatus) {

	gsbManagerErrorCode::GsbManagerErrorCodeEnum errorCode = gsbManagerErrorCode::ERROR_DETECTED;

	GsbParameterCheckErrorCode::GsbParameterCheckErrorCodeEnum error;
	// set the pointer testing the process id of the received TC packet
	uint8_t processId = packet->getCcsdsPacketHeader()->getProcessId(error);

	// set the destination ID using source ID of the received TC packet
	GsbParameterCheckErrorCode::GsbParameterCheckErrorCodeEnum paramError = GsbParameterCheckErrorCode::UNKNOWN_VALUE;
	uint16_t sourceId = this->packet->getDataFieldHeader()->getSourceId(paramError);
	if (paramError != GsbParameterCheckErrorCode::OK) {
		// if an error occurs during the source ID read, write GROUND value as default destination ID 
		sourceId = idbSourceIdType::GROUND;
	}

	if (tmWriter.getPacketWriter()) {

		GsbTime time;
		uint16_t sequenceCount = 0U;
		// if a timeProvider is connected, ask to him the current time for the TM
		if (tmWriter.getTimeProvider()) {
			time = tmWriter.getTimeProvider()->getTime();
		}
		// generate TM then forward it
		GsbPacket* packet = 0;
		if (processId == IdentifierProvider::getNcamaPid()) {
			// Forward to Ncam A
			ncamaProducer.setDestinationId(sourceId);
			packet = ncamaProducer.generateTm(sequenceCount, time);
		}
		else if (processId == IdentifierProvider::getNcambPid()) {
			// Forward to Ncam B
			ncambProducer.setDestinationId(sourceId);
			packet = ncambProducer.generateTm(sequenceCount, time);
		}

		if (tmWriter.getPacketWriter()->write(packet) == gsbAccessError::ACCESS_SUCCESSFUL) {
			errorCode = gsbManagerErrorCode::SUCCESS;
		}
	}

	return errorCode;
}
