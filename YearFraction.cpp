//
//  YearFraction.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 08/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "YearFraction.hpp"
#include <XQLib/Core/Exception.hpp>

namespace XQLib
{

    double yf(Date const& d1, Date const& d2, DayCount const& basis)
    {
        long ndays = (d2 - d1).days();
        switch (basis)
        {
            case Basis::Act360:
                return static_cast<double>(ndays)/360.0;
                break;
            case Basis::Act365:
                return static_cast<double>(ndays)/365.0;
                break;
            default:
                MY_THROW("Daycount convention, not implemented yet!");
                break;
        }
    }
    
    double yf(DateTime const& d1, DateTime const& d2, DayCount const& basis)
    {
        return yf(datetime_to_date(d1),datetime_to_date(d2),basis); // FIXME add time duration?
    }
    
    double yf(Date const& d1,
              Date const& d2,
              DayCount const& basis,
              RollConvention roll_conv,
              Calendar const& calendar) 
    {
        XQLIB_ASSERT(roll_conv == RollConvention::NoAdjustment, "roll conventions not implemented yet");
        XQLIB_ASSERT(calendar.get_holidays().empty(), "Calendar adjustment not implemented yet");
        return yf(d1,d2,basis);
    }

    
    std::vector<double> yf(std::vector<Date> const& dates, DayCount const& basis)
    {
        XQLIB_ASSERT(dates.size() > 1, "Computing year fractions requires at least 2 dates");
        std::vector<double> yfs(dates.size()-1);
        for(size_t i =1; i< dates.size(); ++i)
            yfs[i-1] = yf(dates[0], dates[i],basis);
        return yfs;
    }

    
} // namespace XQLib
