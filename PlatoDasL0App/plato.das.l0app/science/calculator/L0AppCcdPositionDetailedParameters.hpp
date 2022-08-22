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

 L0AppCcdPositionDetailedParameters.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionDetailedParameters.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONDETAILEDPARAMETERS_HPP_
#define L0APPCCDPOSITIONDETAILEDPARAMETERS_HPP_

#include <gericos/blocks/time/GsbTime.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdNumberTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdSideTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/ncam/IdbEvtInfIdNcamEnum.hpp>
#include <plato/das/l0app/science/calculator/L0AppStarsPositionsCalculatorParameterSet.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {
					/**
					 * @brief This class reprends the parameters used during the position TODO
					 *
					 */
					class CcdPositionDetailedParameters {
					private:
						/// Non functional copy constructor
						CcdPositionDetailedParameters(const CcdPositionDetailedParameters& source);
						/// Non functional copy constructor
						CcdPositionDetailedParameters(const CcdPositionDetailedParameters* source);

						// Event progress report id
						idbEvtInfIdNcam::IdbEvtInfIdNcamEnum evtInfId;
						// Descriptor identifier
						uint32_t descrId;
						// Time when the science observation was made.
						GsbTime timestamp;
						// degA Star ascension in the GCRS coordinates
						double gcrsAscension;
						// degA Star declination in the GCRS coordinates
						double gcrsDeclination;
						// Star X position in the camera referential
						double cameraX;
						// Star Y position in the camera referential
						double cameraY;
						// Star Z position in the camera referential
						double cameraZ;
						// Position of the star projected on the focal plane
						double focalPlaneX;
						// Position of the star projected on the focal plan
						double focalPlaneY;
						// CCD identifier
						idbCcdNumberType::IdbCcdNumberTypeEnum ccdId;
						// CCD side
						idbCcdSideType::IdbCcdSideTypeEnum ccdSide;
						// X coordinate of the window
						uint16_t windowX;
						// Y coordinate of the window
						uint16_t windowY;

					void setFromStarsPositionsCalculatorParameterSetCamDirPosFoc(StarsPositionsCalculatorParameterSet& starPosCalcParamSet);

					public:
						/**
						 * @brief Default constructor
						 */
						CcdPositionDetailedParameters() {
							reset();
						}

						void reset() {
							// Set the informative event code
							evtInfId = idbEvtInfIdNcam::EVT_INF_NCAM_CCD_POS_DETAILED;
							// Set all values to 0
							descrId = 0U;
							gcrsAscension = 0.0;
							gcrsDeclination = 0.0;
							cameraX = 0.0;
							cameraY = 0.0;
							cameraZ = 0.0;
							focalPlaneX = 0.0;
							focalPlaneY = 0.0;
							ccdId = idbCcdNumberType::CCD1;		//<0
							ccdSide = idbCcdSideType::RIGHT;	//<0
							windowX = 0U;
							windowY = 0U;
						}

						/// Assignement operator
						CcdPositionDetailedParameters& operator=(const CcdPositionDetailedParameters& source);

						/**
						 * @brief Get event progress report id
						 */
						idbEvtInfIdNcam::IdbEvtInfIdNcamEnum getEvtInfId() const {
							return evtInfId;
						}

						void setFromStarsPositionsCalculatorParameterSet(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, GsbTimeProvider* timeProvider, int32_t descrId);

						/**
						 * @brief Set event progress report id
						 */
						void setEvtInfId(idbEvtInfIdNcam::IdbEvtInfIdNcamEnum value) {
							this->evtInfId = value;
						}

						/**
						 * @brief Get descriptor identifier
						 */
						uint32_t getDescrId() const {
							return descrId;
						}

						/**
						 * @brief Set descriptor identifier
						 */
						void setDescrId(uint32_t value) {
							this->descrId = value;
						}

						/**
						 * @brief Get time when the science observation was made.
						 */
						GsbTime getTimestamp() const {
							return timestamp;
						}

						/**
						 * @brief Set time when the science observation was made.
						 */
						void setTimestamp(GsbTime value) {
							this->timestamp = value;
						}

						/**
						 * @brief Get degA Star ascension in the GCRS coordinates
						 */
						double getGcrsAscension() const {
							return gcrsAscension;
						}

						/**
						 * @brief Set degA Star ascension in the GCRS coordinates
						 */
						void setGcrsAscension(double value) {
							this->gcrsAscension = value;
						}

						/**
						 * @brief Get degA Star declination in the GCRS coordinates
						 */
						double getGcrsDeclination() const {
							return gcrsDeclination;
						}

						/**
						 * @brief Set degA Star declination in the GCRS coordinates
						 */
						void setGcrsDeclination(double value) {
							this->gcrsDeclination = value;
						}

						/**
						 * @brief Get Star X position in the camera referential
						 */
						double getCameraX() const {
							return cameraX;
						}

						/**
						 * @brief Set Star X position in the camera referential
						 */
						void setCameraX(double value) {
							this->cameraX = value;
						}

						/**
						 * @brief Get Star Y position in the camera referential
						 */
						double getCameraY() const {
							return cameraY;
						}

						/**
						 * @brief Set Star Y position in the camera referential
						 */
						void setCameraY(double value) {
							this->cameraY = value;
						}

						/**
						 * @brief Get Star Z position in the camera referential
						 */
						double getCameraZ() const {
							return cameraZ;
						}

						/**
						 * @brief Set Star Z position in the camera referential
						 */
						void setCameraZ(double value) {
							this->cameraZ = value;
						}

						/**
						 * @brief Get Star X position in the focal plane referential
						 */
						double getFocalPlaneX() const {
							return focalPlaneX;
						}

						/**
						 * @brief Set Star X position in the focal plane referential
						 */
						void setFocalPlaneX(double value) {
							this->focalPlaneX = value;
						}

						/**
						 * @brief Get Star Y position in the focal plane referential
						 */
						double getFocalPlaneY() const {
							return focalPlaneY;
						}

						/**
						 * @brief Set Star Y position in the focal plane referential
						 */
						void setFocalPlaneY(double value) {
							this->focalPlaneY = value;
						}

						/**
						 * @brief Get CCD number
						 */
						idbCcdNumberType::IdbCcdNumberTypeEnum getCcdId() const {
							return ccdId;
						}

						/**
						 * @brief Set CCD number
						 */
						void setCcdId(idbCcdNumberType::IdbCcdNumberTypeEnum value) {
							this->ccdId = value;
						}

						/**
						 * @brief Get CCD side
						 */
						idbCcdSideType::IdbCcdSideTypeEnum getCcdSide() const {
							return ccdSide;
						}

						/**
						 * @brief Set CCD side
						 */
						void setCcdSide(idbCcdSideType::IdbCcdSideTypeEnum value) {
							this->ccdSide = value;
						}

						/**
						 * @brief Get X coordinate of the window
						 */
						int16_t getWindowX() const {
							return windowX;
						}

						/**
						 * @brief Set X coordinate of the window
						 */
						void setWindowX(uint16_t value) {
							this->windowX = value;
						}

						/**
						 * @brief Get Y coordinate of the window
						 */
						int16_t getWindowY() const {
							return windowY;
						}

						/**
						 * @brief Set Y coordinate of the window
						 */
						void setWindowY(uint16_t value) {
							this->windowY = value;
						}
					};

				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::CcdPositionDetailedParameters;

#endif /* L0APPCCDPOSITIONDETAILEDPARAMETERS_HPP_ */
