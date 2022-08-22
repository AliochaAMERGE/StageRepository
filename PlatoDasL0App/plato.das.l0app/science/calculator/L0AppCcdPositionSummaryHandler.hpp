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

 L0AppCcdPositionSummaryHandler.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionSummaryHandler.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 17 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONSUMMARYHANDLER_HPP_
#define L0APPCCDPOSITIONSUMMARYHANDLER_HPP_

#include <plato/das/idb/util/identifier/IdbCameraIdentifierEnum.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionSummaryParameters.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {

					class CcdPositionSummaryHandler {
					private:
						/// Non functional copy constructor
						CcdPositionSummaryHandler(CcdPositionSummaryHandler const* source);
						/// Non functional copy constructor
						CcdPositionSummaryHandler(CcdPositionSummaryHandler const& source);
						/// Non functional assignment operator
						CcdPositionSummaryHandler& operator=(CcdPositionSummaryHandler& source);

					public:
						/**
						 * @brief Empty default constructor
						 */
						CcdPositionSummaryHandler() {}
						/**
						 * @brief Handler method for the TM_NCxx_R_EVT_INF_CCD_POS_SUMMARY Packet
						 * @details Initial computation of the star CCD and window positions.
						 *
						 * @param parameters packet parameters
						 */
						virtual void processCcdPositionSummary(const cameraIdentifier::CameraIdentifierEnum ncamId,
								const CcdPositionSummaryParameters& parameters) = 0;
					};
				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::CcdPositionSummaryHandler;

#endif /* L0APPCCDPOSITIONSUMMARYHANDLER_HPP_ */
