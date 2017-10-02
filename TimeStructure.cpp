//
//  TimeStructure.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 07/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "TimeStructure.hpp"
#include "YearFraction.hpp"
#include <algorithm>

namespace XQLib
{

    TimeStructure::TimeStructure(Date const& initial_date,
                                 std::string const& tenor,
                                 std::size_t nb_time_steps)
    {
        dates_.push_back(initial_date);
        Date last_date = initial_date;
        for(std::size_t i =0; i< nb_time_steps; ++i)
        {
            last_date = add_tenor(last_date,tenor);
            dates_.push_back(last_date);
        }
        calc_times();
    }
    
    
    void TimeStructure::calc_times()
    {
        std::size_t n = dates_.size();
        times_.resize(n,0.0);
        for(std::size_t i =0; i< n; ++i)
        {
            times_[i] = yf(dates_.front(),dates_[i]);
        }
    }
    
    std::size_t TimeStructure::getTimeIndex(Date const&date) const
    {
        std::vector<Date>::const_iterator mit = std::find(dates_.begin(),dates_.end(),date);
        if(mit != dates_.end())
            return mit - dates_.begin();
        else
        {
            mit = std::lower_bound(dates_.begin(),dates_.end(),date);
            if(mit == dates_.begin())
                return 0.;
            else
            {
                return mit - dates_.begin() - 1;
            }
        }
    }
    
    std::size_t TimeStructure::getTimeIndex(double time) const
    {
        std::vector<double>::const_iterator mit = std::find(times_.begin(),times_.end(),time);
        if(mit != times_.end())
            return mit - times_.begin();
        else
        {
            mit = std::lower_bound(times_.begin(),times_.end(),time);
            if(mit == times_.begin())
                return 0.;
            else
            {
                return mit - times_.begin() - 1;
            }
        }
    }
    
    double TimeStructure::getTimeStep(std::size_t i) const
    {
        return yf(dates_[i], dates_[i+1]);
    }
    
    double TimeStructure::getTime(std::size_t i) const
    {
        return times_[i];
    }
    
    Date const& TimeStructure::getDate(std::size_t i) const
    {
        return dates_[i];
    }
    
}