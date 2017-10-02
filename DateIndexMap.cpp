//
//  DateIndexMap.cpp
//  XQLib
//
//  Created by Yassine Bounfour on 16/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#include "DateIndexMap.hpp"
#include <XQLib/Core/Exception.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>

namespace XQLib
{
    
    std::size_t index(Date const& date, DateIndexMap const& date_index_map, std::vector<Date> const& dates)
    {
        //MY_DEBUG_ASSERT(date >= pricing_date_, "Index requested for date before pricing date!");
        DateIndexMap::const_iterator mit = date_index_map.find(date);
        if (mit != date_index_map.end())
            return mit->second;
        else
        {
            std::vector<Date>::const_iterator it = std::lower_bound(dates.begin(),dates.end(),date);
            if(it == dates.begin())
                return 0;
            else if(it == dates.end())
                return dates.size() - 1;
            else
                return it - dates.begin() - 1;
            /*
            if(it == dates.end())
                return dates.size()-1;
            else
            {
                if(it == dates.begin())
                    return 0;
                else
                    return it - dates.begin() - 1;
            }
             */
        }
    }
    
    /// helper function to locate the parameter value on term structure
    double term_structure_value(Date const& date,
                                std::vector<Date> const& dates,
                                std::vector<double> const& values)
    {
        MY_ASSERT(!(dates.empty()) && !(values.empty()) && (dates.size() == values.size()), "Empty vector or size mismatch.");
        MY_ASSERT(boost::is_sorted(dates), "Date vector for parameter's term structure has to be sorted.");
        if((date < dates[0]) || (dates.size() == 1)) return values[0];
        std::vector<Date>::const_iterator dIt = dates.begin(), nIt = dIt + 1;
        std::size_t idx = 0;
        while((nIt != dates.end()) && !(*nIt > date))
        {
            ++dIt; ++nIt; ++idx;
        }
        return values[idx];
    }
    
    void merge_dates(std::vector<Date>& abscissa_, std::vector<Date> const& dates) /// Bad! Use std::set!
    {
        abscissa_.insert(abscissa_.end(), dates.begin(), dates.end());
        std::sort(abscissa_.begin(), abscissa_.end());
        /// remove duplicate items
        abscissa_.erase(std::unique(abscissa_.begin(), abscissa_.end()), abscissa_.end());
    }
    
    void erase_previous_dates(Date const& pricing_date_, std::vector<Date>& abscissa_) /// Bad! Use std::set!
    {
        std::vector<Date>::iterator vIt = abscissa_.begin();
        while(vIt != abscissa_.end())
        {
            if(*vIt < pricing_date_)
                vIt = abscissa_.erase( vIt );
            else
                ++vIt;
        }
    }


    void merge_dates(Date const& pricing_date_, std::vector<Date>& abscissa_, std::vector<Date> const& dates) /// Bad! Use std::set!
    {
        merge_dates(abscissa_, dates);
        erase_previous_dates(pricing_date_,abscissa_);
    }


    /*
    std::size_t DateIndexMap::index(Date const& date) const
    {
        MY_DEBUG_ASSERT(date >= pricing_date_, "Index requested for date before pricing date!");
        DateIndexMap::const_iterator mit = map_.find(date);
        if (mit != date_index_map_.end())
            return mit->second;
        else
        {
            std::vector<Date>::const_iterator it = std::lower_bound(dates_.begin(),dates_.end(),date);
            if(it == dates_.end())
                return dates_.size()-1;
            else
            {
                if(it == dates_.begin())
                    return 0;
                else
                    return it - dates_.begin() - 1;
            }
        }
    }
     */


}
