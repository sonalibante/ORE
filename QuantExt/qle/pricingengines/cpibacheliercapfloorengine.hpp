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

/*! \file qle/pricingengines/cpibacheliercapfloorengine.hpp
    \brief CPI cap/floor engine using the Bachelier pricing formula and interpreting the volatility data as normal vols
    \ingroup engines
*/

#ifndef quantext_cpi_bachelier_capfloor_engine_hpp
#define quantext_cpi_bachelier_capfloor_engine_hpp

#include <ql/instruments/cpicapfloor.hpp>
#include <ql/termstructures/volatility/inflation/cpivolatilitystructure.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>

namespace QuantExt {

class CPIBachelierCapFloorEngine : public QuantLib::CPICapFloor::engine {
public:
    CPIBachelierCapFloorEngine(const QuantLib::Handle<QuantLib::YieldTermStructure>& discountCurve,
                               const QuantLib::Handle<QuantLib::CPIVolatilitySurface>& surface);

    virtual void calculate() const;
    virtual std::string name() const { return "CPIBachelierCapFloorEngine"; }

    virtual ~CPIBachelierCapFloorEngine() {}

    void setVolatility(const QuantLib::Handle<QuantLib::CPIVolatilitySurface>& surface) {
        if (!volatilitySurface_.empty())
            unregisterWith(volatilitySurface_);
        volatilitySurface_ = surface;
        registerWith(volatilitySurface_);
        update();
    }

protected:
    QuantLib::Handle<QuantLib::YieldTermStructure> discountCurve_;
    QuantLib::Handle<QuantLib::CPIVolatilitySurface> volatilitySurface_;
};

} // namespace QuantExt

#endif
