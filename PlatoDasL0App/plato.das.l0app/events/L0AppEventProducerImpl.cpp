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


 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 PlatoDasL0App Component

 L0AppEventProducerImpl.cpp

 $Rev: 0 $

 ************************************************************************/

#include <plato/das/l0app/events/L0AppEventProducerImpl.hpp>

EventProducerImpl::EventProducerImpl() {
	//--- constructor : init link to status/error handlers and managers

	//--- spacewire device numbers
	nicuSpacewireDeviceNumber = 0;
	ricuSpacewireDeviceNumber = 0;
	ncamaSpacewireDeviceNumber = 0;
	ncambSpacewireDeviceNumber = 0;

	//--- time handlers
	timeErrorHandler = 0;
	timeUpdateInformativeHandler = 0;

	//--- spacewire error handlers
	nicuSpacewireErrorHandler = 0;
	ricuSpacewireErrorHandler = 0;
	ncamaSpacewireErrorHandler = 0;
	ncambSpacewireErrorHandler = 0;

	//--- spacewire link error handlers
	nicuSpacewireLinkErrorHandler = 0;
	ricuSpacewireLinkErrorHandler = 0;
	ncamaSpacewireLinkErrorHandler = 0;
	ncambSpacewireLinkErrorHandler = 0;

	//--- spacewire link progress handlers
	nicuSpacewireLinkProgressHandler = 0;
	ricuSpacewireLinkProgressHandler = 0;
	ncamaSpacewireLinkProgressHandler = 0;
	ncambSpacewireLinkProgressHandler = 0;

	//--- spacewire timecode error handlers
	ndpuTimecodeErrorHandler = 0;
	ncamaTimecodeErrorHandler = 0;
	ncambTimecodeErrorHandler = 0;

	//--- rmap event handlers
	ncamaRmapEventHandler = 0;
	ncambRmapEventHandler = 0;

	//--- mode transition event handlers
	modeManagerProgressHandler = 0;
	modeManagerErrorHandler = 0;

	//--- memory event handlers
	memoryErrorHandler = 0;
	memoryProtectionErrorHandler = 0;

	//--- monitor event handlers
	monitoringDefinitionErrorHandler = 0;
	monitoringOutOfLimitEventHandler = 0;

	//--- parameter management event handlers
	parameterManagementErrorHandler = 0;

	//--- ndpu science event handlers
	ndpuStarCatalogErrorHandler = 0;
	ndpuMaskLibraryErrorHandler = 0;

	//--- others handlers
	initialisationErrorHandler = 0;
	initialisationProgressHandler = 0;
	circularBufferEventHandler = 0;
	tcVerificationErrorHandler = 0;
	ahbstatErrorHandler = 0;

	//--- camera engine A error handlers
	ncamaDataErrorHandler = 0;
	ncamaDataHeaderWarningHandler = 0;
	ncamaDataContentWarningHandler = 0;
	ncamaDataUnexpectedWarningHandler = 0;
	ncamaTimecodeProgressHandler = 0;
	ncamaModeProgressHandler = 0;
	ncamaModeErrorHandler = 0;
	ncamaEndOfHalfCcdReadoutInformativeHandler = 0;
	ncamaEndOfHalfCcdReadoutErrorHandler = 0;
	ncamaEndOfFullimageAcqInformativeHandler = 0;
	ncamaEndOfFullimageAcqErrorHandler = 0;
	ncamaEndOfParallelTrapPumpingInformativeHandler = 0;
	ncamaEndOfParallelTrapPumpingErrorHandler = 0;
	ncamaEndOfSerialTrapPumpingInformativeHandler = 0;
	ncamaEndOfSerialTrapPumpingErrorHandler = 0;
	ncamCameraFeeConfigurationErrorHandler = 0;
	ncamaEndOfWindowAcquisitionProgressHandler = 0;
	ncamaEndOfWindowAcquisitionErrorHandler = 0;
	ncamaMaskAndPositionUpdateErrorHandler = 0;
	ncamaMaskAndPositionUpdateInformativeHandler = 0;
	ncamaStartMaskAndPositionUpdateErrorHandler = 0;
	ncamaObservationStartErrorHandler = 0;
	ncamaCameraFeeDiagnosisIrHandler = 0;
	ncamaCameraFeeDiagnosisIrErrorHandler = 0;
	ncamaCameraFeeDiagnosisFrHandler = 0;
	ncamaCameraFeeDiagnosisFrErrorHandler = 0;

	//--- camera engine B event handlers
	ncambDataErrorHandler = 0;
	ncambDataHeaderWarningHandler = 0;
	ncambDataContentWarningHandler = 0;
	ncambDataUnexpectedWarningHandler = 0;
	ncambTimecodeProgressHandler = 0;
	ncambModeProgressHandler = 0;
	ncambModeErrorHandler = 0;
	ncambEndOfHalfCcdReadoutInformativeHandler = 0;
	ncambEndOfHalfCcdReadoutErrorHandler = 0;
	ncambEndOfFullimageAcqInformativeHandler = 0;
	ncambEndOfFullimageAcqErrorHandler = 0;
	ncambEndOfParallelTrapPumpingInformativeHandler = 0;
	ncambEndOfParallelTrapPumpingErrorHandler = 0;
	ncambEndOfSerialTrapPumpingInformativeHandler = 0;
	ncambEndOfSerialTrapPumpingErrorHandler = 0;
	ncambEndOfWindowAcquisitionProgressHandler = 0;
	ncambEndOfWindowAcquisitionErrorHandler = 0;
	ncambMaskAndPositionUpdateErrorHandler = 0;
	ncambMaskAndPositionUpdateInformativeHandler = 0;
	ncambStartMaskAndPositionUpdateErrorHandler = 0;
	ncambObservationStartErrorHandler = 0;
	ncambCameraFeeDiagnosisIrHandler = 0;
	ncambCameraFeeDiagnosisIrErrorHandler = 0;
	ncambCameraFeeDiagnosisFrHandler = 0;
	ncambCameraFeeDiagnosisFrErrorHandler = 0;

	//--- camera science configuration event handlers
	ncamDuplicateIdErrorHandler = 0;
	ncamStarDescriptorErrorHandler = 0;
	ncamStarExtendedDescriptorErrorHandler = 0;
	ncamImagetteDescriptorErrorHandler = 0;
	ncamImagetteExtendedDescriptorErrorHandler = 0;
	ncamBackgroundDescriptorErrorHandler = 0;
	ncamOffsetDescriptorErrorHandler = 0;

	//--- camera A window programming event handlers
	ncamaWindowProgrammingErrorHandler = 0;
	ncamaWindowProgrammingWarningHandler = 0;
	ncamaWindowProgrammingProgressHandler = 0;

	//--- camera B window programming event handlers
	ncambWindowProgrammingErrorHandler = 0;
	ncambWindowProgrammingWarningHandler = 0;
	ncambWindowProgrammingProgressHandler = 0;

	//--- camera A and B window descriptor TC error event handler
	ncamWindowDescriptorErrorHandler = 0;

	//--- camera A and B window descriptor TC generation progress event handler
	ncamaWindowGenerationProgressHandler = 0;
	ncambWindowGenerationProgressHandler = 0;

	//--- camera A and B smearing computation warning event handler
	ncamaSmearingWarningHandler = 0;
	ncambSmearingWarningHandler = 0;
	
	//--- camera A and B parameter calculation event handler
	ncamParameterCalculationWarningHandler = 0;
	ncamCcdPositionSummaryHandler = 0;
	ncamCcdPositionDetailedHandler = 0;

}

GsbSpacewireLinkErrorHandler* EventProducerImpl::getSpwLinkErrorHandler(uint32_t deviceNumber) {
	GsbSpacewireLinkErrorHandler* res = 0;
	//--- search the GsbSpacewireLinkErrorHandler corresponding to the input GsbSpacewireManager
	if (deviceNumber == nicuSpacewireDeviceNumber) {
		//--- get NICU spacewire link error handler
		res = nicuSpacewireLinkErrorHandler;
	}
	else if (deviceNumber == ricuSpacewireDeviceNumber) {
		//--- get RICU spacewire link error handler
		res = ricuSpacewireLinkErrorHandler;
	}
	else if (deviceNumber == ncamaSpacewireDeviceNumber) {
		//--- get NCAM-A spacewire link error handler
		res = ncamaSpacewireLinkErrorHandler;
	}
	else if (deviceNumber == ncambSpacewireDeviceNumber) {
		//--- get NCAM-B spacewire link error handler
		res = ncambSpacewireLinkErrorHandler;
	}

	return res;
}

void EventProducerImpl::processSpacewireError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireError::GsbSpacewireErrorEnum errorType, unsigned int errorCounter, const GsbSpacewireErrorParameter& params) {

	if (deviceNumber == nicuSpacewireDeviceNumber) {
		if (nicuSpacewireErrorHandler != 0) {
			//--- the event occurred on nominal ICU link
			nicuSpacewireErrorHandler->processSpacewireError(packetManager, deviceNumber, errorType, errorCounter, params);
		}
	}
	else if (deviceNumber == ricuSpacewireDeviceNumber) {
		if (ricuSpacewireErrorHandler != 0) {
			//--- the event occurred on redundant ICU link
			ricuSpacewireErrorHandler->processSpacewireError(packetManager, deviceNumber, errorType, errorCounter, params);
		}
	}
	else if (deviceNumber == ncamaSpacewireDeviceNumber) {
		if (ncamaSpacewireErrorHandler != 0) {
			//--- the event occurred on NCAM-A link
			ncamaSpacewireErrorHandler->processSpacewireError(packetManager, deviceNumber, errorType, errorCounter, params);
		}
	}
	else if (deviceNumber == ncambSpacewireDeviceNumber) {
		if (ncambSpacewireErrorHandler != 0) {
			//--- the event occurred on NCAM-B link
			ncambSpacewireErrorHandler->processSpacewireError(packetManager, deviceNumber, errorType, errorCounter, params);
		}
	}
}

void EventProducerImpl::processRmapError(GsbRmapManager* rmapManager, uint32_t deviceNumber,
		gsbRmapErrorCode::GsbRmapErrorCodeEnum errorType, unsigned short errorCounter, GsbRmapErrorParameter parameters) {
	//--- delegates Rmap error processing to dedicated Rmap error handler
	if ((ncamaSpacewireDeviceNumber == deviceNumber) && (ncamaRmapEventHandler != 0)) {
		//--- the error occurred on RMAP Driver of core 0 - linked to camera A
		ncamaRmapEventHandler->processRmapError(rmapManager, deviceNumber, errorType, errorCounter, parameters);
	}

	if ((ncambSpacewireDeviceNumber == deviceNumber) && (ncambRmapEventHandler != 0)) {
		//--- the error occurred on RMAP Driver of core 1 - linked to camera B
		ncambRmapEventHandler->processRmapError(rmapManager, deviceNumber, errorType, errorCounter, parameters);
	}
}

void EventProducerImpl::processTimecodeError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbTimecodeError::GsbTimecodeErrorEnum errorType, unsigned int errorCounter, unsigned char currentTimecode,
		unsigned char expectedTimecode) {
	//--- delegates Rmap error processing to dedicated Rmap error handler
	if (((nicuSpacewireDeviceNumber == deviceNumber) || (ricuSpacewireDeviceNumber == deviceNumber)) && (ndpuTimecodeErrorHandler != 0)) {
		//--- the error occurred on nominal ICU link
		ndpuTimecodeErrorHandler->processTimecodeError(packetManager, deviceNumber, errorType, errorCounter, currentTimecode,
				expectedTimecode);
	}
	if ((ncamaSpacewireDeviceNumber == deviceNumber) && (ncamaTimecodeErrorHandler != 0)) {
		//--- the error occurred on N-CAM A link
		ncamaTimecodeErrorHandler->processTimecodeError(packetManager, deviceNumber, errorType, errorCounter, currentTimecode,
				expectedTimecode);
	}
	if ((ncambSpacewireDeviceNumber == deviceNumber) && (ncambTimecodeErrorHandler != 0)) {
		//--- the error occurred on N-CAM B link
		ncambTimecodeErrorHandler->processTimecodeError(packetManager, deviceNumber, errorType, errorCounter, currentTimecode,
				expectedTimecode);
	}
}

void EventProducerImpl::processTimeUpdateInformative(uint32_t syncTime) {
	//--- delegates time update informative processing to the dedicated time update handler
	if (timeUpdateInformativeHandler != 0) {
		timeUpdateInformativeHandler->processTimeUpdateInformative(syncTime);
	}
}

void EventProducerImpl::processTimeError(uint16_t errorCounter) {
	//--- delegates time update error processing to the dedicated time update handler
	if (timeErrorHandler != 0) {
		timeErrorHandler->processTimeError(errorCounter);
	}
}

void EventProducerImpl::processInitialisationError(idbInitErrCodeType::IdbInitErrCodeTypeEnum errCode, uint32_t wrongValue,
		uint32_t usedValue) {
	//--- check that initialisationErrorHandler is defined
	if (initialisationErrorHandler != 0) {
		initialisationErrorHandler->processInitialisationError(errCode, wrongValue, usedValue);
	}
}

void EventProducerImpl::processInitialisationProgress() {
	//--- check that initialisationProgressHandler is defined
	if (initialisationProgressHandler != 0) {
		initialisationProgressHandler->processInitialisationProgress();
	}
}

void EventProducerImpl::processMemoryError(uint32_t nbWrongBytes, uint16_t apid, gsbMemoryErrorCode::GsbMemoryErrorCodeEnum errorType,
		uint32_t errorCounter, uint8_t* failureAddress, uint32_t expectedValue, uint32_t readValue) {
	//--- delegates memory error processing to the dedicated memory error handler
	if (memoryErrorHandler != 0) {
		memoryErrorHandler->processMemoryError(nbWrongBytes, apid, errorType, errorCounter, failureAddress, expectedValue, readValue);
	}
}

void EventProducerImpl::processMemoryProtectionError(uint16_t apid, uint32_t memoryId) {
	//--- delegates memory error processing to the dedicated memory protection error handler
	if (memoryProtectionErrorHandler != 0) {
		memoryProtectionErrorHandler->processMemoryProtectionError(apid, memoryId);
	}
}

void EventProducerImpl::processMonitoringDefinitionError(uint32_t monitoringFunctionSetId,
											gsbMonitoringFunctionError::GsbMonitoringFunctionErrorEnum errorCode,
											const GsbMonitoringFunctionParameters& monitoringFunctionParameters) {
	//--- delegates monitor definition error processing to the dedicated error handler
	if (monitoringDefinitionErrorHandler != 0) {
		monitoringDefinitionErrorHandler->processMonitoringDefinitionError(monitoringFunctionSetId, errorCode, monitoringFunctionParameters);
	}
}


void EventProducerImpl::processMonitoringOutOfLimitEvent(uint32_t monitoringFunctionSetId, uint32_t eventId,
						GsbMonitoringFunctionCoreParameters monitoringFunctionCoreParameters,
						gsbMonitoringFunctionStatus::GsbMonitoringFunctionStatusEnum status) {
	//--- delegates monitored parameter out of limit error processing to the dedicated error handler
	if (monitoringOutOfLimitEventHandler != 0) {
		monitoringOutOfLimitEventHandler->processMonitoringOutOfLimitEvent(monitoringFunctionSetId, eventId, monitoringFunctionCoreParameters, status);
	}
}

void EventProducerImpl::processParameterManagementError(uint32_t parameterId, uint16_t apid) {
	//--- delegates parameter management error processing to the dedicated error handler
	if (parameterManagementErrorHandler != 0) {
		parameterManagementErrorHandler->processParameterManagementError(parameterId, apid);
	}
}

void EventProducerImpl::processCircularBufferEvent(uint32_t bufferId, gscCircularBufferEvent::GscCircularBufferEventEnum eventCode,
		uint32_t eventCounter) {
	//--- delegates circular buffer event processing to dedicated circular buffer event handler
	if (circularBufferEventHandler != 0) {
		circularBufferEventHandler->processCircularBufferEvent(bufferId, eventCode, eventCounter);
	}
}

void EventProducerImpl::processSpacewireLinkError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireLinkError::GsbSpacewireLinkErrorEnum errorType, unsigned int errorCounter) {
	GsbSpacewireLinkErrorHandler* spacewireLinkErrorHandler = 0;
	//--- select a handler according to the errorType
	spacewireLinkErrorHandler = getSpwLinkErrorHandler(deviceNumber);
	//--- if an handler is found according to the errorType, forward it the message.
	if (spacewireLinkErrorHandler != 0) {
		spacewireLinkErrorHandler->processSpacewireLinkError(packetManager, deviceNumber, errorType, errorCounter);
	}
}

void EventProducerImpl::processSpacewireLinkProgress(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
		gsbSpacewireLinkProgress::GsbSpacewireLinkProgressEnum eventType) {
	//--- delegates the over current event processing to the dedicated event handler
	if (deviceNumber == nicuSpacewireDeviceNumber) {
		if (nicuSpacewireLinkProgressHandler != 0) {
			//--- the event occurred on nominal ICU link
			nicuSpacewireLinkProgressHandler->processSpacewireLinkProgress(packetManager, deviceNumber, eventType);
		}
	}
	else if (deviceNumber == ricuSpacewireDeviceNumber) {
		if (ricuSpacewireLinkProgressHandler != 0) {
			//--- the event occurred on redundant ICU link
			ricuSpacewireLinkProgressHandler->processSpacewireLinkProgress(packetManager, deviceNumber, eventType);
		}
	}
	else if (deviceNumber == ncamaSpacewireDeviceNumber) {
		if (ncamaSpacewireLinkProgressHandler != 0) {
			//--- the event occurred on NCAM-A link
			ncamaSpacewireLinkProgressHandler->processSpacewireLinkProgress(packetManager, deviceNumber, eventType);
		}
	}
	else if (deviceNumber == ncambSpacewireDeviceNumber) {
		if (ncambSpacewireLinkProgressHandler != 0) {
			//--- the event occurred on NCAM-B link
			ncambSpacewireLinkProgressHandler->processSpacewireLinkProgress(packetManager, deviceNumber, eventType);
		}
	}
}

void EventProducerImpl::processCameraDataError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter) {
	//--- delegates the camera data error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaDataErrorHandler != 0) {
			ncamaDataErrorHandler->processCameraDataError(ncamId, errorCounter);
		}
	}
	else {
		if (ncambDataErrorHandler != 0) {
			ncambDataErrorHandler->processCameraDataError(ncamId, errorCounter);
		}
	}
}

void EventProducerImpl::processCameraDataHeaderWarning(idbNcamDataHeaderWarCodeType::IdbNcamDataHeaderWarCodeTypeEnum warningCode,
		uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters) {
	//--- delegates the camera data header warning event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaDataHeaderWarningHandler != 0) {
			ncamaDataHeaderWarningHandler->processCameraDataHeaderWarning(warningCode, warningCounter, parameters);
		}
	}
	else {
		if (ncambDataHeaderWarningHandler != 0) {
			ncambDataHeaderWarningHandler->processCameraDataHeaderWarning(warningCode, warningCounter, parameters);
		}
	}
}

void EventProducerImpl::processCameraDataContentWarning(idbNcamDataContentWarCodeType::IdbNcamDataContentWarCodeTypeEnum warningCode,
		uint32_t warningCounter, const CameraDataContentWarningParameters& parameters) {
	//--- delegates the camera data content warning event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaDataContentWarningHandler != 0) {
			ncamaDataContentWarningHandler->processCameraDataContentWarning(warningCode, warningCounter, parameters);
		}
	}
	else {
		if (ncambDataContentWarningHandler != 0) {
			ncambDataContentWarningHandler->processCameraDataContentWarning(warningCode, warningCounter, parameters);
		}
	}
}

void EventProducerImpl::processCameraDataUnexpectedWarning(uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters) {
	//--- delegates the camera data unexpected warning event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaDataUnexpectedWarningHandler != 0) {
			ncamaDataUnexpectedWarningHandler->processCameraDataUnexpectedWarning(warningCounter, parameters);
		}
	}
	else {
		if (ncambDataUnexpectedWarningHandler != 0) {
			ncambDataUnexpectedWarningHandler->processCameraDataUnexpectedWarning(warningCounter, parameters);
		}
	}
}

void EventProducerImpl::processCameraTimecodeProgress(cameraIdentifier::CameraIdentifierEnum ncamId, uint8_t timecode, GsbTime time) {
	//--- delegates the camera timecode progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaTimecodeProgressHandler != 0) {
			ncamaTimecodeProgressHandler->processCameraTimecodeProgress(ncamId, timecode, time);
		}
	}
	else {
		if (ncambTimecodeProgressHandler != 0) {
			ncambTimecodeProgressHandler->processCameraTimecodeProgress(ncamId, timecode, time);
		}
	}
}

void EventProducerImpl::processCameraModeProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbNcamModeType::IdbNcamModeTypeEnum previousMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
		idbTransReasonType::IdbTransReasonTypeEnum reason) {
	//--- delegates the camera mode progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaModeProgressHandler != 0) {
			ncamaModeProgressHandler->processCameraModeProgress(ncamId, previousMode, currentMode, reason);
		}
	}
	else {
		if (ncambModeProgressHandler != 0) {
			ncambModeProgressHandler->processCameraModeProgress(ncamId, previousMode, currentMode, reason);
		}
	}
}

void EventProducerImpl::processCameraModeError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter,
		idbNcamModeType::IdbNcamModeTypeEnum expectedMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
		idbTransReasonType::IdbTransReasonTypeEnum reason) {
	//--- delegates the camera mode progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaModeErrorHandler != 0) {
			ncamaModeErrorHandler->processCameraModeError(ncamId, errorCounter, expectedMode, currentMode, reason);
		}
	}
	else {
		if (ncambModeErrorHandler != 0) {
			ncambModeErrorHandler->processCameraModeError(ncamId, errorCounter, expectedMode, currentMode, reason);
		}
	}
}

void EventProducerImpl::processEndOfHalfCcdReadoutInformative(const CameraEndOfHalfCcdReadoutParameters& parameters) {
	//--- delegates the end of half-CCD readout information event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfHalfCcdReadoutInformativeHandler != 0) {
			ncamaEndOfHalfCcdReadoutInformativeHandler->processEndOfHalfCcdReadoutInformative(parameters);
		}
	}
	else {
		if (ncambEndOfHalfCcdReadoutInformativeHandler != 0) {
			ncambEndOfHalfCcdReadoutInformativeHandler->processEndOfHalfCcdReadoutInformative(parameters);
		}
	}

}

void EventProducerImpl::processEndOfHalfCcdReadoutError(idbEocrErrCodeType::IdbEocrErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfHalfCcdReadoutParameters& parameters, uint32_t expectedPacketNumber, uint32_t expectedPixelNumber) {
	//--- delegates the end of half-CCD readout error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfHalfCcdReadoutErrorHandler != 0) {
			ncamaEndOfHalfCcdReadoutErrorHandler->processEndOfHalfCcdReadoutError(errorCode, errorCounter, parameters, expectedPacketNumber,
					expectedPixelNumber);
		}
	}
	else {
		if (ncambEndOfHalfCcdReadoutErrorHandler != 0) {
			ncambEndOfHalfCcdReadoutErrorHandler->processEndOfHalfCcdReadoutError(errorCode, errorCounter, parameters, expectedPacketNumber,
					expectedPixelNumber);
		}
	}
}

void EventProducerImpl::processEndOfFullimageAcqInformative(const CameraEndOfFullimageAcqParameters& parameters) {
	//--- delegates the end of full-image acquisition information event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfFullimageAcqInformativeHandler != 0) {
			ncamaEndOfFullimageAcqInformativeHandler->processEndOfFullimageAcqInformative(parameters);
		}
	}
	else {
		if (ncambEndOfFullimageAcqInformativeHandler != 0) {
			ncambEndOfFullimageAcqInformativeHandler->processEndOfFullimageAcqInformative(parameters);
		}
	}

}

void EventProducerImpl::processEndOfFullimageAcqError(idbEofaErrCodeType::IdbEofaErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfFullimageAcqParameters& parameters, uint32_t requestedImagesNumber) {
	//--- delegates the end of full-image acquisition error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfFullimageAcqErrorHandler != 0) {
			ncamaEndOfFullimageAcqErrorHandler->processEndOfFullimageAcqError(errorCode, errorCounter, parameters, requestedImagesNumber);
		}
	}
	else {
		if (ncambEndOfFullimageAcqErrorHandler != 0) {
			ncambEndOfFullimageAcqErrorHandler->processEndOfFullimageAcqError(errorCode, errorCounter, parameters, requestedImagesNumber);
		}
	}
}

void EventProducerImpl::processEndOfParallelTrapPumpingInformative(cameraIdentifier::CameraIdentifierEnum ncamId,
		const CameraParallelTrapPumpingParameters& parameters) {
	//--- delegates the end of parallel trap pumping information event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfParallelTrapPumpingInformativeHandler != 0) {
			ncamaEndOfParallelTrapPumpingInformativeHandler->processEndOfParallelTrapPumpingInformative(ncamId, parameters);
		}
	}
	else {
		if (ncambEndOfParallelTrapPumpingInformativeHandler != 0) {
			ncambEndOfParallelTrapPumpingInformativeHandler->processEndOfParallelTrapPumpingInformative(ncamId, parameters);
		}
	}

}

void EventProducerImpl::processEndOfParallelTrapPumpingError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbEoptpErrCodeType::IdbEoptpErrCodeTypeEnum errorCode, uint32_t errorCounter, const CameraParallelTrapPumpingParameters& parameters) {
	//--- delegates the end of parallel trap pumping error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfParallelTrapPumpingErrorHandler != 0) {
			ncamaEndOfParallelTrapPumpingErrorHandler->processEndOfParallelTrapPumpingError(ncamId, errorCode, errorCounter, parameters);
		}
	}
	else {
		if (ncambEndOfParallelTrapPumpingErrorHandler != 0) {
			ncambEndOfParallelTrapPumpingErrorHandler->processEndOfParallelTrapPumpingError(ncamId, errorCode, errorCounter, parameters);
		}
	}
}

void EventProducerImpl::processEndOfSerialTrapPumpingInformative(const CameraEndOfSerialTrapPumpingParameters& parameters) {
	//--- delegates the end of serial trap pumping information event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfSerialTrapPumpingInformativeHandler != 0) {
			ncamaEndOfSerialTrapPumpingInformativeHandler->processEndOfSerialTrapPumpingInformative(parameters);
		}
	}
	else {
		if (ncambEndOfSerialTrapPumpingInformativeHandler != 0) {
			ncambEndOfSerialTrapPumpingInformativeHandler->processEndOfSerialTrapPumpingInformative(parameters);
		}
	}

}

void EventProducerImpl::processEndOfSerialTrapPumpingError(idbEostpErrCodeType::IdbEostpErrCodeTypeEnum errorCode, uint32_t errorCounter,
		const CameraEndOfSerialTrapPumpingParameters& parameters, uint32_t requestedImagesNumber) {
	//--- delegates the end of serial trap pumping error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfSerialTrapPumpingErrorHandler != 0) {
			ncamaEndOfSerialTrapPumpingErrorHandler->processEndOfSerialTrapPumpingError(errorCode, errorCounter, parameters, requestedImagesNumber);
		}
	}
	else {
		if (ncambEndOfSerialTrapPumpingErrorHandler != 0) {
			ncambEndOfSerialTrapPumpingErrorHandler->processEndOfSerialTrapPumpingError(errorCode, errorCounter, parameters, requestedImagesNumber);
		}
	}
}

void EventProducerImpl::processEndOfWindowAcquisitionProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
		const EndOfWindowAcquisitionProgressParameters& endOfWindowAcquisitionProgressParameters) {
	//--- delegates the end of window acquisition progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfWindowAcquisitionProgressHandler != 0) {
			ncamaEndOfWindowAcquisitionProgressHandler->processEndOfWindowAcquisitionProgress(ncamId,
					endOfWindowAcquisitionProgressParameters);
		}
	}
	else {
		if (ncambEndOfWindowAcquisitionProgressHandler != 0) {
			ncambEndOfWindowAcquisitionProgressHandler->processEndOfWindowAcquisitionProgress(ncamId,
					endOfWindowAcquisitionProgressParameters);
		}
	}
}

void EventProducerImpl::processEndOfWindowAcquisitionError(cameraIdentifier::CameraIdentifierEnum ncamId,
		const EndOfWindowAcquisitionErrorParameters& endOfWindowAcquisitionErrorParameters) {
	//--- delegates the end of window acquisition error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaEndOfWindowAcquisitionErrorHandler != 0) {
			ncamaEndOfWindowAcquisitionErrorHandler->processEndOfWindowAcquisitionError(ncamId, endOfWindowAcquisitionErrorParameters);
		}
	}
	else {
		if (ncambEndOfWindowAcquisitionErrorHandler != 0) {
			ncambEndOfWindowAcquisitionErrorHandler->processEndOfWindowAcquisitionError(ncamId, endOfWindowAcquisitionErrorParameters);
		}
	}
}

void EventProducerImpl::processFeeDiagnosisIr(const CameraFeeDiagnosisResultParameters& parameters) {
	//--- delegates the end of fee diagnosis intermediate progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaCameraFeeDiagnosisIrHandler != 0) {
			ncamaCameraFeeDiagnosisIrHandler->processFeeDiagnosisIr(parameters);
		}

	}
	else {
		if (ncambCameraFeeDiagnosisIrHandler != 0) {
			ncambCameraFeeDiagnosisIrHandler->processFeeDiagnosisIr(parameters);
		}
	}
}

void EventProducerImpl::processFeeDiagnosisFr(const CameraFeeDiagnosisResultParameters& parameters) {
	//--- delegates the end of fee diagnosis final progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaCameraFeeDiagnosisFrHandler != 0) {
			ncamaCameraFeeDiagnosisFrHandler->processFeeDiagnosisFr(parameters);
		}
	}
	else {
		if (ncambCameraFeeDiagnosisFrHandler != 0) {
			ncambCameraFeeDiagnosisFrHandler->processFeeDiagnosisFr(parameters);
		}
	}
}

void EventProducerImpl::processFeeDiagnosisIrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
		idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels, uint32_t wrongWindows, uint16_t otherErrors,
		ErroneousPixel& firstErroneousPixel) {
	//--- delegates the end of fee diagnosis error intermediate event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaCameraFeeDiagnosisIrErrorHandler != 0) {
			ncamaCameraFeeDiagnosisIrErrorHandler->processFeeDiagnosisIrError(parameters, errorCounter, phase, wrongPixels, wrongWindows,
					otherErrors, firstErroneousPixel);
		}
	}
	else {
		if (ncambCameraFeeDiagnosisIrErrorHandler != 0) {
			ncambCameraFeeDiagnosisIrErrorHandler->processFeeDiagnosisIrError(parameters, errorCounter, phase, wrongPixels, wrongWindows,
					otherErrors, firstErroneousPixel);
		}
	}
}

void EventProducerImpl::processFeeDiagnosisFrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
		idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels, uint32_t wrongHalfCcds, uint32_t wrongWindows,
		uint16_t otherErrors) {
	//--- delegates the end of fee diagnosis error final event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaCameraFeeDiagnosisFrErrorHandler != 0) {
			ncamaCameraFeeDiagnosisFrErrorHandler->processFeeDiagnosisFrError(parameters, errorCounter, phase, wrongPixels, wrongHalfCcds,
					wrongWindows, otherErrors);
		}
	}
	else {
		if (ncambCameraFeeDiagnosisFrErrorHandler != 0) {
			ncambCameraFeeDiagnosisFrErrorHandler->processFeeDiagnosisFrError(parameters, errorCounter, phase, wrongPixels, wrongHalfCcds,
					wrongWindows, otherErrors);
		}
	}
}

void EventProducerImpl::processFeeConfigurationError(cameraIdentifier::CameraIdentifierEnum ncamId) {
	//--- delegates the nfee configuration error processing to dedicated error handler
	if (ncamCameraFeeConfigurationErrorHandler != 0) {
		ncamCameraFeeConfigurationErrorHandler->processFeeConfigurationError(ncamId);
	}
}

void EventProducerImpl::processWindowProgrammingError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum errorCode, const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId,
		const uint32_t windowId) {
	//--- delegates the end of window programming error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaWindowProgrammingErrorHandler != 0) {
			ncamaWindowProgrammingErrorHandler->processWindowProgrammingError(ncamId, errorCode, ccdId, windowId);
		}
	}
	else {
		if (ncambWindowProgrammingErrorHandler != 0) {
			ncambWindowProgrammingErrorHandler->processWindowProgrammingError(ncamId, errorCode, ccdId, windowId);
		}
	}
}

void EventProducerImpl::processWindowProgrammingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId) {
	//--- delegates the end of window programming warning event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaWindowProgrammingWarningHandler != 0) {
			ncamaWindowProgrammingWarningHandler->processWindowProgrammingWarning(ncamId);
		}
	}
	else {
		if (ncambWindowProgrammingWarningHandler != 0) {
			ncambWindowProgrammingWarningHandler->processWindowProgrammingWarning(ncamId);
		}
	}
}

void EventProducerImpl::processWindowProgrammingProgress(const WindowProgrammingProgressParameters& parameters) {
	//--- delegates the end of window programming progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (parameters.getCameraId() == cameraIdentifier::NCAM_A) {
		if (ncamaWindowProgrammingProgressHandler != 0) {
			ncamaWindowProgrammingProgressHandler->processWindowProgrammingProgress(parameters);
		}
	}
	else {
		if (ncambWindowProgrammingProgressHandler != 0) {
			ncambWindowProgrammingProgressHandler->processWindowProgrammingProgress(parameters);
		}
	}
}

void EventProducerImpl::processMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const MaskAndPositionUpdateParameters& parameters) {
	//--- delegates the end of mask and position update error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaMaskAndPositionUpdateErrorHandler != 0) {
			ncamaMaskAndPositionUpdateErrorHandler->processMaskAndPositionUpdateError(ncamId, parameters);
		}
	}
	else {
		if (ncambMaskAndPositionUpdateErrorHandler != 0) {
			ncambMaskAndPositionUpdateErrorHandler->processMaskAndPositionUpdateError(ncamId, parameters);
		}
	}
}

void EventProducerImpl::processStartMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const StartMaskAndPositionUpdateParameters& parameters) {
	//--- delegates the end of mask and position update error event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaStartMaskAndPositionUpdateErrorHandler != 0) {
			ncamaStartMaskAndPositionUpdateErrorHandler->processStartMaskAndPositionUpdateError(ncamId, parameters);
		}
	}
	else {
		if (ncambStartMaskAndPositionUpdateErrorHandler != 0) {
			ncambStartMaskAndPositionUpdateErrorHandler->processStartMaskAndPositionUpdateError(ncamId, parameters);
		}
	}
}

void EventProducerImpl::processMaskAndPositionUpdateProgress(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const StartMaskAndPositionUpdateParameters& parameters) {
	//--- delegates the end of mask and position update progress event processing to the dedicated event handler
	// switch to the handler responsible of the N-FEE from the origin of the event
	if (ncamId == cameraIdentifier::NCAM_A) {
		if (ncamaMaskAndPositionUpdateInformativeHandler != 0) {
			ncamaMaskAndPositionUpdateInformativeHandler->processMaskAndPositionUpdateProgress(ncamId, parameters);
		}
	}
	else {
		if (ncambMaskAndPositionUpdateInformativeHandler != 0) {
			ncambMaskAndPositionUpdateInformativeHandler->processMaskAndPositionUpdateProgress(ncamId, parameters);
		}
	}
}

void EventProducerImpl::processAhbstatError(GsdAhbstatManager* ahbstatManager, gsdAhbstatError::GsdAhbstatErrorEnum errorType,
		uint32_t ahbFailingAddressCounter, uint32_t ahbStatusRegister, uint32_t ahbFailingAddress, uint32_t ahbFailingAddressValue) {
	//--- delegates the end of mode transition error event processing to the dedicated event handler
	if (ahbstatErrorHandler != 0) {
		ahbstatErrorHandler->processAhbstatError(ahbstatManager, errorType, ahbFailingAddressCounter, ahbStatusRegister, ahbFailingAddress,
				ahbFailingAddressValue);
	}
}

void EventProducerImpl::processDuplicateIdError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t descriptorId,
		idbDescrTypeType::IdbDescrTypeTypeEnum previousType, idbDescrTypeType::IdbDescrTypeTypeEnum requestedType) {
	//--- delegates the duplicate id event processing to the dedicated duplicate id error handler
	if (ncamDuplicateIdErrorHandler != 0) {
		ncamDuplicateIdErrorHandler->processDuplicateIdError(ncamId, descriptorId, previousType, requestedType);
	}
}

void EventProducerImpl::processTransitionModeProgress(idbNdpuModeType::IdbNdpuModeTypeEnum currentMode,
		idbNdpuModeType::IdbNdpuModeTypeEnum previousMode) {
	//--- delegates the end of mode transition progress event processing to the dedicated event handler
	if (modeManagerProgressHandler != 0) {
		modeManagerProgressHandler->processTransitionModeProgress(currentMode, previousMode);
	}

}

void EventProducerImpl::processTransitionModeError(idbModeErrCodeType::IdbModeErrCodeTypeEnum errorCode,
		idbNdpuModeType::IdbNdpuModeTypeEnum currentMode, idbNdpuModeType::IdbNdpuModeTypeEnum expectedMode, unsigned int errorCounter) {
	//--- delegates the end of mode transition error event processing to the dedicated event handler
	if (modeManagerErrorHandler != 0) {
		modeManagerErrorHandler->processTransitionModeError(errorCode, currentMode, expectedMode, errorCounter);
	}
}

void EventProducerImpl::processTcVerificationError(GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>* generator,
		uint16_t errorType, uint32_t errorCounter, const GsbTcVerificationErrorParameter& params) {
	//--- delegates the end of mode transition error event processing to the dedicated event handler
	if (tcVerificationErrorHandler != 0) {
		tcVerificationErrorHandler->processTcVerificationError(generator, errorType, errorCounter, params);
	}
}

void EventProducerImpl::processStarDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbStarDescErrCodeType::IdbStarDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& starDescriptor) {
	//--- delegates star descriptor error processing to dedicated star descriptor error handler
	if (ncamStarDescriptorErrorHandler != 0) {
		ncamStarDescriptorErrorHandler->processStarDescriptorError(ncamId, errorCode, starDescriptor);
	}
}

void EventProducerImpl::processStarExtendedDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbStarExtDescErrCodeType::IdbStarExtDescErrCodeTypeEnum errorCode, uint32_t descriptorId, const StarDescriptor& starDescriptor,
		uint16_t x, uint16_t y) {
	//--- delegates star extended descriptor error processing to dedicated star extended descriptor error handler
	if (ncamStarExtendedDescriptorErrorHandler != 0) {
		ncamStarExtendedDescriptorErrorHandler->processStarExtendedDescriptorError(ncamId, errorCode, descriptorId, starDescriptor, x, y);
	}
}

void EventProducerImpl::processImagetteDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbImgDescErrCodeType::IdbImgDescErrCodeTypeEnum errorCode, uint32_t descriptorId, uint8_t width, uint8_t height) {
	//--- delegates imagette descriptor error processing to dedicated imagette descriptor error handler
	if (ncamImagetteDescriptorErrorHandler != 0) {
		ncamImagetteDescriptorErrorHandler->processImagetteDescriptorError(ncamId, errorCode, descriptorId, width, height);
	}
}

void EventProducerImpl::processImagetteExtendedError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbImgxDescErrCodeType::IdbImgxDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& descriptor) {
	//--- delegates imagette extended descriptor error processing to dedicated imagette extended descriptor error handler
	if (ncamImagetteExtendedDescriptorErrorHandler != 0) {
		ncamImagetteExtendedDescriptorErrorHandler->processImagetteExtendedError(ncamId, errorCode, descriptor);
	}
}

void EventProducerImpl::processOffsetDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbOfstDescErrCodeType::IdbOfstDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor, uint16_t excludedNumber,
		idbEnabledType::IdbEnabledTypeEnum outlierEnable) {
	//--- delegates offset descriptor error processing to dedicated offset descriptor error handler
	if (ncamOffsetDescriptorErrorHandler != 0) {
		ncamOffsetDescriptorErrorHandler->processOffsetDescriptorError(ncamId, errorCode, windowDescriptor, excludedNumber, outlierEnable);
	}
}

void EventProducerImpl::processBackgroundDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		idbBgndDescErrCodeType::IdbBgndDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor) {
	//--- delegates background descriptor error processing to dedicated background descriptor error handler
	if (ncamBackgroundDescriptorErrorHandler != 0) {
		ncamBackgroundDescriptorErrorHandler->processBackgroundDescriptorError(ncamId, errorCode, windowDescriptor);
	}
}

void EventProducerImpl::processWindowDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
		windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum errorCode, const WindowDescriptor& windowDescriptor) {
	//--- delegates window descriptor error processing to dedicated window descriptor error handler
	if (ncamWindowDescriptorErrorHandler != 0) {
		ncamWindowDescriptorErrorHandler->processWindowDescriptorError(ncamId, errorCode, windowDescriptor);
	}
}

void EventProducerImpl::processStarCatalogError(idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum errorCode,
							StarPropertyTcParameters& starPropertyParameters) {
	//--- delegates star catalog error processing to dedicated star catalog error handler
	if (ndpuStarCatalogErrorHandler != 0) {
		ndpuStarCatalogErrorHandler->processStarCatalogError(errorCode, starPropertyParameters);
	}
}

void EventProducerImpl::processMaskLibraryError() {
	//--- delegates mask library error processing to dedicated mask library error handler
	if (ndpuMaskLibraryErrorHandler != 0) {
		ndpuMaskLibraryErrorHandler->processMaskLibraryError();
	}
}

void EventProducerImpl::processWindowGenerationProgress(const WindowGenerationProgressParameters& parameters) {
	//--- delegates window descriptor progression to dedicated window descriptor progress handler
	if ((ncamaWindowGenerationProgressHandler != 0) && (parameters.getCameraId() == cameraIdentifier::NCAM_A)) {
		ncamaWindowGenerationProgressHandler->processWindowGenerationProgress(parameters);
	}
	else if (ncambWindowGenerationProgressHandler != 0) {
		ncambWindowGenerationProgressHandler->processWindowGenerationProgress(parameters);
	}
}

void EventProducerImpl::processSmearingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const idbCcdSideType::IdbCcdSideTypeEnum ccdSide,
		const uint32_t smearingIndex, const uint32_t outlierNumber, const float standardDeviation) {
	//--- delegates smearing warning processing to dedicated window descriptor progress handler
	if ((ncamaSmearingWarningHandler != 0) && (ncamId == cameraIdentifier::NCAM_A)) {
		ncamaSmearingWarningHandler->processSmearingWarning(ncamId, ccdId, ccdSide, smearingIndex, outlierNumber, standardDeviation);
	}
	else if (ncambSmearingWarningHandler != 0) {
		ncambSmearingWarningHandler->processSmearingWarning(ncamId, ccdId, ccdSide, smearingIndex, outlierNumber, standardDeviation);
	}
}

void EventProducerImpl::processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId, 
		const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId,
		const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY) {
	//--- delegates parameter calculation warning processing to dedicated window descriptor progress handler
	if (ncamParameterCalculationWarningHandler != 0) {
		ncamParameterCalculationWarningHandler->processParameterCalculationWarning(ncamId, pCalcWarCode, descrId, ccdId, starCcdX, starCcdY);
	}
}

void EventProducerImpl::processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters){
	//--- delegates ccd position summary processing to dedicated ccd position summary handler
	if (ncamCcdPositionSummaryHandler != 0) {
		ncamCcdPositionSummaryHandler->processCcdPositionSummary(ncamId, parameters);
	}
}

void EventProducerImpl::processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters){
	//--- delegates ccd position Detailed processing to dedicated ccd position Detailed handler
	if (ncamCcdPositionDetailedHandler != 0) {
		ncamCcdPositionDetailedHandler->processCcdPositionDetailed(ncamId, parameters);
	}
}

void EventProducerImpl::processObservationStartError(const cameraIdentifier::CameraIdentifierEnum ncamId,
		const idbObstErrCodeType::IdbObstErrCodeTypeEnum errorCode) {
	//--- delegates observation start error processing to the dedicated handler
	if ((ncamaObservationStartErrorHandler != 0) && (ncamId == cameraIdentifier::NCAM_A)) {
		ncamaObservationStartErrorHandler->processObservationStartError(ncamId, errorCode);
	}
	else if (ncambObservationStartErrorHandler != 0) {
		ncambObservationStartErrorHandler->processObservationStartError(ncamId, errorCode);
	}
}
