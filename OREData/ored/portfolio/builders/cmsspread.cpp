/*
 Copyright (C) 2018 Quaternion Risk Management Ltd
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

#include <ored/portfolio/builders/cmsspread.hpp>
#include <ored/utilities/log.hpp>
#include <ored/utilities/parsers.hpp>
#include <qle/termstructures/correlationtermstructure.hpp>

#include <boost/make_shared.hpp>

using namespace QuantLib;

namespace ore {
namespace data {

boost::shared_ptr<FloatingRateCouponPricer>
CmsSpreadCouponPricerBuilder::engineImpl(const Currency& ccy, const string& index1, const string& index2,
                                         const boost::shared_ptr<CmsCouponPricer>& cmsPricer) {

    QuantLib::Handle<QuantExt::CorrelationTermStructure> corrCurve =
        market_->correlationCurve(index1, index2, configuration(MarketContext::pricing));
    if (corrCurve.empty())
        corrCurve = market_->correlationCurve(index2, index1, configuration(MarketContext::pricing));

    QL_REQUIRE(!corrCurve.empty(), "no correlation curve found for " << index1 << ":" << index2);

    const string& ccyCode = ccy.code();
    return boost::make_shared<QuantExt::LognormalCmsSpreadPricer>(
        cmsPricer, corrCurve, market_->discountCurve(ccyCode, configuration(MarketContext::pricing)),
        parseInteger(engineParameter("IntegrationPoints")));
}

} // namespace data
} // namespace ore
