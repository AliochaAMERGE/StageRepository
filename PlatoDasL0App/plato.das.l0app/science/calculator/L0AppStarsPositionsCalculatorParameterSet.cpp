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

 L0AppStarPositionsCalculatorParameters.cpp

 $Rev: 000 $

 ************************************************************************/

#include <plato/das/l0app/science/calculator/L0AppStarsPositionsCalculatorParameterSet.hpp>

StarsPositionsCalculatorParameterSet::StarsPositionsCalculatorParameterSet() {
	// reset all parameters to default values
	reset();
}

StarsPositionsCalculatorParameterSet::StarsPositionsCalculatorParameterSet(const StarsPositionsCalculatorParameterSet& other) {
	// copy all parameters from other
	this->operator=(other);
}

StarsPositionsCalculatorParameterSet& StarsPositionsCalculatorParameterSet::operator=(const StarsPositionsCalculatorParameterSet& source) {
	// check that the source and the destination are not the same object
	if (&source != this) {
		// copy all parameters from source
		this->starRightAscension = source.starRightAscension;
		this->starDeclination = source.starDeclination;
		this->starCoordinatesGcrs[0] = source.starCoordinatesGcrs[0];
		this->starCoordinatesGcrs[1] = source.starCoordinatesGcrs[1];
		this->starCoordinatesGcrs[2] = source.starCoordinatesGcrs[2];
		// copy all parameters from source
		this->cameraDirections[0] = source.cameraDirections[0];
		this->cameraDirections[1] = source.cameraDirections[1];
		this->cameraDirections[2] = source.cameraDirections[2];
		this->posFocalPlane[0] = source.posFocalPlane[0];
		this->posFocalPlane[1] = source.posFocalPlane[1];
		// copy all parameters from source
		this->starSD[0] = source.starSD[0];
		this->starSD[1] = source.starSD[1];
		this->starFee[0] = source.starFee[0];
		this->starFee[1] = source.starFee[1];
		// copy all parameters from source
		this->windowX = source.windowX;
		this->windowY = source.windowY;
		this->currentTime = source.currentTime;
		this->ccdId = source.ccdId;
		this->ccdSide = source.ccdSide;
	}
	// return this object
	return *this;
}

void StarsPositionsCalculatorParameterSet::reset() {
	// Reset all parameters to their default values (0)
	starRightAscension = 0.0;
	starDeclination = 0.0;
	// Reset all parameters to their default values (0)
	starCoordinatesGcrs[0] = 0.0;
	starCoordinatesGcrs[1] = 0.0;
	starCoordinatesGcrs[2] = 0.0;
	cameraDirections[0] = 0.0;
	cameraDirections[1] = 0.0;
	cameraDirections[2] = 0.0;
	// Reset all parameters to their default values (0)
	posFocalPlane[0] = 0.0;
	posFocalPlane[1] = 0.0;
	starSD[0] = 0.0;
	starSD[1] = 0.0;
	starFee[0] = 0.0;
	starFee[1] = 0.0;
	windowX = 0.0;
	windowY = 0.0;
	ccdId = idbCcdNumberType::CCD1;		//< 0
	ccdSide = idbCcdSideType::RIGHT;	//< 0
	currentTime = 0.0;
}
