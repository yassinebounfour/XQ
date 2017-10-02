//
//  YearFractionCalculator.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 17/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef TimeCalculator_hpp
#define TimeCalculator_hpp

#include <XQLib/DateTime/YearFraction.hpp>

namespace XQLib
{
    /// TODO : add Calendar
    /// Essentially a binding of year fraction and business days function to some conventions
    class YearFractionCalculator
    {
    public:
        YearFractionCalculator(std::vector<Date> const& holidays = std::vector<Date>(),
                               Basis::Enum              basis = Basis::Act365,
                               double                   holiday_weight = 1.0)
        : holidays_(holidays)
        , basis_(basis)
        , holiday_weight_(holiday_weight)
        {}
        
        inline double yf(Date const& d1, Date const& d2) const
        {
            return XQLib::yf(d1,d2,basis_);
        }
        
        std::vector<Date> business_days(Date const& start, Date const& end) const
        {
            std::vector<Date> res;
            XQLIB_ASSERT(start < end,"Time calculator: start date should be before end date.");
            for(Date date = start; date <= end; date += DateDuration(1))
            {
                if((date.day_number() + 5)%7 < 5)
                    res.push_back(date);
            }
            return res;
        }
        
        
    private:
        std::vector<Date>   holidays_;
        Basis::Enum         basis_;
        double              holiday_weight_;
    };


}
#endif /* TimeCalculator_hpp */
