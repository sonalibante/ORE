/*
 Copyright (C) 2016 Quaternion Risk Management Ltd
 All rights reserved.

 This file is part of ORE, a free-software/open-source library
 for transparent pricing and risk analysis - http://opensourcerisk.org

 ORE is free software: you can redistribute it and/or modify it
 under the terms of the Modified BSD License.  You should have received a
 copy of the license along with this program.
 The license is also available online at <http://opensourcerisk.org>

 This program is distributed on the basis that it will form a useful
 contribution to risk analytics and model standardisation, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
*/

#include <ored/portfolio/builders/capfloorediborleg.hpp>
#include <ored/utilities/log.hpp>

#include <boost/make_shared.hpp>

namespace ore {
namespace data {

boost::shared_ptr<FloatingRateCouponPricer> CapFlooredIborLegEngineBuilder::engineImpl(const Currency& ccy) {

    // Check if we already have a pricer for this ccy
    const string& ccyCode = ccy.code();
    if (engines_.find(ccyCode) == engines_.end()) {
        Handle<YieldTermStructure> yts = market_->discountCurve(ccyCode, configuration(MarketContext::pricing));
        QL_REQUIRE(!yts.empty(), "engineFactory error: yield term structure not found for currency " << ccyCode);
        Handle<OptionletVolatilityStructure> ovs = market_->capFloorVol(ccyCode, configuration(MarketContext::pricing));
        BlackIborCouponPricer::TimingAdjustment timingAdjustment = BlackIborCouponPricer::Black76;
        boost::shared_ptr<SimpleQuote> correlation = boost::make_shared<SimpleQuote>(1.0);
        // for backwards compatibility we do not require the additional timing adjustment fields
        if (engineParameters_.find("TimingAdjustment") != engineParameters_.end()) {
            string adjStr = engineParameter("TimingAdjustment");
            if (adjStr == "Black76")
                timingAdjustment = BlackIborCouponPricer::Black76;
            else if (adjStr == "BivariateLognormal")
                timingAdjustment = BlackIborCouponPricer::BivariateLognormal;
            else {
                QL_FAIL("timing adjustment parameter (" << adjStr << ") not recognised.");
            }
            correlation->setValue(parseReal(engineParameter("Correlation")));
        }
        boost::shared_ptr<FloatingRateCouponPricer> pricer =
            boost::make_shared<BlackIborCouponPricer>(ovs, timingAdjustment, Handle<Quote>(correlation));
        engines_[ccyCode] = pricer;
    }

    // Return the cached pricer
    return engines_[ccyCode];
}
} // namespace data
} // namespace ore
