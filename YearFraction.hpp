//
//  YearFraction.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 08/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef YearFraction_hpp
#define YearFraction_hpp

#include <XQLib/DateTime/Calendar.hpp>
#include <XQLib/DateTime/Date.hpp>
#include <XQLib/DateTime/DayCount.hpp>
#include <XQLib/DateTime/RollConvention.hpp>

namespace XQLib
{

    double yf(Date const& d1, Date const& d2, DayCount const& basis = Basis::Act365);
    
    double yf(Date const& d1,
              Date const& d2,
              DayCount const& basis,
              RollConvention roll_conv,
              Calendar const& calendar = Calendar());
    
    double yf(DateTime const& d1, DateTime const& d2, DayCount const& basis = Basis::Act365);
    
    
    std::vector<double> yf(std::vector<Date> const& dates, DayCount const& basis = Basis::Act365);

}

#endif /* YearFraction_hpp */
