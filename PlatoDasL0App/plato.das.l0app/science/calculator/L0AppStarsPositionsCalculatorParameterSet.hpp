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

 L0AppStarsPositionsCalculatorParameterSet.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppStarsPositionsCalculatorParameterSet.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines the super set of parameters used in the stars positions calculator.

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 25 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPSTARSPOSITIONSCALCULATORPARAMETERSET_HPP_
#define L0APPSTARSPOSITIONSCALCULATORPARAMETERSET_HPP_

#include <gericos/blocks/time/GsbTimeProvider.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdNumberTypeEnum.hpp>
#include <plato/das/idb/common/enumerations/miscEnumerations/IdbCcdSideTypeEnum.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {

					class StarsPositionsCalculatorParameterSet {
					private:
						idbCcdNumberType::IdbCcdNumberTypeEnum ccdId;
						idbCcdSideType::IdbCcdSideTypeEnum ccdSide;
						double starRightAscension;			///< Star coordinates Ascension (input of starBcrs2CcdPosition)
						double starDeclination;					///< Star coordinates Declination (input of starBcrs2CcdPosition)
						double starCoordinatesGcrs[3];	///< Output of bcrs2GcrsAberrationCorrection
						double cameraDirections[3];			///< Output of gcrs2DirectionCamBoresight
						double posFocalPlane[2];				///< Output of gnomonicDistortion
						double starSD[2];								///< Output of fp2SiliconDie
						double starFee[2];							///< Output of siliconDie2FeeCcd
						uint16_t windowX;								///< Window position in X
						uint16_t windowY;								///< Window position in Y
						double currentTime;				///< The number of seconds elapsed from the J2000 time origin for the normal camera.

						/// Non functional copy constructor
						StarsPositionsCalculatorParameterSet(const StarsPositionsCalculatorParameterSet* source);

					public:
						/// Default constructor.
						StarsPositionsCalculatorParameterSet();
						/// Copy constructor
						StarsPositionsCalculatorParameterSet(const StarsPositionsCalculatorParameterSet& source);
						/// Overload of the assignement operator
						StarsPositionsCalculatorParameterSet& operator=(const StarsPositionsCalculatorParameterSet& other);

						/**
						 * @brief Reset all parameters to their default values
						 */
						void reset();

						/**
						 * @brief Access the Star Right Ascension
						 */
						double getStarRightAscension() const {
							return starRightAscension;
						}
						/**
						 * @brief Set the Star Right Ascension
						 */
						inline void setStarRightAscension(double starRightAscension) {
							this->starRightAscension = starRightAscension;
						}
						/**
						 * @brief Access the Star Declination
						 */
						double getStarDeclination() const {
							return starDeclination;
						}
						/**
						 * @brief Set the Star Declination
						 */
						inline void setStarDeclination(double starDeclination) {
							this->starDeclination = starDeclination;
						}
						/**
						 * @brief Access the Star Coordinates
						 */
						double getStarCoordinatesGcrs(int index) const {
							return index < 3 ? starCoordinatesGcrs[index] : 0.0;
						}
						/**
						 * @brief Set the Star Coordinates
						 */
						inline void setStarCoordinatesGcrs(int index, double starCoordinatesGcrs) {
							if (index < 3) {
								this->starCoordinatesGcrs[index] = starCoordinatesGcrs;
							}
						}
						/**
						 * @brief Access the Camera Directions
						 */
						double getCameraDirections(int index) const {
							return index < 3 ? cameraDirections[index] : 0.0;
						}
						/**
						 * @brief Set the Camera Directions
						 */
						inline void setCameraDirections(int index, double cameraDirections) {
							if (index < 3) {
								this->cameraDirections[index] = cameraDirections;
							}
						}
						/**
						 * @brief Access the Pos Focal Plane
						 */
						double getPosFocalPlane(int index) const {
							return index < 2 ? posFocalPlane[index] : 0.0;
						}
						/**
						 * @brief Set the Pos Focal Plane
						 */
						inline void setPosFocalPlane(int index, double posFocalPlane) {
							if (index < 2) {
								this->posFocalPlane[index] = posFocalPlane;
							}
						}
						/**
						 * @brief Access the Star coordinate in the silicon die reference frame
						 */
						double getStarSD(int index) const {
							return index < 2 ? starSD[index] : 0.0;
						}
						/**
						 * @brief Set the Star coordinate in the silicon die reference frame
						 */
						inline void setStarSD(int index, double starSD) {
							if (index < 2) {
								this->starSD[index] = starSD;
							}
						}
						/**
						 * @brief Access the Star coordinate in the FEE reference frame
						 */
						double getStarFee(int index) const {
							return index < 2 ? starFee[index] : 0.0;
						}
						/**
						 * @brief Set the Star coordinate in the FEE reference frame
						 */
						inline void setStarFee(int index, double starFee) {
							if (index < 2) {
								this->starFee[index] = starFee;
							}
						}
						/**
						 * @brief Access the Window X
						 */
						uint16_t getWindowX() const {
							return windowX;
						}
						/**
						 * @brief Set the Window X
						 */
						inline void setWindowX(uint16_t windowX) {
							this->windowX = windowX;
						}
						/**
						 * @brief Access the Window Y
						 */
						uint16_t getWindowY() const {
							return windowY;
						}
						/**
						 * @brief Set the Window Y
						 */
						inline void setWindowY(uint16_t windowY) {
							this->windowY = windowY;
						}
						/**
						 * @brief Access the Current Time
						 */
						double getCurrentTime() const {
							return currentTime;
						}
						/**
						 * @brief Set the Current Time
						 */
						inline void setCurrentTime(double currentTime) {
							this->currentTime = currentTime;
						}
						/**
						 * @brief Access the ccd Identifier
						 */
						idbCcdNumberType::IdbCcdNumberTypeEnum getCcdId() const {
							return ccdId;
						}
						/**
						 * @brief Set the ccd Identifier
						 */
						inline void setCcdId(idbCcdNumberType::IdbCcdNumberTypeEnum ccdId) {
							this->ccdId = ccdId;
						}
						/**
						 * @brief Access the ccd Side
						 */
						idbCcdSideType::IdbCcdSideTypeEnum getCcdSide() const {
							return ccdSide;
						}
						/**
						 * @brief Set the ccd Side
						 */
						inline void setCcdSide(idbCcdSideType::IdbCcdSideTypeEnum ccdSide) {
							this->ccdSide = ccdSide;
						}
					};
				}
			}
		}
	}
}
using plato::das::l0app::science::calculator::StarsPositionsCalculatorParameterSet;

#endif /* L0APPSTARSPOSITIONSCALCULATORPARAMETERSET_HPP_ */
