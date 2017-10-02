//  \file RangeAccrualCouponSchedule.hpp
//
//  RangeAccrualCouponSchedule.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef RangeAccrualCouponSchedule_hpp
#define RangeAccrualCouponSchedule_hpp

#include <string>
#include <vector>
#include <XQLib/DateTime/Date.hpp>
#include <XQLib/DateTime/ShifterType.hpp>



namespace XQLib
{
    class Calendar;
    //struct ShifterType;
    
    class RangeAccrualCouponSchedule
    {
    public:
        /// constructor
        RangeAccrualCouponSchedule(){}
        /// constructor - Define all schedule dates and accrual factors explicitly
        /// mappings is an optional argument
        RangeAccrualCouponSchedule(std::vector<Date> const& observation_dates,
                                   std::vector<double> const& weights)
        : observation_dates_(observation_dates)
        , weights_(weights)
        {}
        
        /// accessors
        std::size_t                 size()              const { return observation_dates_.size(); }
        std::vector<Date> const&    observation_dates() const { return observation_dates_; }
        std::vector<double> const&  weights()           const { return weights_; }
        
    private:
        std::vector<Date> observation_dates_;
        std::vector<double> weights_;
    };
    
    RangeAccrualCouponSchedule generate_range_accrual_schedule(Date const& start_date,
                                                               Date const& end_date,
                                                               ShifterType const& shiftertype,
                                                               std::string const& shifterdelay,
                                                               Calendar const& shiftercalendar,
                                                               std::string const& rangefrequency,
                                                               std::string const& rangecutoff);
    
    
} // namespace XQLib

#endif /* RangeAccrualCouponSchedule_hpp */
