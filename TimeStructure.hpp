//
//  TimeStructure.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 07/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef TimeStructure_hpp
#define TimeStructure_hpp

#include "Date.hpp"
#include <XQLib/Core/Exception.hpp>

// FIXME

namespace XQLib
{

    class TimeStructure
    {
    public:
        TimeStructure() {}
        TimeStructure(std::vector<Date> const& dates) : dates_(dates) { calc_times();};
        TimeStructure(Date const& initial_date,
                      std::string const& tenor,
                      std::size_t nb_time_steps);
        
        
        std::vector<Date> const&    dates() const { return dates_; }
        std::size_t                 getSize() const { return dates_.size() - 1; }
        Date const&                 getFirstDate() const { return dates_.front(); }
        Date const&                 getLastDate() const { return dates_.back(); }
        Date const&                 getDate(std::size_t index) const;
        double                      getTime(std::size_t i) const;
        std::size_t                 getTimeIndex(Date const& d) const;
        std::size_t                 getTimeIndex(double time) const;
        double                      getTimeStep(std::size_t i) const;
        
    private:
        void calc_times();
    private:
        std::vector<Date> dates_;
        std::vector<double> times_;
    };

}

#endif /* TimeStructure_hpp */
