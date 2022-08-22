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

	L0AppTcNdpuParameterCalcEnterParser.cpp

	$Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/telecommand/parser/modeManagement/L0AppTcNdpuParameterCalcEnterParser.hpp>

TcNdpuParameterCalcEnterParser::TcNdpuParameterCalcEnterParser() {
  // init pointers
  ncamaScienceProcessor = 0;
  ncambScienceProcessor = 0;
}

gsbManagerErrorCode::GsbManagerErrorCodeEnum TcNdpuParameterCalcEnterParser::processEndOfPacket(GsbAuxiliaryParameters& executionStatus) {
	
  gsbManagerErrorCode::GsbManagerErrorCodeEnum managerErrorCode = TcNdpuModeEnterParser::processEndOfPacket(idbNdpuModeType::NDPU_PARAMETER_CALC);
  if (managerErrorCode == gsbManagerErrorCode::SUCCESS) {
		// the mode transition is authorized :
		if (ncamaScienceProcessor != 0) {
			// call the start of the window programming procedure for the NCAM-A
			ncamaScienceProcessor->calculateStarsPositions();
		}
		if (ncambScienceProcessor != 0) {
			// call the start of the window programming procedure for the NCAM-B
			ncambScienceProcessor->calculateStarsPositions();
		}
	}
	return managerErrorCode;
}