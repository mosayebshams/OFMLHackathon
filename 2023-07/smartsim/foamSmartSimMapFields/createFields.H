/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2023 TU Darmstadt  
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    foamSmartSimMapFields

Description

    Uses SmartSim/SmartRedis to train an ML model on a field on a input mesh
    resolution and then approximates the field on the output mesh resolution
    using the ML model.

\*---------------------------------------------------------------------------*/

#include "regIOobject.H"
tmp<volScalarField> inputVolScalarFieldTmp(nullptr); 
tmp<volVectorField> inputVolVectorFieldTmp(nullptr); 

const word field = args.get<word>("field");

// TODO(TM): hardcoded to select latest time from the input case, add options.
instantList inputTimeDirs = timeSelector::select0(runTimeInput, args);
const auto latestTime = inputTimeDirs[inputTimeDirs.size() - 1];
IOobject fieldHeader
(
    field, 
    latestTime.name(), 
    runTimeInput,  
    IOobject::MUST_READ
);
    
// If the field file header corresponds to a cell-centered scalar field
if (fieldHeader.typeHeaderOk<volScalarField>(true, true, false))
{
    Info <<  "Reading input field " <<  fieldHeader.objectPath() << endl;
    inputVolScalarFieldTmp = new volScalarField(fieldHeader, inputMesh);
}
// If the field file header corresponds to a cell-centered vector field
else if (fieldHeader.typeHeaderOk<volVectorField>(true, true, false)) 
{
    Info <<  "Reading input field " <<  fieldHeader.objectPath() << endl;
    inputVolVectorFieldTmp = new volVectorField(fieldHeader, inputMesh);
}

// ************************************************************************* //
