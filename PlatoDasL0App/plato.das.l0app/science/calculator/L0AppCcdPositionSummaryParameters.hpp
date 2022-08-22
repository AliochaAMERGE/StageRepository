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

 L0AppCcdPositionSummaryParameters.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionSummaryParameters.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 17 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONSUMMARYPARAMETERS_HPP_
#define L0APPCCDPOSITIONSUMMARYPARAMETERS_HPP_

#include <gericos/blocks/time/GsbTime.hpp>
#include <plato/das/idb/common/enumerations/ncam/IdbEvtInfIdNcamEnum.hpp>
#include <plato/das/idb/util/identifier/IdbCameraIdentifierEnum.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {
					/**
					 * @brief This class represent the parameters used during the production of
					 * TM_NCxx_R_EVT_INF_CCD_POS_SUMMARY Packet
					 *
					 */
					class CcdPositionSummaryParameters {
					private:
						/// Non functional copy constructor
						CcdPositionSummaryParameters(CcdPositionSummaryParameters const* source);
						/// Non functional copy constructor
						CcdPositionSummaryParameters(CcdPositionSummaryParameters const& source);

						// Informative event code
						idbEvtInfIdNcam::IdbEvtInfIdNcamEnum evtInfId;
						// Time when the science obbservation was made
						GsbTime timestamp;
						// Number of star positions computed by the service
						uint32_t numStarPosComp;
						// Number of stars that have been positioned on the CCD1 right side
						uint16_t starNumCcd1Right;
						// Number of stars that have been positioned on the CCD1 left side
						uint16_t starNumCcd1Left;
						// Number of stars that have been positioned on the CCD2 right side
						uint16_t starNumCcd2Right;
						// Number of stars that have been positioned on the CCD2 left side
						uint16_t starNumCcd2Left;
						// Number of stars that have been positioned on the CCD3 right side
						uint16_t starNumCcd3Right;
						// Number of stars that have been positioned on the CCD3 left side
						uint16_t starNumCcd3Left;
						// Number of stars that have been positioned on the CCD4 right side
						uint16_t starNumCcd4Right;
						// Number of stars that have been positioned on the CCD4 left side
						uint16_t starNumCcd4Left;

					public:
						/**
						 * @brief Default constructor
						 */
						CcdPositionSummaryParameters() {
							reset();
						}

						/**
						 * @brief Set the informative event code and reset all other parameters
						 */
						void reset() {
							// Set the informative event code
							evtInfId = idbEvtInfIdNcam::EVT_INF_NCAM_CCD_POS_SUMMARY;
							// set all values to 0
							numStarPosComp = 0U;
							starNumCcd1Right = 0U;
							starNumCcd1Left = 0U;
							starNumCcd2Right = 0U;
							starNumCcd2Left = 0U;
							starNumCcd3Right = 0U;
							starNumCcd3Left = 0U;
							starNumCcd4Right = 0U;
							starNumCcd4Left = 0U;
						}

						/// Assignement operator
						CcdPositionSummaryParameters& operator=(CcdPositionSummaryParameters const& source);

						/**
						 * @brief Get the event code
						 */
						idbEvtInfIdNcam::IdbEvtInfIdNcamEnum getEvtInfId() const {
							return evtInfId;
						}
						/**
						 * @brief Get the Time
						 */
						GsbTime getTimestamp() const {
							return timestamp;
						}
						/**
						 * @brief Get the number of star positions computed by the service
						 */
						uint32_t getNumStarPosComp() const {
							return numStarPosComp;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD1 right side
						 */
						uint16_t getStarNumCcd1Right() const {
							return starNumCcd1Right;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD1 left side
						 */
						uint16_t getStarNumCcd1Left() const {
							return starNumCcd1Left;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD2 right side
						 */
						uint16_t getStarNumCcd2Right() const {
							return starNumCcd2Right;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD2 left side
						 */
						uint16_t getStarNumCcd2Left() const {
							return starNumCcd2Left;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD3 right side
						 */
						uint16_t getStarNumCcd3Right() const {
							return starNumCcd3Right;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD3 left side
						 */
						uint16_t getStarNumCcd3Left() const {
							return starNumCcd3Left;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD4 right side
						 */
						uint16_t getStarNumCcd4Right() const {
							return starNumCcd4Right;
						}
						/**
						 * @brief Get the number of stars that have been positioned on the CCD4 left side
						 */
						uint16_t getStarNumCcd4Left() const {
							return starNumCcd4Left;
						}

						/**
						 * @brief Set the event code
						 */
						void setEvtInfId(idbEvtInfIdNcam::IdbEvtInfIdNcamEnum evtInfId) {
							this->evtInfId = evtInfId;
						}
						/**
						 * @brief Set the Time
						 */
						void setTimestamp(GsbTime timestamp) {
							this->timestamp = timestamp;
						}
						/**
						 * @brief Set the number of star positions computed by the service
						 */
						void setNumStarPosComp(uint32_t numStarPosComp) {
							this->numStarPosComp = numStarPosComp;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD1 right side
						 */
						void setStarNumCcd1Right(uint16_t starNumCcd1Right) {
							this->starNumCcd1Right = starNumCcd1Right;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD1 left side
						 */
						void setStarNumCcd1Left(uint16_t starNumCcd1Left) {
							this->starNumCcd1Left = starNumCcd1Left;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD2 right side
						 */
						void setStarNumCcd2Right(uint16_t starNumCcd2Right) {
							this->starNumCcd2Right = starNumCcd2Right;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD2 left side
						 */
						void setStarNumCcd2Left(uint16_t starNumCcd2Left) {
							this->starNumCcd2Left = starNumCcd2Left;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD3 right side
						 */
						void setStarNumCcd3Right(uint16_t starNumCcd3Right) {
							this->starNumCcd3Right = starNumCcd3Right;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD3 left side
						 */
						void setStarNumCcd3Left(uint16_t starNumCcd3Left) {
							this->starNumCcd3Left = starNumCcd3Left;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD4 right side
						 */
						void setStarNumCcd4Right(uint16_t starNumCcd4Right) {
							this->starNumCcd4Right = starNumCcd4Right;
						}
						/**
						 * @brief Set the number of stars that have been positioned on the CCD4 left side
						 */
						void setStarNumCcd4Left(uint16_t starNumCcd4Left) {
							this->starNumCcd4Left = starNumCcd4Left;
						}

						/**
						 * @brief Set left sides values
						 */
						void setLeftSides(uint16_t starNumCcd1Left, uint16_t starNumCcd2Left, uint16_t starNumCcd3Left,
								uint16_t starNumCcd4Left) {
							this->starNumCcd1Left = starNumCcd1Left;
							this->starNumCcd2Left = starNumCcd2Left;
							this->starNumCcd3Left = starNumCcd3Left;
							this->starNumCcd4Left = starNumCcd4Left;
						}

						/**
						 * @brief Set right sides values
						 */
						void setRightSides(uint16_t starNumCcd1Right, uint16_t starNumCcd2Right, uint16_t starNumCcd3Right,
								uint16_t starNumCcd4Right) {
							this->starNumCcd1Right = starNumCcd1Right;
							this->starNumCcd2Right = starNumCcd2Right;
							this->starNumCcd3Right = starNumCcd3Right;
							this->starNumCcd4Right = starNumCcd4Right;
						}
					};
				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::CcdPositionSummaryParameters;

#endif /* L0APPCCDPOSITIONSUMMARYPARAMETERS_HPP_ */
