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

 L0AppStarsPositionsCalculator.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppStarsPositionsCalculator.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines the implementation of the target star CCD position and initial window position calculation

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 5 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPSTARSPOSITIONSCALCULATOR_HPP_
#define L0APPSTARSPOSITIONSCALCULATOR_HPP_

#include <gericos/blocks/time/GsbTimeProvider.hpp>
#include <plato/das/idb/processes/ncam/parameters/IdbNcamDataPool.hpp>
#include <plato/das/l0app/camera/engine/L0AppCameraConfiguration.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppParameterCalculationWarningHandler.hpp>
#include <plato/das/l0app/science/calculator/L0AppStarsPositionsCalculatorParameterSet.hpp>
#include <plato/das/l0app/science/camera/L0AppAttitude.hpp>
#include <plato/das/l0app/science/camera/L0AppCameraDataSet.hpp>
#include <plato/das/l0app/science/camera/L0AppCommonScienceParameterSet.hpp>
#include <plato/das/l0app/science/camera/L0AppIgmGlobalParameterSet.hpp>
#include <plato/das/l0app/science/star/L0AppStarCatalogue.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {

					/**
					 * @brief This class implement the target star CCD position and initial window position calculation
					 * 
					 * PLATO: Target star CCD position Calculation ATBD, Ref.: PLATO-UOL-PDC-DD-0002
					 * PLATO: N-DPU Onboard star window position ATBD, Ref.: PLATO-LESIA-PDC-DD-0018
					 */
					class StarsPositionsCalculator {
					private:
						static const uint32_t SERIAL_PRESCAN_COL_NUMBER = 25U;	///< number of columns to skip at the beginning of the half-CCD lines
						static const uint32_t CCD_SIZE_PIXEL = 4510U;						///< CCD size in pixels

						/// Non functional copy constructor
						StarsPositionsCalculator(StarsPositionsCalculator const* source);
						/// Non functional copy constructor
						StarsPositionsCalculator(StarsPositionsCalculator const& source);
						/// Non functional overload of the assignment operator
						StarsPositionsCalculator& operator=(StarsPositionsCalculator& source);

						/// Identifier of the camera
						cameraIdentifier::CameraIdentifierEnum cameraId;
						/// Camera data set
						CameraDataSet* cameraDataSet;
						/// Provide time
						GsbTimeProvider* gsbTimeProvider;
						/// Star catalogue containing the stars to be used
						StarCatalogue* starCatalogue;
						/// The pointer to the camera configuration, used to check if a side is activated
						CameraConfiguration* cameraConfiguration;
						/// DataPool used to extract TC parameters
						IdbNcamDataPool* dataPool;

						// ***** Events handlers : *****

						/// Used to emit a TM_NC{#cun}_R_EVT_WAR_PARAMETER_CALCULATION packet when
						/// - a dangerous numerical computation has been encountered (NDPU-SRS-7605),
						/// - a star whose window does not fit inside the CCD side (NDPU-SRS-7611),
						/// - a star whose window is in a CCD side that is disabled (NDPU-SRS-8849),
						/// - the maximum number of N-DPU window descriptors for this CCD has been exceeded (NDPU-SRS-8871).
						ParameterCalculationWarningHandler* parameterCalculationWarningHandler;
						/// Used to emit a TM_NC{#cun}_R_EVT_INF_CCD_POS_SUMMARY packet when all the star window positions have been computed (NDPU-SRS-7766)
						CcdPositionSummaryHandler* ccdPositionSummaryHandler;
						/// Used to emit a TM_NC{#cun}_R_EVT_INF_CCD_POS_DETAILED packet after computing each star window position (NDPU-SRS-7768)
						CcdPositionDetailedHandler* ccdPositionDetailedHandler;

						/// Telescope attitude matrix
						Attitude attitude;
						/// Common science parameters set, used to retrieve the pixel size and the epoch offset
						CommonScienceParameterSet commonScienceParameterSet;
						/// Image geometry model global parameter set, provide the camera offsets and rotation and the distortion coefficients
						IgmGlobalParameterSet igmGlobalParameterSet;

						// Used in the ccdPositionSummary event
						uint32_t numStarPosComp;		//< number of star positions computed
						uint16_t starNumCcd1Right;	// < number of stars in the CCD1 right side
						uint16_t starNumCcd2Right;	// < number of stars in the CCD2 right side
						uint16_t starNumCcd3Right;	// < number of stars in the CCD3 right side
						uint16_t starNumCcd4Right;	// < number of stars in the CCD4 right side
						uint16_t starNumCcd1Left;		// < number of stars in the CCD1 left side
						uint16_t starNumCcd2Left;		// < number of stars in the CCD2 left side
						uint16_t starNumCcd3Left;		// < number of stars in the CCD3 left side
						uint16_t starNumCcd4Left;		// < number of stars in the CCD4 left side

						/**
						 * @brief increment the number of processed stars depending on the CCD id - right side
						 * 
						 * @param ccdId the CCD identifier (the side is already determined to be RIGHT)
						 * @param ccdSide the side of the CCD (RIGHT or LEFT)
						 */
						void incrementNumberOfStarProcessed(idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, idbCcdSideType::IdbCcdSideTypeEnum ccdSide);

						/**
						 * @brief Top level algorithm for the stars window position calculation
						 * 
						 * Ref.: ONB_POSCAL_010
						 * This function is the top level algorithm which calls all the sub-routines.
						 * 
						 * @param errorCode the errorCode to be set in case of error
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * @param commonScienceParameterSet the common science parameter set (used here : pixelSize, epochOffset)
						 * @param igmGlobalParameterSet the image geometry model global parameter set used to retrieve the SD offsets and rotation and the distortion coefficients
						 * @param starProperty the star property extracted from the star catalogue, containing the star coordinates
						 * @param attitude the camera attitude matrix
						 */
						void starBcrs2CcdPosition(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
								StarsPositionsCalculatorParameterSet& starPosCalcParamSet,
								CommonScienceParameterSet commonScienceParameterSet, IgmGlobalParameterSet igmGlobalParameterSet,
								StarProperty* starProperty, Attitude attitude);

						/**
						 * @brief Conversion of catalogue reference star position from BCRS to GRCS, aberration effect
						 * 
						 * Ref.: ONB_POSCAL_020
						 *
						 * Input : epochOffset, currentTime, starCoordinates[2]
						 * Output : starCoordinatesGcrs[3]
						 *
						 * @param errorCode the errorCode to be set in case of error
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * (used here : input : epochOffset, currentTime, starCoordinates[2]; Output : starCoordinatesGcrs[3])
						 * @param commonScienceParameterSet the common science parameter set (used here : epochOffset)
						 *
						 */
						void bcrs2GcrsAberrationCorrection(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
								StarsPositionsCalculatorParameterSet& starPosCalcParamSet,
								CommonScienceParameterSet commonScienceParameterSet);

						/**
						 * @brief Star positions conversion from GCRS to direction in camera boresight reference frame
						 * 
						 * Ref.: ONB_POSCAL_030
						 *
						 * Input : starCoordinatesGcrs[3], attitude[9]
						 * Output : cameraDirection[3]
						 *
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * (used here : input : starCoordinatesGcrs[3], attitude[9]; Output : cameraDirection[3])
						 * @param attitude the camera attitude matrix
						 */
						void gcrs2DirectionCamBoresight(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, Attitude attitude);

						/**
						 * @brief Gnomonic projection and simulation of optical distortion
						 * 
						 * Ref.: ONB_POSCAL_040
						 *
						 * Input : cameraDirection[3], igmGlobalParameterSet
						 * Output : posFocalPlane[2]
						 *
						 * @param errorCode the errorCode to be set in case of error
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * (used here : input : cameraDirection[3]; Output : posFocalPlane[2])
						 * @param imageGeometryModel  the image geometry model global parameter set (used here : distortion coefficients and focalDistance)
						 */
						void gnomonicDistortion(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
								StarsPositionsCalculatorParameterSet& starPosCalcParamSet, IgmGlobalParameterSet imageGeometryModel);

						/**
						 * @brief Coordinate transformation from Focal plane to Silicon die reference frame
						 * 
						 * Ref.: ONB_POSCAL_050 and ONB_POSCAL_060
						 *
						 * Input : posFocalPlane[2], igmGlobalParameterSet, commonScienceParameterSet
						 * Output : ccdId, starSD[2]
						 *
						 * @param errorCode the errorCode to be set in case of error
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * (used here : input : posFocalPlane[2]; Output : ccdId, starSD[2])
						 * @param imageGeometryModel  the image geometry model global parameter set (used here : SD offsets and rotations).
						 * @param commonScienceParameterSet the common science parameter set (used here : pixelSize)
						 */
						void fp2SiliconDie(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
								StarsPositionsCalculatorParameterSet& starPosCalcParamSet,
								CommonScienceParameterSet commonScienceParameterSet, IgmGlobalParameterSet imageGeometryModel);

						/**
						 * @brief Coordinate transformation from Silicon die to CCD/FEE reference frame
						 * 
						 * Ref.: ONB_POSCAL_070
						 *
						 * Input : starSD[2]n, ccdId
						 * Output : starFee[2], ccdSide
						 *
						 * @param errorCode the errorCode to be set in case of error
						 * @param starPosCalcParamSet the super set of the parameters for the stars window position calculation
						 * (used here : input : StarCcd[2]; Output : ccdId, starCcd[2], ccdSide)
						 */
						void siliconDie2FeeCcd(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
								StarsPositionsCalculatorParameterSet& starPosCalcParamSet);

						/**
						 * @brief Check if the star window fit in the CCD Side
						 *
						 * @param starPosCalcParamSet  the super set of the parameters for the stars window position calculation
						 * (used here : input : starFee[2]; Output : starWindowFee[2])
						 * @param windowDescriptor the window descriptor to be set (used here : windowWidth, windowHeight)
						 * @return true if the window fit in the CCD Side, false otherwise
						 */
						bool fitInsideCcd(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, WindowDescriptor* windowDescriptor);

						/**
						 * @brief Once the window origin has been calculated and it fit in the CCD Side, update the window descriptor with the computed values (NDPU-SRS-7765).
						 */
						void processWindow(WindowDescriptorSet* windowDescriptorSet, WindowDescriptor* windowDescriptor, uint32_t starId, StarsPositionsCalculatorParameterSet& starPosCalcParamSet);

						/**
						 * @brief Process Parameter Calculation Warning
						 */
						void processParameterCalcWarning(ParameterCalculationWarningHandler* parameterCalculationWarningHandler, cameraIdentifier::CameraIdentifierEnum cameraId,
								idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum code, uint32_t starId, StarsPositionsCalculatorParameterSet starPosCalcParamSet);

						/**
						 * @brief Process a ccd position summary event
						 */
						void processSummaryEvent(CcdPositionSummaryHandler* ccdPositionSummaryHandler,cameraIdentifier::CameraIdentifierEnum cameraId);

						/**
						 * @brief Reduce an angle (in radian) in the range [-180,180] to the range [-90, 90]
						 * @param angle the angle (in radian) to be reduced
						 */
						double reduceAngle(double angle) {
							// Pi in double precision
							static const double DOUBLE_PI = 3.141592653589793116;
							// DOUBLE_PI / 180 : 0x3F91DF46A2529D39 - convert degrees to radians
							static const double D2R = 0.01745329251994329547;
							double result = angle;
							if ((angle / D2R) > 90.) {
								// If the angle is greater than 90 degrees, reduce it to the range [-90,90]
								result = DOUBLE_PI - angle;
							}
							else if ((angle / D2R) < -90.) {
								// If the angle is lower than -90 degrees, reduce it to the range [-90,90]
								result = -(DOUBLE_PI + angle);
							}
							return result;
						}

					 public:
						/// Default constructor
						StarsPositionsCalculator();
						
						/**
						 * @brief Main function which will calculate the target star CCD position and initial window position calculation for every stars on board in the star catalogue.
						 */
						void calculateStarsPositions();

						/**
						 * @brief Set the connection with the camera data set, the starCatalog and the camera configuration
						 * 
						 * @param cameraDataSet pointer to the camera data set
						 * @param cameraId the camera identifier
						 */
						void init(CameraDataSet* cameraDataSet, const cameraIdentifier::CameraIdentifierEnum cameraId, IdbNcamDataPool* dataPool) {
							this->cameraDataSet = cameraDataSet;
							this->cameraId = cameraId;
							this->dataPool = dataPool;
						}

						/**
						 * @brief Set the connection with the camera data set
						 * 
						 * @param cameraDataSet camera data set
						 */
						void setCameraDataSet(CameraDataSet* cameraDataSet) {
							this->cameraDataSet = cameraDataSet;
						}

						/**
							 * @brief Setter of dataPool
							 * @param dataPool The new dataPool to set
							 */
							void setDataPool(IdbNcamDataPool* dataPool) {
								this->dataPool = dataPool;
							}

						/**
						 * @brief Setter of GsbTimeProvider.
						 * 
						 * @param gsbTimeProvider pointer of the GsbTimeProvider
						 */
						void setGsbTimeProvider(GsbTimeProvider* gsbTimeProvider) {
							this->gsbTimeProvider = gsbTimeProvider;
						}

						/**
						 * @brief Set the connection with the star catalogue.
						 * 
						 * @param starCatalogue pointer of the StarCatalogue
						 */
						void setStarCatalogue(StarCatalogue* starCatalogue) {
							this->starCatalogue = starCatalogue;
						}

						/**
						 * @brief Set the connection with the parameter calculation warning handler.
						 * 
						 * @param handler pointer of the parameter calculation warning handler
						 */
						void setParameterCalculationWarningHandler(ParameterCalculationWarningHandler* parameterCalculationWarningHandler) {
							this->parameterCalculationWarningHandler = parameterCalculationWarningHandler;
						}

						/**
						 * @brief Set the connection with the informative event CcdPosSummary.
						 * 
						 * @param handler pointer of the informative event CcdPosSummary
						 */
						void setCcdPositionSummaryHandler(CcdPositionSummaryHandler* ccdPositionSummaryHandler) {
							this->ccdPositionSummaryHandler = ccdPositionSummaryHandler;
						}
						/**
						 * @brief Set the connection with the informative event CcdPosDetailed.
						 * 
						 * @param handler pointer of the informative event CcdPosDetailed
						 */
						void setCcdPositionDetailedHandler(CcdPositionDetailedHandler* ccdPositionDetailedHandler) {
							this->ccdPositionDetailedHandler = ccdPositionDetailedHandler;
						}
						/**
						 * @brief Set the connection with the camera configuration.
						 *
						 * @param cameraConfiguration pointer to the camera configuration
						 */
						void setCameraConfiguration(CameraConfiguration* cameraConfiguration) {
							this->cameraConfiguration = cameraConfiguration;
						}
					};
				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::StarsPositionsCalculator;

#endif /* L0APPSTARSPOSITIONSCALCULATOR_HPP_ */
