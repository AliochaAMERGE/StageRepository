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

 L0AppTcNcamSIgmLoadParser.h

 $Rev: 000 $

 ************************************************************************/

/************************************************************************

 COMPONENT DEFINITION							RELEASE

 L0AppTcNcamSIgmLoadParser.h									0.1.0.0

 AUTHOR

 Aliocha AMERGE, LESIA.

 DESCRIPTION

 This file defines ... TO BE COMPLETED ...

 RELEASE HISTORY

 DATE				NAME							DESCRIPTION

 19 avr. 2022		Aliocha AMERGE			Initial Version 	0.1.0.0

 ************************************************************************/

#ifndef L0APPTCNCAMSIGMLOADPARSER_HPP_
#define L0APPTCNCAMSIGMLOADPARSER_HPP_

#include <plato/das/idb/processes/ncam/packets/telecommand/ncamScienceConfiguration/IdbTcNcamSIgmLoadParser.hpp>
#include <plato/das/idb/processes/ncam/parameters/IdbNcamDataPool.hpp>
#include <plato/das/l0app/science/camera/L0AppCameraDataSet.hpp>

namespace plato {
	namespace das {
		namespace l0app {
			namespace telecommand {
				namespace parser {
					namespace ncamScienceConfiguration {

						/**
						 * This class is used to process a TC packet to load the parameters of the image geometry model.
						 */
						class TcNcamSIgmLoadParser: public IdbTcNcamSIgmLoadParser {
						private:
							/// Non functional copy constructor
							TcNcamSIgmLoadParser(const TcNcamSIgmLoadParser* other);
							/// Non functional copy constructor
							TcNcamSIgmLoadParser(const TcNcamSIgmLoadParser& other);
							/// Non functional copy assignment operator
							TcNcamSIgmLoadParser& operator=(const TcNcamSIgmLoadParser& other);

							// Used attached elements
							/// DataPool used to extract TC parameters for the ncamA
							IdbNcamDataPool* nCamADataPool;
							/// DataPool used to extract TC parameters for the ncamB
							IdbNcamDataPool* nCamBDataPool;

							CameraDataSet* cameraDataSet;
							/// ncamA dataSet containing the image geometry model parameters
							CameraDataSet* nCamACameraDataSet;
							/// ncamB dataSet containing the image geometry model parameters
							CameraDataSet* nCamBCameraDataSet;

							/**
							 * @brief Update the calibration CCD parameters of a IgmGlobalParameterSet with the dataPool values.
							 * @param imageGeometryModel The image geometry model parameters set to update.
							 */
							void loadIgmGlobalCCDParameterSet(IgmGlobalParameterSet& imageGeometryModel);

						public:
							/// default constructor
							TcNcamSIgmLoadParser();

							/**
							 * @brief Overloaded method to select the right data pool in base class before TC execution
							 */
							gsbManagerErrorCode::GsbManagerErrorCodeEnum execute(uint32_t& bytePosition, uint32_t& receivedValue,
									GsbAuxiliaryParameters& executionStatus);

							/**
							 * @brief Load the image geometry model parameters in the dataset of the camera associated to the packets's PID.
							 */
							gsbManagerErrorCode::GsbManagerErrorCodeEnum processEndOfPacket(GsbAuxiliaryParameters& executionStatus);

							/**
							 * @brief Setter of nCamACameraDataSet
							 * @param nCamACameraDataSet The new nCamACameraDataSet to set
							 */
							void setNCamACameraDataSet(CameraDataSet* nCamACameraDataSet) {
								this->nCamACameraDataSet = nCamACameraDataSet;
							}

							/**
							 * @brief Setter of nCamBCameraDataSet
							 * @param nCamBCameraDataSet The new nCamBCameraDataSet to set
							 */
							void setNCamBCameraDataSet(CameraDataSet* nCamBCameraDataSet) {
								this->nCamBCameraDataSet = nCamBCameraDataSet;
							}

							/**
							 * @brief Setter of nCamADataPool
							 * @param nCamADataPool The new nCamADataPool to set
							 */
							void setNCamADataPool(IdbNcamDataPool* nCamADataPool) {
								this->nCamADataPool = nCamADataPool;
							}

							/**
							 * @brief Setter of nCamBDataPool
							 * @param nCamBDataPool The new nCamBDataPool to set
							 */
							void setNCamBDataPool(IdbNcamDataPool* nCamBDataPool) {
								this->nCamBDataPool = nCamBDataPool;
							}
						};

					}
				}
			}
		}
	}
}

using plato::das::l0app::telecommand::parser::ncamScienceConfiguration::TcNcamSIgmLoadParser;

#endif /* L0APPTCNCAMSIGMLOADPARSER_HPP_ */
