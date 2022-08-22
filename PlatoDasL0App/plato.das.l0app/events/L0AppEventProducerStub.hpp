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

 L0AppEventProducerStub.hpp

 $Rev: 0 $

 ************************************************************************/

#ifndef L0APPEVENTPRODUCERSTUB_HPP
#define L0APPEVENTPRODUCERSTUB_HPP

#include <stdint.h>
#include <plato/das/l0app/events/L0AppEventProducer.hpp>
#include <gericos/core/realtime/GscTask.hpp>
#include <plato/das/l0app/time/L0AppTimeUpdateInformativeHandler.hpp>
#include <gericos/blocks/math/GsbIeee754.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				class EventProducerStub: public GscTask, public EventProducer {
				private:
					EventProducerStub(EventProducerStub const* source);					///< Non functional copy constructor
					EventProducerStub(EventProducerStub const& source);					///< Non functional copy constructor
					EventProducerStub& operator=(EventProducerStub& eventProducerStub); ///< Non functional overload of the assignment operator

				protected:
					EventProducer* eventProducer;

				public:
					enum MESSAGES {
						PROCESS_SPACEWIRE_ERROR = 1,
						PROCESS_RMAP_ERROR = 2,
						PROCESS_INITIALISATION_PROGRESS = 3,
						PROCESS_MEMORY_ERROR = 4,
						PROCESS_CIRCULAR_BUFFER_EVENT = 5,
						PROCESS_TIMECODE_ERROR = 6,
						PROCESS_SPACEWIRE_LINK_ERROR = 8,
						PROCESS_SPACEWIRE_LINK_PROGRESS = 9,
						PROCESS_CAMERA_DATA_ERROR = 10,
						PROCESS_CAMERA_DATA_HEADER_WARNING = 11,
						PROCESS_CAMERA_DATA_CONTENT_WARNING = 12,
						PROCESS_CAMERA_DATA_UNEXPECTED_WARNING = 13,
						PROCESS_CAMERA_TIMECODE_PROGRESS = 14,
						PROCESS_CAMERA_MODE_PROGRESS = 15,
						PROCESS_CAMERA_MODE_ERROR = 16,
						PROCESS_EOCR_INFORMATIVE = 17,
						PROCESS_EOCR_ERROR = 18,
						PROCESS_EOFA_INFORMATIVE = 19,
						PROCESS_EOFA_ERROR = 20,
						PROCESS_EOPTP_INFORMATIVE = 21,
						PROCESS_EOPTP_ERROR = 22,
						PROCESS_EOSTP_INFORMATIVE = 23,
						PROCESS_EOSTP_ERROR = 24,
						PROCESS_INITIALISATION_ERROR = 25,
						PROCESS_TRANSITION_MODE_PROGRESS = 26,
						PROCESS_TRANSITION_MODE_ERROR = 27,
						PROCESS_CAMERA_DIAGNOSIS_IR = 28,
						PROCESS_CAMERA_DIAGNOSIS_IR_ERR = 29,
						PROCESS_CAMERA_DIAGNOSIS_FR = 30,
						PROCESS_CAMERA_DIAGNOSIS_FR_ERR = 31,
						PROCESS_TIME_UPDATE_INFORMATIVE = 32,
						PROCESS_TIME_ERROR = 33,
						PROCESS_TC_VERIFICATION_ERROR = 34,
						PROCESS_MEMORY_PROTECTION_ERROR = 35,
						PROCESS_WINDOW_DESCRIPTOR_ERROR = 36,
						PROCESS_WINDOW_PROGRAMMING_ERROR = 37,
						PROCESS_WINDOW_PROGRAMMING_WARNING = 38,
						PROCESS_WINDOW_PROGRAMMING_PROGRESS = 39,
						PROCESS_MASK_AND_POSITION_UPDATE_ERROR = 40,
						PROCESS_MASK_AND_POSITION_UPDATE_INFORMATIVE = 41,
						PROCESS_WINDOW_GENERATION_PROGRESS = 42,
						PROCESS_EOWA_PROGRESS = 43,
						PROCESS_EOWA_ERROR = 44,
						PROCESS_AHBSTAT_ERROR = 45,
						PROCESS_DUPLICATE_ID_ERROR = 46,
						PROCESS_STAR_EXTENDED_DESCRIPTOR_ERROR = 47,
						PROCESS_MONITORING_DEFINITION_ERROR = 48,
						PROCESS_MONITORING_OUT_OF_LIMIT_EVENT = 49,
						PROCESS_OFFSET_DESCRIPTOR_ERROR = 50,
						PROCESS_BACKGROUND_DESCRIPTOR_ERROR = 51,
						PROCESS_STAR_CATALOG_ERROR = 52,
						PROCESS_STAR_DESCRIPTOR_ERROR = 53,
						PROCESS_IMAGETTE_DESCRIPTOR_ERROR = 54,
						PROCESS_IMAGETTE_EXTENDED_DESCRIPTOR_ERROR = 55,
						PROCESS_MASK_LIBRARY_ERROR = 56,
						PROCESS_NFEE_CONFIGURATION_ERROR = 57,
						PROCESS_SMEARING_WARNING = 58,
						PROCESS_OBSERVATION_START_ERROR = 59,
						PROCESS_PARAMETER_MANAGEMENT_ERROR = 60,
						PROCESS_START_MASK_AND_POSITION_UPDATE_ERROR = 61,
						PROCESS_PARAMETER_CALCULATION_WARNING = 62,
						PROCESS_CCD_POSITION_SUMMARY = 63,
						PROCESS_CCD_POSITION_DETAILED = 64
					};

					EventProducerStub();

					/**
					 * Set eventProducer which is NULL at the beginning
					 * @param eventProducer new value of attribute eventProducer
					 */
					void setEventProducer(EventProducer* eventProducer);

					/**
					 * Inherited from EventProducer
					 */
					virtual void setNicuSpacewireDeviceNumber(const uint32_t deviceNumber);

					/**
					 * Inherited from EventProducer
					 */
					virtual void setRicuSpacewireDeviceNumber(const uint32_t deviceNumber);

					/**
					 * Inherited from EventProducer
					 */
					virtual void setNcamaSpacewireDeviceNumber(const uint32_t deviceNumber);

					/**
					 * Inherited from EventProducer
					 */
					virtual void setNcambSpacewireDeviceNumber(const uint32_t deviceNumber);

					/**
					 * Call one of the method of gsdGrspwManager.
					 * @param message define the method to call.
					 */
					virtual void processMessage(GscTaskMessage* message);

					/**
					 * Inherited from GsbSpacewireErrorHandler.
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processSpacewireError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireError::GsbSpacewireErrorEnum errorType, unsigned int errorCounter,
							const GsbSpacewireErrorParameter& params);

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
					 * Inherited from GsbRmapErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processRmapError(GsbRmapManager* rmapManager, uint32_t deviceNumber,
							gsbRmapErrorCode::GsbRmapErrorCodeEnum errorType, unsigned short errorCounter, GsbRmapErrorParameter parameters);
					/**
					 * Inherited from GsbTimecodeErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processTimecodeError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbTimecodeError::GsbTimecodeErrorEnum errorType, unsigned int errorCounter, unsigned char currentTimecode,
							unsigned char expectedTimecode);
					/**
					 * Inherited from InitialisationErrorHandler::processInitialisationError
					 */
					virtual void processInitialisationError(idbInitErrCodeType::IdbInitErrCodeTypeEnum errCode, uint32_t wrongValue,
							uint32_t usedValue);
					/**
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processInitialisationProgress();

					/**
					 * Inherited from GsbMemoryErrorHandler
					 */
					virtual void processMemoryError(uint32_t nbWrongBytes, uint16_t apid, gsbMemoryErrorCode::GsbMemoryErrorCodeEnum errorType,
							uint32_t errorCounter, uint8_t* failureAddress, uint32_t expectedValue, uint32_t readValue);

					/**
					 * Inherited from GsbMemoryProtectionErrorHandler
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
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCircularBufferEvent(uint32_t bufferId, gscCircularBufferEvent::GscCircularBufferEventEnum eventCode,
							uint32_t eventCounter);

					/**
					 * Inherited from GsbSpacewireLinkErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processSpacewireLinkError(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireLinkError::GsbSpacewireLinkErrorEnum errorType, unsigned int errorCounter);

					/**
					 * Inherited from GsbSpacewireLinkProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processSpacewireLinkProgress(GsbSpacewireManager* packetManager, uint32_t deviceNumber,
							gsbSpacewireLinkProgress::GsbSpacewireLinkProgressEnum eventType);

					/**
					 * Inherited from CameraDataErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraDataError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter);

					/**
					 * Inherited from CameraDataHeaderWarningHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraDataHeaderWarning(idbNcamDataHeaderWarCodeType::IdbNcamDataHeaderWarCodeTypeEnum warningCode,
							uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters);

					/**
					 * Inherited from CameraDataContentWarningHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraDataContentWarning(idbNcamDataContentWarCodeType::IdbNcamDataContentWarCodeTypeEnum warningCode,
							uint32_t warningCounter, const CameraDataContentWarningParameters& parameters);

					/**
					 * Inherited from CameraDataUnexpectedWarningHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraDataUnexpectedWarning(uint32_t warningCounter, const CameraDataHeaderWarningParameters& parameters);

					/**
					 * Inherited from CameraTimecodeProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraTimecodeProgress(cameraIdentifier::CameraIdentifierEnum ncamId, uint8_t timecode, GsbTime time);

					/**
					 * Inherited from CameraModeProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraModeProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
							idbNcamModeType::IdbNcamModeTypeEnum previousMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
							idbTransReasonType::IdbTransReasonTypeEnum reason);

					/**
					 * Inherited from CameraModeErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processCameraModeError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t errorCounter,
							idbNcamModeType::IdbNcamModeTypeEnum expectedMode, idbNcamModeType::IdbNcamModeTypeEnum currentMode,
							idbTransReasonType::IdbTransReasonTypeEnum reason);

					/**
					 * Inherited from CameraEndOfHalfCcdReadoutInformativeHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfHalfCcdReadoutInformative(const CameraEndOfHalfCcdReadoutParameters& parameters);

					/**
					 * Inherited from CameraEndOfHalfCcdReadoutErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfHalfCcdReadoutError(idbEocrErrCodeType::IdbEocrErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraEndOfHalfCcdReadoutParameters& parameters, uint32_t expectedPacketNumber, uint32_t expectedPixelNumber);

					/**
					 * Inherited from CameraEndOfFullimageAcqInformativeHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfFullimageAcqInformative(const CameraEndOfFullimageAcqParameters& parameters);

					/**
					 * Inherited from CameraEndOfFullimageAcqErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfFullimageAcqError(idbEofaErrCodeType::IdbEofaErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraEndOfFullimageAcqParameters& parameters, uint32_t errorFramesNumber);

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
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfSerialTrapPumpingInformative(const CameraEndOfSerialTrapPumpingParameters& parameters);

					/**
					 * Inherited from CameraEndOfSerialTrapPumpingErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfSerialTrapPumpingError(idbEostpErrCodeType::IdbEostpErrCodeTypeEnum errorCode, uint32_t errorCounter,
							const CameraEndOfSerialTrapPumpingParameters& parameters, uint32_t requestedImagesNumber);

					/**
					 * Inherited from EndOfWindowAcquisitionProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfWindowAcquisitionProgress(cameraIdentifier::CameraIdentifierEnum ncamId,
													const EndOfWindowAcquisitionProgressParameters& parameters);

					/**
					 * Inherited from EndOfWindowAcquisitionErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processEndOfWindowAcquisitionError(cameraIdentifier::CameraIdentifierEnum ncamId,
													const EndOfWindowAcquisitionErrorParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const MaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processStartMaskAndPositionUpdateError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const StartMaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from MaskAndPositionUpdateInformativeHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processMaskAndPositionUpdateProgress(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const StartMaskAndPositionUpdateParameters& parameters);

					/**
					 * Inherited from CameraFeeDiagnosisIrHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processFeeDiagnosisIr(const CameraFeeDiagnosisResultParameters& parameters);

					/**
					 * Inherited from CameraFeeDiagnosisFrHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processFeeDiagnosisFr(const CameraFeeDiagnosisResultParameters& parameters);

					/**
					 * Inherited from CameraFeeDiagnosisIrErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processFeeDiagnosisIrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
							idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels,
							uint32_t wrongWindows, uint16_t otherErrors,
							ErroneousPixel& firstErroneousPixel);

					/**
					 * Inherited from CameraFeeDiagnosisFrErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processFeeDiagnosisFrError(const CameraFeeDiagnosisResultParameters& parameters, uint32_t errorCounter,
							idbDiagErrCodeType::IdbDiagErrCodeTypeEnum phase, uint32_t wrongPixels,
							uint32_t wrongHalfCcds, uint32_t wrongWindows,
							uint16_t otherErrors);

					/**
					 * Inherited from CameraFeeConfigurationErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processFeeConfigurationError(cameraIdentifier::CameraIdentifierEnum ncamId);

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
					 * Inherited from StarCatalogErrorHandler
					 */
					virtual void processStarCatalogError(idbStarCatalogErrCodeType::IdbStarCatalogErrCodeTypeEnum errorCode,
							StarPropertyTcParameters& starPropertyParameters);

					/**
					 * Inherited from MaskLibraryErrorHandler
					 */
					virtual void processMaskLibraryError();

					/**
					 * Inherited from WindowProgrammingErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processWindowProgrammingError(const cameraIdentifier::CameraIdentifierEnum ncamId,
							const idbWinProgErrCodeType::IdbWinProgErrCodeTypeEnum errorCode,
							const idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, const uint32_t windowId);

					/**
					 * Inherited from WindowProgrammingWarningHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processWindowProgrammingWarning(const cameraIdentifier::CameraIdentifierEnum ncamId);

					/**
					 * Inherited from WindowProgrammingProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processWindowProgrammingProgress(const WindowProgrammingProgressParameters& parameters);

					/**
					 * Inherited from GsdAhbstatErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processAhbstatError(GsdAhbstatManager* ahbstatManager, gsdAhbstatError::GsdAhbstatErrorEnum errorType,
							uint32_t ahbFailingAddressCounter, uint32_t ahbStatusRegister, uint32_t ahbFailingAddress, uint32_t ahbFailingAddressValue);

					/**
					 * Inherited from DuplicateIdErrorHandler
					 */
					virtual void processDuplicateIdError(cameraIdentifier::CameraIdentifierEnum ncamId, uint32_t descriptorId,
							idbDescrTypeType::IdbDescrTypeTypeEnum previousType, idbDescrTypeType::IdbDescrTypeTypeEnum requestedType);

					/**
					 * Inherited from ModeManagerProgressHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processTransitionModeProgress(idbNdpuModeType::IdbNdpuModeTypeEnum currentMode,
							idbNdpuModeType::IdbNdpuModeTypeEnum previousMode);

					/**
					 * Inherited from ModeManagerErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processTransitionModeError(idbModeErrCodeType::IdbModeErrCodeTypeEnum errorCode,
							idbNdpuModeType::IdbNdpuModeTypeEnum currentMode, idbNdpuModeType::IdbNdpuModeTypeEnum expectedMode,
							unsigned int errorCounter);

					/**
					 * Inherited from GsbTcVerificationErrorHandler
					 * Puts the message in the queue to be treat in the task's thread.
					 */
					virtual void processTcVerificationError(GsbTcVerificationReportGenerator<GsbEncapsulatedCcsdsTcPacketInst>* generator,
							uint16_t errorType, uint32_t errorCounter, const GsbTcVerificationErrorParameter& params);

					/**
					 * Inherited from WindowDescriptorErrorHandler
					 */
					virtual void processWindowDescriptorError(cameraIdentifier::CameraIdentifierEnum ncamId,
							windowDescriptorErrorCodeEnum::WindowDescriptorErrorCodeEnum errorCode, const WindowDescriptor& windowDescriptor);

					/**
					 * Inherited from WindowDescriptorErrorHandler
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
				};
			}
		}
	}
}

using plato::das::l0app::events::EventProducerStub;

#endif /* L0APPEVENTPRODUCERSTUB_HPP_ */
