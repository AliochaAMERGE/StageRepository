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

 L0AppCcdPositionDetailedParameters.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/science/calculator/L0AppCcdPositionDetailedParameters.hpp>


CcdPositionDetailedParameters& CcdPositionDetailedParameters::operator=(CcdPositionDetailedParameters const& source) {
	// check that the source and the destination are not the same object
	if (&source != this) {
		// Copy all element from source to this
		evtInfId = source.evtInfId;
		descrId = source.descrId;
		timestamp = source.timestamp;
		gcrsAscension = source.gcrsAscension;
		gcrsDeclination = source.gcrsDeclination;
		// copy these elements too
		cameraX = source.cameraX;
		cameraY = source.cameraY;
		cameraZ = source.cameraZ;
		focalPlaneX = source.focalPlaneX;
		focalPlaneY = source.focalPlaneY;
		// and these too
		ccdId = source.ccdId;
		ccdSide = source.ccdSide;
		windowX = source.windowX;
		windowY = source.windowY;
	}
	// return a reference to this
	return *this;
}

void CcdPositionDetailedParameters::setFromStarsPositionsCalculatorParameterSet(StarsPositionsCalculatorParameterSet& starPosCalcParamSet, GsbTimeProvider* timeProvider, int32_t descrId) {
	// set the event information id
	this->setDescrId(descrId);
	// set the timestamp
	this->setTimestamp(timeProvider->getTime());
	// set the gcrs coordinates
	this->setGcrsAscension(starPosCalcParamSet.getStarRightAscension());
	this->setGcrsDeclination(starPosCalcParamSet.getStarDeclination());
	// Set the cameraDirection and the Focal plane coordinates
	setFromStarsPositionsCalculatorParameterSetCamDirPosFoc(starPosCalcParamSet);
	// set the ccd id
	this->setCcdId(starPosCalcParamSet.getCcdId());
	// set the ccd side
	this->setCcdSide(starPosCalcParamSet.getCcdSide());
	// set the window coordinates
	this->setWindowX(starPosCalcParamSet.getWindowX());
	this->setWindowY(starPosCalcParamSet.getWindowY());
}

void CcdPositionDetailedParameters::setFromStarsPositionsCalculatorParameterSetCamDirPosFoc(StarsPositionsCalculatorParameterSet& starPosCalcParamSet) {
	
	// set the camera direction
	this->setCameraX(starPosCalcParamSet.getCameraDirections(0));
	this->setCameraY(starPosCalcParamSet.getCameraDirections(1));
	this->setCameraZ(starPosCalcParamSet.getCameraDirections(2));
	// set the focal plane coordinates
	this->setFocalPlaneX(starPosCalcParamSet.getPosFocalPlane(0));
	this->setFocalPlaneY(starPosCalcParamSet.getPosFocalPlane(1));
}