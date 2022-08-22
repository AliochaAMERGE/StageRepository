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

 L0AppTcNcamSReqIgmDumpParser.h

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTcNcamSReqIgmDumpParser.h									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 21 avr. 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTCNCAMSREQIGMDUMPPARSER_HPP_
#define L0APPTCNCAMSREQIGMDUMPPARSER_HPP_

#include <gericos/blocks/telemetry/GsbTmWriter.hpp>
#include <plato/das/idb/processes/ncam/packets/telecommand/ncamScienceConfiguration/IdbTcNcamSReqIgmDumpParser.hpp>
#include <plato/das/idb/processes/ncam/parameters/IdbNcamDataPool.hpp>
#include <plato/das/idb/processes/ncama/packets/telemetry/ncamScienceConfiguration/IdbTmNcamaSImageGeometryModelDumpReportProducer.hpp>
#include <plato/das/idb/processes/ncamb/packets/telemetry/ncamScienceConfiguration/IdbTmNcambSImageGeometryModelDumpReportProducer.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace telecommand {
				namespace parser {
					namespace ncamScienceConfiguration {

						/**
						 * This class is used to process a TC packet to request a dump of the parameters of the image geometry model.
						 */
						class TcNcamSReqIgmDumpParser: public IdbTcNcamSReqIgmDumpParser {
						private:
							/// Non functional copy constructor
							TcNcamSReqIgmDumpParser(const TcNcamSReqIgmDumpParser* source);
							/// Non functional copy constructor
							TcNcamSReqIgmDumpParser(const TcNcamSReqIgmDumpParser& source);
							/// Non functional overload of the assignment operator
							TcNcamSReqIgmDumpParser& operator=(const TcNcamSReqIgmDumpParser& source);

							/// Writer used to generate the TM packet
							GsbTmWriter tmWriter;
							/// CamA meta defined TM packet producer
							IdbTmNcamaSImageGeometryModelDumpReportProducer ncamaProducer;
							/// CamB meta defined TM packet producer
							IdbTmNcambSImageGeometryModelDumpReportProducer ncambProducer;

						public:
							/**
							 * @brief Default empty constructor.
							 * Initialize the pointers to 0 and the TM packet
							 */
							TcNcamSReqIgmDumpParser() {
								// Empty constructor
							}

							/**
							 * @brief Register the GsbPacketWriter needed by the tmWriter included in the class.
							 * @param packetWriter the GsbPacketerWriter to register.
							 */
							inline void setPacketWriter(GsbPacketWriter* packetWriter) {
								tmWriter.setPacketWriter(packetWriter);
							}

							/**
							 * @brief Register the GsbTimeProvider needed by the tmWriter included in the class.
							 * @param timeProvider the GsbTimeProvider to register.
							 */
							inline void setTimeProvider(GsbTimeProvider* timeProvider) {
								tmWriter.setTimeProvider(timeProvider);
							}

							/**
							 * @brief Register the data pool needed by the camA TM packet producer included in the class.
							 * @param dataPool pointer on the instance of the data pool to attach
							 */
							void setNcamaDataPool(IdbNcamDataPool* dataPool) {
								ncamaProducer.setDataPool(dataPool);
							}

							/**
							 * @brief Register the data pool needed by the camB TM packet producer included in the class.
							 * @param dataPool pointer on the instance of the data pool to attach
							 */
							void setNcambDataPool(IdbNcamDataPool* dataPool) {
								ncambProducer.setDataPool(dataPool);
							}

							/**
							 * @brief Verify the incoming TC packet and generate the ncamA or ncamB image geometry model parameters dump
							 * Inherited from GsbMetaDefinedPacketParser
							 */
							virtual gsbManagerErrorCode::GsbManagerErrorCodeEnum processEndOfPacket(
									GsbAuxiliaryParameters& executionStatus);
						};
					}
				}
			}
		}
	}
}

using plato::das::l0app::telecommand::parser::ncamScienceConfiguration::TcNcamSReqIgmDumpParser;

#endif /* L0APPTCNCAMSREQIGMDUMPPARSER_H_ */
