//
//  RangeAccrualCouponSchedule.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "RangeAccrualCouponSchedule.hpp"

#include <XQLib/Core/Exception.hpp>
#include <XQLib/DateTime/Tenor.hpp>
#include <XQLib/DateTime/DateTenor.hpp>
#include <XQLib/DateTime/Calendar.hpp>
#include <XQLib/DateTime/ShifterType.hpp>

#include <boost/lexical_cast.hpp>
#include <XQLib/Core/Memory.hpp>
#include <boost/make_shared.hpp>



namespace XQLib
{
    
    RangeAccrualCouponSchedule generate_range_accrual_schedule(Date const& startdate,
                                                               Date const& enddate,
                                                               ShifterType const& shiftertype,
                                                               Tenor2 const& shifter_delay,
                                                               Calendar const& shiftercalendar,
                                                               Tenor2 const& range_frequency,
                                                               Tenor2 const& range_cutoff)
    {
        XQLIB_ASSERT(startdate < enddate, "StartDate cannot be after end_date");
        XQLIB_ASSERT(shifter_delay.unit() != TimeUnit::Month &&
                     range_frequency.unit() != TimeUnit::Month &&
                     range_cutoff.unit() != TimeUnit::Month &&
                     shifter_delay.unit() != TimeUnit::Year &&
                     range_frequency.unit() != TimeUnit::Year &&
                     range_cutoff.unit() != TimeUnit::Year ,
                     "Product does not support monthly and yearly delays");
        
        //First set the denominator for the weights calculations:
        double denom = static_cast<double>((enddate-startdate).days());
        //Set the end_date according to the shifter:
        Date adj_end_date = add(enddate, range_cutoff, -1);
        
        if(!(shiftertype == ShifterTypes::NoShifter))
            adj_end_date = add(adj_end_date, shifter_delay, -1);
        
        //Observation dates results vector.
        std::vector<Date> observation_dates;
        
        //Now recursively populate observation_dates:
        Date result, prev;
        result = adj_end_date;
        prev = adj_end_date;
        
        while( prev >= startdate)
        {
            observation_dates.insert(observation_dates.begin(), result);
            result = add(*observation_dates.begin(), range_frequency, -1);
            prev = result;
        }
        
        //Tidy up the start date according to the shiftertype:
        
        Date adj_start_date = startdate;
        
        //If not noShifter, fill in all the days inbetween adjusted start and observation date
        if(!(shiftertype == ShifterTypes::NoShifter))
        {
            adj_start_date = add(adj_start_date, shifter_delay, -1);
            
            result = add(observation_dates.front(), range_frequency, -1);
            prev = result;
            
            while(prev > adj_start_date)
            {
                observation_dates.insert(observation_dates.begin(), result);
                result = add(*observation_dates.begin(), range_frequency, -1);
                prev = result;
            }
        }
        
        //Make sure duplications aren't inserted at the start:
        if( observation_dates.front() != adj_start_date )
            observation_dates.insert(observation_dates.begin(), adj_start_date);
        
        //Now calculate the weights:
        
        std::vector<double> weights;
        for(std::size_t i = 0; i < observation_dates.size() - 1; ++i)
            weights.push_back(static_cast<double>((observation_dates[i+1] - observation_dates[i]).days())/denom);
        
        //Now add the last one: if no shifter: end_date - observation_dates.back())/denom:
        if(shiftertype == ShifterTypes::NoShifter)
        {
            weights.push_back(static_cast<double>((adj_end_date - observation_dates.back()).days())/denom);
        }
        else //Need to correct the observation_dates.back() solely for weights purposes: (Doesn't need to update observation_dates)
        {
            Date corrected_last_end_date = add(adj_end_date, range_cutoff, -1);
            weights.push_back(static_cast<double>((adj_end_date - corrected_last_end_date).days())/denom);
        }
        
        return RangeAccrualCouponSchedule(observation_dates, weights);
        
    }
    
    
} //namespace XQLib

