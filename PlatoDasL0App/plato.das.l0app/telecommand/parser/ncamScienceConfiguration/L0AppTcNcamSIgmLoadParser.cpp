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

 L0AppTcNcamSIgmLoadParser.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/idb/processes/ncam/parameters/IdbNcamItemIdEnum.hpp>
#include <plato/das/idb/util/identifier/IdbIdentifierProvider.hpp>
#include <plato/das/l0app/telecommand/parser/ncamScienceConfiguration/L0AppTcNcamSIgmLoadParser.hpp>

TcNcamSIgmLoadParser::TcNcamSIgmLoadParser() {
	// Initialize the pointer to null
	nCamADataPool = 0;
	nCamBDataPool = 0;
	// Initialize the pointer to null
	nCamACameraDataSet = 0;
	nCamBCameraDataSet = 0;
	cameraDataSet = 0;
	// Once enabled, the TC parser start a transaction on the datapool during TC parsing
	enableDataPoolTransaction();
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSIgmLoadParser::execute(uint32_t& bytePosition, uint32_t& receivedValue,
		GsbAuxiliaryParameters& executionStatus) {
	GsbParameterCheckErrorCode::GsbParameterCheckErrorCodeEnum error;
	// get process id from the received TC packet
	uint8_t processId = packet->getCcsdsPacketHeader()->getProcessId(error);
	// select corresponding data pool
	if (processId == IdentifierProvider::getNcamaPid()) {
		dataPool = nCamADataPool;
		cameraDataSet = nCamACameraDataSet;
	}
	else if (processId == IdentifierProvider::getNcambPid()) {
		dataPool = nCamBDataPool;
		cameraDataSet = nCamBCameraDataSet;
	}
	else {
		dataPool = 0;
		cameraDataSet = 0;
	}
	// call base class method to update datapool items values with TC parameters
	return GsbMetaDefinedPacketParserBase<GsbEncapsulatedCcsdsTcPacketInst>::execute(bytePosition, receivedValue, executionStatus);
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNcamSIgmLoadParser::processEndOfPacket(GsbAuxiliaryParameters& executionStatus) {

	gsbManagerErrorCode::GsbManagerErrorCodeEnum errorCode = gsbManagerErrorCode::ERROR_DETECTED;
	// If the dataPool and cameraDataSet have been set
	if ((dataPool != 0) && (cameraDataSet != 0)) {
		IgmGlobalParameterSet imageGeometryModel;
		// NDPU-SRS-6514 : load IGM TC parameters into camera
		// Get the image geometry model parameters values
		imageGeometryModel.setDistorsionK1(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_K1));
		imageGeometryModel.setDistorsionK2(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_K2));
		imageGeometryModel.setDistorsionK3(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_K3));
		imageGeometryModel.setDistorsionP1(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_P1));
		imageGeometryModel.setDistorsionP2(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_P2));
		// Get the image geometry model parameters values
		imageGeometryModel.setDistorsionQ1(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_Q1));
		imageGeometryModel.setDistorsionQ2(dataPool->getItemValue<float>(idbNcamItemId::NCAM_DISTORSION_Q2));
		imageGeometryModel.setFocalDistance(dataPool->getItemValue<float>(idbNcamItemId::NCAM_FOCAL_DISTANCE));

		// Set the image geometry model CCD parameters values
		loadIgmGlobalCCDParameterSet(imageGeometryModel);
		cameraDataSet->setImageGeometryModel(imageGeometryModel);
		errorCode = gsbManagerErrorCode::SUCCESS;
	}
	return errorCode;
}

void TcNcamSIgmLoadParser::loadIgmGlobalCCDParameterSet(IgmGlobalParameterSet& imageGeometryModel) {
	// If dataPool is attached
	if (dataPool != 0) {
		// Get the image geometry model CCD1 parameters values
		imageGeometryModel.setCcd1OffsetX(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD1_OFFSET_X));
		imageGeometryModel.setCcd1OffsetY(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD1_OFFSET_Y));
		imageGeometryModel.setCcd1Rotation(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD1_ROTATION));
		// Get the image geometry model CCD2 parameters values
		imageGeometryModel.setCcd2OffsetX(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD2_OFFSET_X));
		imageGeometryModel.setCcd2OffsetY(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD2_OFFSET_Y));
		imageGeometryModel.setCcd2Rotation(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD2_ROTATION));
		// Get the image geometry model CCD3 parameters values
		imageGeometryModel.setCcd3OffsetX(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD3_OFFSET_X));
		imageGeometryModel.setCcd3OffsetY(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD3_OFFSET_Y));
		imageGeometryModel.setCcd3Rotation(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD3_ROTATION));
		// Get the image geometry model CCD4 parameters values
		imageGeometryModel.setCcd4OffsetX(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD4_OFFSET_X));
		imageGeometryModel.setCcd4OffsetY(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD4_OFFSET_Y));
		imageGeometryModel.setCcd4Rotation(dataPool->getItemValue<float>(idbNcamItemId::NCAM_CCD4_ROTATION));
	}
}
