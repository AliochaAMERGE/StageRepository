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

 L0AppTcNcamSReqSciAttiDumpParser.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTcNcamSReqSciAttiDumpParser.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 2 mai 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTCNCAMSREQSCIATTIDUMPPARSER_HPP_
#define L0APPTCNCAMSREQSCIATTIDUMPPARSER_HPP_

#include <gericos/blocks/telemetry/GsbTmWriter.hpp>
#include <plato/das/idb/processes/ncam/packets/telecommand/ncamParameterCalculation/IdbTcNcamSReqSciAttiDumpParser.hpp>
#include <plato/das/instances/gericos/blocks/telemetry/packet/GsbTmRepeatedStructurePacketInst.hpp>
#include <plato/das/l0app/science/camera/L0AppCameraDataSet.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace telecommand {
				namespace parser {
					namespace ncamParameterCalculation {
						/**
						 * @brief This class is used to process a TC_NC01_S_REQ_SCI_ATTI_DUMP packet (Request a dump of the attitude data)
						 * Produces a TM_NC01_S_SCI_ATTI_DUMP packet
						 */
						class TcNcamSReqSciAttiDumpParser : public IdbTcNcamSReqSciAttiDumpParser {
						private:
							/// Non functionnal copy constructor
							TcNcamSReqSciAttiDumpParser(const TcNcamSReqSciAttiDumpParser* other);
							/// Non functional copy constructor
							TcNcamSReqSciAttiDumpParser(const TcNcamSReqSciAttiDumpParser& other);
							/// Non functional overload of the assignment operator
							TcNcamSReqSciAttiDumpParser& operator=(const TcNcamSReqSciAttiDumpParser& other);

							CameraDataSet* ncamaCameraDataSet;		///< ncamA dataSet containing the offset descriptor sets
							CameraDataSet* ncambCameraDataSet;		///< ncamB dataSet containing the offset descriptor sets

							TmRepeatedStructurePacket tmAttiPacket;			///< Packet of the camera attitude parameters Tm
							GsbTmWriter tmWriter;							///< writer used to generate the Tm
							CcsdsTmRepeatedStructurePacket* ccsdsTmPacket;	///< ccsdsPacket of the camera attitude parameters Tm

							/**
							 * @brief	Finalize the TM packet and forward it to the tm packets writter
							 *
							 * @param dataFieldSize[in]	Size of the data field of the packet to produce
							 * @param accessError[out]	Status of the packet's finalization
							 */
							void finalizeAndDumpPacket(uint32_t dataFieldSize, gsbAccessError::GsbAccessErrorEnum& accessError);

							/**
							 * @brief	Build the user dataField of the TM packet (ccd infos and camera attitude matrix)
							 *
							 * @param	cameraDataSet[in]	Pointer towards the used camera data set
							 * @param	ccdId[in]			Id of the current ccd
							 * @param	ccdSide[in]			Side of the current ccd
							 * @param	accessError[out]	Status of the data field building
							 */
							uint32_t buildPacketDataField(CameraDataSet* cameraDataSet, gsbAccessError::GsbAccessErrorEnum& accessError);

						 public:
							/// Default constructor
							TcNcamSReqSciAttiDumpParser();

							/**
							 * Setter of ncamaCameraDataSet
							 * @param ncamaCameraDataSet The new ncamaCameraDataSet to set
							 */
							void setNcamaCameraDataSet(CameraDataSet* ncamaCameraDataSet) {
								this->ncamaCameraDataSet = ncamaCameraDataSet;
							}

							/**
							 * Setter of ncambCameraDataSet
							 * @param ncambCameraDataSet The new ncambCameraDataSet to set
							 */
							void setNcambCameraDataSet(CameraDataSet* ncambCameraDataSet) {
								this->ncambCameraDataSet = ncambCameraDataSet;
							}

							/**
							 * Register the GsbPacketWriter needed by the tmWriter included in the class.
							 * @param packetWriter
							 */
							inline void setPacketWriter(GsbPacketWriter* packetWriter) {
								tmWriter.setPacketWriter(packetWriter);
							}

							/**
							 * Register the GsbTimeProvider needed by the tmWriter included in the class.
							 * @param timeProvider
							 */
							inline void setTimeProvider(GsbTimeProvider* timeProvider) {
								tmWriter.setTimeProvider(timeProvider);
							}

							/**
							 * Inherited from GsbMetaDefinedPacketParser
							 * Verify the connection with all the elements (timerProvider, packetWriter, packets and camera data set)
							 * Generate the TM packet
							 */
							gsbManagerErrorCode::GsbManagerErrorCodeEnum processEndOfPacket(GsbAuxiliaryParameters& executionStatus);
						};
					}
				}
			}
		}
	}
}

using plato::das::l0app::telecommand::parser::ncamParameterCalculation::TcNcamSReqSciAttiDumpParser;
#endif /* L0APPTCNCAMSREQSCIATTIDUMPPARSER_HPP_ */
