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

 L0AppTcNcamSSciAttitudeLoadParser.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/telecommand/parser/NcamParameterCalculation/L0AppTcNcamSSciAttitudeLoadParser.hpp>
#include <plato/das/idb/util/identifier/IdbIdentifierProvider.hpp>
#include <plato/das/l0app/telecommand/parser/NcamParameterCalculation/L0AppTcNcamSSciAttitudeLoadParser.hpp>

TcNcamSSciAttitudeLoadParser::TcNcamSSciAttitudeLoadParser() {
	// Initialize the pointers to null
	ncamaDataPool = 0;
	ncambDataPool = 0;
	// Initialize the pointers to null
	cameraDataSet = 0;
	ncamaCameraDataSet = 0;
	ncambCameraDataSet = 0;
	// set the other parameters
	currentIndex = 0;
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSSciAttitudeLoadParser::execute(uint32_t& bytePosition, uint32_t& receivedValue,
		GsbAuxiliaryParameters& executionStatus) {
	GsbParameterCheckErrorCode::GsbParameterCheckErrorCodeEnum error;
	// get process id from the received TC packet
	uint8_t processId = packet->getCcsdsPacketHeader()->getProcessId(error);
	// select corresponding data pool
	if (processId == IdentifierProvider::getNcamaPid()) {
		// Select the camA elements
		dataPool = ncamaDataPool;
		cameraDataSet = ncamaCameraDataSet;
	}
	else if (processId == IdentifierProvider::getNcambPid()) {
		// Select the camB elements
		dataPool = ncambDataPool;
		cameraDataSet = ncambCameraDataSet;
	}
	else {
		// Invalid Pid
		dataPool = 0;
		cameraDataSet = 0;
	}

	// call base class method to update datapool items values with TC parameters
	// NDPU-SRS-6986
	return GsbMetaDefinedPacketParserBase<GsbEncapsulatedCcsdsTcPacketInst>::execute(bytePosition, receivedValue, executionStatus);
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSSciAttitudeLoadParser::processStartOfLoop(uint32_t level, uint32_t structureId,
		GsbAuxiliaryParameters& executionStatus) {
	gsbManagerErrorCode::GsbManagerErrorCodeEnum managerError;

	// checks that the loop has only one level
	if (level == 0) {
		// Select the start of this loop
		currentIndex = 0;
		managerError = gsbManagerErrorCode::SUCCESS;
	}
	// no others loops should be detected in this TC
	else {
		managerError = gsbManagerErrorCode::INCONSISTENT_PARAMETER;
	}
	return managerError;
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSSciAttitudeLoadParser::processEndOfLoopStructure(uint32_t level,
		uint32_t structureIndex, uint32_t structureId, GsbAuxiliaryParameters& executionStatus) {
	// default output error code is ERROR_DETECTED
	gsbManagerErrorCode::GsbManagerErrorCodeEnum managerError = gsbManagerErrorCode::ERROR_DETECTED;

	// checks pointers needed
	if ((dataPool != 0) && (cameraDataSet != 0)) {
		cameraDataSet->getAttitudeMatrix().setAttitude(currentIndex, dataPool->getItemValue<float>(idbNcamItemId::NCAM_ATTITUDE_MATRIX));

		// next loop will load the next coefficient
		currentIndex++;

		managerError = gsbManagerErrorCode::SUCCESS;
	}

	return managerError;
}
