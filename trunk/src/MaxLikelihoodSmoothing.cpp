////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008, Massachusetts Institute of Technology              //
// All rights reserved.                                                   //
//                                                                        //
// Redistribution and use in source and binary forms, with or without     //
// modification, are permitted provided that the following conditions are //
// met:                                                                   //
//                                                                        //
//     * Redistributions of source code must retain the above copyright   //
//       notice, this list of conditions and the following disclaimer.    //
//                                                                        //
//     * Redistributions in binary form must reproduce the above          //
//       copyright notice, this list of conditions and the following      //
//       disclaimer in the documentation and/or other materials provided  //
//       with the distribution.                                           //
//                                                                        //
//     * Neither the name of the Massachusetts Institute of Technology    //
//       nor the names of its contributors may be used to endorse or      //
//       promote products derived from this software without specific     //
//       prior written permission.                                        //
//                                                                        //
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS    //
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      //
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  //
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   //
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  //
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT       //
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  //
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  //
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT    //
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  //
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.   //
////////////////////////////////////////////////////////////////////////////

#include "Types.h"
#include "NgramLM.h"
#include "Mask.h"
#include "Smoothing.h"
#include "MaxLikelihoodSmoothing.h"

////////////////////////////////////////////////////////////////////////////////

Smoothing::~Smoothing() { }

////////////////////////////////////////////////////////////////////////////////

void
MaxLikelihoodSmoothing::Initialize(NgramLM *pLM, size_t order) {
    assert(order != 0);
    _pLM = pLM;
    _order = order;
}

bool
MaxLikelihoodSmoothing::Estimate(const ParamVector &params,
                                 const NgramLMMask *pMask,
                                 ProbVector &probs,
                                 ProbVector &bows) {
    if (!_estimated) {
        const CountVector &counts(_pLM->counts(_order));
        const IndexVector &hists(_pLM->hists(_order));

        // Compute inverse of sum of adjusted counts for each history.
        CountVector histCounts(_pLM->sizes(_order - 1), 0);
        ProbVector  invHistCounts(histCounts.length());
        BinCount(hists, histCounts);
        invHistCounts = 1.0 / asDouble(histCounts);

        // Compute maximum likelihood probability.  0 backoff.
        probs = counts * invHistCounts[hists];
        bows.set(0);
        _estimated = true;
    }
    return true;
}
