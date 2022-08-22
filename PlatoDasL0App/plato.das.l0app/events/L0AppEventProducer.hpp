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

 L0AppEventProducer.hpp

 $Rev: 0 $

 ************************************************************************/

#ifndef L0APPEVENTPRODUCER_HPP
#define L0APPEVENTPRODUCER_HPP

#include <gericos/blocks/memoryManager/GsbMemoryErrorHandler.hpp>
#include <gericos/blocks/memoryManager/GsbMemoryProtectionErrorHandler.hpp>
#include <gericos/blocks/monitor/GsbMonitoringDefinitionErrorHandler.hpp>
#include <gericos/blocks/monitor/GsbMonitoringOutOfLimitEventHandler.hpp>
#include <gericos/blocks/parameterManagement/GsbParameterManagementErrorHandler.hpp>
#include <gericos/blocks/spacewire/GsbSpacewireErrorHandler.hpp>
#include <gericos/blocks/spacewire/GsbSpacewireLinkErrorHandler.hpp>
#include <gericos/blocks/spacewire/GsbSpacewireLinkProgressHandler.hpp>
#include <gericos/blocks/spacewire/GsbTimecodeErrorHandler.hpp>
#include <gericos/blocks/spacewire/rmap/GsbRmapErrorHandler.hpp>
#include <gericos/core/memory/GscCircularBufferEventHandler.hpp>
#include <gericos/drivers/ahbstat/manager/GsdAhbstatErrorHandler.hpp>
#include <plato/das/instances/gericos/blocks/telecommand/interpreter/GsbTcVerificationErrorHandlerInst.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraDataContentWarningHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraDataErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraDataHeaderWarningHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraDataUnexpectedWarningHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfFullimageAcqErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfFullimageAcqInformativeHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfHalfCcdReadoutErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfHalfCcdReadoutInformativeHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfParallelTrapPumpingErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfParallelTrapPumpingInformativeHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfSerialTrapPumpingErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraEndOfSerialTrapPumpingInformativeHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraFeeDiagnosisFrErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraFeeDiagnosisFrHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraFeeDiagnosisIrErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraFeeDiagnosisIrHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraModeErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraModeProgressHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraTimecodeProgressHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppEndOfWindowAcquisitionErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppEndOfWindowAcquisitionProgressHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppCameraFeeConfigurationErrorHandler.hpp>
#include <plato/das/l0app/camera/events/L0AppObservationStartErrorHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppMaskAndPositionUpdateErrorHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppStartMaskAndPositionUpdateErrorHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppMaskAndPositionUpdateInformativeHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppWindowProgrammingErrorHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppWindowProgrammingProgressHandler.hpp>
#include <plato/das/l0app/camera/windowing/L0AppWindowProgrammingWarningHandler.hpp>
#include <plato/das/l0app/configuration/core0/L0AppInitialisationErrorHandler.hpp>
#include <plato/das/l0app/configuration/core0/L0AppInitialisationProgressHandler.hpp>
#include <plato/das/l0app/mode/L0AppModeManagerErrorHandler.hpp>
#include <plato/das/l0app/mode/L0AppModeManagerProgressHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppSmearingWarningHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppParameterCalculationWarningHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppDuplicateIdErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppStarDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppStarExtendedDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppImagetteDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppImagetteExtendedDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppWindowDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppWindowGenerationProgressHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppOffsetDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/descriptor/L0AppBackgroundDescriptorErrorHandler.hpp>
#include <plato/das/l0app/science/star/L0AppStarCatalogErrorHandler.hpp>
#include <plato/das/l0app/science/mask/L0AppMaskLibraryErrorHandler.hpp>
#include <plato/das/l0app/time/L0AppTimeErrorHandler.hpp>
#include <plato/das/l0app/time/L0AppTimeUpdateInformativeHandler.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				/**
				 * Interface of a task which thread the event handlers.
				 * It's implement the interface of every errorHandler and dispatches the message to the right eventProducer.
				 */
				class EventProducer:
						// buffer, memory
						public virtual GscCircularBufferEventHandler, public virtual GsbMemoryErrorHandler, public virtual GsbMemoryProtectionErrorHandler,
						// monitor
						public virtual GsbMonitoringDefinitionErrorHandler, public virtual GsbMonitoringOutOfLimitEventHandler,
						// parameter management
						public virtual GsbParameterManagementErrorHandler,
						// spacewire, rmap
						public virtual GsbSpacewireErrorHandler, public virtual GsbSpacewireLinkErrorHandler, public virtual GsbSpacewireLinkProgressHandler,
						public virtual GsbTimecodeErrorHandler, public virtual GsbRmapErrorHandler,
						// ahbstat
						public virtual GsdAhbstatErrorHandler,
						// duplicateId
						public virtual DuplicateIdErrorHandler,
						// camera
						public virtual CameraDataErrorHandler, public virtual CameraDataHeaderWarningHandler, public virtual CameraDataContentWarningHandler,
						public virtual CameraDataUnexpectedWarningHandler, public virtual CameraModeProgressHandler, public virtual CameraModeErrorHandler,
						public virtual CameraTimecodeProgressHandler, public virtual CameraFeeConfigurationErrorHandler,
						// full-image acquisition
						public virtual CameraEndOfHalfCcdReadoutInformativeHandler, public virtual CameraEndOfHalfCcdReadoutErrorHandler,
						public virtual CameraEndOfFullimageAcqInformativeHandler, public virtual CameraEndOfFullimageAcqErrorHandler,
						// parallel trap pumping
						public virtual CameraEndOfParallelTrapPumpingInformativeHandler, public virtual CameraEndOfParallelTrapPumpingErrorHandler,
						// serial trap pumping
						public virtual CameraEndOfSerialTrapPumpingInformativeHandler, public virtual CameraEndOfSerialTrapPumpingErrorHandler,
						// window acquisition
						public virtual EndOfWindowAcquisitionProgressHandler, public virtual EndOfWindowAcquisitionErrorHandler,
						// mask and position update
						public virtual MaskAndPositionUpdateInformativeHandler, public virtual MaskAndPositionUpdateErrorHandler,
						public virtual StartMaskAndPositionUpdateErrorHandler,
						// camera diagnosis
						public virtual CameraFeeDiagnosisIrHandler, public virtual CameraFeeDiagnosisIrErrorHandler,
						public virtual CameraFeeDiagnosisFrHandler, public virtual CameraFeeDiagnosisFrErrorHandler,
						// observation
						public virtual ObservationStartErrorHandler,
						// window programming
						public virtual WindowProgrammingErrorHandler, public virtual WindowProgrammingWarningHandler, public virtual WindowProgrammingProgressHandler,
						// init
						public virtual InitialisationErrorHandler, public virtual InitialisationProgressHandler,
						// mode
						public virtual ModeManagerProgressHandler, public virtual ModeManagerErrorHandler,
						// science - calculator
						public virtual SmearingWarningHandler,
						public virtual ParameterCalculationWarningHandler,
						public virtual CcdPositionSummaryHandler,
						public virtual CcdPositionDetailedHandler,
						// science - descriptor
						public virtual StarDescriptorErrorHandler, public virtual StarExtendedDescriptorErrorHandler,
						public virtual ImagetteDescriptorErrorHandler, public virtual ImagetteExtendedDescriptorErrorHandler,
						public virtual OffsetDescriptorErrorHandler, public virtual BackgroundDescriptorErrorHandler,
						public virtual WindowDescriptorErrorHandler, public virtual WindowGenerationProgressHandler,
						// science
						public virtual StarCatalogErrorHandler, public virtual MaskLibraryErrorHandler,
						// TC
						public virtual GsbTcVerificationErrorHandlerInst,
						// time
						public virtual TimeUpdateInformativeHandler, public virtual TimeErrorHandler
						{
				private:
					EventProducer(EventProducer const* source);					///< Non functional copy constructor
					EventProducer(EventProducer const& source);					///< Non functional copy constructor
					EventProducer& operator=(EventProducer& eventProducerStub); ///< Non functional overload of the assignment operator

				public:
					/**
					 * Empty default constructor.
					 */
					EventProducer() {
					}

					virtual void setNicuSpacewireDeviceNumber(const uint32_t deviceNumber) = 0;
					virtual void setRicuSpacewireDeviceNumber(const uint32_t deviceNumber) = 0;
					virtual void setNcamaSpacewireDeviceNumber(const uint32_t deviceNumber) = 0;
					virtual void setNcambSpacewireDeviceNumber(const uint32_t deviceNumber) = 0;
				};
			}
		}
	}
}

using plato::das::l0app::events::EventProducer;

#endif /* L0APPEVENTPRODUCER_HPP */
