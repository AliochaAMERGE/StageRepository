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

 L0AppTcNcamSSciAttitudeLoadParser.hpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTcNcamSSciAttitudeLoadParser.hpp									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 28 avr. 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTCNCAMSSCIATTITUDELOADPARSER_HPP_
#define L0APPTCNCAMSSCIATTITUDELOADPARSER_HPP_

#include <plato/das/idb/processes/ncam/parameters/IdbNcamDataPool.hpp>
#include <plato/das/l0app/science/camera/L0AppCameraDataSet.hpp>
#include <plato/das/idb/processes/ncam/packets/telecommand/ncamParameterCalculation/IdbTcNcamSSciAttitudeLoadParser.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace telecommand {
				namespace parser {
					namespace nCamparameterCalculation {

						/**
						 * @brief This class is ued to process a TC_NC01_S_SCI_ATTITUDE_LOAD Packet
						 * Load or update the camera attitude.
						 */
						class TcNcamSSciAttitudeLoadParser: public IdbTcNcamSSciAttitudeLoadParser {
						private:
							/// Non functionnal copy constructor
							TcNcamSSciAttitudeLoadParser(const TcNcamSSciAttitudeLoadParser* other);
							/// Non functionnal copy constructor
							TcNcamSSciAttitudeLoadParser(const TcNcamSSciAttitudeLoadParser& other);
							/// Nno functionnal overload of the assignment operator
							TcNcamSSciAttitudeLoadParser& operator=(const TcNcamSSciAttitudeLoadParser* other);

							IdbNcamDataPool* ncamaDataPool;	 ///< DataPool used to extract TC parameters for the ncamA
							IdbNcamDataPool* ncambDataPool;	 ///< DataPool used to extract TC parameters for the ncamB

							CameraDataSet* cameraDataSet;		///< currently used camera data set
							CameraDataSet* ncamaCameraDataSet;	///< ncamA dataSet containing the offset descriptor sets
							CameraDataSet* ncambCameraDataSet;	///< ncamB dataSet containing the offset descriptor sets

							uint8_t currentIndex; ///< current index of the matrix to be loaded

						public:
							/// Default constructor
							TcNcamSSciAttitudeLoadParser();

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
							 * Setter of ncamaDataPool
							 * @param ncamaDataPool The new ncamaDataPool to set
							 */
							void setNcamaDataPool(IdbNcamDataPool* ncamaDataPool) {
								this->ncamaDataPool = ncamaDataPool;
							}

							/**
							 * Setter of ncambDataPool
							 * @param ncambDataPool The new ncambDataPool to set
							 */
							void setNcambDataPool(IdbNcamDataPool* ncambDataPool) {
								this->ncambDataPool = ncambDataPool;
							}

							/**
							 * Inherited from GsbMetaDefinedPacketParser
							 * Set the data pool, camera data set to camA or camB based on the packet's Pid
							 */
							gsbManagerErrorCode::GsbManagerErrorCodeEnum execute(uint32_t& bytePosition, uint32_t& receivedValue,
									GsbAuxiliaryParameters& executionStatus);

							/**
							 * Inherited from GsbMetaDefinedPacketParser
							 * Set TcNcamSSciAttitudeLoadParser::currentIndex to 0
							 */
							virtual gsbManagerErrorCode::GsbManagerErrorCodeEnum processStartOfLoop(uint32_t level, uint32_t structureId,
									GsbAuxiliaryParameters& executionStatus);

							/**
							 * Inherited from GsbMetaDefinedPacketParser
							 * Load an element of the camera Attitude matrix
							 * Increments currentIndex
							 */
							virtual gsbManagerErrorCode::GsbManagerErrorCodeEnum processEndOfLoopStructure(uint32_t level,
									uint32_t structureIndex, uint32_t structureId, GsbAuxiliaryParameters& executionStatus);
						};
					}
				}
			}
		}
	}
}

using plato::das::l0app::telecommand::parser::nCamparameterCalculation::TcNcamSSciAttitudeLoadParser;

#endif /* L0APPTCNCAMSSCIATTITUDELOADPARSER_HPP_ */

