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

 L0AppStarsPositionsCalculator.cpp

 $Rev: 000 $

 ************************************************************************/
#include <gericos/blocks/math/GsbIeee754.hpp>
#include <gericos/blocks/memory/GsbAccessError.hpp>
#include <gericos/blocks/util/GsbMath.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdNumberTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbInitializedTypeEnum.hpp>
#include <plato/das/idb/processes/ncam/parameters/IdbNcamDataPool.hpp>
#include <plato/das/l0app/science/calculator/L0AppStarsPositionsCalculator.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbServiceActivityTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbWlistStatTypeEnum.hpp>

StarsPositionsCalculator::StarsPositionsCalculator() {
	// Reset pointers
	cameraDataSet = 0;
	starCatalogue = 0;
	cameraConfiguration = 0;
	dataPool = 0;
	// Event handler
	parameterCalculationWarningHandler = 0;
	ccdPositionSummaryHandler = 0;
	ccdPositionDetailedHandler = 0;
	// Set default values (0)
	cameraId = cameraIdentifier::NCAM_A;	// < 0
	// Number of processed stars on CCD 1
	starNumCcd1Right = 0U;
	starNumCcd1Left = 0U;
	// Number of processed stars on CCD 2
	starNumCcd2Right = 0U;
	starNumCcd2Left = 0U;
	// Number of processed stars on CCD 3
	starNumCcd3Right = 0U;
	starNumCcd3Left = 0U;
	// Number of processed stars on CCD 4
	starNumCcd4Right = 0U;
	starNumCcd4Left = 0U;
}

/// @metric_deviation[dc_calls] : Reducing direct calls number would create unclear sub-functions which will reduce code visibility.
void StarsPositionsCalculator::calculateStarsPositions() {
	if ((cameraDataSet != 0) && (starCatalogue != 0) && (gsbTimeProvider != 0U) && (cameraConfiguration != 0U) && (dataPool != 0U)) {
		// Retrieve the window descriptor set from the camera data set
		WindowDescriptorSet& windowDescriptorSet = cameraDataSet->getWindowDescriptorSet();
		// Retrieve the star descriptor set from the camera data set
		StarDescriptorSet& starDescriptorSet = cameraDataSet->getStarDescriptorSet();
		// Current window descriptor
		WindowDescriptor* windowDescriptor;
		// Superset of parameters used in the position computation
		StarsPositionsCalculatorParameterSet starPosCalcParamSet;

		// NDPU-SRS-8538 : set PARAM_CALC_ACTV service to WORKING and WLIST_STAT to NON_PROGRAMMED
		dataPool->setItemValue<idbServiceActivityType::IdbServiceActivityTypeEnum>(idbNcamItemId::NCAM_PARAM_CALC_ACTV, idbServiceActivityType::WORKING);
		dataPool->setItemValue<idbWlistStatType::IdbWlistStatTypeEnum>(idbNcamItemId::NCAM_CCD1_WLIST_STAT, idbWlistStatType::NON_PROGRAMMED);
		dataPool->setItemValue<idbWlistStatType::IdbWlistStatTypeEnum>(idbNcamItemId::NCAM_CCD2_WLIST_STAT, idbWlistStatType::NON_PROGRAMMED);
		dataPool->setItemValue<idbWlistStatType::IdbWlistStatTypeEnum>(idbNcamItemId::NCAM_CCD3_WLIST_STAT, idbWlistStatType::NON_PROGRAMMED);
		dataPool->setItemValue<idbWlistStatType::IdbWlistStatTypeEnum>(idbNcamItemId::NCAM_CCD4_WLIST_STAT, idbWlistStatType::NON_PROGRAMMED);

		// Error code set to ERROR_DETECTED first, will be changed to SUCCESS if no error is detected later on
		gsbManagerErrorCode::GsbManagerErrorCodeEnum errorCode;
		// Restult of the function call fitInsideCcd()
		bool isInsideCcd;
		for (uint32_t j = 0U; j < starDescriptorSet.getStarDescriptorNumber(); ++j) {
			// Current star descriptor
			StarDescriptor* starDescriptor = starDescriptorSet.getStarDescriptor(j);
			// NDU-SRS-7567 : Reset the following fileds of each star descriptor for which the positionComputing field is ON_BOARD
			if (starDescriptor->getPositionComputing() == positionComputing::ON_BOARD) {
				windowDescriptor = windowDescriptorSet.getWindowDescriptor(starDescriptor->getWindowDescriptorIndex());
				windowDescriptor->setInitialization(idbInitializedType::NOT_INITIALIZED);
				windowDescriptor->setX(0U);
				windowDescriptor->setY(0U);

				// NDPU-SRS-7569 : For each star in the star descriptor list that has an uninitialized window position
				// Compute the predicted position of the star on the CCD (including determining which CCD number and side the star is)
				// Reset the parameters superset for consistency
				starPosCalcParamSet.reset();
				errorCode = gsbManagerErrorCode::ERROR_DETECTED;
				// NDPU-SRS-7570 - At the beginning of the target star CCD position calculation, the N-DPU ASW shall retrieve the current time and use the same time for all the calculations.
				starPosCalcParamSet.setCurrentTime(static_cast<double>(gsbTimeProvider->getTime().convertIntoMicroseconds()) / 1000000.0);
				// Retrieve the star property from the star catalogue (retrieve the star's coordinates)
				// Current star property
				StarProperty* starProperty = starCatalogue->getStarProperty(starDescriptor->getStarPropertyIndex());
				// Call the top level algorithm which will compute the position of the star on the CCD
				starBcrs2CcdPosition(errorCode, starPosCalcParamSet, cameraDataSet->getCommonScienceParameterSet(),
						cameraDataSet->getImageGeometryModel(), starProperty, cameraDataSet->getAttitudeMatrix());
				// NDPU-SRS-7569 : Compute the initial position of the star window if no error has been previously detected
				if (errorCode == gsbManagerErrorCode::SUCCESS) {
					// Compute the window position and check if it fit inside the CCD
					isInsideCcd = fitInsideCcd(starPosCalcParamSet, windowDescriptor);
				}
				// Error detected during the calculation
				if (errorCode != gsbManagerErrorCode::SUCCESS) {
					// NDPU-SRS-7605 : Dangerouns numerical computation encountered.
					windowDescriptor->setInitialization(idbInitializedType::ERROR_POS_CALC);
					processParameterCalcWarning(parameterCalculationWarningHandler, cameraId, idbParamCalcWarCodeType::NUMERIC_ERROR, starProperty->getId(), starPosCalcParamSet);
				}
				else if (isInsideCcd) {
					processWindow(&windowDescriptorSet, windowDescriptor, starProperty->getId(), starPosCalcParamSet);
				}
				else {
					// NDPU-SRS-7611 - If the star window do not fit in the CCD side, send a warning event message
					windowDescriptor->setInitialization(idbInitializedType::ERROR_POS_CALC);
					processParameterCalcWarning(parameterCalculationWarningHandler,cameraId, idbParamCalcWarCodeType::WINDOW_OUT_OF_CCD, starProperty->getId(), starPosCalcParamSet);
				}
			}
		}
		// NDPU-SRS-7766 When all the star window positions have been computed, the N-DPU ASW shall emit an informative event message TM_NC{$cun}_R_EVT_INF_CCD_POS_SUMMARY
		processSummaryEvent(ccdPositionSummaryHandler, cameraId);
		// NDPU-SRS-8539 : Set PARAM_CALC_ACTV service to STOPPED
		dataPool->setItemValue<idbServiceActivityType::IdbServiceActivityTypeEnum>(idbNcamItemId::NCAM_PARAM_CALC_ACTV, idbServiceActivityType::STOPPED);
	}
}

void StarsPositionsCalculator::processWindow(WindowDescriptorSet* windowDescriptorSet, WindowDescriptor* windowDescriptor, uint32_t starId, StarsPositionsCalculatorParameterSet& starPosCalcParamSet) {
	// No error detected, the window fit in the CCD
	// NDPU-SRS-7765 update the window descriptor with the computed values
	windowDescriptor->setX(starPosCalcParamSet.getWindowX());
	windowDescriptor->setY(starPosCalcParamSet.getWindowY());
	windowDescriptor->setCcdId(starPosCalcParamSet.getCcdId());
	windowDescriptor->setCcdSide(starPosCalcParamSet.getCcdSide());
	// NDPU-SRS-6049 - update housekeeping parameters
	if (windowDescriptorSet->getWindowDescriptorNumber() + 1U < windowDescriptorSet->WINDOW_COUNT) {
		// Increment the number of windows
		windowDescriptorSet->incrementWindowDescriptorNumberPerCcdSide(starPosCalcParamSet.getCcdId(),
				starPosCalcParamSet.getCcdSide());
		// NDPU-SRS-7666 : When the window has been successfully computed, set the initialization flag of the window descriptor to INITIALIZED
		windowDescriptor->setInitialization(idbInitializedType::INITIALIZED);
		// NDPU-SRS-7768 : After computing each star window position, emit an informative event message TM_NC{#cun}_R_EVT_INF_CCD_POS_DETAILED
		if ((ccdPositionDetailedHandler != 0) && false) { // TODO change false to the newly created idb parameter
			CcdPositionDetailedParameters ccdPositionDetailedParameters;
			ccdPositionDetailedParameters.setFromStarsPositionsCalculatorParameterSet(starPosCalcParamSet, gsbTimeProvider, starId);
			// send the informative event
			ccdPositionDetailedHandler->processCcdPositionDetailed(cameraId, ccdPositionDetailedParameters);
		}
		// Increment the number of stars processed per CCD per side
		incrementNumberOfStarProcessed(starPosCalcParamSet.getCcdId(), starPosCalcParamSet.getCcdSide());
	}
	else {
		// NDPU-SRS-8871 - If allocating the window to one of the CCDs would exceed the maximum number of N-DPU window descriptors for this CCD
		// Stop the computation, send a parameter calculation warning event
		processParameterCalcWarning(parameterCalculationWarningHandler, cameraId, idbParamCalcWarCodeType::OVERFLOW_FOR_CCD, starId, starPosCalcParamSet);
	}
}

void StarsPositionsCalculator::starBcrs2CcdPosition(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
		StarsPositionsCalculatorParameterSet& starPosCalcParamSet, CommonScienceParameterSet commonScienceParameterSet,
		IgmGlobalParameterSet igmGlobalParameterSet, StarProperty* starProperty, Attitude attitude) {
	
	// Retrieve the star coordinate from the star property (issued from the star catalog)
	starPosCalcParamSet.setStarRightAscension(starProperty->getRightAscension());
	starPosCalcParamSet.setStarDeclination(starProperty->getDeclination());


	// 1. For star coordinates (at start time of the RUN), conversion from BCRS to GCRS reference frame using ONB- POSCAL-020,-060 algorithms.
	bcrs2GcrsAberrationCorrection(errorCode, starPosCalcParamSet, commonScienceParameterSet);

	if (errorCode == gsbManagerErrorCode::SUCCESS) {
		// 2. Determination of the star position in Camera reference frame, ONB-POSCAL-030
		// 2.1. Convert the star positions from spherical to Cartesian coordinates
		// 2.2. Compute the position in Camera Boresight Reference Frame using telescope attitude.
		gcrs2DirectionCamBoresight(starPosCalcParamSet, attitude);
		// No error code is returned by this function.

	
		// 3. Calculate the distorted position on the Focal plane using tangent of x and y field angles and the coefficients of distortion model, ONB-POSCAL-040
		gnomonicDistortion(errorCode, starPosCalcParamSet, igmGlobalParameterSet);
	}
	if (errorCode == gsbManagerErrorCode::SUCCESS) {
		// 4. Determine the CCD number and the positions on the Silicon Die reference frame, ONB_POSCAL-050.
		fp2SiliconDie(errorCode, starPosCalcParamSet, commonScienceParameterSet, igmGlobalParameterSet);

	}
	if (errorCode == gsbManagerErrorCode::SUCCESS) {
		// Determine the positions on the CCD in FEE/CCD reference frame and the CCD side, ONB-POSCAL-070.
		siliconDie2FeeCcd(errorCode, starPosCalcParamSet);

	}
}

void StarsPositionsCalculator::bcrs2GcrsAberrationCorrection(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
		StarsPositionsCalculatorParameterSet& starPosCalcParamSet, CommonScienceParameterSet commonScienceParameterSet) {
	// 0x400921FB54442D18 - pi's value in double precision
	static const double DOUBLE_PI = 3.141592653589793116;
	// DOUBLE_PI / 180 : 0x3F91DF46A2529D39 - convert degrees to radians
	static const double D2R = 0.01745329251994329547;
	
	// If an error is detected, the error code won't be changed, it will be set to SUCCESS otherwise
	errorCode = gsbManagerErrorCode::INCONSISTENT_PARAMETER;

	double starRightAscension = starPosCalcParamSet.getStarRightAscension();
	double starDeclination = starPosCalcParamSet.getStarDeclination();
	// outputs
	double xGCRS = 0.0;
	double yGCRS = 0.0;
	double zGCRS = 0.0;

	// NDPU-SRS-7574 : Calculate time in Julian centuries from the epoch J2000.
	double const timeJulian = ((starPosCalcParamSet.getCurrentTime() / 86400.) + commonScienceParameterSet.getEpochOffset()) / 36525.;
	double const timeJulianSquared = timeJulian * timeJulian;
	double const timeJulianPow3 = timeJulianSquared * timeJulian;
	// NDPU-SRS-7575 : Calculate positions of the Earth and the Sun.
	// The obliquity of the ecliptic
	double eps0 = (23.43929111111111 - (0.013004166666666666 * timeJulian) - (1.638888888888889e-07 * timeJulianSquared))	+ (5.036111111111111e-07 * timeJulianPow3);
	// The geometric mean longitude of the sun (in degrees)
	double geoMeanLong = (280.46646 + (36000.76983 * timeJulian) + (0.0003032 * timeJulianSquared));
	double geoMeanLongTemp = GsbIeee754::floor(geoMeanLong / 360.);
	geoMeanLong -= (geoMeanLongTemp * 360.);	// geoMeanLong modulo 360
	// The mean anomaly of the Sun
	double meanAnomaly = (357.52772 + (35999.050340 * timeJulian) + (0.0001603 * timeJulianSquared) + (timeJulianPow3 / 300000.));
	double meanAnomalyTemp = GsbIeee754::floor(meanAnomaly / 360.);
	meanAnomaly -= (meanAnomalyTemp * 360.);	// meanAnomaly modulo 360
	// The sun's equation of the center
	double eqOTCenter = (1.914602 - (0.004817 * timeJulian) - ((0.000014 * timeJulianSquared) * GsbIeee754::sin(meanAnomaly * D2R)))
			+ (0.019993 - ((0.000101 * timeJulian) * GsbIeee754::sin(2. * meanAnomaly * D2R)))
			+ (0.000289 * GsbIeee754::sin(3. * meanAnomaly * D2R));
	// The Sun's true longitude (in radians)
	double lonSun = (geoMeanLong + eqOTCenter) * D2R;
	// NDPU-SRS-7578 : Earth orbit
	// The longitude of the perihelion of earth's orbit
	double lonEarthPeri = (102.937348 + (1.7195366 * timeJulian) + (0.00045688 * timeJulianSquared)) - (1.8e-08 * timeJulianPow3);
	// The excentricity of earth's orbit
	double excEarth = 0.016708634 - ((0.42037e-4 * timeJulian) + (0.1267e-6 * timeJulianSquared));
	// Constant of aberration
	double const aberr = 9.936508497454117e-5;
	// NDPU-SRS-7579 : Trigonometric functions of the star's coordinates
	double cosPeri = GsbIeee754::cos(lonEarthPeri * D2R);
	double sinPeri = GsbIeee754::sin(lonEarthPeri * D2R);
	double cosLonSun = GsbIeee754::cos(lonSun);
	double sinLonSun = GsbIeee754::sin(lonSun);
	double cosEps0 = GsbIeee754::cos(eps0 * D2R);
	double sinEps0 = GsbIeee754::sin(eps0 * D2R);

	double cosDecl = GsbIeee754::cos(starDeclination * D2R);
	double sinDecl = GsbIeee754::sin(starDeclination * D2R);
	double cosRightAsc = GsbIeee754::cos(starRightAscension * D2R);
	double sinRightAsc = GsbIeee754::sin(starRightAscension * D2R);
	// NDPU-SRS-7583 : If the absolute value of the star declination is greater than 83 degrees, the N-DPU ASW shall compute the aberrational displacements.
	// A star declination > 83 deg mean that the star is near the pole, so the aberration is expressed in ecliptic coordinates to avoid division by zero
	if (GsbMath::fabs(starDeclination) > 83.) {
		// NDPU-SRS-7581 : convert equatorial coordinates to ecliptical - trigonometry
		// Compute the coordinates projections
		double sinBeta = (sinDecl * cosEps0) - (cosDecl * sinEps0 * sinRightAsc);
		double cbcl = cosDecl * cosRightAsc;
		double cbsl = (sinDecl * sinEps0) + (cosDecl * cosEps0 * sinRightAsc);
		// NDPU-SRS-7582 : Convert equatorial coordinates to ecliptical - computation
		// Computation of the ecliptical coordinates of the star
		double lambdaRad = GsbIeee754::atan2(cbsl, cbcl);
		// Reduce lambdaRad to the range [0, 360] degrees
		if (lambdaRad < 0.) {
			lambdaRad += 2. * DOUBLE_PI;
		}
		double radius = GsbMath::sqrt((cbsl * cbsl) + (cbcl * cbcl));
		double betaRad = GsbIeee754::atan2(sinBeta, radius);

		// Reduce betaRad to the range [-90, 90] degrees
		betaRad = reduceAngle(betaRad);

		// NDPU-SRS-7587 : Compute the aberrational displacement in ecliptical coordinates
		// Compute the aberration displacements of the star :
		// Check for numerical errors
		double cosBetaRad = GsbIeee754::cos(betaRad);
		if (GsbMath::fabs(cosBetaRad) > 1e-40) {
			double deltaLambda = ((-aberr) / cosBetaRad)	* 
				(GsbIeee754::cos(lonSun - lambdaRad) - ((excEarth * GsbIeee754::cos((lonEarthPeri * D2R))) - lambdaRad));
			double deltaBeta = (-aberr) * GsbIeee754::sin(betaRad)
					* (GsbIeee754::sin(lonSun - lambdaRad) - (excEarth * GsbIeee754::sin((lonEarthPeri * D2R) - lambdaRad)));
			// Compute the corrected ecliptical coordinates :
			lambdaRad += deltaLambda;
			betaRad += deltaBeta;
			// NDPU-SRS-7588 : Convert ecliptical coordinates to cartesian equatorial
			xGCRS = GsbIeee754::cos(lambdaRad) * GsbIeee754::cos(betaRad);
			yGCRS = (cosEps0 * GsbIeee754::sin(lambdaRad) * GsbIeee754::cos(betaRad)) - (sinEps0 * GsbIeee754::sin(betaRad));
			zGCRS = (sinEps0 * GsbIeee754::sin(lambdaRad) * GsbIeee754::cos(betaRad)) + (cosEps0 * GsbIeee754::sin(betaRad));
			errorCode = gsbManagerErrorCode::SUCCESS;
		}
	}
	else {
		// if the star is not near the poles
		// NDPU-SRS-7585 : Coordinate converion - non-poles conversion
		// Calculate the changes in the Right Ascension and in the Declination of the star due to annual aberration
		// Check for numerical errors
		// NB : cosDecl should never be close to 0 thanks to the previous condition (NDPU-SRS-7583 : handle star near the pole)
		if (GsbMath::fabs(cosEps0) > 1e-40) {
			double deltaAsc = ((-aberr) / cosDecl) * (((cosRightAsc * cosLonSun * cosEps0) + (sinRightAsc * sinLonSun)) - (excEarth * ((cosRightAsc * cosPeri * cosEps0) + (sinRightAsc * sinPeri))));
			double deltaDecl = ((-aberr) * ((cosLonSun * cosEps0 * (((sinEps0 / cosEps0) * cosDecl) - (sinRightAsc * sinDecl))) + (cosRightAsc * sinDecl * sinLonSun))) +
												 ((aberr * excEarth) * (((cosPeri * cosEps0 * ((sinEps0 / cosEps0) * cosDecl)) - (sinRightAsc * sinDecl)) + (cosRightAsc * sinDecl * sinPeri)));
			// Compute the corrected ascension and declination
			starRightAscension += (deltaAsc / D2R);
			starDeclination += (deltaDecl / D2R);
			// NDPU-SRS-7589 : Transform the GCRS coordinates into a vector
			xGCRS = GsbIeee754::cos(starRightAscension * D2R) * GsbIeee754::cos(starDeclination * D2R);
			yGCRS = GsbIeee754::sin(starRightAscension * D2R) * GsbIeee754::cos(starDeclination * D2R);
			zGCRS = GsbIeee754::sin(starDeclination * D2R);
			errorCode = gsbManagerErrorCode::SUCCESS;
		}
	}
	if (errorCode != gsbManagerErrorCode::INCONSISTENT_PARAMETER) {
		// If no error has been detected, return the computed coordinates
		starPosCalcParamSet.setStarCoordinatesGcrs(0, xGCRS);
		starPosCalcParamSet.setStarCoordinatesGcrs(1, yGCRS);
		starPosCalcParamSet.setStarCoordinatesGcrs(2, zGCRS);
		errorCode = gsbManagerErrorCode::SUCCESS;
	}
}

void StarsPositionsCalculator::gcrs2DirectionCamBoresight(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, Attitude attitude) {
	// NDPU-SRS-7590 : Coordinate conversion from GCRS to camera optical frame
	// Multiply the attitude rotation matrix (3x3) by the star coordinate vector :
	// [vinX, vinY, vinZ] = [attitude] * [xGCRS, yGCRS, zGCRS]
	double vinX = starPosCalcParamSet.getStarCoordinatesGcrs(0);
	double vinY = starPosCalcParamSet.getStarCoordinatesGcrs(1);
	double vinZ = starPosCalcParamSet.getStarCoordinatesGcrs(2);

	starPosCalcParamSet.setCameraDirections(0, (attitude.getAttitude(0) * vinX) + (attitude.getAttitude(1) * vinY) + (attitude.getAttitude(2) * vinZ));
	starPosCalcParamSet.setCameraDirections(1, (attitude.getAttitude(3) * vinX) + (attitude.getAttitude(4) * vinY) + (attitude.getAttitude(5) * vinZ));
	starPosCalcParamSet.setCameraDirections(2, (attitude.getAttitude(6) * vinX) + (attitude.getAttitude(7) * vinY) + (attitude.getAttitude(8) * vinZ));
}

void StarsPositionsCalculator::gnomonicDistortion(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
		StarsPositionsCalculatorParameterSet& starPosCalcParamSet, IgmGlobalParameterSet imageGeometryModel) {
	// NDPU-SRS-7591 : Projection of coordinates on CCD focal place : undistorded angular
	// Check for numerical errors
	errorCode = gsbManagerErrorCode::INCONSISTENT_PARAMETER;

	if (GsbMath::fabs(starPosCalcParamSet.getCameraDirections(2)) > 1e-40) {
		// Step 1 : Compute the undistorted angular coordinates
		double undistX = (-starPosCalcParamSet.getCameraDirections(0)) / starPosCalcParamSet.getCameraDirections(2);
		double undistY = (-starPosCalcParamSet.getCameraDirections(1)) / starPosCalcParamSet.getCameraDirections(2);
		// NDPU-SRS-7592 : Step 2 : Undistorded radius
		double radius2 = (undistX * undistX) + (undistY * undistY);
		double radius4 = radius2 * radius2;
		double radius6 = radius4 * radius2;
		// NDPU-SRS-7593 : Step 3 : Generate distortion according to Radial model of degrees3
		double dist = 1. + (static_cast<double>(imageGeometryModel.getDistorsionK1()) * radius2)
				+ (static_cast<double>(imageGeometryModel.getDistorsionK2()) * radius4)
				+ (static_cast<double>(imageGeometryModel.getDistorsionK3()) * radius6);

		double tempX = (undistX * dist) + (static_cast<double>(imageGeometryModel.getDistorsionQ1()) * radius2) +
									 (undistX * ((static_cast<double>(imageGeometryModel.getDistorsionP1()) * undistX) + (static_cast<double>(imageGeometryModel.getDistorsionP2()) * undistY)));

		double tempY = (undistY * dist) + (static_cast<double>(imageGeometryModel.getDistorsionQ2()) * radius2) +
									 (undistY * ((static_cast<double>(imageGeometryModel.getDistorsionP1()) * undistX) + (static_cast<double>(imageGeometryModel.getDistorsionP2()) * undistY)));
		// NDPU-SRS-7594 : Step 4 : Compute the star position on the CCD focal place (in millimeters)
		starPosCalcParamSet.setPosFocalPlane(0, tempX * static_cast<double>(imageGeometryModel.getFocalDistance()));
		starPosCalcParamSet.setPosFocalPlane(1, tempY * static_cast<double>(imageGeometryModel.getFocalDistance()));
		errorCode = gsbManagerErrorCode::SUCCESS;
	}
}

void StarsPositionsCalculator::fp2SiliconDie(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
		StarsPositionsCalculatorParameterSet& starPosCalcParamSet, CommonScienceParameterSet commonScienceParameterSet,
		IgmGlobalParameterSet imageGeometryModel) {
	// NDPU-SRS-7597 : CCD identification and conversion to CCD internal coordinates: initial

	errorCode = gsbManagerErrorCode::INCONSISTENT_PARAMETER;

	//             Y camera
	//              ^
	//         x    |
	// +---------+  |  +---------+
	// |------<--|  |  |        ||
	// |         |  |  |        || <-- bottom of the CCD
	// |       y v  |  |        ||
	// |         |  |  |      y ^|
	// |    CCD#1|  |  |CCD#4   ||
	// +---------+  |  +------<--+
	//              |         x
	//              +--------------->
	//                             X camera
	// +-->------+     +---------+
	// || y CCD#2|     |CCD#3    |
	// |v x      |     |         |
	// ||        |     ^y        |
	// ||        |     |  x      |
	// ||        |     |-->------|
	// +---------+     +---------+

	// DOUBLE_PI / 180 : 0x3F91DF46A2529D39 - convert degrees to radians
	static const double D2R = 0.01745329251994329547;

	double ccdSOffsetX;
	double ccdSOffsetY;
	double ccdSdRotation;

	if (starPosCalcParamSet.getPosFocalPlane(0) > 0.0) {
		if (starPosCalcParamSet.getPosFocalPlane(1) > 0.0) {
			// CCD#4
			starPosCalcParamSet.setCcdId(idbCcdNumberType::CCD4);
		}
		else {
			// if PosFocalPlane(1) < 0
			// CCD#3
			starPosCalcParamSet.setCcdId(idbCcdNumberType::CCD3);
		}
	}
	else {
		// posFocalPlaneX < 0
		if (starPosCalcParamSet.getPosFocalPlane(1) < 0.0) {
			// CCD#2
			starPosCalcParamSet.setCcdId(idbCcdNumberType::CCD2);
		}
		else {
			// posFocalPlaneY > 0
			// CCD#1
			starPosCalcParamSet.setCcdId(idbCcdNumberType::CCD1);
		}
	}

	ccdSOffsetX = imageGeometryModel.getCcdSOffsetX(starPosCalcParamSet.getCcdId());
	ccdSOffsetY = imageGeometryModel.getCcdSOffsetY(starPosCalcParamSet.getCcdId());
	ccdSdRotation = imageGeometryModel.getCcdSRotation(starPosCalcParamSet.getCcdId());

	// NDPU-SRS-7599 : compute the rotation matrix of the active CCD
	double rotMatrix[4];
	rotMatrix[0] = GsbIeee754::cos(ccdSdRotation * D2R);
	rotMatrix[1] = -GsbIeee754::sin(ccdSdRotation * D2R);
	rotMatrix[2] = GsbIeee754::sin(ccdSdRotation * D2R);
	rotMatrix[3] = GsbIeee754::cos(ccdSdRotation * D2R);

	// NDPU-SRS-7600 : Compute the coordinates rotated to the CCD silicon die reference frame.
	double starCcdX = (rotMatrix[1] * starPosCalcParamSet.getPosFocalPlane(1)) - (rotMatrix[0] * starPosCalcParamSet.getPosFocalPlane(0));
	double starCcdY = (rotMatrix[3] * starPosCalcParamSet.getPosFocalPlane(1)) - (rotMatrix[2] * starPosCalcParamSet.getPosFocalPlane(0));

	// Check for numerical errors
	if (commonScienceParameterSet.getPixelSize() > 1e-40) {
		starPosCalcParamSet.setStarSD(0, ((starCcdX + ccdSOffsetX) / commonScienceParameterSet.getPixelSize()) + 1.);
		starPosCalcParamSet.setStarSD(1, ((starCcdY + ccdSOffsetY) / commonScienceParameterSet.getPixelSize()) + 1.);
		errorCode = gsbManagerErrorCode::SUCCESS;
	}
}

void StarsPositionsCalculator::siliconDie2FeeCcd(gsbManagerErrorCode::GsbManagerErrorCodeEnum& errorCode,
		StarsPositionsCalculatorParameterSet& starPosCalcParamSet) {
	errorCode = gsbManagerErrorCode::ERROR_DETECTED;

	double starFee[2];
	starFee[0] = starPosCalcParamSet.getStarSD(0) - 1.;
	starFee[1] = (CCD_SIZE_PIXEL - starPosCalcParamSet.getStarSD(1)) + 1.;

	double starFeeX;
	double starFeeY = starFee[1];

	// NDPU-SRS-7602 : Compute the position of the star in the CCD side reference coordinates.
	if (starFee[0] < (CCD_SIZE_PIXEL / 2)) {
		// The star is located on the left side of the CCD
		starPosCalcParamSet.setCcdSide(idbCcdSideType::LEFT);
		starFeeX = starFee[0] + SERIAL_PRESCAN_COL_NUMBER;
	}
	else {
		// The star is located on the right side of the CCD
		starPosCalcParamSet.setCcdSide(idbCcdSideType::RIGHT);
		starFeeX = (CCD_SIZE_PIXEL - starFee[0]) + SERIAL_PRESCAN_COL_NUMBER;
	}
	starPosCalcParamSet.setStarFee(0, starFeeX);
	starPosCalcParamSet.setStarFee(1, starFeeY);

	// NDPU-SRS-8849 : boundary error event : check if the ccd side associated to the star window is disabled
	// The camera configuration should have been initialized before calling this function.
	if (cameraConfiguration->getCcdSideActivation(starPosCalcParamSet.getCcdId(), starPosCalcParamSet.getCcdSide()) == true) {
		// Ccd side activated
		errorCode = gsbManagerErrorCode::SUCCESS;
	}
}

bool StarsPositionsCalculator::fitInsideCcd(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, WindowDescriptor* windowDescriptor) {
	// NDPU-SRS-7607 : For each star, compute the star descriptor list that has an uninitialized window position, the initial position of the N-DPU window of the star.
	// NDPU-SRS-7609 : compute the origin of the star window
	double tempX0 = starPosCalcParamSet.getStarFee(0) - (windowDescriptor->getWidth() / 2.);
	double tempY0 = starPosCalcParamSet.getStarFee(1) - (windowDescriptor->getHeight() / 2.);
	tempX0 = GsbIeee754::floor((tempX0 * 100.) + 0.5);
	tempY0 = GsbIeee754::floor((tempY0 * 100.) + 0.5);
	// Origin of the star window
	int16_t windowX = static_cast<int16_t>(GsbIeee754::floor((tempX0 / 100.) + 0.5));
	int16_t windowY = static_cast<int16_t>(GsbIeee754::floor((tempY0 / 100.) + 0.5));
	bool isInsideCcd = false;
	// NDPU-SRS-7610 : Check wether the entirety of the window fits inside the CCD side
	if ((windowX >= static_cast<int32_t>(SERIAL_PRESCAN_COL_NUMBER))
			&& (windowX <= (((static_cast<int32_t>(CCD_SIZE_PIXEL / 2U)) + static_cast<int32_t>(SERIAL_PRESCAN_COL_NUMBER)) - windowDescriptor->getWidth()))
			&& (windowY >= 0) 
			&& (windowY <= (static_cast<int32_t>(CCD_SIZE_PIXEL)) - windowDescriptor->getHeight())) {
		// The window fit in the CCD side
		isInsideCcd = true;
	}
	starPosCalcParamSet.setWindowX(static_cast<uint16_t>(windowX));
	starPosCalcParamSet.setWindowY(static_cast<uint16_t>(windowY));
	return isInsideCcd;
}

void StarsPositionsCalculator::processParameterCalcWarning(ParameterCalculationWarningHandler* parameterCalculationWarningHandler, cameraIdentifier::CameraIdentifierEnum cameraId,
		idbParamCalcWarCodeType::IdbParamCalcWarCodeTypeEnum code, uint32_t starId, StarsPositionsCalculatorParameterSet starPosCalcParamSet) {
	// If the event handler is correctly initialized

	if (parameterCalculationWarningHandler != 0) {
		// Send a warning event
		parameterCalculationWarningHandler->processParameterCalculationWarning(
				cameraId, code, starId, starPosCalcParamSet.getCcdId(), starPosCalcParamSet.getStarFee(0), starPosCalcParamSet.getStarFee(1));
	}
}

void StarsPositionsCalculator::processSummaryEvent(CcdPositionSummaryHandler* ccdPositionSummaryHandler, cameraIdentifier::CameraIdentifierEnum cameraId) {
	// If the event handler is correctly initialized
	if (ccdPositionSummaryHandler != 0U) {
		// Set the summary event parameters then send the event
		CcdPositionSummaryParameters ccdPositionSummaryParameters;
		ccdPositionSummaryParameters.setTimestamp(gsbTimeProvider->getTime());
		
		ccdPositionSummaryParameters.setNumStarPosComp(numStarPosComp);
		ccdPositionSummaryParameters.setLeftSides(starNumCcd1Left, starNumCcd2Left, starNumCcd3Left, starNumCcd4Left);
		ccdPositionSummaryParameters.setRightSides(starNumCcd1Right, starNumCcd2Right, starNumCcd3Right, starNumCcd4Right);
		ccdPositionSummaryHandler->processCcdPositionSummary(cameraId, ccdPositionSummaryParameters);
	}
}

void StarsPositionsCalculator::incrementNumberOfStarProcessed(idbCcdNumberType::IdbCcdNumberTypeEnum ccdId, idbCcdSideType::IdbCcdSideTypeEnum ccdSide) {
	// Increment the number of star processed
	++numStarPosComp;
	// If the CCD side is right, increment the counter depending on the CCD id
	switch (ccdId) {
		case idbCcdNumberType::CCD1:
			// if the CCD id is 1, increment the associated counter
			if (ccdSide == idbCcdSideType::RIGHT) {
				++starNumCcd1Right;
			}
			else {
				++starNumCcd1Left;
			}
			break;
		case idbCcdNumberType::CCD2:
			// if the CCD id is 2, increment the associated counter
			if (ccdSide == idbCcdSideType::RIGHT) {
				++starNumCcd2Right;
			}
			else {
				++starNumCcd2Left;
			}
			break;
		case idbCcdNumberType::CCD3:
			// if the CCD id is 3, increment the associated counter
			if (ccdSide == idbCcdSideType::RIGHT) {
				++starNumCcd3Right;
			}
			else {
				++starNumCcd3Left;
			}
			break;
		case idbCcdNumberType::CCD4:
			// if the CCD id is 4, increment the associated counter
			if (ccdSide == idbCcdSideType::RIGHT) {
				++starNumCcd4Right;
			}
			else {
				++starNumCcd4Left;
			}
			break;
		default:
			// Shall not happen
			break;
	}
}