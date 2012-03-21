// @HEADER
// ***********************************************************************
//
//           Panzer: A partial differential equation assembly
//       engine for strongly coupled complex multiphysics systems
//                 Copyright (2011) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Roger P. Pawlowski (rppawlo@sandia.gov) and
// Eric C. Cyr (eccyr@sandia.gov)
// ***********************************************************************
// @HEADER

#ifndef PANZER_GATHER_INTEGRATION_COORDINATES_IMPL_HPP
#define PANZER_GATHER_INTEGRATION_COORDINATES_IMPL_HPP

#include "Teuchos_Assert.hpp"
#include "Phalanx_DataLayout.hpp"

#include "Panzer_IntegrationRule.hpp"
#include "Panzer_Workset_Utilities.hpp"

#include "Teuchos_FancyOStream.hpp"

template<typename EvalT,typename Traits>
std::string 
panzer::GatherIntegrationCoordinates<EvalT, Traits>::
fieldName(int degree)
{
   std::stringstream ss; 
   ss << "IR_" << degree << " IntegrationCoordinates";
   return ss.str();
}

template<typename EvalT,typename Traits>
panzer::GatherIntegrationCoordinates<EvalT, Traits>::
GatherIntegrationCoordinates(const panzer::IntegrationRule & quad)
{ 
  quadDegree_ = quad.cubature_degree;

  quadCoordinates_ = PHX::MDField<ScalarT,Cell,Point,Dim>(fieldName(quadDegree_),quad.dl_vector);

  this->addEvaluatedField(quadCoordinates_);

  this->setName("Gather "+fieldName(quadDegree_));
}

// **********************************************************************
template<typename EvalT,typename Traits>
void panzer::GatherIntegrationCoordinates<EvalT, Traits>::
postRegistrationSetup(typename Traits::SetupData sd, 
		      PHX::FieldManager<Traits>& fm)
{
  this->utils.setFieldData(quadCoordinates_,fm);

  quadIndex_ = panzer::getIntegrationRuleIndex(quadDegree_, (*sd.worksets_)[0]);
}

// **********************************************************************
template<typename EvalT,typename Traits> 
void panzer::GatherIntegrationCoordinates<EvalT, Traits>::
evaluateFields(typename Traits::EvalData workset)
{ 
  const Intrepid::FieldContainer<double> & quadCoords = workset.int_rules[quadIndex_]->ip_coordinates;  

  // just copy the array
  for(int i=0;i<quadCoords.size();i++)
     quadCoordinates_[i] = quadCoords[i];
}

// **********************************************************************
#endif
