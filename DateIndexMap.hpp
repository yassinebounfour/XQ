//
//  DateIndexMap.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 16/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef DateIndexMap_hpp
#define DateIndexMap_hpp

#include <map>
#include <XQLib/DateTime/Date.hpp>


/// Utils to avoid duplications
/// NB: these functions might be used only because of poor choices of containers.

namespace XQLib
{
    
    typedef std::map<Date,std::size_t> DateIndexMap; // FIXME
    
    std::size_t index(Date const& date, DateIndexMap const& date_index_map, std::vector<Date> const& dates);
    
    void merge_dates(std::vector<Date>& dates, std::vector<Date> const& others);
    
    void merge_dates(Date const& initial_date, std::vector<Date>& dates, std::vector<Date> const& others);
    
    /// helper function to locate the parameter value on term structure
    double term_structure_value(Date const& date,
                                std::vector<Date> const& dates,
                                std::vector<double> const& values);
    /*
    class DateIndexMap
    {
    public:
        typedef std::map<Date,std::size_t>          map_t;
        typedef typename map_t::const_iterator   const_iterator;
        
        std::size_t index(Date const& date) const;
        std::size_t operator[](Date const& date) const { return map_.at(date); }
        
        const_iterator begin()  const { return map_.begin(); }
        const_iterator end()    const { return map_.end(); }
    private:
        std::map<Date,std::size_t>  map_;
        std::vector<Date>           dates_;
    };
    */
    
}

#endif /* DateIndexMap_hpp */
