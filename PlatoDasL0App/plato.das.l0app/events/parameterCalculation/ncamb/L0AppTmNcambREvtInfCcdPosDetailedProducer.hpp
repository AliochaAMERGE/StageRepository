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

 L0AppTmNcambREvtInfCcdPosDetailedProducer.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTmNcambREvtInfCcdPosDetailedProducer.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTMNCAMBREVTINFCCDPOSDETAILEDPRODUCER_HPP_
#define L0APPTMNCAMBREVTINFCCDPOSDETAILEDPRODUCER_HPP_

#include <plato/das/l0app/events/parameterCalculation/ncam/L0AppTmNcamREvtInfCcdPosDetailedProducer.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace events {
				namespace parameterCalculation {
					namespace ncamb {
						/**
						 * @brief Template class instantiation for N-FEE A to handle N-FEE informative event upon star position calculation progress
						 * to produce a meta-defined event packet.
						 */
						typedef TmNcamREvtInfCcdPosDetailedProducer<cameraIdentifier::NCAM_B> TmNcambREvtInfCcdPosDetailedProducer;
					}
				}
			}
		}
	}
}

using plato::das::l0app::events::parameterCalculation::ncamb::TmNcambREvtInfCcdPosDetailedProducer;


#endif /* L0APPTMNCAMBREVTINFCCDPOSDETAILEDPRODUCER_HPP_ */
