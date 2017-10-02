//*! \file DateTenor.hpp Arithmetics of dates and tenors
//
//  DateTenor.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 18/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef DateTenor_hpp
#define DateTenor_hpp

#include <XQLib/Core/Maybe.hpp>
#include <XQLib/DateTime/Date.hpp>
#include <XQLib/DateTime/Tenor.hpp>
#include <XQLib/DateTime/RollConvention.hpp>

namespace XQLib
{
    class Calendar;

    Date add(Date const& date,
             Tenor2 const& tenor,
             boost::optional<int> count = 1,
             boost::optional<int> rollday = 0,
             boost::optional<RollConvention> roll_conv = Adjustment::NoAdjustment,
             boost::optional<Calendar> calendar = none);
    
    /// Computes distance between two dates in tenor units
    long sub(Date const& a,
             Date const& b,
             Tenor2 const& tenor,
             boost::optional<int> rollday = 0,
             boost::optional<RollConvention> roll_conv = Adjustment::NoAdjustment,
             boost::optional<Calendar> calendar = none);

}

#endif /* DateTenor_hpp */
