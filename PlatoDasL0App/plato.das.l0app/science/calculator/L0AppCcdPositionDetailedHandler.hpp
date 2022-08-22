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

 L0AppCcdPositionDetailedHandler.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppCcdPositionDetailedHandler.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPCCDPOSITIONDETAILEDHANDLER_HPP_
#define L0APPCCDPOSITIONDETAILEDHANDLER_HPP_

#include <plato/das/idb/util/identifier/IdbCameraIdentifierEnum.hpp>
#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedParameters.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace science {
				namespace calculator {

					class CcdPositionDetailedHandler {
					private:
						/// Non functioal copy constructor
						CcdPositionDetailedHandler(const CcdPositionDetailedHandler* source);
						/// Non functioal copy constructor
						CcdPositionDetailedHandler(const CcdPositionDetailedHandler& source);
						/// Non functional overload of the assignement operator
						CcdPositionDetailedHandler& operator=(const CcdPositionDetailedHandler& source);

					public:
						/**
						 * @brief Empty constructor
						 */
						CcdPositionDetailedHandler() {}
						/**
						 * @brief Handler method for the TM_NCxx_R_EVT_INF_CCD_POS_DETAILED packet
						 * @details Detailed information about one star CCD and window position computation
						 *
						 * @param parameters The parameters of the packet
						 */
						virtual void processCcdPositionDetailed(const cameraIdentifier::CameraIdentifierEnum ncamId,
								const CcdPositionDetailedParameters& parameters) = 0;
					};
				}
			}
		}
	}
}

using plato::das::l0app::science::calculator::CcdPositionDetailedHandler;

#endif /* L0APPCCDPOSITIONDETAILEDHANDLER_HPP_ */
