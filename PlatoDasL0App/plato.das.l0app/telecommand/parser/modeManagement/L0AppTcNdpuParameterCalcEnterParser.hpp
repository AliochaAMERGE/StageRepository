/*************************************************************************

 Copyright (c) 2019 by CNRS/LESIA

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

 L0AppTcNdpuParameterCalcEnterParser.hpp

 $Rev: 25439 $

 ************************************************************************/

#ifndef L0APPTCNDPUPARAMETERCALCENTERPARSER_HPP_
#define L0APPTCNDPUPARAMETERCALCENTERPARSER_HPP_

#include <plato/das/idb/processes/ndpu/packets/telecommand/modeManagement/IdbTcNdpuRParamCalcEnterParser.hpp>
#include <plato/das/l0app/telecommand/parser/modeManagement/L0AppTcNdpuModeEnterParser.hpp>
#include <plato/das/l0app/science/L0AppScienceProcessor.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace telecommand {
				namespace parser {
					namespace modeManagement {
						/**
						 * This class is used to process a TC packet to enter in parameter calc mode.
						 */
						class TcNdpuParameterCalcEnterParser: public IdbTcNdpuRParamCalcEnterParser, public TcNdpuModeEnterParser {
						private:
							TcNdpuParameterCalcEnterParser(TcNdpuParameterCalcEnterParser const* source); ///< Non functional copy constructor
							TcNdpuParameterCalcEnterParser(TcNdpuParameterCalcEnterParser const& source); ///< Non functional copy constructor
							TcNdpuParameterCalcEnterParser& operator=(TcNdpuParameterCalcEnterParser& source);///< Non functional overload of the assignment operator

						protected:

							ScienceProcessor* ncamaScienceProcessor;
							ScienceProcessor* ncambScienceProcessor;

							/**
							 * Call the start of the stars positions calculation procedure for the two cameras after the call of the same method of the base class TcNdpuModeEnterParser, if the mode transition is
							 * authorized.
							 *
							 * @retval value returned by the same method of the base class TcNdpuModeEnterParser
							 */
							virtual gsbManagerErrorCode::GsbManagerErrorCodeEnum processEndOfPacket(GsbAuxiliaryParameters& executionStatus);

						public:
							/// Default constructor
							TcNdpuParameterCalcEnterParser();

							/**
							 * Set connection with the science processor task of the NCAM-A
							 * @param modeManager pointer on the window programmer task.
							 */
							void setNcamaScienceProcessor(ScienceProcessor* scienceProcessor) {
								this->ncamaScienceProcessor = scienceProcessor;
							}

							/**
							 * Set connection with the science processor task of the NCAM-B
							 * @param modeManager pointer on the window programmer task proxy.
							 */
							void setNcambScienceProcessor(ScienceProcessor* scienceProcessor) {
								this->ncambScienceProcessor = scienceProcessor;
							}

						};
					}
				}
			}
		}
	}
}

using plato::das::l0app::telecommand::parser::modeManagement::TcNdpuParameterCalcEnterParser;

#endif /* L0APPTCNDPUPARAMETERCALCENTERPARSER_HPP_ */
