//
//  TimeLine.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 14/06/2017.
//  Copyright © 2017 Yassine Bounfour. All rights reserved.
//

#ifndef TimeLine_hpp
#define TimeLine_hpp

#include "Date.hpp"
#include <XQLib/Core/Exception.hpp>
#include <set>


// FIXME

namespace XQLib
{
    
    class TimeMetric
    {};
    
    class TimeLine
    {
    public:
        using iterator = std::set::iterator;
        using const_iterator = std::set::iterator;
        
        TimeLine() {}
        TimeLine(std::set<Date> const& dates) : dates_(dates) { calc_times();};
        TimeLine(Date const& initial_date,
                 std::string const& tenor,
                 std::size_t nb_time_steps);
        
        
        std::set<Date> const&       dates() const { return dates_; }
        std::size_t                 size() const { return dates_.size(); }
        Date const&                 first() const { return *(dates_.cbegin()); }
        Date const&                 last() const { return *(dates_.rbegin()); }
        const_iterator              date(std::size_t index) const;
        //double                      time(std::size_t i) const;
        const_iterator              find(Date const& d) const ;
        const_iterator                 find(double time) const;
        double                      getTimeStep(std::size_t i) const;
        
    private:
        void calc_times();
    private:
        std::map<Date,double> map_;
        std::set<Date> dates_;
        std::set<double> times_;
        TimeMetric time_metric_;
    };
    
}


#endif /* TimeLine_hpp */
