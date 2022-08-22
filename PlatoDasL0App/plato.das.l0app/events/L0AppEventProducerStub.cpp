/*************************************************************************

 Copyright (c) 2017 by CNRS/LESIA

 This software is copyrighted by and is the sole property of CNRS/LESIA.
 All rights, title, ownership, or other interests in the software remain
 the property of CNRS/LESIA.
 This software may only be used in accordance with the corresponding
 license agreement. Any unauthorized use, duplication, transmission,
 distribution, or disclosure of this software is expressly forbidden.

 This Copyright notice may not be removed or modified without prior
 written consent of CNRS/LESIA.
 ²

 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 PlatoDasL0App Component

 L0AppEventProducerStub.cpp

 $Rev: 0 $

 ************************************************************************/

#include <plato/das/l0app/events/L0AppEventProducerStub.hpp>
#include <gericos/blocks/memory/GsbMemory.hpp>
EventProducerStub::EventProducerStub() {
	//--- constructor : link to eventProducerImpl is null at S/W start
	eventProducer = 0;
}

void EventProducerStub::setEventProducer(EventProducer* eventProducer) {
	//--- set the eventProducer
	this->eventProducer = eventProducer;
}

/// @metric_deviation [ct_vg, dc_calls, lc_stat] this switch voluntary exceed metrics to cover all enumeration's values
void EventProducerStub::processMessage(GscTaskMessage* message) {
	//--- responsibility : process the error messages received
	//--- the message are sorted by ID and error processing delegated to the eventProducerImpl
	if ((eventProducer != 0) && (message != 0)) {
		switch (message->id) {
			// if message = SpaceWire error
			case PROCESS_SPACEWIRE_ERROR: {
				gsbSpacewireError::GsbSpacewireErrorEnum errorType = static_cast<gsbSpacewireError::GsbSpacewireErrorEnum>(message->param[2]
						& 0xFF);
				GsbSpacewireErrorParameter params;
				params.setPaDpuEventSpwByteRcv((message->param[2] >> 8U) & 0xFFFFU);
				params.setPaDpuEventErFlag(((message->param[2] >> (3U * 8U)) & 1U) ? true : false);
				params.setPaDpuEventSpwWord1(message->param[4]);
				params.setPaDpuEventSpwWord2(message->param[5]);
				params.setPaDpuEventSpwWord3(message->param[6]);
				params.setPaDpuEventSpwWord4(message->param[7]);
				eventProducer->processSpacewireError(reinterpret_cast<GsbSpacewireManager*>(message->param[0]), message->param[1],
						errorType, message->param[3], params);
				break;
			}
				// if message = Rmap error
			case PROCESS_RMAP_ERROR: {
				gsbRmapErrorCode::GsbRmapErrorCodeEnum rmapErrorType =
						static_cast<gsbRmapErrorCode::GsbRmapErrorCodeEnum>(message->param[1]);
				uint8_t* destination = reinterpret_cast<uint8_t*>(message->param[4]);
				GsbRmapErrorParameter parameters;
				parameters.setInstruction(message->param[3]);
				parameters.setDestination(destination);
				parameters.setSize(message->param[5]);
				parameters.setRmapTransactionId(message->param[6]);
				parameters.setRetryNumber(message->param[8]);
				eventProducer->processRmapError(reinterpret_cast<GsbRmapManager*>(message->param[0]), message->param[7], rmapErrorType,
						message->param[2], parameters);
				break;
			}
				// if message = initialisation progress
			case PROCESS_INITIALISATION_PROGRESS: {
				eventProducer->processInitialisationProgress();
				break;
			}
				// if message = memory error
			case PROCESS_MEMORY_ERROR: {
				gsbMemoryErrorCode::GsbMemoryErrorCodeEnum memoryErrorType =
						static_cast<gsbMemoryErrorCode::GsbMemoryErrorCodeEnum>(message->param[2]);
				uint8_t* failureAddress = reinterpret_cast<uint8_t*>(message->param[4]);
				eventProducer->processMemoryError(message->param[0], message->param[1], memoryErrorType, message->param[3], failureAddress,
						message->param[5], message->param[6]);
				break;
			}
				// if message = circular buffer error
			case PROCESS_CIRCULAR_BUFFER_EVENT:
				eventProducer->processCircularBufferEvent(message->param[0],
						static_cast<gscCircularBufferEvent::GscCircularBufferEventEnum>(message->param[1]), message->param[2]);
				break;
			// message id = PROCESS_TIMECODE_ERROR
			case PROCESS_TIMECODE_ERROR:
				eventProducer->processTimecodeError(reinterpret_cast<GsbSpacewireManager*>(message->param[0]), message->param[1],
						static_cast<gsbTimecodeError::GsbTimecodeErrorEnum>(message->param[2]), message->param[3], message->param[4],
						message->param[5]);
				break;
			// message id = PROCESS_SPACEWIRE_LINK_ERROR
			case PROCESS_SPACEWIRE_LINK_ERROR:
				eventProducer->processSpacewireLinkError(reinterpret_cast<GsbSpacewireManager*>(message->param[0]), message->param[1],
						static_cast<gsbSpacewireLinkError::GsbSpacewireLinkErrorEnum>(message->param[2]), message->param[3]);
				break;
			// message id = PROCESS_SPACEWIRE_LINK_PROGRESS
			case PROCESS_SPACEWIRE_LINK_PROGRESS:
				eventProducer->processSpacewireLinkProgress(reinterpret_cast<GsbSpacewireManager*>(message->param[0]), message->param[1],
						static_cast<gsbSpacewireLinkProgress::GsbSpacewireLinkProgressEnum>(message->param[2]));
				break;
			// message id = PROCESS_CAMERA_DATA_ERROR
			case PROCESS_CAMERA_DATA_ERROR:
				eventProducer->processCameraDataError(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						message->param[1]);
				break;
			// message id = PROCESS_CAMERA_DATA_HEADER_WARNING
			case PROCESS_CAMERA_DATA_HEADER_WARNING: {
				CameraDataHeaderWarningParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[2]));
				parameters.setDataLength(message->param[3]);
				parameters.setDataType(message->param[4]);
				parameters.setFrameCounter(message->param[5]);
				parameters.setSequenceCounter(message->param[6]);
				parameters.setSpwDataLength(message->param[7]);
				parameters.setPreviousDataType(message->param[8]);
				parameters.setPreviousFrameCounter(message->param[9]);
				parameters.setPreviousSequenceCounter(message->param[10]);
				eventProducer->processCameraDataHeaderWarning(
						static_cast<idbNcamDataHeaderWarCodeType::IdbNcamDataHeaderWarCodeTypeEnum>(message->param[0]), message->param[1],
						parameters);
				break;
			}
			// message id = PROCESS_CAMERA_DATA_CONTENT_WARNING
			case PROCESS_CAMERA_DATA_CONTENT_WARNING: {
				CameraDataContentWarningParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[2]));
				parameters.setSequenceCounter(message->param[3]);
				parameters.setExpectedCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[4]));
				parameters.setReceivedCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[5]));
				parameters.setExpectedCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[6]));
				parameters.setReceivedCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[7]));
				parameters.setCurrentFrame(static_cast<idbFrameNumberType::IdbFrameNumberTypeEnum>(message->param[8]));
				parameters.setExpectedPixelNumber(message->param[9]);
				parameters.setReceivedPixelNumber(message->param[10]);
				eventProducer->processCameraDataContentWarning(
						static_cast<idbNcamDataContentWarCodeType::IdbNcamDataContentWarCodeTypeEnum>(message->param[0]), message->param[1],
						parameters);
				break;
			}
			// message id = PROCESS_CAMERA_DATA_UNEXPECTED_WARNING
			case PROCESS_CAMERA_DATA_UNEXPECTED_WARNING: {
				CameraDataHeaderWarningParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[1]));
				parameters.setDataLength(message->param[2]);
				parameters.setDataType(message->param[3]);
				parameters.setFrameCounter(message->param[4]);
				parameters.setSequenceCounter(message->param[5]);
				parameters.setSpwDataLength(message->param[6]);
				parameters.setPreviousDataType(message->param[7]);
				parameters.setPreviousFrameCounter(message->param[8]);
				parameters.setPreviousSequenceCounter(message->param[9]);
				eventProducer->processCameraDataUnexpectedWarning(message->param[0], parameters);
				break;
			}
			// message id = PROCESS_CAMERA_TIMECODE_PROGRESS
			case PROCESS_CAMERA_TIMECODE_PROGRESS:
				eventProducer->processCameraTimecodeProgress(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						static_cast<uint8_t>(message->param[1]), GsbTime(message->param[2], message->param[3]));
				break;
			// message id = PROCESS_CAMERA_MODE_PROGRESS
			case PROCESS_CAMERA_MODE_PROGRESS:
				eventProducer->processCameraModeProgress(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						static_cast<idbNcamModeType::IdbNcamModeTypeEnum>(message->param[1]),
						static_cast<idbNcamModeType::IdbNcamModeTypeEnum>(message->param[2]),
						static_cast<idbTransReasonType::IdbTransReasonTypeEnum>(message->param[3]));
				break;
			// message id = PROCESS_CAMERA_MODE_ERROR
			case PROCESS_CAMERA_MODE_ERROR:
				eventProducer->processCameraModeError(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						message->param[1],
						static_cast<idbNcamModeType::IdbNcamModeTypeEnum>(message->param[2]),
						static_cast<idbNcamModeType::IdbNcamModeTypeEnum>(message->param[3]),
						static_cast<idbTransReasonType::IdbTransReasonTypeEnum>(message->param[4]));
				break;
			// message id = PROCESS_EOCR_INFORMATIVE
			case PROCESS_EOCR_INFORMATIVE: {
				CameraEndOfHalfCcdReadoutParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]));
				parameters.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[2]));
				parameters.setFrameNumber(static_cast<idbFrameNumberType::IdbFrameNumberTypeEnum>(message->param[3]));
				parameters.setFrameCounter(message->param[4]);
				parameters.setReceivedPacketNumber(message->param[5]);
				parameters.setReceivedPixelNumber(message->param[6]);
				eventProducer->processEndOfHalfCcdReadoutInformative(parameters);
				break;
			}
			// message id = PROCESS_EOCR_ERROR
			case PROCESS_EOCR_ERROR: {
				CameraEndOfHalfCcdReadoutParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[2]));
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[3]));
				parameters.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[4]));
				parameters.setFrameNumber(static_cast<idbFrameNumberType::IdbFrameNumberTypeEnum>(message->param[5]));
				parameters.setFrameCounter(message->param[6]);
				parameters.setReceivedPacketNumber(message->param[7]);
				parameters.setReceivedPixelNumber(message->param[8]);
				eventProducer->processEndOfHalfCcdReadoutError(static_cast<idbEocrErrCodeType::IdbEocrErrCodeTypeEnum>(message->param[0]),
						message->param[1], parameters, message->param[9], message->param[10]);
				break;
			}
			// message id = PROCESS_EOFA_INFORMATIVE
			case PROCESS_EOFA_INFORMATIVE: {
				CameraEndOfFullimageAcqParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideRequestType::IdbCcdSideRequestTypeEnum>(message->param[1]));
				parameters.setRequestedCycleNumber(message->param[2]);
				parameters.setRequestedFrame1Transmission(static_cast<bool>(message->param[3]));
				parameters.setRequestedFrame2Transmission(static_cast<bool>(message->param[4]));
				parameters.setRequestedFrame3Transmission(static_cast<bool>(message->param[5]));
				parameters.setRequestedFrame4Transmission(static_cast<bool>(message->param[6]));
				parameters.setRequestedFramesNumber(message->param[7]);
				eventProducer->processEndOfFullimageAcqInformative(parameters);
				break;
			}
			// message id = PROCESS_EOFA_ERROR
			case PROCESS_EOFA_ERROR: {
				CameraEndOfFullimageAcqParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[2]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideRequestType::IdbCcdSideRequestTypeEnum>(message->param[3]));
				parameters.setRequestedCycleNumber(message->param[4]);
				parameters.setRequestedFrame1Transmission(static_cast<bool>(message->param[5]));
				parameters.setRequestedFrame2Transmission(static_cast<bool>(message->param[6]));
				parameters.setRequestedFrame3Transmission(static_cast<bool>(message->param[7]));
				parameters.setRequestedFrame4Transmission(static_cast<bool>(message->param[8]));
				parameters.setRequestedFramesNumber(message->param[9]);
				eventProducer->processEndOfFullimageAcqError(static_cast<idbEofaErrCodeType::IdbEofaErrCodeTypeEnum>(message->param[0]),
						message->param[1], parameters, message->param[10]);
				break;
			}
			// message id = PROCESS_EOPTP_INFORMATIVE
			case PROCESS_EOPTP_INFORMATIVE: {
				CameraParallelTrapPumpingParameters parameters;
				parameters.setRequestedMode(static_cast<idbParaPumpModeType::IdbParaPumpModeTypeEnum>(message->param[1]));
				parameters.setRequestedCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[2]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[3]));
				eventProducer->processEndOfParallelTrapPumpingInformative(
						static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]), parameters);
				break;
			}
			// message id = PROCESS_EOPTP_ERROR
			case PROCESS_EOPTP_ERROR: {
				CameraParallelTrapPumpingParameters parameters;
				parameters.setRequestedMode(static_cast<idbParaPumpModeType::IdbParaPumpModeTypeEnum>(message->param[3]));
				parameters.setRequestedCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[4]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[5]));
				eventProducer->processEndOfParallelTrapPumpingError(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						static_cast<idbEoptpErrCodeType::IdbEoptpErrCodeTypeEnum>(message->param[1]),
						message->param[2], parameters);
				break;
			}
			// message id = PROCESS_EOSTP_INFORMATIVE
			case PROCESS_EOSTP_INFORMATIVE: {
				CameraEndOfSerialTrapPumpingParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setRequestedMode(static_cast<idbSerialPumpModeType::IdbSerialPumpModeTypeEnum>(message->param[1]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideRequestType::IdbCcdSideRequestTypeEnum>(message->param[2]));
				parameters.setRequestedCycleNumber(message->param[3]);
				parameters.setRequestedFrame1Transmission(static_cast<bool>(message->param[4]));
				parameters.setRequestedFrame2Transmission(static_cast<bool>(message->param[5]));
				parameters.setRequestedFrame3Transmission(static_cast<bool>(message->param[6]));
				parameters.setRequestedFrame4Transmission(static_cast<bool>(message->param[7]));
				parameters.setRequestedFramesNumber(message->param[8]);
				eventProducer->processEndOfSerialTrapPumpingInformative(parameters);
				break;
			}
			// message id = PROCESS_EOSTP_ERROR
			case PROCESS_EOSTP_ERROR: {
				CameraEndOfSerialTrapPumpingParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[2]));
				parameters.setRequestedMode(static_cast<idbSerialPumpModeType::IdbSerialPumpModeTypeEnum>(message->param[3]));
				parameters.setRequestedCcdSide(static_cast<idbCcdSideRequestType::IdbCcdSideRequestTypeEnum>(message->param[4]));
				parameters.setRequestedCycleNumber(message->param[5]);
				parameters.setRequestedFrame1Transmission(static_cast<bool>(message->param[6]));
				parameters.setRequestedFrame2Transmission(static_cast<bool>(message->param[7]));
				parameters.setRequestedFrame3Transmission(static_cast<bool>(message->param[8]));
				parameters.setRequestedFrame4Transmission(static_cast<bool>(message->param[9]));
				parameters.setRequestedFramesNumber(message->param[10]);
				eventProducer->processEndOfSerialTrapPumpingError(static_cast<idbEostpErrCodeType::IdbEostpErrCodeTypeEnum>(message->param[0]),
						message->param[1], parameters, message->param[11]);
				break;
			}
			// message id = PROCESS_EOWA_PROGRESS
			case PROCESS_EOWA_PROGRESS: {
				EndOfWindowAcquisitionProgressParameters parameters;
				parameters.setRequestedCycleNumber(message->param[1]);
				parameters.setRequestedFramesNumber(message->param[2]);
				eventProducer->processEndOfWindowAcquisitionProgress(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						parameters);
				break;
			}
			// message id = PROCESS_EOWA_ERROR
			case PROCESS_EOWA_ERROR: {
				EndOfWindowAcquisitionErrorParameters parameters;
				parameters.setErrorCode(static_cast<idbEowaErrCodeType::IdbEowaErrCodeTypeEnum>(message->param[1]));
				parameters.setErrorCounter(message->param[2]);
				parameters.setRequestedCycleNumber(message->param[3]);
				parameters.setErrorFramesNumber(message->param[4]);
				parameters.setRequestedFramesNumber(message->param[5]);
				eventProducer->processEndOfWindowAcquisitionError(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						parameters);
				break;
			}
			// message id = PROCESS_INITIALISATION_ERROR
			case PROCESS_INITIALISATION_ERROR: {
				idbInitErrCodeType::IdbInitErrCodeTypeEnum errCode =
						static_cast<idbInitErrCodeType::IdbInitErrCodeTypeEnum>(message->param[0]);
				uint32_t wrongValue = static_cast<uint32_t>(message->param[1]);
				uint32_t usedValue = static_cast<uint32_t>(message->param[2]);
				eventProducer->processInitialisationError(errCode, wrongValue, usedValue);
				break;
			}
			// message id = PROCESS_TRANSITION_MODE_PROGRESS
			case PROCESS_TRANSITION_MODE_PROGRESS: {
				idbNdpuModeType::IdbNdpuModeTypeEnum currentMode = static_cast<idbNdpuModeType::IdbNdpuModeTypeEnum>(message->param[0]);
				idbNdpuModeType::IdbNdpuModeTypeEnum previousMode = static_cast<idbNdpuModeType::IdbNdpuModeTypeEnum>(message->param[1]);
				eventProducer->processTransitionModeProgress(currentMode, previousMode);
				break;
			}
			// message id = PROCESS_TRANSITION_MODE_ERROR
			case PROCESS_TRANSITION_MODE_ERROR: {
				idbModeErrCodeType::IdbModeErrCodeTypeEnum errorCode =
						static_cast<idbModeErrCodeType::IdbModeErrCodeTypeEnum>(message->param[0]);
				idbNdpuModeType::IdbNdpuModeTypeEnum currentMode = static_cast<idbNdpuModeType::IdbNdpuModeTypeEnum>(message->param[1]);
				idbNdpuModeType::IdbNdpuModeTypeEnum expectedMode = static_cast<idbNdpuModeType::IdbNdpuModeTypeEnum>(message->param[2]);
				eventProducer->processTransitionModeError(errorCode, currentMode, expectedMode, message->param[3]);
				break;
			}
			// message id = PROCESS_CAMERA_DIAGNOSIS_IR
			case PROCESS_CAMERA_DIAGNOSIS_IR: {
				CameraFeeDiagnosisResultParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setVerifiedPixels(message->param[1]);
				parameters.setVerifiedHalfCcds(message->param[2]);
				parameters.setVerifiedWindows(message->param[3]);
				eventProducer->processFeeDiagnosisIr(parameters);
				break;
			}
			// message id = PROCESS_CAMERA_DIAGNOSIS_FR
			case PROCESS_CAMERA_DIAGNOSIS_FR: {
				CameraFeeDiagnosisResultParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setVerifiedPixels(message->param[1]);
				parameters.setVerifiedHalfCcds(message->param[2]);
				parameters.setVerifiedWindows(message->param[3]);
				eventProducer->processFeeDiagnosisFr(parameters);
				break;
			}
			// message id = PROCESS_CAMERA_DIAGNOSIS_IR_ERR
			case PROCESS_CAMERA_DIAGNOSIS_IR_ERR: {
				CameraFeeDiagnosisResultParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>((message->param[0] & 0xF)));
				idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase =
						static_cast<idbDiagErrCodeType::IdbDiagErrCodeTypeEnum>(message->param[0] >> 16);
				uint32_t errorCounter = static_cast<uint32_t>(message->param[1]);
				parameters.setVerifiedPixels(message->param[2]);
				uint32_t wrongPixels = static_cast<uint32_t>(message->param[3]);
				parameters.setVerifiedWindows(message->param[4]);
				uint32_t wrongWindows = static_cast<uint32_t>(message->param[5]);
				uint16_t otherErrors = static_cast<uint16_t>(message->param[6]);
				ErroneousPixel pixel;
				pixel.setCcdNumber(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[7]));
				pixel.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[8]));
				pixel.setColumn(static_cast<uint16_t>(message->param[9]));
				pixel.setRow(static_cast<uint16_t>(message->param[10]));
				pixel.setExpected(static_cast<uint16_t>(message->param[11]));
				pixel.setReceived(static_cast<uint16_t>(message->param[12]));
				eventProducer->processFeeDiagnosisIrError(parameters, errorCounter, phase, wrongPixels, wrongWindows, otherErrors, pixel);
				break;
			}
			// message id = PROCESS_CAMERA_DIAGNOSIS_FR_ERR
			case PROCESS_CAMERA_DIAGNOSIS_FR_ERR: {
				CameraFeeDiagnosisResultParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				uint32_t errorCounter = message->param[1];
				idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase =
						static_cast<idbDiagErrCodeType::IdbDiagErrCodeTypeEnum>(message->param[2]);
				parameters.setVerifiedPixels(message->param[3]);
				uint32_t wrongPixels = static_cast<uint32_t>(message->param[4]);
				parameters.setVerifiedHalfCcds(message->param[5]);
				uint32_t wrongHalfCcds = static_cast<uint32_t>(message->param[6]);
				parameters.setVerifiedWindows(message->param[7]);
				uint32_t wrongWindows = static_cast<uint32_t>(message->param[8]);
				uint16_t otherErrors = static_cast<uint16_t>(message->param[9]);
				eventProducer->processFeeDiagnosisFrError(parameters, errorCounter, phase, wrongPixels,
						wrongHalfCcds, wrongWindows, otherErrors);
				break;
			}
			// message id = PROCESS_TIME_UPDATE_INFORMATIVE
			case PROCESS_TIME_UPDATE_INFORMATIVE: {
				uint32_t syncTime = static_cast<uint32_t>(message->param[0]);
				eventProducer->processTimeUpdateInformative(syncTime);
				break;
			}
			// message id = PROCESS_TIME_ERROR
			case PROCESS_TIME_ERROR: {
				uint16_t errorCounter = static_cast<uint16_t>(message->param[0]);
				eventProducer->processTimeError(errorCounter);
				break;
			}
			// message id = PROCESS_TC_VERIFICATION_ERROR
			case PROCESS_TC_VERIFICATION_ERROR: {
				GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>* generator =
						reinterpret_cast<GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>*>(message->param[0]);
				uint16_t errorType = static_cast<uint16_t>(message->param[1]);
				uint32_t errorCounter =  message->param[2];
				GsbTcVerificationErrorParameter params;
				// Take care that only a subset of the structure GsbTcVerificationErrorParameter is forwarded
				params.setTcVerificationErrorStep(static_cast<gsbTcVerificationStep::GsbTcVerificationStepEnum>(message->param[3]));
				params.setPacketId(static_cast<uint16_t>(message->param[4]));
				params.setSequenceControl(static_cast<uint16_t>(message->param[5]));
				params.setFailureId(message->param[6]);
				params.setServiceType(static_cast<uint8_t>(message->param[7]));
				params.setServiceSubType(static_cast<uint8_t>(message->param[8]));
				params.setSourceId(static_cast<uint16_t>(message->param[9]));
				params.setReceivedCrc(static_cast<uint16_t>(message->param[10]));
				eventProducer->processTcVerificationError(generator, errorType, errorCounter, params);
				break;
			}
			// message id = PROCESS_MEMORY_PROTECTION_ERROR
			case PROCESS_MEMORY_PROTECTION_ERROR : {
				eventProducer->processMemoryProtectionError(message->param[0], message->param[1]);
				break;
			}
			// message id = PROCESS_WINDOW_DESCRIPTOR_ERROR
			case PROCESS_WINDOW_DESCRIPTOR_ERROR: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum errorCode =
						static_cast<windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum>(message->param[1]);
				WindowDescriptor windowDescriptor;
				windowDescriptor.setId(static_cast<uint32_t>(message->param[2]));
				windowDescriptor.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[3]));
				windowDescriptor.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[4]));
				windowDescriptor.setX(static_cast<uint16_t>(message->param[5]));
				windowDescriptor.setY(static_cast<uint16_t>(message->param[6]));
				windowDescriptor.setWidth(static_cast<uint8_t>(message->param[7]));
				windowDescriptor.setHeight(static_cast<uint8_t>(message->param[8]));
				eventProducer->processWindowDescriptorError(cameraId, errorCode, windowDescriptor);
				break;
			}
			// message id = PROCESS_NFEE_CONFIGURATION_ERROR
			case PROCESS_NFEE_CONFIGURATION_ERROR: {
				// get the ncam id from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				// forward the function call to the event producer
				eventProducer->processFeeConfigurationError(ncamId);
				break;
			}
			// message id = PROCESS_STAR_DESCRIPTOR_ERROR
			case PROCESS_STAR_DESCRIPTOR_ERROR: {
				// get the ncam id and the error code from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbStarDescErrCodeType::IdbStarDescErrCodeTypeEnum errorCode = static_cast<idbStarDescErrCodeType::IdbStarDescErrCodeTypeEnum>(message->param[1]);
				// create a star parameters and set it the parameters extracted from the message parameters
				StarDescriptorTcParameter starDescriptor;
				starDescriptor.setWindowId(message->param[2]);
				starDescriptor.setMaskIndex(message->param[3]);
				starDescriptor.setExtendedMaskIndex(message->param[4]);
				starDescriptor.setOutlierDetectionState(static_cast<outlierDetectionState::OutlierDetectionStateEnum>(message->param[5]));
				starDescriptor.setStarProcessingType(static_cast<idbStarProcessType::IdbStarProcessTypeEnum>(message->param[6]));
				// forward the function call to the event producer
				eventProducer->processStarDescriptorError(ncamId, errorCode, starDescriptor);
				break;
			}
			// message id = PROCESS_STAR_EXTENDED_DESCRIPTOR_ERROR
			case PROCESS_STAR_EXTENDED_DESCRIPTOR_ERROR: {
				// get the ncam id, the error code and the star id from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbStarExtDescErrCodeType::IdbStarExtDescErrCodeTypeEnum errorCode = static_cast<idbStarExtDescErrCodeType::IdbStarExtDescErrCodeTypeEnum>(message->param[1]);
				uint32_t descriptorId = message->param[2];
				// create a star parameters and set it the parameters extracted from the message parameters
				StarDescriptor starDescriptor;
				starDescriptor.setNominalMaskIndex(message->param[3]);
				starDescriptor.setExtendedMaskIndex(message->param[4]);
				starDescriptor.setWindowDescriptorIndex(message->param[5]);
				starDescriptor.setOutlierDetectionState(static_cast<outlierDetectionState::OutlierDetectionStateEnum>(message->param[6]));
				starDescriptor.setStarProcessingType(static_cast<idbStarProcessType::IdbStarProcessTypeEnum>(message->param[7]));
				// get the star position from the message parameters
				uint16_t x = message->param[8];
				uint16_t y = message->param[9];
				// forward the function call to the event producer
				eventProducer->processStarExtendedDescriptorError(ncamId, errorCode, descriptorId, starDescriptor, x, y);
				break;
			}
			// message id = PROCESS_IMAGETTE_DESCRIPTOR_ERROR
			case PROCESS_IMAGETTE_DESCRIPTOR_ERROR: {
				// get the ncam id, the error code and the star id and position from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbImgDescErrCodeType::IdbImgDescErrCodeTypeEnum errorCode = static_cast<idbImgDescErrCodeType::IdbImgDescErrCodeTypeEnum>(message->param[1]);
				uint32_t descriptorId = message->param[2];
				uint8_t width = static_cast<uint8_t>(message->param[3]);
				uint8_t height = static_cast<uint8_t>(message->param[4]);
				// forward the function call to the event producer
				eventProducer->processImagetteDescriptorError(ncamId, errorCode, descriptorId, width, height);
				break;
			}
			// message id = PROCESS_IMAGETTE_EXTENDED_DESCRIPTOR_ERROR
			case PROCESS_IMAGETTE_EXTENDED_DESCRIPTOR_ERROR: {
				// get the ncam id and the error code from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbImgxDescErrCodeType::IdbImgxDescErrCodeTypeEnum errorCode = static_cast<idbImgxDescErrCodeType::IdbImgxDescErrCodeTypeEnum>(message->param[1]);
				// create a star parameters and set it the parameters extracted from the message parameters
				StarDescriptorTcParameter descriptor;
				descriptor.setWindowId(message->param[2]);
				descriptor.setWindowWidth(message->param[3]);
				descriptor.setWindowHeight(message->param[4]);
				descriptor.setWindowX(message->param[5]);
				descriptor.setWindowY(message->param[6]);
				// forward the function call to the event producer
				eventProducer->processImagetteExtendedError(ncamId, errorCode, descriptor);
				break;
			}
			// message id = PROCESS_OFFSET_DESCRIPTOR_ERROR
			case PROCESS_OFFSET_DESCRIPTOR_ERROR: {
				// get the ncam id and the error code from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbOfstDescErrCodeType::IdbOfstDescErrCodeTypeEnum errorCode = static_cast<idbOfstDescErrCodeType::IdbOfstDescErrCodeTypeEnum>(message->param[1]);
				// get the offet excluded number and outlier enabled from the message parameters
				uint16_t excludedNumber = static_cast<uint16_t>(message->param[5]);
				idbEnabledType::IdbEnabledTypeEnum outlierEnable = static_cast<idbEnabledType::IdbEnabledTypeEnum>(message->param[6]);
				// create a window descriptor and set it the parameters extracted from the message parameters
				WindowDescriptor windowDescriptor;
				windowDescriptor.setId(message->param[4]);
				windowDescriptor.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[2]));
				windowDescriptor.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[3]));
				windowDescriptor.setWidth(static_cast<uint8_t>(message->param[7]));
				windowDescriptor.setHeight(static_cast<uint8_t>(message->param[8]));
				windowDescriptor.setX(static_cast<uint16_t>(message->param[9]));
				windowDescriptor.setY(static_cast<uint16_t>(message->param[10]));
				// forward the function call to the event producer
				eventProducer->processOffsetDescriptorError(ncamId, errorCode, windowDescriptor, excludedNumber, outlierEnable);
				break;
			}
			// message id = PROCESS_BACKGROUND_DESCRIPTOR_ERROR
			case PROCESS_BACKGROUND_DESCRIPTOR_ERROR: {
				// get the ncam id and the error code from the message parameters
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbBgndDescErrCodeType::IdbBgndDescErrCodeTypeEnum errorCode = static_cast<idbBgndDescErrCodeType::IdbBgndDescErrCodeTypeEnum>(message->param[1]);
				// create a window descriptor and set it the parameters extracted from the message parameters
				WindowDescriptor windowDescriptor;
				windowDescriptor.setId(message->param[4]);
				windowDescriptor.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[2]));
				windowDescriptor.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[3]));
				windowDescriptor.setWidth(static_cast<uint8_t>(message->param[5]));
				windowDescriptor.setHeight(static_cast<uint8_t>(message->param[6]));
				windowDescriptor.setX(static_cast<uint16_t>(message->param[7]));
				windowDescriptor.setY(static_cast<uint16_t>(message->param[8]));
				// forward the function call to the event producer
				eventProducer->processBackgroundDescriptorError(ncamId, errorCode, windowDescriptor);
				break;
			}
			// message id = PROCESS_STAR_CATALOG_ERROR
			case PROCESS_STAR_CATALOG_ERROR: {
				// message id = PROCESS_STAR_CATALOG_ERROR
				idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum errorCode;
				StarPropertyTcParameters starParameters;
				// Retrieve the error code
				errorCode = static_cast<idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum>(message->param[0]);
				// Retrieve the star property element
				starParameters.setId(message->param[1]);
				// Use direct memory access instead of type casting to not lose the float precision
				starParameters.setDeclination(GsbMemory::get<float>(&(message->param[2]), 0));
				starParameters.setRightAscension(GsbMemory::get<float>(&(message->param[3]), 0));
				starParameters.setMagnitude(GsbMemory::get<float>(&(message->param[4]), 0));
				starParameters.setColor(GsbMemory::get<float>(&(message->param[5]), 0));
				// forward the function call to the event producer
				eventProducer->processStarCatalogError(errorCode, starParameters);
				break;
			}
			//message id = PROCESS_MASK_LIBRARY_ERROR
			case PROCESS_MASK_LIBRARY_ERROR: {
				// forward the function call to the event producer
				eventProducer->processMaskLibraryError();
				break;
			}
			// message id = PROCESS_WINDOW_PROGRAMMING_ERROR
			case PROCESS_WINDOW_PROGRAMMING_ERROR: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum errorCode =
						static_cast<idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum>(message->param[1]);
				idbCcdNumberType::IdbCcdNumberTypeEnum ccdId = static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[2]);
				uint32_t windowId = message->param[3];
				eventProducer->processWindowProgrammingError(cameraId, errorCode, ccdId, windowId);
				break;
			}
			// message id = PROCESS_WINDOW_PROGRAMMING_WARNING
			case PROCESS_WINDOW_PROGRAMMING_WARNING: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				eventProducer->processWindowProgrammingWarning(cameraId);
				break;
			}
			// message id = PROCESS_WINDOW_PROGRAMMING_PROGRESS
			case PROCESS_WINDOW_PROGRAMMING_PROGRESS: {
				WindowProgrammingProgressParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]));
				parameters.setDpuWindowNumber(message->param[2]);
				parameters.setLookupTableSize(message->param[3]);
				parameters.setFeeWindowNumber(message->param[4]);
				parameters.setFeeWindowListAddress(message->param[5]);
				parameters.setPixelStoreSize(message->param[6]);
				eventProducer->processWindowProgrammingProgress(parameters);
				break;
			}
			// message id = PROCESS_MASK_AND_POSITION_UPDATE_ERROR
			case PROCESS_MASK_AND_POSITION_UPDATE_ERROR: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				MaskAndPositionUpdateParameters parameters;
				parameters.setErrorCode(static_cast<idbMaskPosUpdateErrCodeType::IdbMaskPosUpdateErrCodeTypeEnum>(message->param[1]));
				parameters.setStarId(message->param[2]);
				parameters.setMaskIndex(message->param[3]);
				parameters.setExtendedMaskIndex(message->param[4]);
				parameters.setWindowX(message->param[5]);
				parameters.setWindowY(message->param[6]);
				eventProducer->processMaskAndPositionUpdateError(cameraId, parameters);
				break;
			}
			// message id = PROCESS_START_MASK_AND_POSITION_UPDATE_ERROR
			case PROCESS_START_MASK_AND_POSITION_UPDATE_ERROR: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				StartMaskAndPositionUpdateParameters parameters;
				parameters.setErrorCode(static_cast<idbStartMaskPosUpdateErrCodeType::IdbStartMaskPosUpdateErrCodeTypeEnum>(message->param[1]));
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[2]));
				parameters.setUpdatedMaskNumber(message->param[3]);
				parameters.setUpdatedPositionNumber(message->param[4]);
				parameters.setUpdatedSaturedStarNumber(message->param[5]);
				eventProducer->processStartMaskAndPositionUpdateError(cameraId, parameters);
				break;
			}
			// message id = PROCESS_MASK_AND_POSITION_UPDATE_INFORMATIVE
			case PROCESS_MASK_AND_POSITION_UPDATE_INFORMATIVE: {
				cameraIdentifier::CameraIdentifierEnum cameraId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				StartMaskAndPositionUpdateParameters parameters;
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]));
				parameters.setUpdatedMaskNumber(message->param[2]);
				parameters.setUpdatedPositionNumber(message->param[3]);
				parameters.setUpdatedSaturedStarNumber(message->param[4]);
				GsbTime timestamp(message->param[5], message->param[6]);
				parameters.setTimestamp(timestamp);
				eventProducer->processMaskAndPositionUpdateProgress(cameraId, parameters);
				break;
			}
			// message id = PROCESS_WINDOW_GENERATION_PROGRESS
			case PROCESS_WINDOW_GENERATION_PROGRESS : {
				WindowGenerationProgressParameters parameters;
				parameters.setCameraId(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]));
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]));
				parameters.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[2]));
				parameters.setNumberGeneratedWindows(message->param[3]);
				parameters.setLastWindowId(message->param[4]);
				parameters.setLastWindowX(message->param[5]);
				parameters.setLastWindowY(message->param[6]);
				eventProducer->processWindowGenerationProgress(parameters);
				break;
			}
			// message id = PROCESS_AHBSTAT_ERROR
			case PROCESS_AHBSTAT_ERROR: {
				eventProducer->processAhbstatError(reinterpret_cast<GsdAhbstatManager*>(message->param[0]),
						static_cast<gsdAhbstatError::GsdAhbstatErrorEnum>(message->param[1]), message->param[2], message->param[3],
						message->param[4], message->param[5]);
				break;
			}
			// message id = PROCESS_DUPLICATE_ID_ERROR
			case PROCESS_DUPLICATE_ID_ERROR: {
				cameraIdentifier::CameraIdentifierEnum ncamId = static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]);
				uint32_t descriptorId = message->param[1];
				idbDescrTypeType::IdbDescrTypeTypeEnum previousType = static_cast<idbDescrTypeType::IdbDescrTypeTypeEnum>(message->param[2]);
				idbDescrTypeType::IdbDescrTypeTypeEnum requestedType = static_cast<idbDescrTypeType::IdbDescrTypeTypeEnum>(message->param[3]);
				eventProducer->processDuplicateIdError(ncamId, descriptorId, previousType, requestedType);
				break;
			}
			// message id = PROCESS_MONITORING_DEFINITION_ERROR
			case PROCESS_MONITORING_DEFINITION_ERROR: {
				uint32_t monitoringFunctionSetId = message->param[0];
				gsbMonitoringFunctionError::GsbMonitoringFunctionErrorEnum errorCode = static_cast<gsbMonitoringFunctionError::GsbMonitoringFunctionErrorEnum>(message->param[1]);
				GsbMonitoringFunctionParameters monitoringFunctionParameters;
				monitoringFunctionParameters.getCoreParameters().setId(message->param[2]);
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterCheckType(message->param[3]);
				// mix message->param[4] and message->param[5] to get the 64 bits data
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterExpectedValue((static_cast<uint64_t>(message->param[4]) << 32) | message->param[5]);
				// mix message->param[6] and message->param[7] to get the 64 bits data
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterHighLimit((static_cast<uint64_t>(message->param[6]) << 32) | message->param[7]);
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterId(message->param[8]);
				// mix message->param[9] and message->param[10] to get the 64 bits data
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterLowLimit((static_cast<uint64_t>(message->param[9]) << 32) | message->param[10]);
				// mix message->param[11] and message->param[12] to get the 64 bits data
				monitoringFunctionParameters.getCoreParameters().setMonitoredParameterMask((static_cast<uint64_t>(message->param[11]) << 32) | message->param[12]);
				// get the LSB part of message->param[13] toget the 16 bits data
				monitoringFunctionParameters.setMonitoredParameterInterval(message->param[13] & 0xFFFF);
				// get the MSB part of message->param[13] toget the 16 bits data
				monitoringFunctionParameters.setMonitoredParameterRepetitionNumber(message->param[13] >> 16);
				// mix message->param[14] and message->param[15] to get the 64 bits data
				monitoringFunctionParameters.getValidityParameters().setValidityExpectedValue((static_cast<uint64_t>(message->param[14]) << 32) | message->param[15]);
				monitoringFunctionParameters.getValidityParameters().setValidityParameterId(message->param[16]);
				// mix message->param[16] and message->param[17] to get the 64 bits data
				monitoringFunctionParameters.getValidityParameters().setValidityParameterMask((static_cast<uint64_t>(message->param[17]) << 32) | message->param[18]);
				monitoringFunctionParameters.getEventIdParameters().setExpectedEventId(message->param[19]);
				monitoringFunctionParameters.getEventIdParameters().setLowEventId(message->param[20]);
				monitoringFunctionParameters.getEventIdParameters().setHighEventId(message->param[21]);	
				eventProducer->processMonitoringDefinitionError(monitoringFunctionSetId, errorCode, monitoringFunctionParameters);
				break;
			}
			// message id = PROCESS_MONITORING_OUT_OF_LIMIT_EVENT
			case PROCESS_MONITORING_OUT_OF_LIMIT_EVENT: {
				uint32_t monitoringFunctionSetId = message->param[0];
				uint32_t eventId = message->param[1];
				GsbMonitoringFunctionCoreParameters monitoringFunctionCoreParameters;
				monitoringFunctionCoreParameters.setId(message->param[2]);
				monitoringFunctionCoreParameters.setMonitoredParameterCheckType(message->param[3]);
				monitoringFunctionCoreParameters.setMonitoredParameterExpectedValue(message->param[4]);
				monitoringFunctionCoreParameters.setMonitoredParameterHighLimit(message->param[5]);
				monitoringFunctionCoreParameters.setMonitoredParameterId(message->param[6]);
				monitoringFunctionCoreParameters.setMonitoredParameterLowLimit(message->param[7]);
				monitoringFunctionCoreParameters.setMonitoredParameterMask(message->param[8]);
				monitoringFunctionCoreParameters.setMonitoredParameterValue(message->param[9]);
				gsbMonitoringFunctionStatus::GsbMonitoringFunctionStatusEnum status = static_cast<gsbMonitoringFunctionStatus::GsbMonitoringFunctionStatusEnum>(message->param[10]);
				eventProducer->processMonitoringOutOfLimitEvent(monitoringFunctionSetId, eventId, monitoringFunctionCoreParameters, status);
				break;
			}
			// message id = PROCESS_PARAMETER_MANAGEMENT_ERROR
			case PROCESS_PARAMETER_MANAGEMENT_ERROR: {
				eventProducer->processParameterManagementError(message->param[0], static_cast<uint16_t>(message->param[1]));
				break;
			}
			// message id = PROCESS_SMEARING_WARNING
			case PROCESS_SMEARING_WARNING: {
				eventProducer->processSmearingWarning(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[1]),
						static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[2]),
						message->param[3],
						message->param[4],
						// Use direct memory access instead of type casting to not lose the float precision
						GsbMemory::get<float>(&(message->param[5]), 0));
				break;
			}
			// message id = PROCESS_OBSERVATION_START_ERROR
			case PROCESS_OBSERVATION_START_ERROR: {
				eventProducer->processObservationStartError(static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						static_cast<idbObstErrCodeType::IdbObstErrCodeTypeEnum>(message->param[1]));
				break;
			}
			case PROCESS_PARAMETER_CALCULATION_WARNING: {
				eventProducer->processParameterCalculationWarning(
					// ncamId
					static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
					// pCalcWarCode
					static_cast<idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum>(message->param[1]),
					// descrId
					message->param[2],
					// ccdId
					static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>(message->param[3]),
					// Use direct memory access instead of type casting to not lose the float precision
					// starCcdX
					GsbMemory::get<double>(&message->param[4], 0),
					// starCcdY
					GsbMemory::get<double>(&(message->param[5]), 0));
				break;
			}
			case PROCESS_CCD_POSITION_SUMMARY: {
				CcdPositionSummaryParameters parameters;
				parameters.setEvtInfId(static_cast<idbEvtInfIdNcam::IdbEvtInfIdNcamEnum>(message->param[1]));
				GsbTime timestamp(message->param[2], message->param[3]);
				parameters.setTimestamp(timestamp);
				parameters.setNumStarPosComp(static_cast<uint32_t>(message->param[4]));
				parameters.setStarNumCcd1Right(static_cast<uint16_t>(message->param[5]));
				parameters.setStarNumCcd1Left(static_cast<uint16_t>(message->param[6]));
				parameters.setStarNumCcd2Right(static_cast<uint16_t>(message->param[7]));
				parameters.setStarNumCcd2Left(static_cast<uint16_t>(message->param[8]));
				parameters.setStarNumCcd3Right(static_cast<uint16_t>(message->param[9]));
				parameters.setStarNumCcd3Left(static_cast<uint16_t>(message->param[10]));
				parameters.setStarNumCcd4Right(static_cast<uint16_t>(message->param[11]));
				parameters.setStarNumCcd4Left(static_cast<uint16_t>(message->param[12]));
				eventProducer->processCcdPositionSummary(
						// ncamId
						static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0]),
						// parameters
						parameters);
				break;
			}
			case PROCESS_CCD_POSITION_DETAILED: {
				CcdPositionDetailedParameters parameters;
				// Reconstruct the parameters
				// 8 bits elements
				parameters.setCcdId(static_cast<idbCcdNumberType::IdbCcdNumberTypeEnum>((message->param[0] >> 8) & 0xFF));
				// 16 bits elements
				parameters.setCcdSide(static_cast<idbCcdSideType::IdbCcdSideTypeEnum>(message->param[0] >> 16));
				parameters.setWindowX(static_cast<uint16_t>(message->param[1] & 0xFFFF));
				parameters.setWindowY(static_cast<uint16_t>(message->param[1] >> 16));
				// 32 bits elements
				parameters.setEvtInfId(static_cast<idbEvtInfIdNcam::IdbEvtInfIdNcamEnum>(message->param[2]));
				parameters.setDescrId(message->param[3]);
				GsbTime timestamp(message->param[4], message->param[5]);
				parameters.setTimestamp(timestamp);
				// 64 bits elements mix a couple of 32 bits message in order to get a 64 bits
				double temp = 0.0;
				// set gcrsAscension
				GsbIeee754::setHighWord(temp, message->param[6]);
				GsbIeee754::setLowWord(temp, message->param[7]);
				parameters.setGcrsAscension(temp);
				// set gcrsDeclination
				GsbIeee754::setHighWord(temp, message->param[8]);
				GsbIeee754::setLowWord(temp, message->param[9]);
				parameters.setGcrsDeclination(temp);
				// set set cameraX
				GsbIeee754::setHighWord(temp, message->param[10]);
				GsbIeee754::setLowWord(temp, message->param[11]);
				parameters.setCameraX(temp);
				// set cameraY
				GsbIeee754::setHighWord(temp, message->param[12]);
				GsbIeee754::setLowWord(temp, message->param[13]);
				parameters.setCameraY(temp);
				// set cameraZ
				GsbIeee754::setHighWord(temp, message->param[14]);
				GsbIeee754::setLowWord(temp, message->param[15]);
				parameters.setCameraZ(temp);
				// set focalPlaneX
				GsbIeee754::setHighWord(temp, message->param[16]);
				GsbIeee754::setLowWord(temp, message->param[17]);
				parameters.setFocalPlaneX(temp);
				// set focalPlaneY
				GsbIeee754::setHighWord(temp, message->param[18]);
				GsbIeee754::setLowWord(temp, message->param[19]);
				parameters.setFocalPlaneY(temp);
				eventProducer->processCcdPositionDetailed(
						// ncamId
						static_cast<cameraIdentifier::CameraIdentifierEnum>(message->param[0] & 0xFF),
						// parameters
						parameters);
				break;

			}
			default: //--- useless default
				break;
		}
	}
}

void EventProducerStub::setNicuSpacewireDeviceNumber(const uint32_t deviceNumber) {
	// direct call
	if (eventProducer != 0) {
		eventProducer->setNicuSpacewireDeviceNumber(deviceNumber);
	}
}

void EventProducerStub::setRicuSpacewireDeviceNumber(const uint32_t deviceNumber) {
	// direct call
	if (eventProducer != 0) {
		eventProducer->setRicuSpacewireDeviceNumber(deviceNumber);
	}
}
void EventProducerStub::setNcamaSpacewireDeviceNumber(const uint32_t deviceNumber) {
	// direct call
	if (eventProducer != 0) {
		eventProducer->setNcamaSpacewireDeviceNumber(deviceNumber);
	}
}

void EventProducerStub::setNcambSpacewireDeviceNumber(const uint32_t deviceNumber) {
	// direct call
	if (eventProducer != 0) {
		eventProducer->setNcambSpacewireDeviceNumber(deviceNumber);
	}
}

void EventProducerStub::processSpacewireError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireError::GsbSpacewireErrorEnum errorType, unsigned int errorCounter, const GsbSpacewireErrorParameter& params) {
	//--- responsibility : process to the SpaceWire errors
	//--- 1. prepare the message gathering SpaceWire error data
	GscTaskMessage message;
	message.id = PROCESS_SPACEWIRE_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(packetManager);
	message.param[1] = deviceNumber;
	message.param[2] = (((errorType & 0xFFU) | ((params.getPaDpuEventSpwByteRcv() & 0xFFFFU) << 8U))
			| ((params.getPaDpuEventErFlag() & 1U) << (3U * 8U)));
	message.param[3] = errorCounter;
	message.param[4] = params.getPaDpuEventSpwWord1();
	message.param[5] = params.getPaDpuEventSpwWord2();
	message.param[6] = params.getPaDpuEventSpwWord3();
	message.param[7] = params.getPaDpuEventSpwWord4();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processRmapError(GsbRmapManager* rmapManager, uint32_t deviceNumber,
		gsbRmapErrorCode::GsbRmapErrorCodeEnum errorType, unsigned short errorCounter, GsbRmapErrorParameter paramters) {
	//--- responsibility : process to the Rmap errors
	//--- 1. prepare the message gathering Rmap error data
	GscTaskMessage message;
	message.id = PROCESS_RMAP_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(rmapManager);
	message.param[1] = static_cast<uint32_t>(errorType);
	message.param[2] = errorCounter;
	message.param[3] = paramters.getInstruction();
	message.param[4] = reinterpret_cast<uint32_t>(paramters.getDestination());
	message.param[5] = paramters.getSize();
	message.param[6] = paramters.getRmapTransactionId();
	message.param[7] = deviceNumber;
	message.param[8] = paramters.getRetryNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processTimecodeError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbTimecodeError::GsbTimecodeErrorEnum errorType, unsigned int errorCounter, unsigned char currentTimecode,
		unsigned char expectedTimecode) {
	//--- 1. prepare the message gathering initialization progress data
	GscTaskMessage message;
	message.id = PROCESS_TIMECODE_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(packetManager);
	message.param[1] = deviceNumber;
	message.param[2] = errorType;
	message.param[3] = errorCounter;
	message.param[4] = currentTimecode;
	message.param[5] = expectedTimecode;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processInitialisationError(idbInitErrCodeType::IdbInitErrCodeTypeEnum errCode, uint32_t wrongValue,
		uint32_t usedValue) {
	//--- 1. prepare the message gathering initialization error data
	GscTaskMessage message;
	message.id = PROCESS_INITIALISATION_ERROR;
	message.param[0] = errCode;
	message.param[1] = wrongValue;
	message.param[2] = usedValue;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processInitialisationProgress() {
	//--- 1. prepare the message gathering initialization progress data
	GscTaskMessage message;
	message.id = PROCESS_INITIALISATION_PROGRESS;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processTimeUpdateInformative(uint32_t syncTime) {
	//--- 1. prepare the message gathering initialization progress data
	GscTaskMessage message;
	message.id = PROCESS_TIME_UPDATE_INFORMATIVE;
	message.param[0] = syncTime;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processTimeError(uint16_t errorCounter) {
	//--- 1. prepare the message gathering initialization progress data
	GscTaskMessage message;
	message.id = PROCESS_TIME_ERROR;
	message.param[0] = errorCounter;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMemoryError(uint32_t nbWrongBytes, uint16_t apid, gsbMemoryErrorCode::GsbMemoryErrorCodeEnum errorType,
		uint32_t errorCounter, uint8_t* failureAddress, uint32_t expectedValue, uint32_t readValue) {
	//--- responsibility : process memory management errors
	//--- 1. prepare the message gathering memory error data
	GscTaskMessage message;
	message.id = PROCESS_MEMORY_ERROR;
	message.param[0] = nbWrongBytes;
	message.param[1] = apid;
	message.param[2] = static_cast<uint32_t>(errorType);
	message.param[3] = errorCounter;
	message.param[4] = reinterpret_cast<uint32_t>(failureAddress);
	message.param[5] = expectedValue;
	message.param[6] = readValue;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMemoryProtectionError(uint16_t apid, uint32_t memoryId) {
	//--- responsibility : process memory protection errors
	//--- 1. prepare the message gathering memory protection error data
	GscTaskMessage message;
	message.id = PROCESS_MEMORY_PROTECTION_ERROR;
	message.param[0] = apid;
	message.param[1] = memoryId;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMonitoringDefinitionError(uint32_t monitoringFunctionSetId,
											gsbMonitoringFunctionError::GsbMonitoringFunctionErrorEnum errorCode,
											const GsbMonitoringFunctionParameters& monitoringFunctionParameters) {
	//--- responsibility : process monitoring definition errors
	//--- 1. prepare the message gathering monitoring definition error data
	GscTaskMessage message;
	message.id = PROCESS_MONITORING_DEFINITION_ERROR;
	message.param[0] = monitoringFunctionSetId;
	message.param[1] = errorCode;					// 8 bits
	message.param[2] = monitoringFunctionParameters.getCoreParameters().getId();
	message.param[3] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterCheckType();
	message.param[4] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterExpectedValue() >> 32;
	message.param[5] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterExpectedValue() & 0xFFFFFFFF ;
	message.param[6] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterHighLimit() >> 32;
	message.param[7] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterHighLimit() & 0xFFFFFFFF;
	message.param[8] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterId();
	message.param[9]  = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterLowLimit() >> 32;
	message.param[10] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterLowLimit() & 0xFFFFFFFF;
	message.param[11] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterMask() >> 32;
	message.param[12] = monitoringFunctionParameters.getCoreParameters().getMonitoredParameterMask() & 0xFFFFFFFF;
	message.param[13] = monitoringFunctionParameters.getMonitoredParameterInterval();								// 16 bits
	message.param[13] |= monitoringFunctionParameters.getMonitoredParameterRepetitionNumber()<<16;					// 16 bits
	message.param[14] = monitoringFunctionParameters.getValidityParameters().getValidityExpectedValue() >> 32;
	message.param[15] = monitoringFunctionParameters.getValidityParameters().getValidityExpectedValue() & 0xFFFFFFFF;
	message.param[16] = monitoringFunctionParameters.getValidityParameters().getValidityParameterId();
	message.param[17] = monitoringFunctionParameters.getValidityParameters().getValidityParameterMask() >> 32;
	message.param[18] = monitoringFunctionParameters.getValidityParameters().getValidityParameterMask() & 0xFFFFFFFF;
	message.param[19] = monitoringFunctionParameters.getEventIdParameters().getExpectedEventId();
	message.param[20] = monitoringFunctionParameters.getEventIdParameters().getLowEventId();
	message.param[21] = monitoringFunctionParameters.getEventIdParameters().getHighEventId();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMonitoringOutOfLimitEvent(uint32_t monitoringFunctionSetId, uint32_t eventId,
											GsbMonitoringFunctionCoreParameters monitoringFunctionCoreParameters,
											gsbMonitoringFunctionStatus::GsbMonitoringFunctionStatusEnum status) {
	//--- responsibility : process monitoring out of limit errors
	//--- 1. prepare the message gathering monitoring  out of limit error data
	GscTaskMessage message;
	message.id = PROCESS_MONITORING_OUT_OF_LIMIT_EVENT;
	message.param[0] = monitoringFunctionSetId;
	message.param[1] = eventId;
	message.param[2] = monitoringFunctionCoreParameters.getId();
	message.param[3] = monitoringFunctionCoreParameters.getMonitoredParameterCheckType();
	message.param[4] = monitoringFunctionCoreParameters.getMonitoredParameterExpectedValue();
	message.param[5] = monitoringFunctionCoreParameters.getMonitoredParameterHighLimit();
	message.param[6] = monitoringFunctionCoreParameters.getMonitoredParameterId();
	message.param[7] = monitoringFunctionCoreParameters.getMonitoredParameterLowLimit();
	message.param[8] = monitoringFunctionCoreParameters.getMonitoredParameterMask();
	message.param[9] = monitoringFunctionCoreParameters.getMonitoredParameterValue();
	message.param[10] = status;
	//--- 2. send the message
	sendMessage(&message);
}

 void EventProducerStub::processParameterManagementError(uint32_t parameterId, uint16_t apid) {
		//--- responsibility : process parameter management errors
		//--- 1. prepare the message
		GscTaskMessage message;
		message.id = PROCESS_PARAMETER_MANAGEMENT_ERROR;
		message.param[0] = parameterId;
		message.param[1] = static_cast<uint32_t>(apid);
		//--- 2. send the message
		sendMessage(&message);
 }

void EventProducerStub::processCircularBufferEvent(uint32_t bufferId, gscCircularBufferEvent::GscCircularBufferEventEnum eventCode,
		uint32_t eventCounter) {
	//--- responsibility : process to the circular buffer errors
	//--- 1. prepare the message gathering circular buffer error data
	GscTaskMessage message;
	message.id = PROCESS_CIRCULAR_BUFFER_EVENT;
	message.param[0] = bufferId;
	message.param[1] = static_cast<uint8_t>(eventCode);
	message.param[2] = eventCounter;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processSpacewireLinkError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireLinkError::GsbSpacewireLinkErrorEnum errorType, unsigned int errorCounter) {
	//--- 1. prepare the message gathering spacewire link error data
	GscTaskMessage message;
	message.id = PROCESS_SPACEWIRE_LINK_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(packetManager);
	message.param[1] = deviceNumber;
	message.param[2] = static_cast<uint32_t>(errorType);
	message.param[3] = errorCounter;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processSpacewireLinkProgress(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireLinkProgress::GsbSpacewireLinkProgressEnum eventType) {
	//--- 1. prepare the message gathering spacewire link progress data
	GscTaskMessage message;
	message.id = PROCESS_SPACEWIRE_LINK_PROGRESS;
	message.param[0] = reinterpret_cast<uint32_t>(packetManager);
	message.param[1] = deviceNumber;
	message.param[2] = static_cast<uint32_t>(eventType);
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraDataError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter) {
	//--- 1. prepare the message gathering camera data error data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_DATA_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = errorCounter;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraDataHeaderWarning(idbNcamDataHeaderWarCodeType::IdbNcamDataHeaderWarCodeTypeEnum warningCode,
		uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters) {
	//--- 1. prepare the message gathering camera data header warning data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_DATA_HEADER_WARNING;
	message.param[0] = static_cast<uint32_t>(warningCode);
	message.param[1] = warningCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[2] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[3] = parameters.getDataLength();
	message.param[4] = parameters.getDataType();
	message.param[5] = parameters.getFrameCounter();
	message.param[6] = parameters.getSequenceCounter();
	message.param[7] = parameters.getSpwDataLength();
	message.param[8] = parameters.getPreviousDataType();
	message.param[9] = parameters.getPreviousFrameCounter();
	message.param[10] = parameters.getPreviousSequenceCounter();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraDataContentWarning(idbNcamDataContentWarCodeType::IdbNcamDataContentWarCodeTypeEnum warningCode,
		uint32_t warningCounter, const CameraDataContentWarningParameters& parameters) {
	//--- 1. prepare the message gathering camera data content warning data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_DATA_CONTENT_WARNING;
	message.param[0] = static_cast<uint32_t>(warningCode);
	message.param[1] = warningCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[2] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[3] = static_cast<uint32_t>(parameters.getSequenceCounter());
	message.param[4] = static_cast<uint32_t>(parameters.getExpectedCcdId());
	message.param[5] = static_cast<uint32_t>(parameters.getReceivedCcdId());
	message.param[6] = static_cast<uint32_t>(parameters.getExpectedCcdSide());
	message.param[7] = static_cast<uint32_t>(parameters.getReceivedCcdSide());
	message.param[8] = static_cast<uint32_t>(parameters.getCurrentFrame());
	message.param[9] = parameters.getExpectedPixelNumber();
	message.param[10] = parameters.getReceivedPixelNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraDataUnexpectedWarning(uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters) {
	//--- 1. prepare the message gathering camera data header warning data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_DATA_UNEXPECTED_WARNING;
	message.param[0] = warningCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[1] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[2] = parameters.getDataLength();
	message.param[3] = parameters.getDataType();
	message.param[4] = parameters.getFrameCounter();
	message.param[5] = parameters.getSequenceCounter();
	message.param[6] = parameters.getSpwDataLength();
	message.param[7] = parameters.getPreviousDataType();
	message.param[8] = parameters.getPreviousFrameCounter();
	message.param[9] = parameters.getPreviousSequenceCounter();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraTimecodeProgress(cameraIdentifier::CameraIdentifierEnum ncamId, uint8_t timecode, GsbTime time) {
	//--- 1. prepare the message gathering camera timecode progress data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_TIMECODE_PROGRESS;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(timecode);
	message.param[2] = time.getCoarseTime();
	message.param[3] = time.getFineTime();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraModeProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbNcamModeType::IdbNcamModeTypeEnum previousMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
		idbTransReasonType::IdbTransReasonTypeEnum reason) {
	//--- 1. prepare the message gathering camera mode progress data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_MODE_PROGRESS;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(previousMode);
	message.param[2] = static_cast<uint32_t>(currentMode);
	message.param[3] = static_cast<uint32_t>(reason);
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processCameraModeError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter,
		idbNcamModeType::IdbNcamModeTypeEnum expectedMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
		idbTransReasonType::IdbTransReasonTypeEnum reason) {
	//--- 1. prepare the message gathering camera mode error data
	GscTaskMessage message;
	message.id = PROCESS_CAMERA_MODE_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = errorCounter;
	message.param[2] = static_cast<uint32_t>(expectedMode);
	message.param[3] = static_cast<uint32_t>(currentMode);
	message.param[4] = static_cast<uint32_t>(reason);
	//--- 2. send the message
	sendMessage(&message);

}

void EventProducerStub::processEndOfHalfCcdReadoutInformative(const CameraEndOfHalfCcdReadoutParameters& parameters) {
	//--- 1. prepare the message gathering end of half-CCD readout information data
	GscTaskMessage message;
	message.id = PROCESS_EOCR_INFORMATIVE;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = static_cast<uint32_t>(parameters.getCcdId());
	message.param[2] = static_cast<uint32_t>(parameters.getCcdSide());
	message.param[3] = static_cast<uint32_t>(parameters.getFrameNumber());
	message.param[4] = parameters.getFrameCounter();
	message.param[5] = parameters.getReceivedPacketNumber();
	message.param[6] = parameters.getReceivedPixelNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfHalfCcdReadoutError(idbEocrErrCodeType::IdbEocrErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfHalfCcdReadoutParameters& parameters, uint32_t expectedPacketNumber, uint32_t expectedPixelNumber) {
	//--- 1. prepare the message gathering end of half-CCD readout error data
	GscTaskMessage message;
	message.id = PROCESS_EOCR_ERROR;
	message.param[0] = static_cast<uint32_t>(errorCode);
	message.param[1] = errorCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[2] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[3] = static_cast<uint32_t>(parameters.getCcdId());
	message.param[4] = static_cast<uint32_t>(parameters.getCcdSide());
	message.param[5] = static_cast<uint32_t>(parameters.getFrameNumber());
	message.param[6] = parameters.getFrameCounter();
	message.param[7] = parameters.getReceivedPacketNumber();
	message.param[8] = parameters.getReceivedPixelNumber();
	message.param[9] = expectedPacketNumber;
	message.param[10] = expectedPixelNumber;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfFullimageAcqInformative(const CameraEndOfFullimageAcqParameters& parameters) {
	//--- 1. prepare the message gathering end of full-image acquisition information data
	GscTaskMessage message;
	message.id = PROCESS_EOFA_INFORMATIVE;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	message.param[2] = parameters.getRequestedCycleNumber();
	message.param[3] = static_cast<uint32_t>(parameters.getRequestedFrame1Transmission());
	message.param[4] = static_cast<uint32_t>(parameters.getRequestedFrame2Transmission());
	message.param[5] = static_cast<uint32_t>(parameters.getRequestedFrame3Transmission());
	message.param[6] = static_cast<uint32_t>(parameters.getRequestedFrame4Transmission());
	message.param[7] = parameters.getRequestedFramesNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfFullimageAcqError(idbEofaErrCodeType::IdbEofaErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfFullimageAcqParameters& parameters, uint32_t errorFramesNumber) {
	//--- 1. prepare the message gathering end of full-image acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_EOFA_ERROR;
	message.param[0] = static_cast<uint32_t>(errorCode);
	message.param[1] = errorCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[2] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[3] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	message.param[4] = parameters.getRequestedCycleNumber();
	message.param[5] = static_cast<uint32_t>(parameters.getRequestedFrame1Transmission());
	message.param[6] = static_cast<uint32_t>(parameters.getRequestedFrame2Transmission());
	message.param[7] = static_cast<uint32_t>(parameters.getRequestedFrame3Transmission());
	message.param[8] = static_cast<uint32_t>(parameters.getRequestedFrame4Transmission());
	message.param[9] = parameters.getRequestedFramesNumber();
	message.param[10] = errorFramesNumber;
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfParallelTrapPumpingInformative(cameraIdentifier::CameraIdentifierEnum ncamId,
		const CameraParallelTrapPumpingParameters& parameters) {
	//--- 1. prepare the message gathering end of full-image acquisition information data
	GscTaskMessage message;
	message.id = PROCESS_EOPTP_INFORMATIVE;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getRequestedMode());
	message.param[2] = static_cast<uint32_t>(parameters.getRequestedCcdId());
	message.param[3] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfParallelTrapPumpingError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbEoptpErrCodeType::IdbEoptpErrCodeTypeEnum errorCode, uint32_t errorCounter, const CameraParallelTrapPumpingParameters& parameters) {
	//--- 1. prepare the message gathering end of full-image acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_EOPTP_ERROR;
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	message.param[2] = static_cast<uint32_t>(errorCounter);
	message.param[3] = static_cast<uint32_t>(parameters.getRequestedMode());
	message.param[4] = static_cast<uint32_t>(parameters.getRequestedCcdId());
	message.param[5] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfSerialTrapPumpingInformative(const CameraEndOfSerialTrapPumpingParameters& parameters) {
	//--- 1. prepare the message gathering end of full-image acquisition information data
	GscTaskMessage message;
	message.id = PROCESS_EOSTP_INFORMATIVE;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = static_cast<uint32_t>(parameters.getRequestedMode());
	message.param[2] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	message.param[3] = parameters.getRequestedCycleNumber();
	message.param[4] = static_cast<uint32_t>(parameters.getRequestedFrame1Transmission());
	message.param[5] = static_cast<uint32_t>(parameters.getRequestedFrame2Transmission());
	message.param[6] = static_cast<uint32_t>(parameters.getRequestedFrame3Transmission());
	message.param[7] = static_cast<uint32_t>(parameters.getRequestedFrame4Transmission());
	message.param[8] = parameters.getRequestedFramesNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfSerialTrapPumpingError(idbEostpErrCodeType::IdbEostpErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfSerialTrapPumpingParameters& parameters, uint32_t requestedImagesNumber) {
	//--- 1. prepare the message gathering end of full-image acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_EOSTP_ERROR;
	message.param[0] = static_cast<uint32_t>(errorCode);
	message.param[1] = errorCounter;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[2] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[3] = static_cast<uint32_t>(parameters.getRequestedMode());
	message.param[4] = static_cast<uint32_t>(parameters.getRequestedCcdSide());
	message.param[5] = parameters.getRequestedCycleNumber();
	message.param[6] = static_cast<uint32_t>(parameters.getRequestedFrame1Transmission());
	message.param[7] = static_cast<uint32_t>(parameters.getRequestedFrame2Transmission());
	message.param[8] = static_cast<uint32_t>(parameters.getRequestedFrame3Transmission());
	message.param[9] = static_cast<uint32_t>(parameters.getRequestedFrame4Transmission());
	message.param[10] = parameters.getRequestedFramesNumber();
	message.param[11] = requestedImagesNumber;
	//--
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfWindowAcquisitionProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
								const EndOfWindowAcquisitionProgressParameters& parameters) {
	//--- 1. prepare the message gathering end of window acquisition progress data
	GscTaskMessage message;
	message.id = PROCESS_EOWA_PROGRESS;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = parameters.getRequestedCycleNumber();
	message.param[2] = parameters.getRequestedFramesNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processEndOfWindowAcquisitionError(cameraIdentifier::CameraIdentifierEnum ncamId,
								const EndOfWindowAcquisitionErrorParameters& parameters) {
	//--- 1. prepare the message gathering end of window acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_EOWA_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getErrorCode());
	message.param[2] = parameters.getErrorCounter();
	message.param[3] = parameters.getRequestedCycleNumber();
	message.param[4] = parameters.getErrorFramesNumber();
	message.param[5] = parameters.getRequestedFramesNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const MaskAndPositionUpdateParameters& parameters) {
	//--- 1. prepare the message gathering end of mask and position update error data
	GscTaskMessage message;
	message.id = PROCESS_MASK_AND_POSITION_UPDATE_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getErrorCode());
	message.param[2] = parameters.getStarId();
	message.param[3] = parameters.getMaskIndex();
	message.param[4] = parameters.getExtendedMaskIndex();
	message.param[5] = parameters.getWindowX();
	message.param[6] = parameters.getWindowY();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processStartMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const StartMaskAndPositionUpdateParameters& parameters) {
	//--- 1. prepare the message gathering end of mask and position update error data
	GscTaskMessage message;
	message.id = PROCESS_START_MASK_AND_POSITION_UPDATE_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getErrorCode());
	message.param[2] = static_cast<uint32_t>(parameters.getCcdId());
	message.param[3] = parameters.getUpdatedMaskNumber();
	message.param[4] = parameters.getUpdatedPositionNumber();
	message.param[5] = parameters.getUpdatedSaturedStarNumber();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processMaskAndPositionUpdateProgress(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const StartMaskAndPositionUpdateParameters& parameters) {
	//--- 1. prepare the message gathering end of mask and position update informative data
	GscTaskMessage message;
	message.id = PROCESS_MASK_AND_POSITION_UPDATE_INFORMATIVE;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getCcdId());
	message.param[2] = parameters.getUpdatedMaskNumber();
	message.param[3] = parameters.getUpdatedPositionNumber();
	message.param[4] = parameters.getUpdatedSaturedStarNumber();
	message.param[5] = parameters.getTimestamp().getCoarseTime();
	message.param[6] = parameters.getTimestamp().getFineTime();
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processAhbstatError(GsdAhbstatManager* ahbstatManager, gsdAhbstatError::GsdAhbstatErrorEnum errorType,
							uint32_t ahbFailingAddressCounter, uint32_t ahbStatusRegister, uint32_t ahbFailingAddress, uint32_t ahbFailingAddressValue){
	//--- 1. prepare the message gathering end of window acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_AHBSTAT_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(ahbstatManager);
	message.param[1] = static_cast<uint32_t>(errorType);
	message.param[2] = ahbFailingAddressCounter;
	message.param[3] = ahbStatusRegister;
	message.param[4] = ahbFailingAddress;
	message.param[5] = ahbFailingAddressValue;
	//---2 send the message
	sendMessage(&message);
}

void EventProducerStub::processDuplicateIdError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t descriptorId,
							idbDescrTypeType::IdbDescrTypeTypeEnum previousType, idbDescrTypeType::IdbDescrTypeTypeEnum requestedType) {
	//--- 1. prepare the message gathering end of window acquisition error data
	GscTaskMessage message;
	message.id = PROCESS_DUPLICATE_ID_ERROR;
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = descriptorId;
	message.param[2] = static_cast<uint32_t>(previousType);
	message.param[3] = static_cast<uint32_t>(requestedType);
	//---2 send the message
	sendMessage(&message);
}

void EventProducerStub::processTransitionModeProgress(idbNdpuModeType::IdbNdpuModeTypeEnum currentMode,
		idbNdpuModeType::IdbNdpuModeTypeEnum previousMode) {
	//--- 1. prepare the message gathering mode transition information data
	GscTaskMessage message;
	message.id = PROCESS_TRANSITION_MODE_PROGRESS;
	message.param[0] = static_cast<uint32_t>(currentMode);
	message.param[1] = static_cast<uint32_t>(previousMode);
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processTransitionModeError(idbModeErrCodeType::IdbModeErrCodeTypeEnum errorCode,
		idbNdpuModeType::IdbNdpuModeTypeEnum currentMode, idbNdpuModeType::IdbNdpuModeTypeEnum expectedMode,
		unsigned int errorCounter) {
	//--- 1. prepare the message gathering mode transition error data
	GscTaskMessage message;
	message.id = PROCESS_TRANSITION_MODE_ERROR;
	message.param[0] = static_cast<uint32_t>(errorCode);
	message.param[1] = static_cast<uint32_t>(currentMode);
	message.param[2] = static_cast<uint32_t>(expectedMode);
	message.param[3] = static_cast<uint32_t>(errorCounter);
	//--- 2. send the message
	sendMessage(&message);
}

void EventProducerStub::processFeeDiagnosisIr(const CameraFeeDiagnosisResultParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering intermediate diagnosis data
	message.id = PROCESS_CAMERA_DIAGNOSIS_IR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = parameters.getVerifiedPixels();
	message.param[2] = parameters.getVerifiedHalfCcds();
	message.param[3] = parameters.getVerifiedWindows();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processFeeDiagnosisFr(const CameraFeeDiagnosisResultParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis data
	message.id = PROCESS_CAMERA_DIAGNOSIS_FR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = parameters.getVerifiedPixels();
	message.param[2] = parameters.getVerifiedHalfCcds();
	message.param[3] = parameters.getVerifiedWindows();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processFeeDiagnosisIrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
		idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels,
		uint32_t wrongWindows, uint16_t otherErrors, ErroneousPixel& firstErroneousPixel) {
	GscTaskMessage message;
	// 1- prepare the message gathering intermediate diagnosis error data
	message.id = PROCESS_CAMERA_DIAGNOSIS_IR_ERR;
	 // cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = (static_cast<uint32_t>(parameters.getCameraId()) | (static_cast<uint32_t>(phase) << 16U));
	message.param[1] = errorCounter;
	message.param[2] = parameters.getVerifiedPixels();
	message.param[3] = wrongPixels;
	message.param[4] = parameters.getVerifiedWindows();
	message.param[5] = wrongWindows;
	message.param[6] = static_cast<uint32_t>(otherErrors);
	message.param[7] = static_cast<uint32_t>(firstErroneousPixel.getCcdNumber());
	message.param[8] = static_cast<uint32_t>(firstErroneousPixel.getCcdSide());
	message.param[9] = static_cast<uint32_t>(firstErroneousPixel.getColumn());
	message.param[10] = static_cast<uint32_t>(firstErroneousPixel.getRow());
	message.param[11] = static_cast<uint32_t>(firstErroneousPixel.getExpected());
	message.param[12] = static_cast<uint32_t>(firstErroneousPixel.getReceived());
	//--
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processFeeDiagnosisFrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
		idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels,
		uint32_t wrongHalfCcds, uint32_t wrongWindows, uint16_t otherErrors) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_CAMERA_DIAGNOSIS_FR_ERR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = errorCounter;
	message.param[2] = static_cast<uint32_t>(phase);
	message.param[3] = parameters.getVerifiedPixels();
	message.param[4] = static_cast<uint32_t>(wrongPixels);
	message.param[5] = parameters.getVerifiedHalfCcds();
	message.param[6] = static_cast<uint32_t>(wrongHalfCcds);
	message.param[7] = parameters.getVerifiedWindows();
	message.param[8] = static_cast<uint32_t>(wrongWindows);
	message.param[9] = static_cast<uint32_t>(otherErrors);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processFeeConfigurationError(cameraIdentifier::CameraIdentifierEnum ncamId) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_NFEE_CONFIGURATION_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processStarDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
								idbStarDescErrCodeType::IdbStarDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& starDescriptor) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_STAR_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	// 1- transmit the star parameters
	message.param[2] = starDescriptor.getWindowId();
	message.param[3] = starDescriptor.getMaskIndex();
	message.param[4] = starDescriptor.getExtendedMaskIndex();
	message.param[5] = starDescriptor.getOutlierDetectionState();
	message.param[6] = starDescriptor.getStarProcessingType();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processStarExtendedDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbStarExtDescErrCodeType::IdbStarExtDescErrCodeTypeEnum errorCode, uint32_t descriptorId, const StarDescriptor& starDescriptor,
		uint16_t x, uint16_t y) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_STAR_EXTENDED_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	// 1- transmit the star parameters
	message.param[2] = descriptorId;
	message.param[3] = starDescriptor.getNominalMaskIndex();
	message.param[4] = starDescriptor.getExtendedMaskIndex();
	message.param[5] = starDescriptor.getWindowDescriptorIndex();
	// 1- transmit the star parameters
	message.param[6] = starDescriptor.getOutlierDetectionState();
	message.param[7] = starDescriptor.getStarProcessingType();
	message.param[8] = x;
	message.param[9] = y;
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processImagetteDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbImgDescErrCodeType::IdbImgDescErrCodeTypeEnum errorCode, uint32_t descriptorId, uint8_t width, uint8_t height) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_IMAGETTE_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	// 1- transmit the star parameters
	message.param[2] = descriptorId;
	message.param[3] = static_cast<uint32_t>(width);
	message.param[4] = static_cast<uint32_t>(height);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processImagetteExtendedError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbImgxDescErrCodeType::IdbImgxDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& descriptor) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_IMAGETTE_EXTENDED_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	// 1- transmit the star parameters
	message.param[2] = descriptor.getWindowId();
	message.param[3] = static_cast<uint32_t>(descriptor.getWindowWidth());
	message.param[4] = static_cast<uint32_t>(descriptor.getWindowHeight());
	message.param[5] = static_cast<uint32_t>(descriptor.getWindowX());
	message.param[6] = static_cast<uint32_t>(descriptor.getWindowY());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processOffsetDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbOfstDescErrCodeType::IdbOfstDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor, uint16_t excludedNumber,
		idbEnabledType::IdbEnabledTypeEnum outlierEnable) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	// message id
	message.id = PROCESS_OFFSET_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	message.param[2] = static_cast<uint32_t>(windowDescriptor.getCcdId());
	message.param[3] = static_cast<uint32_t>(windowDescriptor.getCcdSide());
	message.param[4] = windowDescriptor.getId();
	// 1- offset descriptor elements
	message.param[5] = static_cast<uint32_t>(excludedNumber);
	message.param[6] = static_cast<uint32_t>(outlierEnable);
	// 1- window descriptor elements
	message.param[7] = windowDescriptor.getWidth();
	message.param[8] = windowDescriptor.getHeight();
	message.param[9] = windowDescriptor.getX();
	message.param[10] = windowDescriptor.getY();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processBackgroundDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbBgndDescErrCodeType::IdbBgndDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	// message id
	message.id = PROCESS_BACKGROUND_DESCRIPTOR_ERROR;
	// 1- error parameters
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	message.param[2] = static_cast<uint32_t>(windowDescriptor.getCcdId());
	message.param[3] = static_cast<uint32_t>(windowDescriptor.getCcdSide());
	message.param[4] = windowDescriptor.getId();
	// 1- window descriptor elements
	message.param[5] = windowDescriptor.getWidth();
	message.param[6] = windowDescriptor.getHeight();
	message.param[7] = windowDescriptor.getX();
	message.param[8] = windowDescriptor.getY();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processStarCatalogError(idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum errorCode,
		StarPropertyTcParameters& starPropertyParameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	// message id
	message.id = PROCESS_STAR_CATALOG_ERROR;
	// error parameter
	message.param[0] = static_cast<uint32_t>(errorCode);
	// rejected star parameters
	message.param[1] = starPropertyParameters.getId();
	// Use direct memory access instead of type casting to not lose the float precision
	GsbMemory::set<float>(&(message.param[2]), starPropertyParameters.getDeclination(), 0);
	GsbMemory::set<float>(&(message.param[3]), starPropertyParameters.getRightAscension(), 0);
	GsbMemory::set<float>(&(message.param[4]), starPropertyParameters.getMagnitude(), 0);
	GsbMemory::set<float>(&(message.param[5]), starPropertyParameters.getColor(), 0);

	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processMaskLibraryError() {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	// message id
	message.id = PROCESS_MASK_LIBRARY_ERROR;

	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processWindowProgrammingError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum errorCode,
		const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const uint32_t windowId) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_WINDOW_PROGRAMMING_ERROR;
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	message.param[2] = static_cast<uint32_t>(ccdId);
	message.param[3] = windowId;
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processWindowProgrammingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis warning data
	message.id = PROCESS_WINDOW_PROGRAMMING_WARNING;
	message.param[0] = static_cast<uint32_t>(ncamId);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processWindowProgrammingProgress(const WindowProgrammingProgressParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering final diagnosis error data
	message.id = PROCESS_WINDOW_PROGRAMMING_PROGRESS;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(parameters.getCameraId());
	message.param[1] = static_cast<uint32_t>(parameters.getCcdId());
	message.param[2] = parameters.getDpuWindowNumber();
	message.param[3] = parameters.getLookupTableSize();
	message.param[4] = parameters.getFeeWindowNumber();
	message.param[5] = parameters.getFeeWindowListAddress();
	message.param[6] = parameters.getPixelStoreSize();
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processTcVerificationError(GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>* generator,
							uint16_t errorType, uint32_t errorCounter, const GsbTcVerificationErrorParameter& params) {
	GscTaskMessage message;
	// 1- prepare the message gathering TC verification failure data
	// Take care that only a subset of the structure GsbTcVerificationErrorParameter is forwarded
	message.id = PROCESS_TC_VERIFICATION_ERROR;
	message.param[0] = reinterpret_cast<uint32_t>(generator);
	message.param[1] = static_cast<uint32_t>(errorType);
	message.param[2] = errorCounter;
	message.param[3] = static_cast<uint32_t>(params.getTcVerificationErrorStep());
	message.param[4] = static_cast<uint32_t>(params.getPacketId());
	message.param[5] = static_cast<uint32_t>(params.getSequenceControl());
	message.param[6] = params.getFailureId();
	message.param[7] = static_cast<uint32_t>(params.getServiceType());
	message.param[8] = static_cast<uint32_t>(params.getServiceSubType());
	message.param[9] = static_cast<uint32_t>(params.getSourceId());
	message.param[10] = static_cast<uint32_t>(params.getReceivedCrc());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processWindowDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum errorCode, const WindowDescriptor& windowDescriptor) {
	GscTaskMessage message;
	// 1- prepare the message gathering TC verification failure data
	message.id = PROCESS_WINDOW_DESCRIPTOR_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint8_t>(ncamId);
	message.param[1] = static_cast<uint8_t>(errorCode);
	message.param[2] = static_cast<uint32_t>(windowDescriptor.getId());
	message.param[3] = static_cast<uint8_t>(windowDescriptor.getCcdId());
	message.param[4] = static_cast<uint8_t>(windowDescriptor.getCcdSide());
	message.param[5] = static_cast<uint16_t>(windowDescriptor.getX());
	message.param[6] = static_cast<uint16_t>(windowDescriptor.getY());
	message.param[7] = static_cast<uint8_t>(windowDescriptor.getWidth());
	message.param[8] = static_cast<uint8_t>(windowDescriptor.getHeight());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processWindowGenerationProgress(const WindowGenerationProgressParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering TC verification failure data
	message.id = PROCESS_WINDOW_GENERATION_PROGRESS;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint8_t>(parameters.getCameraId());
	message.param[1] = static_cast<uint8_t>(parameters.getCcdId());
	message.param[2] = static_cast<uint8_t>(parameters.getCcdSide());
	message.param[3] = static_cast<uint32_t>(parameters.getNumberGeneratedWindows());
	message.param[4] = static_cast<uint32_t>(parameters.getLastWindowId());
	message.param[5] = static_cast<uint16_t>(parameters.getLastWindowX());
	message.param[6] = static_cast<uint16_t>(parameters.getLastWindowY());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processSmearingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const idbCcdSideType::IdbCcdSideTypeEnum ccdSide,
		const uint32_t smearingIndex, const uint32_t outlierNumber, const float standardDeviation) {
	GscTaskMessage message;
	// 1- prepare the message gathering smearing warning data
	message.id = PROCESS_SMEARING_WARNING;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(ccdId);
	message.param[2] = static_cast<uint32_t>(ccdSide);
	message.param[3] = smearingIndex;
	message.param[4] = outlierNumber;
	// Use direct memory access instead of type casting to not lose the float precision
	GsbMemory::set<float>(&(message.param[5]), standardDeviation, 0);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId, 
						const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId,
						const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY) {
	GscTaskMessage message;
	// 1- prepare the message gathering parameter calculation warning data
	message.id = PROCESS_PARAMETER_CALCULATION_WARNING;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(pCalcWarCode);
	message.param[2] = descrId;
	message.param[3] = static_cast<uint32_t>(ccdId);
	// Use direct memory access instead of type casting to not lose the float precision
	GsbMemory::set<double>(&(message.param[4]), starCcdX, 0);
	GsbMemory::set<double>(&(message.param[5]), starCcdY, 0);
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering informative summary data
	message.id = PROCESS_CCD_POSITION_SUMMARY;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(parameters.getEvtInfId());
	// Use direct memory access instead of type casting to not lose the float precision
	message.param[2] = parameters.getTimestamp().getCoarseTime();
	message.param[3] = parameters.getTimestamp().getFineTime();
	// Total number of stars processed
	message.param[4] = parameters.getNumStarPosComp();
	// Number of stars processed per ccd per side
	message.param[5] = static_cast<uint16_t>(parameters.getStarNumCcd1Right());
	message.param[6] = static_cast<uint16_t>(parameters.getStarNumCcd1Left());
	message.param[7] = static_cast<uint16_t>(parameters.getStarNumCcd2Right());
	message.param[8] = static_cast<uint16_t>(parameters.getStarNumCcd2Left());
	message.param[9] = static_cast<uint16_t>(parameters.getStarNumCcd3Right());
	message.param[10] = static_cast<uint16_t>(parameters.getStarNumCcd3Left());
	message.param[11] = static_cast<uint16_t>(parameters.getStarNumCcd4Right());
	message.param[12] = static_cast<uint16_t>(parameters.getStarNumCcd4Left());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters) {
	GscTaskMessage message;
	// 1- prepare the message gathering informative detailed data
	message.id = PROCESS_CCD_POSITION_DETAILED;
	// cameraIdentifier will be used to dispatch event to the right event producer
	// 8 bits elements
	message.param[0] = static_cast<uint8_t>(ncamId);
	message.param[0] |= static_cast<uint8_t>(parameters.getCcdId()) << 8;
	message.param[0] |= static_cast<uint8_t>(parameters.getCcdSide()) << 16;
	// 16 bits elements
	message.param[1] = static_cast<uint16_t>(parameters.getWindowX());
	message.param[1] |= static_cast<uint16_t>(parameters.getWindowY()) << 16;
	// 32 bits elements
	message.param[2] = static_cast<uint32_t>(parameters.getEvtInfId());
	message.param[3] = parameters.getDescrId();
	message.param[4] = parameters.getTimestamp().getCoarseTime();
	message.param[5] = parameters.getTimestamp().getFineTime();
	// 64 bits elements
	// Reduced double values to 2 32 bits values in order to keep the precision
	message.param[6] = GsbIeee754::getHighWord(parameters.getGcrsAscension());
	message.param[7] = GsbIeee754::getLowWord(parameters.getGcrsAscension());
	message.param[8] = GsbIeee754::getHighWord(parameters.getGcrsDeclination());
	message.param[9] = GsbIeee754::getLowWord(parameters.getGcrsDeclination());
	message.param[10] = GsbIeee754::getHighWord(parameters.getCameraX());
	message.param[11] = GsbIeee754::getLowWord(parameters.getCameraX());
	message.param[12] = GsbIeee754::getHighWord(parameters.getCameraY());
	message.param[13] = GsbIeee754::getLowWord(parameters.getCameraY());
	message.param[14] = GsbIeee754::getHighWord(parameters.getCameraZ());
	message.param[15] = GsbIeee754::getLowWord(parameters.getCameraZ());
	message.param[16] = GsbIeee754::getHighWord(parameters.getFocalPlaneX());
	message.param[17] = GsbIeee754::getLowWord(parameters.getFocalPlaneX());
	message.param[18] = GsbIeee754::getHighWord(parameters.getFocalPlaneY());
	message.param[19] = GsbIeee754::getLowWord(parameters.getFocalPlaneY());
	// 2- send the message
	sendMessage(&message);
}

void EventProducerStub::processObservationStartError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbObstErrCodeType::IdbObstErrCodeTypeEnum errorCode) {
	GscTaskMessage message;
	// 1- prepare the message gathering observation start error data
	message.id = PROCESS_OBSERVATION_START_ERROR;
	// cameraIdentifier will be used to dispatch event to the right event producer
	message.param[0] = static_cast<uint32_t>(ncamId);
	message.param[1] = static_cast<uint32_t>(errorCode);
	// 2- send the message
	sendMessage(&message);
}
