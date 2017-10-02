//
//  EventSchedule.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 24/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "EventSchedule.hpp"

#include <vector>


namespace XQLib
{
    
    /// simple constructor
    EventSchedule::EventSchedule(std::vector<Date>    const&    event_dates)
    : event_dates_(event_dates)
    , fee_dates_()
    , fee_values_()
    , payCoupon_()
    , optionOnly_()
    , linearZero_()
    {}
    
    /// constructor with all the data
    EventSchedule::EventSchedule(std::vector<Date>    const&    effective_event_dates,
                                 std::vector<Date>    const&    effective_fee_dates,
                                 std::vector<double>  const&    exercise_fees,
                                 bool                            payCoupon,
                                 bool                            optionOnly,
                                 bool                            linearZero)
    : event_dates_(effective_event_dates)
    , fee_dates_(effective_fee_dates)
    , fee_values_(exercise_fees)
    , payCoupon_(payCoupon)
    , optionOnly_(optionOnly)
    , linearZero_(linearZero)
    {}
    
    
    
    /// constructor with all the data
    EventSchedule::EventSchedule(std::vector<Date>    const&    event_dates,
                                 std::vector<bool>    const&    event_applicable,
                                 std::vector<Date>    const&    fee_dates,
                                 std::vector<double>  const&    exercise_fees,
                                 bool                            payCoupon,
                                 bool                            optionOnly,
                                 bool                            linearZero)
    : event_dates_(event_dates)
    , fee_dates_(fee_dates)
    , fee_values_(exercise_fees)
    //, payCoupon_(payCoupon)
    //, optionOnly_(optionOnly)
    //, linearZero_(linearZero)
    {
        std::vector<Date> effective_event_dates;
        std::vector<Date> effective_fee_dates;
        std::vector<double> effective_fee_values;
        for(size_t i=0; i<event_dates_.size(); ++i)
        {
            if(event_applicable[i])
            {
                effective_event_dates.push_back(event_dates_[i]);
                effective_fee_dates.push_back(fee_dates_[i]);
                effective_fee_values.push_back(fee_values_[i]);
            }
        }
        event_dates_ = effective_event_dates;
        fee_dates_ =  effective_fee_dates;
        fee_values_ = effective_fee_values;
    }
    
    
    
} // namespace XQLib

