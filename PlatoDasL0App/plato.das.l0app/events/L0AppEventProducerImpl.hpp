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

 L0AppEventProducerImpl.hpp

 $Rev: 0 $

 ************************************************************************/

#ifndef L0APPEVENTPRODUCERIMPL_HPP
#define L0APPEVENTPRODUCERIMPL_HPP

#include <plato/das/l0app/events/L0AppEventProducer.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				class EventProducerImpl: public EventProducer {
				private:
					EventProducerImpl(EventProducerImpl const* source);					///< Non functional copy constructor
					EventProducerImpl(EventProducerImpl const& source);					///< Non functional copy constructor
					EventProducerImpl& operator=(EventProducerImpl& eventProducerImpl); ///< Non functional overload of the assignment operator

				protected:
					/// Origin of the messages to forward to nicuSpacewireErrorHandler for the nominal connection.
					uint32_t nicuSpacewireDeviceNumber;
					/// Origin of the messages to forward to ricuSpacewireErrorHandler for the redundant connection.
					uint32_t ricuSpacewireDeviceNumber;
					/// Origin of the messages to forward to ncamaSpacewireErrorHandler. Used by Spacewire related handlers.
					uint32_t ncamaSpacewireDeviceNumber;
					/// Origin of the messages to forward to ncambSpacewireErrorHandler. Used by Spacewire related handlers.
					uint32_t ncambSpacewireDeviceNumber;

					/// Recipient of the messages from icuSpacewireManager on nominal connection. Used by processSpacewireError().
					GsbSpacewireErrorHandler* nicuSpacewireErrorHandler;
					/// Recipient of the messages from icuSpacewireManager on redundant connection. Used by processSpacewireError().
					GsbSpacewireErrorHandler* ricuSpacewireErrorHandler;
					/// Recipient of the messages from nfeeaSpacewireManager. Used by processSpacewireError().
					GsbSpacewireErrorHandler* ncamaSpacewireErrorHandler;
					/// Recipient of the messages from nfeebSpacewireManager. Used by processSpacewireError().
					GsbSpacewireErrorHandler* ncambSpacewireErrorHandler;
					/// Recipient of the spacecraft spacewire error event on nominal connection. Used by processSpacewireLinkError().
					GsbSpacewireLinkErrorHandler* nicuSpacewireLinkErrorHandler;
					/// Recipient of the spacecraft spacewire error event on redundant connection. Used by processSpacewireLinkError().
					GsbSpacewireLinkErrorHandler* ricuSpacewireLinkErrorHandler;
					/// Recipient of the N-CAM A spacewire error event. Used by processSpacewireLinkError().
					GsbSpacewireLinkErrorHandler* ncamaSpacewireLinkErrorHandler;
					/// Recipient of the N-CAM B spacewire error event. Used by processSpacewireLinkError().
					GsbSpacewireLinkErrorHandler* ncambSpacewireLinkErrorHandler;
					/// Recipient of the spacecraft spacewire progress link event on nominal connection. Used by processSpacewireLinkProgress().
					GsbSpacewireLinkProgressHandler* nicuSpacewireLinkProgressHandler;
					/// Recipient of the spacecraft spacewire progress link event on redundant connection. Used by processSpacewireLinkProgress().
					GsbSpacewireLinkProgressHandler* ricuSpacewireLinkProgressHandler;
					/// Recipient of the N-CAM A spacewire progress link event. Used by processSpacewireLinkProgress().
					GsbSpacewireLinkProgressHandler* ncamaSpacewireLinkProgressHandler;
					/// Recipient of the N-CAM B spacewire progress link event. Used by processSpacewireLinkProgress().
					GsbSpacewireLinkProgressHandler* ncambSpacewireLinkProgressHandler;

					/// Recipient of the messages from cameraDriver of core0. Used by processRmapError().
					GsbRmapErrorHandler* ncamaRmapEventHandler;
					/// Recipient of the messages from cameraDriver of core1. Used by processRmapError().
					GsbRmapErrorHandler* ncambRmapEventHandler;

					/// Recipient of the messages from memoryManager. Used by processMemoryError().
					GsbMemoryErrorHandler* memoryErrorHandler;
					/// Recipient of the messages from memoryProtectionErrorHandler. Used by processMemoryProtectionError().
					GsbMemoryProtectionErrorHandler* memoryProtectionErrorHandler;

					/// Recipient of the messages from monitoringFunctionSet. Used by processMonitoringDefinitionError().
					GsbMonitoringDefinitionErrorHandler* monitoringDefinitionErrorHandler;
					/// Recipient of the messages from monitoringFunctionSet. Used by processMonitoringOutOfLimitEvent().
					GsbMonitoringOutOfLimitEventHandler* monitoringOutOfLimitEventHandler;

					/// Recipient of the messages from parameterManagement parser. Used by processParameterManagementError().
					GsbParameterManagementErrorHandler* parameterManagementErrorHandler;

					/// Recipient of the messages from ahbstatErrorHandler. Used by processAhbstatError().
					GsdAhbstatErrorHandler* ahbstatErrorHandler;

					/// Recipient of the messages from duplicate id error handler
					DuplicateIdErrorHandler* ncamDuplicateIdErrorHandler;

					/// Recipient of the messages from configurationManager. Used by processInitialisationProgress().
					InitialisationProgressHandler* initialisationProgressHandler;
					///
					InitialisationErrorHandler* initialisationErrorHandler;

					/// Recipient of the messages from circularBufferEvent. Used by processCircularBufferEvent().
					GscCircularBufferEventHandler* circularBufferEventHandler;

					/// Recipient of the spacecraft timecode error event on nominal connection. Used by processTimecodeError().
					GsbTimecodeErrorHandler* ndpuTimecodeErrorHandler;
					/// Recipient of the N-CAM A timecode error event. Used by processTimecodeError().
					GsbTimecodeErrorHandler* ncamaTimecodeErrorHandler;
					/// Recipient of the N-CAM B timecode error event. Used by processTimecodeError().
					GsbTimecodeErrorHandler* ncambTimecodeErrorHandler;

					/// Recipient of the time update event
					TimeUpdateInformativeHandler* timeUpdateInformativeHandler;
					/// Recipient of the time update event
					TimeErrorHandler* timeErrorHandler;

					/// Recipient of the mode manager transition progress event. Used by processTransitionModeProgress().
					ModeManagerProgressHandler* modeManagerProgressHandler;
					/// Recipient of the mode manager transition error event. Used by processTransitionModeError().
					ModeManagerErrorHandler* modeManagerErrorHandler;

					// Recipient of the tc verification error event. Used by processTcVerificationError();
					GsbTcVerificationErrorHandlerInst* tcVerificationErrorHandler;

					///
					CameraDataErrorHandler* ncamaDataErrorHandler;
					///
					CameraDataHeaderWarningHandler* ncamaDataHeaderWarningHandler;
					///
					CameraDataContentWarningHandler* ncamaDataContentWarningHandler;
					///
					CameraDataUnexpectedWarningHandler* ncamaDataUnexpectedWarningHandler;
					///
					CameraTimecodeProgressHandler* ncamaTimecodeProgressHandler;
					///
					CameraModeProgressHandler* ncamaModeProgressHandler;
					///
					CameraModeErrorHandler* ncamaModeErrorHandler;
					///
					CameraEndOfHalfCcdReadoutInformativeHandler* ncamaEndOfHalfCcdReadoutInformativeHandler;
					///
					CameraEndOfHalfCcdReadoutErrorHandler* ncamaEndOfHalfCcdReadoutErrorHandler;
					///
					CameraEndOfFullimageAcqInformativeHandler* ncamaEndOfFullimageAcqInformativeHandler;
					///
					CameraEndOfFullimageAcqErrorHandler* ncamaEndOfFullimageAcqErrorHandler;
					///
					CameraEndOfParallelTrapPumpingInformativeHandler* ncamaEndOfParallelTrapPumpingInformativeHandler;
					///
					CameraEndOfParallelTrapPumpingErrorHandler* ncamaEndOfParallelTrapPumpingErrorHandler;
					///
					CameraEndOfSerialTrapPumpingInformativeHandler* ncamaEndOfSerialTrapPumpingInformativeHandler;
					///
					CameraEndOfSerialTrapPumpingErrorHandler* ncamaEndOfSerialTrapPumpingErrorHandler;
					///
					EndOfWindowAcquisitionProgressHandler* ncamaEndOfWindowAcquisitionProgressHandler;
					///
					EndOfWindowAcquisitionErrorHandler* ncamaEndOfWindowAcquisitionErrorHandler;
					///
					CameraDataErrorHandler* ncambDataErrorHandler;
					///
					CameraDataHeaderWarningHandler* ncambDataHeaderWarningHandler;
					///
					CameraDataContentWarningHandler* ncambDataContentWarningHandler;
					///
					CameraDataUnexpectedWarningHandler* ncambDataUnexpectedWarningHandler;
					///
					CameraTimecodeProgressHandler* ncambTimecodeProgressHandler;
					///
					CameraModeProgressHandler* ncambModeProgressHandler;
					///
					CameraModeErrorHandler* ncambModeErrorHandler;
					///
					CameraEndOfHalfCcdReadoutInformativeHandler* ncambEndOfHalfCcdReadoutInformativeHandler;
					///
					CameraEndOfHalfCcdReadoutErrorHandler* ncambEndOfHalfCcdReadoutErrorHandler;
					///
					CameraEndOfFullimageAcqInformativeHandler* ncambEndOfFullimageAcqInformativeHandler;
					///
					CameraEndOfFullimageAcqErrorHandler* ncambEndOfFullimageAcqErrorHandler;
					///
					CameraEndOfParallelTrapPumpingInformativeHandler* ncambEndOfParallelTrapPumpingInformativeHandler;
					///
					CameraEndOfParallelTrapPumpingErrorHandler* ncambEndOfParallelTrapPumpingErrorHandler;
					///
					CameraEndOfSerialTrapPumpingInformativeHandler* ncambEndOfSerialTrapPumpingInformativeHandler;
					///
					CameraEndOfSerialTrapPumpingErrorHandler* ncambEndOfSerialTrapPumpingErrorHandler;
					///
					EndOfWindowAcquisitionProgressHandler* ncambEndOfWindowAcquisitionProgressHandler;
					///
					EndOfWindowAcquisitionErrorHandler* ncambEndOfWindowAcquisitionErrorHandler;
					/// Recipient of the intermediate informative diagnosis event on camera A
					CameraFeeDiagnosisIrHandler* ncamaCameraFeeDiagnosisIrHandler;
					/// Recipient of the intermediate informative diagnosis event on camera B
					CameraFeeDiagnosisIrHandler* ncambCameraFeeDiagnosisIrHandler;
					/// Recipient of the final informative diagnosis event on camera A
					CameraFeeDiagnosisFrHandler* ncamaCameraFeeDiagnosisFrHandler;
					/// Recipient of the final informative diagnosis event on camera B
					CameraFeeDiagnosisFrHandler* ncambCameraFeeDiagnosisFrHandler;
					/// Recipient of the intermediate error diagnosis event on camera A
					CameraFeeDiagnosisIrErrorHandler* ncamaCameraFeeDiagnosisIrErrorHandler;
					/// Recipient of the intermediate error diagnosis event on camera B
					CameraFeeDiagnosisIrErrorHandler* ncambCameraFeeDiagnosisIrErrorHandler;
					/// Recipient of the final error diagnosis event on camera A
					CameraFeeDiagnosisFrErrorHandler* ncamaCameraFeeDiagnosisFrErrorHandler;
					/// Recipient of the final error diagnosis event on camera B
					CameraFeeDiagnosisFrErrorHandler* ncambCameraFeeDiagnosisFrErrorHandler;
					///Recipient of the NFEE configuration error event
					CameraFeeConfigurationErrorHandler* ncamCameraFeeConfigurationErrorHandler;
					/// Recipient of the window programming error event on camera A
					WindowProgrammingErrorHandler* ncamaWindowProgrammingErrorHandler;
					/// Recipient of the window programming error event on camera B
					WindowProgrammingErrorHandler* ncambWindowProgrammingErrorHandler;
					/// Recipient of the window programming warning event on camera A
					WindowProgrammingWarningHandler* ncamaWindowProgrammingWarningHandler;
					/// Recipient of the window programming warning event on camera B
					WindowProgrammingWarningHandler* ncambWindowProgrammingWarningHandler;
					/// Recipient of the window programming informative event on camera A
					WindowProgrammingProgressHandler* ncamaWindowProgrammingProgressHandler;
					/// Recipient of the window programming informative event on camera B
					WindowProgrammingProgressHandler* ncambWindowProgrammingProgressHandler;
					/// Recipient of the mask and position update error event on camera A
					MaskAndPositionUpdateErrorHandler* ncamaMaskAndPositionUpdateErrorHandler;
					/// Recipient of the mask and position update error event on camera B
					MaskAndPositionUpdateErrorHandler* ncambMaskAndPositionUpdateErrorHandler;
					/// Recipient of the mask and position update error event on camera A
					StartMaskAndPositionUpdateErrorHandler* ncamaStartMaskAndPositionUpdateErrorHandler;
					/// Recipient of the mask and position update error event on camera B
					StartMaskAndPositionUpdateErrorHandler* ncambStartMaskAndPositionUpdateErrorHandler;
					/// Recipient of the mask and position update informative event on camera A
					MaskAndPositionUpdateInformativeHandler* ncamaMaskAndPositionUpdateInformativeHandler;
					/// Recipient of the mask and position update informative event on camera B
					MaskAndPositionUpdateInformativeHandler* ncambMaskAndPositionUpdateInformativeHandler;
					/// Recipient of the star descriptor TC error event on camera A and B
					StarDescriptorErrorHandler* ncamStarDescriptorErrorHandler;
					/// Recipient of the star extended descriptor TC error event on camera A and B
					StarExtendedDescriptorErrorHandler* ncamStarExtendedDescriptorErrorHandler;
					/// Recipient of the imagette descriptor TC error event on camera A and B
					ImagetteDescriptorErrorHandler* ncamImagetteDescriptorErrorHandler;
					/// Recipient of the imagette extended descriptor TC error event on camera A and B
					ImagetteExtendedDescriptorErrorHandler* ncamImagetteExtendedDescriptorErrorHandler;
					/// Recipient of the background descriptor TC error event on camera A and B
					BackgroundDescriptorErrorHandler* ncamBackgroundDescriptorErrorHandler;
					/// Recipient of the offset descriptor TC error event on camera A and B
					OffsetDescriptorErrorHandler* ncamOffsetDescriptorErrorHandler;
					/// Recipient of the window descriptor TC error event on camera A and B
					WindowDescriptorErrorHandler* ncamWindowDescriptorErrorHandler;
					/// Recipient of the star catalog TC error event on ndpu
					StarCatalogErrorHandler* ndpuStarCatalogErrorHandler;
					/// Recipient of the mask library TC error event on ndpu
					MaskLibraryErrorHandler* ndpuMaskLibraryErrorHandler;
					/// Recipient of the window descriptor TC generation progression event on camera A
					WindowGenerationProgressHandler* ncamaWindowGenerationProgressHandler;
					/// Recipient of the window descriptor TC generation progression event on camera B
					WindowGenerationProgressHandler* ncambWindowGenerationProgressHandler;
					/// Recipient of the smearing computation warning event on camera A
					SmearingWarningHandler* ncamaSmearingWarningHandler;
					/// Recipient of the smearing computation warning event on camera B
					SmearingWarningHandler* ncambSmearingWarningHandler;
					/// Recipient of the parameter calculation warning event on camera A and B
					ParameterCalculationWarningHandler* ncamParameterCalculationWarningHandler;
					/// Recipient of the parameter calculation informative summary event on camera A and B
					CcdPositionSummaryHandler* ncamCcdPositionSummaryHandler;
					/// Recipient of the parameter calculation informative detailed event on camera A and B
					CcdPositionDetailedHandler* ncamCcdPositionDetailedHandler;
					///
					ObservationStartErrorHandler* ncamaObservationStartErrorHandler;
					///
					ObservationStartErrorHandler* ncambObservationStartErrorHandler;

					/// get the producer of error spacewire link, recognized by its SpaceWire device number
					GsbSpacewireLinkErrorHandler* getSpwLinkErrorHandler(uint32_t deviceNumber);

				public:
					/// constructor
					EventProducerImpl();

					/*////////////////////////////////////////////*/
					/*///////////////// handlers /////////////////*/
					/*////////////////////////////////////////////*/

					/**
					 * Inherited from InitialisationErrorHandler::processInitialisationError
					 */
					virtual void processInitialisationError(idbInitErrCodeType::IdbInitErrCodeTypeEnum errCode, uint32_t wrongValue,
							uint32_t usedValue);

					/**
					 * Inherited from GsbSpacewireErrorHandler::processSpacewireError()
					 */
					virtual void processSpacewireError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireError::GsbSpacewireErrorEnum errorType, unsigned int errorCounter,
							const GsbSpacewireErrorParameter& params);
					/**
					 * Inherited from GsbRmapErrorHandler::processRmapError()
					 */
					virtual void processRmapError(GsbRmapManager* rmapManager, uint32_t deviceNumber,
							gsbRmapErrorCode::GsbRmapErrorCodeEnum errorType, unsigned short errorCounter,
							GsbRmapErrorParameter parameters);
					/**
					 * Inherited from GsbTimecodeErrorHandler::processTimecodeError()
					 */
					virtual void processTimecodeError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbTimecodeError::GsbTimecodeErrorEnum errorType, unsigned int errorCounter, unsigned char currentTimecode,
							unsigned char expectedTimecode);

					/**
					 * Handler method for time update informative event
					 * @param syncTime The synchronized time
					 */
					virtual void processTimeUpdateInformative(uint32_t syncTime);

					/**
					 * Handler method for time error event
					 */
					virtual void processTimeError(uint16_t errorCounter);

					/**
					 *
					 */
					virtual void processInitialisationProgress();

					/**
					 * Inherited from GsbMemoryErrorHandler::processMemoryError()
					 */
					virtual void processMemoryError(uint32_t nbWrongBytes, uint16_t apid,
							gsbMemoryErrorCode::GsbMemoryErrorCodeEnum errorType, uint32_t errorCounter, uint8_t* failureAddress,
							uint32_t expectedValue, uint32_t readValue);

					/**
					 * Inherited from GsbMemoryProtectionErrorHandler::processMemoryProtectionError()
					 */
					virtual void processMemoryProtectionError(uint16_t apid, uint32_t memoryId);

					/**
					 * Inherited from GsbMonitoringDefinitionErrorHandler
					 */
					virtual void processMonitoringDefinitionError(uint32_t monitoringFunctionSetId,
											gsbMonitoringFunctionError::GsbMonitoringFunctionErrorEnum errorCode,
											const GsbMonitoringFunctionParameters& monitoringFunctionParameters);

					/**
					 * Inherited from GsbMonitoringOutOfLimitEventHandler
					 */
					virtual void processMonitoringOutOfLimitEvent(uint32_t monitoringFunctionSetId, uint32_t eventId,
											GsbMonitoringFunctionCoreParameters monitoringFunctionCoreParameters,
											gsbMonitoringFunctionStatus::GsbMonitoringFunctionStatusEnum status);

					/**
					 * Inherited from GsbParameterManagementErrorHandler
					 */
					 virtual void processParameterManagementError(uint32_t parameterId, uint16_t apid);

					/**
					 * Inherited from GscCircularBufferEventHandler
					 * Forward message to circularBufferEventHandler::processCircularBufferEvent()
					 */
					virtual void processCircularBufferEvent(uint32_t bufferId, gscCircularBufferEvent::GscCircularBufferEventEnum eventCode,
							uint32_t eventCounter);
					/**
					 * Inherited from GsbSpacewireLinkErrorHandler::processSpacewireLinkError()
					 * Forward the message to an handler, according to the \a erroType:
					 *  - INI_TIMEOUT : not managed
					 *  - FATAL : not managed
					 *  - TX_TIMEOUT : not managed
					 *  - RX_TIMEOUT : not managed
					 *  - HB_TIMEOUT : EventProducerImpl::lowScSpwLinkErrorHandler
					 *  - HB_MISSING : EventProducerImpl::mediumSpwLinkErrorHandler
					 */
					virtual void processSpacewireLinkError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireLinkError::GsbSpacewireLinkErrorEnum errorType, unsigned int errorCounter);

					/**
					 * Inherited from GsbSpacewireLinkProgressHandler::processSpacewireLinkProgress()
					 */
					virtual void processSpacewireLinkProgress(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireLinkProgress::GsbSpacewireLinkProgressEnum eventType);

					/**
					 * Inherited from CameraDataErrorHandler
					 */
					virtual void processCameraDataError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter);

					/**
					 * Inherited from CameraDataHeaderWarningHandler
					 */
					virtual void processCameraDataHeaderWarning(idbNcamDataHeaderWarCodeType::IdbNcamDataHeaderWarCodeTypeEnum warningCode,
							uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters);

					/**
					 * Inherited from CameraDataContentWarningHandler
					 */
					virtual void processCameraDataContentWarning(
							idbNcamDataContentWarCodeType::IdbNcamDataContentWarCodeTypeEnum warningCode, uint32_t warningCounter,
							const CameraDataContentWarningParameters& parameters);

					/**
					 * Inherited from CameraDataUnexpectedWarningHandler
					 */
					virtual void processCameraDataUnexpectedWarning(uint32_t warningCounter,
							const CameraDataHeaderWarningParameters& parameters);

					/**
					 * Inherited from CameraTimecodeProgressHandler
					 */
					virtual void processCameraTimecodeProgress(cameraIdentifier::CameraIdentifierEnum ncamId, uint8_t timecode,
							GsbTime time);

					/**
					 * Inherited from CameraModeProgressHandler
					 */
					virtual void processCameraModeProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbNcamModeType::IdbNcamModeTypeEnum previousMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
							idbTransReasonType::IdbTransReasonTypeEnum reason);

					/**
					 * Inherited from CameraModeErrorHandler
					 */
					virtual void processCameraModeError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter,
							idbNcamModeType::IdbNcamModeTypeEnum expectedMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
							idbTransReasonType::IdbTransReasonTypeEnum reason);

					/**
					 * Inherited from CameraEndOfHalfCcdReadoutInformativeHandler
					 */
					virtual void processEndOfHalfCcdReadoutInformative(const CameraEndOfHalfCcdReadoutParameters& parameters);

					/**
					 * Inherited from CameraEndOfHalfCcdReadoutErrorHandler
					 */
					virtual void processEndOfHalfCcdReadoutError(idbEocrErrCodeType::IdbEocrErrCodeTypeEnum errorCode,
							uint32_t errorCounter, const CameraEndOfHalfCcdReadoutParameters& parameters, uint32_t expectedPacketNumber,
							uint32_t expectedPixelNumber);

					/**
					 * Inherited from CameraEndOfFullimageAcqInformativeHandler
					 */
					virtual void processEndOfFullimageAcqInformative(const CameraEndOfFullimageAcqParameters& parameters);

					/**
					 * Inherited from CameraEndOfFullimageAcqErrorHandler
					 */
					virtual void processEndOfFullimageAcqError(idbEofaErrCodeType::IdbEofaErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraEndOfFullimageAcqParameters& parameters, uint32_t requestedImagesNumber);

					/**
					 * Inherited from CameraEndOfParallelTrapPumpingInformativeHandler
					 */
					virtual void processEndOfParallelTrapPumpingInformative(cameraIdentifier::CameraIdentifierEnum ncamId,
							const CameraParallelTrapPumpingParameters& parameters);

					/**
					 * Inherited from CameraEndOfParallelTrapPumpingErrorHandler
					 */
					virtual void processEndOfParallelTrapPumpingError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbEoptpErrCodeType::IdbEoptpErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraParallelTrapPumpingParameters& parameters);

					/**
					 * Inherited from CameraEndOfSerialTrapPumpingInformativeHandler
					 */
					virtual void processEndOfSerialTrapPumpingInformative(const CameraEndOfSerialTrapPumpingParameters& parameters);

					/**
					 * Inherited from CameraEndOfSerialTrapPumpingErrorHandler
					 */
					virtual void processEndOfSerialTrapPumpingError(idbEostpErrCodeType::IdbEostpErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraEndOfSerialTrapPumpingParameters& parameters, uint32_t requestedImagesNumber);

					/**
					 * Inherited from EndOfWindowAcquisitionProgressHandler
					 */
					virtual void processEndOfWindowAcquisitionProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
							const EndOfWindowAcquisitionProgressParameters& endOfWindowAcquisitionProgressParameters);

					/**
					 * Inherited from EndOfWindowAcquisitionErrorHandler
					 */
					virtual void processEndOfWindowAcquisitionError(cameraIdentifier::CameraIdentifierEnum ncamId,
							const EndOfWindowAcquisitionErrorParameters& endOfWindowAcquisitionErrorParameters);

					/**
					 * Inherited from CameraFeeDiagnosisIrHandler
					 */
					virtual void processFeeDiagnosisIr(const CameraFeeDiagnosisResultParameters& parameters);

					/**
					 * Inherited from CameraFeeDiagnosisFrHandler
					 */
					virtual void processFeeDiagnosisFr(const CameraFeeDiagnosisResultParameters& parameters);

					/**
					 * Inherited from CameraFeeDiagnosisIrErrorHandler
					 */
					virtual void processFeeDiagnosisIrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
							idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels, uint32_t wrongWindows,
							uint16_t otherErrors, ErroneousPixel& firstErroneousPixel);

					/**
					 * Inherited from CameraFeeDiagnosisFrErrorHandler
					 */
					virtual void processFeeDiagnosisFrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
							idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels, uint32_t wrongHalfCcds,
							uint32_t wrongWindows, uint16_t otherErrors);

					/**
					 * Inherited from CameraFeeConfigurationErrorHandler
					 */
					virtual void processFeeConfigurationError(cameraIdentifier::CameraIdentifierEnum ncamId);

					/**
					 * Inherited from WindowProgrammingErrorHandler
					 */
					virtual void processWindowProgrammingError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum errorCode,
							const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const uint32_t windowId);

					/**
					 * Inherited from WindowProgrammingWarningHandler
					 */
					virtual void processWindowProgrammingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId);

					/**
					 * Inherited from WindowProgrammingProgressHandler
					 */
					virtual void processWindowProgrammingProgress(const WindowProgrammingProgressParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateErrorHandler
					 */
					virtual void processMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const MaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateErrorHandler
					 */
					virtual void processStartMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const StartMaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateInformativeHandler
					 */
					virtual void processMaskAndPositionUpdateProgress(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const StartMaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from GsdAhbstatErrorHandler
					 */
					virtual void processAhbstatError(GsdAhbstatManager* ahbstatManager, gsdAhbstatError::GsdAhbstatErrorEnum errorType,
							uint32_t ahbFailingAddressCounter, uint32_t ahbStatusRegister, uint32_t ahbFailingAddress, uint32_t ahbFailingAddressValue);

					/**
					 * Inherited from DuplicateIdErrorHanlder
					 */
					virtual void processDuplicateIdError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t descriptorId,
							idbDescrTypeType::IdbDescrTypeTypeEnum previousType, idbDescrTypeType::IdbDescrTypeTypeEnum requestedType);

					/**
					 * Inherited from ModeManagerProgressHandler
					 */
					virtual void processTransitionModeProgress(idbNdpuModeType::IdbNdpuModeTypeEnum currentMode,
							idbNdpuModeType::IdbNdpuModeTypeEnum previousMode);

					/**
					 * Inherited from ModeManagerErrorHandler
					 */
					virtual void processTransitionModeError(idbModeErrCodeType::IdbModeErrCodeTypeEnum errorCode,
							idbNdpuModeType::IdbNdpuModeTypeEnum currentMode, idbNdpuModeType::IdbNdpuModeTypeEnum expectedMode,
							unsigned int errorCounter);

					/**
					 * Inherited from GsbTcVerificationErrorHandler
					 */
					virtual void processTcVerificationError(GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>* generator,
							uint16_t errorType, uint32_t errorCounter, const GsbTcVerificationErrorParameter& params);

					/**
					 * Inherited from StarDescriptorErrorHandler
					 */
					virtual void processStarDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbStarDescErrCodeType::IdbStarDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& starDescriptor);

					/**
					 * Inherited from StarExtendedDescriptorErrorHandler
					 */
					virtual void processStarExtendedDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbStarExtDescErrCodeType::IdbStarExtDescErrCodeTypeEnum errorCode, uint32_t descriptorId,
							const StarDescriptor& starDescriptor, uint16_t x, uint16_t y);

					/**
					 * Inherited from ImagetteDescriptorErrorHandler
					 */
					virtual void processImagetteDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbImgDescErrCodeType::IdbImgDescErrCodeTypeEnum errorCode, uint32_t descriptorId, uint8_t width, uint8_t height);

					/**
					 * Inherited from ImagetteExtendedDescriptorErrorHandler
					 */
					virtual void processImagetteExtendedError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbImgxDescErrCodeType::IdbImgxDescErrCodeTypeEnum errorCode, StarDescriptorTcParameter& descriptor);

					/**
					 * Inherited from OffsetDescriptorErrorHandler
					 */
					virtual void processOffsetDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbOfstDescErrCodeType::IdbOfstDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor,
							uint16_t excludedNumber, idbEnabledType::IdbEnabledTypeEnum outlierEnable);

					/**
					 * Inherited from BackgroundDescriptorErrorHandler
					 */
					virtual void processBackgroundDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbBgndDescErrCodeType::IdbBgndDescErrCodeTypeEnum errorCode, WindowDescriptor& windowDescriptor);

					/**
					 * Inherited from WindowDescriptorErrorHandler
					 */
					virtual void processWindowDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum errorCode,
							const WindowDescriptor& windowDescriptor);

					/**
					 * Inherited from StarCatalogErrorHandler
					 */
					virtual void processStarCatalogError(idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum errorCode,
							StarPropertyTcParameters& starPropertyParameters);

					/**
					 * Inherited from MaskLibraryErrorHandler
					 */
					virtual void processMaskLibraryError();

					/**
					 * Inherited from WindowGenerationProgressHandler
					 */
					virtual void processWindowGenerationProgress(const WindowGenerationProgressParameters& parameters);

					/**
					 * Inherited from SmearingWarningHandler
					 */
					virtual void processSmearingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const idbCcdSideType::IdbCcdSideTypeEnum ccdSide,
							const uint32_t smearingIndex, const uint32_t outlierNumber, const float standardDeviation);

					/**
					 * @brief Inherited from ParameterCalculationWarningHandler
					 */
					virtual void processParameterCalculationWarning(const cameraIdentifier::CameraIdentifierEnum ncamId, 
						const idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum pCalcWarCode, const uint32_t descrId,
						const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const double starCcdX, const double starCcdY);

						/**
						 * @brief Inherited from CcdPositionSummaryHandler
						 */
					virtual void processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionSummaryParameters& parameters);
						
						/**
						 * @brief Inherited from CcdPositionDetailedHandler
						 */
					virtual void processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId, const CcdPositionDetailedParameters& parameters);

					/**
					 * Inherited from ObservationStartErrorHandler
					 */
					virtual void processObservationStartError(const cameraIdentifier::CameraIdentifierEnum ncamId,
													const idbObstErrCodeType::IdbObstErrCodeTypeEnum errorCode);

					/*////////////////////////////////////////////*/
					/*///////////////// setters //////////////////*/
					/*////////////////////////////////////////////*/

					/// set the device number of the nominal ICU spacewire interface
					virtual void setNicuSpacewireDeviceNumber(const uint32_t deviceNumber) {
						nicuSpacewireDeviceNumber = deviceNumber;
					}

					/// set the device number of the redundant ICU spacewire interface
					virtual void setRicuSpacewireDeviceNumber(const uint32_t deviceNumber) {
						ricuSpacewireDeviceNumber = deviceNumber;
					}

					/// set the device number of the NCAM-A spacewire interface
					virtual void setNcamaSpacewireDeviceNumber(const uint32_t deviceNumber) {
						ncamaSpacewireDeviceNumber = deviceNumber;
					}

					/// set the device number of the NCAM-B spacewire interface
					virtual void setNcambSpacewireDeviceNumber(const uint32_t deviceNumber) {
						ncambSpacewireDeviceNumber = deviceNumber;
					}

					/// set the attribute EventProducerImpl::nicuSpacewireErrorHandler
					void setNicuSpacewireErrorHandler(GsbSpacewireErrorHandler* spacewireErrorHandler) {
						nicuSpacewireErrorHandler = spacewireErrorHandler;
					}

					/// set the attribute EventProducerImpl::ricuSpacewireErrorHandler
					void setRicuSpacewireErrorHandler(GsbSpacewireErrorHandler* spacewireErrorHandler) {
						ricuSpacewireErrorHandler = spacewireErrorHandler;
					}

					/// set the attribute EventProducerImpl::ncamaSpacewireErrorHandler
					void setNcamaSpacewireErrorHandler(GsbSpacewireErrorHandler* spacewireErrorHandler) {
						ncamaSpacewireErrorHandler = spacewireErrorHandler;
					}

					/// set the attribute EventProducerImpl::ncambSpacewireErrorHandler
					void setNcambSpacewireErrorHandler(GsbSpacewireErrorHandler* spacewireErrorHandler) {
						ncambSpacewireErrorHandler = spacewireErrorHandler;
					}

					/// set the attribute EventProducerImpl::ncamaRmapErrorHandler
					void setNcamaRmapEventHandler(GsbRmapErrorHandler* eventHandler) {
						ncamaRmapEventHandler = eventHandler;
					}

					/// set the attribute EventProducerImpl::ncamaRmapErrorHandler
					void setNcambRmapEventHandler(GsbRmapErrorHandler* eventHandler) {
						ncambRmapEventHandler = eventHandler;
					}

					/// set the attribute EventProducerImpl::initialisationProgressHandler
					void setInitialisationProgressHandler(InitialisationProgressHandler* initialisationProgressHandler) {
						this->initialisationProgressHandler = initialisationProgressHandler;
					}

					/// set the attribute EventProducerImpl::initialisationErrorHandler
					void setInitialisationErrorHandler(InitialisationErrorHandler* initialisationErrorHandler) {
						this->initialisationErrorHandler = initialisationErrorHandler;
					}

					/// set the attribute EventProducerImpl::ncamaCameraFeeDiagnosisIrHandler
					void setNcamaCameraFeeDiagnosisIrHandler(CameraFeeDiagnosisIrHandler* handler) {
						ncamaCameraFeeDiagnosisIrHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisIrHandler
					void setNcambCameraFeeDiagnosisIrHandler(CameraFeeDiagnosisIrHandler* handler) {
						ncambCameraFeeDiagnosisIrHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisIrHandler
					void setNcamaCameraFeeDiagnosisFrHandler(CameraFeeDiagnosisFrHandler* handler) {
						ncamaCameraFeeDiagnosisFrHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisFrHandler
					void setNcambCameraFeeDiagnosisFrHandler(CameraFeeDiagnosisFrHandler* handler) {
						ncambCameraFeeDiagnosisFrHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaCameraFeeDiagnosisIrErrorHandler
					void setNcamaCameraFeeDiagnosisIrErrorHandler(CameraFeeDiagnosisIrErrorHandler* handler) {
						ncamaCameraFeeDiagnosisIrErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisIrErrorHandler
					void setNcambCameraFeeDiagnosisIrErrorHandler(CameraFeeDiagnosisIrErrorHandler* handler) {
						ncambCameraFeeDiagnosisIrErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisIrErrorHandler
					void setNcamaCameraFeeDiagnosisFrErrorHandler(CameraFeeDiagnosisFrErrorHandler* handler) {
						ncamaCameraFeeDiagnosisFrErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambCameraFeeDiagnosisFrErrorHandler
					void setNcambCameraFeeDiagnosisFrErrorHandler(CameraFeeDiagnosisFrErrorHandler* handler) {
						ncambCameraFeeDiagnosisFrErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamCameraFeeConfigurationErrorHandler
					void setNcamCameraFeeConfigurationErrorHandler(CameraFeeConfigurationErrorHandler* handler) {
						this->ncamCameraFeeConfigurationErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::memoryErrorHandler
					void setMemoryErrorHandler(GsbMemoryErrorHandler* memoryErrorHandler) {
						this->memoryErrorHandler = memoryErrorHandler;
					}

					/// set the attribute EventProducerImpl::memoryProtectionErrorHandler
					void setMemoryProtectionErrorHandler(GsbMemoryProtectionErrorHandler* memoryProtectionErrorHandler) {
						this->memoryProtectionErrorHandler = memoryProtectionErrorHandler;
					}

					/// set the attribute EventProducerImpl::monitoringDefinitionErrorHandler
					void setMonitoringDefinitionErrorHandler(GsbMonitoringDefinitionErrorHandler* monitoringDefinitionErrorHandler) {
						this->monitoringDefinitionErrorHandler = monitoringDefinitionErrorHandler;
					}

					/// set the attribute EventProducerImpl::monitoringOutOfLimitEventHandler
					void setMonitoringOutOfLimitEventHandler(GsbMonitoringOutOfLimitEventHandler* monitoringOutOfLimitEventHandler) {
						this->monitoringOutOfLimitEventHandler = monitoringOutOfLimitEventHandler;
					}

					/// et the attribute EventProducerImpl::parameterManagementErrorHandler
					void setParameterManagementErrorHandler(GsbParameterManagementErrorHandler* handler) {
						this->parameterManagementErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ahbstatErrorHandler
					void setAhbstatErrorHandler(GsdAhbstatErrorHandler* handler) {
						this->ahbstatErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamDuplicateIdErrorHandler
					void setNcamDuplicateIdErrorHandler(DuplicateIdErrorHandler* ncamDuplicateIdErrorHandler) {
						this->ncamDuplicateIdErrorHandler = ncamDuplicateIdErrorHandler;;
					}

					/// set the attribute EventProducerImpl::circularBufferEventHandler
					void setCircularBufferEventHandler(GscCircularBufferEventHandler* circularBufferEventHandler) {
						this->circularBufferEventHandler = circularBufferEventHandler;
					}

					/// set the attribute EventProducerImpl::nicuSpacewireLinkErrorHandler
					void setNicuSpacewireLinkErrorHandler(GsbSpacewireLinkErrorHandler* handler) {
						nicuSpacewireLinkErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ricuSpacewireLinkErrorHandler
					void setRicuSpacewireLinkErrorHandler(GsbSpacewireLinkErrorHandler* handler) {
						ricuSpacewireLinkErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaSpacewireLinkErrorHandler
					void setNcamaSpacewireLinkErrorHandler(GsbSpacewireLinkErrorHandler* handler) {
						ncamaSpacewireLinkErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambSpacewireLinkErrorHandler
					void setNcambSpacewireLinkErrorHandler(GsbSpacewireLinkErrorHandler* handler) {
						ncambSpacewireLinkErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::nicuSpacewireLinkProgressHandler
					void setNicuSpacewireLinkProgressHandler(GsbSpacewireLinkProgressHandler* handler) {
						nicuSpacewireLinkProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ricuSpacewireLinkProgressHandler
					void setRicuSpacewireLinkProgressHandler(GsbSpacewireLinkProgressHandler* handler) {
						ricuSpacewireLinkProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaSpacewireLinkProgressHandler
					void setNcamaSpacewireLinkProgressHandler(GsbSpacewireLinkProgressHandler* handler) {
						ncamaSpacewireLinkProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambSpacewireLinkProgressHandler
					void setNcambSpacewireLinkProgressHandler(GsbSpacewireLinkProgressHandler* handler) {
						ncambSpacewireLinkProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ndpuTimecodeErrorHandler
					void setNdpuTimecodeErrorHandler(GsbTimecodeErrorHandler* handler) {
						ndpuTimecodeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaTimecodeErrorHandler
					void setNcamaTimecodeErrorHandler(GsbTimecodeErrorHandler* handler) {
						ncamaTimecodeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambTimecodeErrorHandler
					void setNcambTimecodeErrorHandler(GsbTimecodeErrorHandler* handler) {
						ncambTimecodeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaDataErrorHandler
					void setNcamaCameraDataErrorHandler(CameraDataErrorHandler* handler) {
						ncamaDataErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaDataHeaderWarningHandler
					void setNcamaCameraDataHeaderWarningHandler(CameraDataHeaderWarningHandler* handler) {
						ncamaDataHeaderWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaDataContentWarningHandler
					void setNcamaCameraDataContentWarningHandler(CameraDataContentWarningHandler* handler) {
						ncamaDataContentWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaDataUnexpectedWarningHandler
					void setNcamaCameraDataUnexpectedWarningHandler(CameraDataUnexpectedWarningHandler* handler) {
						ncamaDataUnexpectedWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaTimecodeProgressHandler
					void setNcamaCameraTimecodeProgressHandler(CameraTimecodeProgressHandler* handler) {
						ncamaTimecodeProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaModeProgressHandler
					void setNcamaCameraModeProgressHandler(CameraModeProgressHandler* handler) {
						ncamaModeProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaModeErrorHandler
					void setNcamaCameraModeErrorHandler(CameraModeErrorHandler* handler) {
						ncamaModeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfHalfCcdReadoutInformativeHandler
					void setNcamaCameraEocrInformativeHandler(CameraEndOfHalfCcdReadoutInformativeHandler* handler) {
						ncamaEndOfHalfCcdReadoutInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfHalfCcdReadoutErrorHandler
					void setNcamaCameraEocrErrorHandler(CameraEndOfHalfCcdReadoutErrorHandler* handler) {
						ncamaEndOfHalfCcdReadoutErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfFullimageAcqInformativeHandler
					void setNcamaCameraEofaInformativeHandler(CameraEndOfFullimageAcqInformativeHandler* handler) {
						ncamaEndOfFullimageAcqInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfFullimageAcqErrorHandler
					void setNcamaCameraEofaErrorHandler(CameraEndOfFullimageAcqErrorHandler* handler) {
						ncamaEndOfFullimageAcqErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfParallelTrapPumpingInformativeHandler
					void setNcamaEndOfParallelTrapPumpingInformativeHandler(CameraEndOfParallelTrapPumpingInformativeHandler* handler) {
						ncamaEndOfParallelTrapPumpingInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfParallelTrapPumpingErrorHandler
					void setNcamaEndOfParallelTrapPumpingErrorHandler(CameraEndOfParallelTrapPumpingErrorHandler* handler) {
						ncamaEndOfParallelTrapPumpingErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfSerialTrapPumpingInformativeHandler
					void setNcamaEndOfSerialTrapPumpingInformativeHandler(CameraEndOfSerialTrapPumpingInformativeHandler* handler) {
						ncamaEndOfSerialTrapPumpingInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfSerialTrapPumpingErrorHandler
					void setNcamaEndOfSerialTrapPumpingErrorHandler(CameraEndOfSerialTrapPumpingErrorHandler* handler) {
						ncamaEndOfSerialTrapPumpingErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfWindowAcquisitionProgressHandler
					void setNcamaEndOfWindowAcquisitionProgressHandler(EndOfWindowAcquisitionProgressHandler* handler) {
						ncamaEndOfWindowAcquisitionProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaEndOfWindowAcquisitionErrorHandler
					void setNcamaEndOfWindowAcquisitionErrorHandler(EndOfWindowAcquisitionErrorHandler* handler) {
						ncamaEndOfWindowAcquisitionErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaMaskAndPositionUpdateErrorHandler
					void setNcamaMaskAndPositionUpdateErrorHandler(MaskAndPositionUpdateErrorHandler* handler) {
						ncamaMaskAndPositionUpdateErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaStartMaskAndPositionUpdateErrorHandler
					void setNcamaStartMaskAndPositionUpdateErrorHandler(StartMaskAndPositionUpdateErrorHandler* handler) {
						ncamaStartMaskAndPositionUpdateErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamaMaskAndPositionUpdateInformativeHandler
					void setNcamaMaskAndPositionUpdateInformativeHandler(MaskAndPositionUpdateInformativeHandler* handler) {
						ncamaMaskAndPositionUpdateInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambDataErrorHandler
					void setNcambCameraDataErrorHandler(CameraDataErrorHandler* handler) {
						ncambDataErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambDataHeaderWarningHandler
					void setNcambCameraDataHeaderWarningHandler(CameraDataHeaderWarningHandler* handler) {
						ncambDataHeaderWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambDataContentWarningHandler
					void setNcambCameraDataContentWarningHandler(CameraDataContentWarningHandler* handler) {
						ncambDataContentWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambDataUnexpectedWarningHandler
					void setNcambCameraDataUnexpectedWarningHandler(CameraDataUnexpectedWarningHandler* handler) {
						ncambDataUnexpectedWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambTimecodeProgressHandler
					void setNcambCameraTimecodeProgressHandler(CameraTimecodeProgressHandler* handler) {
						ncambTimecodeProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambModeProgressHandler
					void setNcambCameraModeProgressHandler(CameraModeProgressHandler* handler) {
						ncambModeProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambModeErrorHandler
					void setNcambCameraModeErrorHandler(CameraModeErrorHandler* handler) {
						ncambModeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfHalfCcdReadoutInformativeHandler
					void setNcambCameraEocrInformativeHandler(CameraEndOfHalfCcdReadoutInformativeHandler* handler) {
						ncambEndOfHalfCcdReadoutInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfHalfCcdReadoutErrorHandler
					void setNcambCameraEocrErrorHandler(CameraEndOfHalfCcdReadoutErrorHandler* handler) {
						ncambEndOfHalfCcdReadoutErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfFullimageAcqInformativeHandler
					void setNcambCameraEofaInformativeHandler(CameraEndOfFullimageAcqInformativeHandler* handler) {
						ncambEndOfFullimageAcqInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfFullimageAcqErrorHandler
					void setNcambCameraEofaErrorHandler(CameraEndOfFullimageAcqErrorHandler* handler) {
						ncambEndOfFullimageAcqErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfParallelTrapPumpingInformativeHandler
					void setNcambEndOfParallelTrapPumpingInformativeHandler(CameraEndOfParallelTrapPumpingInformativeHandler* handler) {
						ncambEndOfParallelTrapPumpingInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfParallelTrapPumpingErrorHandler
					void setNcambEndOfParallelTrapPumpingErrorHandler(CameraEndOfParallelTrapPumpingErrorHandler* handler) {
						ncambEndOfParallelTrapPumpingErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfSerialTrapPumpingInformativeHandler
					void setNcambEndOfSerialTrapPumpingInformativeHandler(CameraEndOfSerialTrapPumpingInformativeHandler* handler) {
						ncambEndOfSerialTrapPumpingInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfSerialTrapPumpingErrorHandler
					void setNcambEndOfSerialTrapPumpingErrorHandler(CameraEndOfSerialTrapPumpingErrorHandler* handler) {
						ncambEndOfSerialTrapPumpingErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfWindowAcquisitionProgressHandler
					void setNcambEndOfWindowAcquisitionProgressHandler(EndOfWindowAcquisitionProgressHandler* handler) {
						ncambEndOfWindowAcquisitionProgressHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambEndOfWindowAcquisitionErrorHandler
					void setNcambEndOfWindowAcquisitionErrorHandler(EndOfWindowAcquisitionErrorHandler* handler) {
						ncambEndOfWindowAcquisitionErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambMaskAndPositionUpdateErrorHandler
					void setNcambMaskAndPositionUpdateErrorHandler(MaskAndPositionUpdateErrorHandler* handler) {
						ncambMaskAndPositionUpdateErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambStartMaskAndPositionUpdateErrorHandler
					void setNcambStartMaskAndPositionUpdateErrorHandler(StartMaskAndPositionUpdateErrorHandler* handler) {
						ncambStartMaskAndPositionUpdateErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncambMaskAndPositionUpdateInformativeHandler
					void setNcambMaskAndPositionUpdateInformativeHandler(MaskAndPositionUpdateInformativeHandler* handler) {
						ncambMaskAndPositionUpdateInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::modeManagerProgressHandler
					void setModeManagerProgressHandler(ModeManagerProgressHandler* eventProducer) {
						this->modeManagerProgressHandler = eventProducer;
					}

					/// set the attribute EventProducerImpl::modeManagerErrorHandler
					void setModeManagerErrorHandler(ModeManagerErrorHandler* eventProducer) {
						this->modeManagerErrorHandler = eventProducer;
					}

					/// set the attribute EventProducerImpl::timeUpdateInformativeHandler
					void setTimeUpdateInformativeHandler(TimeUpdateInformativeHandler* handler) {
						this->timeUpdateInformativeHandler = handler;
					}

					/// set the attribute EventProducerImpl::timeErrorHandler
					void setTimeErrorHandler(TimeErrorHandler* handler) {
						this->timeErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::tcVerificationErrorHandler
					void setTcVerificationErrorHandler(GsbTcVerificationErrorHandlerInst* handler) {
						this->tcVerificationErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaWindowProgrammingErrorHandler
					void setNcamaWindowProgrammingErrorHandler(WindowProgrammingErrorHandler* handler) {
						this->ncamaWindowProgrammingErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambWindowProgrammingErrorHandler
					void setNcambWindowProgrammingErrorHandler(WindowProgrammingErrorHandler* handler) {
						this->ncambWindowProgrammingErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaWindowProgrammingWarningHandler
					void setNcamaWindowProgrammingWarningHandler(WindowProgrammingWarningHandler* handler) {
						this->ncamaWindowProgrammingWarningHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambWindowProgrammingWarningHandler
					void setNcambWindowProgrammingWarningHandler(WindowProgrammingWarningHandler* handler) {
						this->ncambWindowProgrammingWarningHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaWindowProgrammingProgressHandler
					void setNcamaWindowProgrammingProgressHandler(WindowProgrammingProgressHandler* handler) {
						this->ncamaWindowProgrammingProgressHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambWindowProgrammingProgressHandler
					void setNcambWindowProgrammingProgressHandler(WindowProgrammingProgressHandler* handler) {
						this->ncambWindowProgrammingProgressHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamStarDescriptorErrorHandler
					void setNcamStarDescriptorErrorHandler(StarDescriptorErrorHandler* handler) {
						this->ncamStarDescriptorErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamStarExtendedDescriptorErrorHandler
					void setNcamStarExtendedDescriptorErrorHandler(StarExtendedDescriptorErrorHandler* ncamStarExtendedDescriptorErrorHandler) {
						this->ncamStarExtendedDescriptorErrorHandler = ncamStarExtendedDescriptorErrorHandler;
					}

					///set the attribute EventProducerImpl::ncamOffsetDescriptorErrorHandler
					void setNcamOffsetDescriptorErrorHandler(OffsetDescriptorErrorHandler* handler) {
						this->ncamOffsetDescriptorErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamImagetteDescriptorErrorHandler
					void setNcamImagetteDescriptorErrorHandler(ImagetteDescriptorErrorHandler* handler) {
						this->ncamImagetteDescriptorErrorHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamImagetteExtendedDescriptorErrorHandler
					void setNcamImagetteExtendedDescriptorErrorHandler(ImagetteExtendedDescriptorErrorHandler* handler) {
						this->ncamImagetteExtendedDescriptorErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamBackgroundDescriptorErrorHandler
					void setNcamBackgroundDescriptorErrorHandler(BackgroundDescriptorErrorHandler* handler) {
						this->ncamBackgroundDescriptorErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamWindowDescriptorErrorHandler
					void setNcamWindowDescriptorErrorHandler(WindowDescriptorErrorHandler* handler) {
						this->ncamWindowDescriptorErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ndpuStarCatalogErrorHandler
					void setNdpuStarCatalogErrorHandler(StarCatalogErrorHandler* handler) {
						this->ndpuStarCatalogErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ndpuMaskLibraryErrorHandler
					void setNdpuMaskLibraryErrorHandler(MaskLibraryErrorHandler* handler) {
						this->ndpuMaskLibraryErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaWindowGenerationProgressHandler
					void setNcamaWindowGenerationProgressHandler(WindowGenerationProgressHandler* handler) {
						this->ncamaWindowGenerationProgressHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambWindowGenerationProgressHandler
					void setNcambWindowGenerationProgressHandler(WindowGenerationProgressHandler* handler) {
						this->ncambWindowGenerationProgressHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaSmearingWarningHandler
					void setNcamaSmearingWarningHandler(SmearingWarningHandler* handler) {
						this->ncamaSmearingWarningHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambSmearingWarningHandler
					void setNcambSmearingWarningHandler(SmearingWarningHandler* handler) {
						this->ncambSmearingWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamParameterCalculationWarningHandler
					void setNcamParameterCalculationWarningHandler(ParameterCalculationWarningHandler* handler) {
						this->ncamParameterCalculationWarningHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamCcdPositionSummaryHandler
					void setNcamCcdPositionSummaryHandler(CcdPositionSummaryHandler* handler) {
						this->ncamCcdPositionSummaryHandler = handler;
					}

					/// set the attribute EventProducerImpl::ncamCcdPositionDetailedHandler
					void setNcamCcdPositionDetailedHandler(CcdPositionDetailedHandler* handler) {
						this->ncamCcdPositionDetailedHandler = handler;
					}

					///set the attribute EventProducerImpl::ncamaObservationStartErrorHandler
					void setNcamaObservationStartErrorHandler(ObservationStartErrorHandler* handler) {
						this->ncamaObservationStartErrorHandler = handler;
					}

					///set the attribute EventProducerImpl::ncambObservationStartErrorHandler
					void setNcambObservationStartErrorHandler(ObservationStartErrorHandler* handler) {
						this->ncambObservationStartErrorHandler = handler;
					}
				};
			}
		}
	}
}

using plato::das::l0app::events::EventProducerImpl;

#endif /* L0APPEVENTPRODUCERIMPL_HPP_ */
