//*!  \file EvenSchedule.hpp
//
//  EventSchedule.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 24/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef EventSchedule_hpp
#define EventSchedule_hpp

#include <map>
#include <set>
#include <vector>
#include <XQLib/DateTime/Date.hpp>


namespace XQLib
{
    
    /// Mapping between event and cashflows
    /// NB: Several encodings are possible here
    ///     For simplicity we chose a vector of size N exercise dates
    ///     whose i-th element is the set of indices of cash-flows paid during ]T_{i},T_{i+1}]
    /// NB: Could (should?) be included in the event schedule.
    /// NB: We also use a separate set for cash-flows before the first even date
    ///     This is to keep size of containers intuitive during the first implementation but
    ///     should be reviewed.
    /// TODO : Make a real class out of this and the CallablePayoffImpl::generate_event_mapping function
    typedef std::vector<std::set<std::size_t> > event_mapping_t;
    
    template<typename K,typename V>
    std::set<K> antecedent(std::map<K,V> const& map,V const& value)
    {
        std::set<K> keys;
        for (typename std::map<K,V>::const_iterator it = map.begin(); it != map.end(); ++it)
        {
            if (it->second == value)
                keys.insert(it->first);
        }
        return keys;
    }
    
    template<typename K,typename V>
    std::vector<std::set<K> > antecedents(std::map<K,V> const& map, std::vector<V> const& values)
    {
        std::vector<std::set<K> > keys(values.size());
        for (std::size_t i = 0; i<keys.size(); ++i)
        {
            keys[i] = antecedent(map,values[i]);
        }
        return keys;
    }
    
    
    
    
    /// \class EventSchedule
    /// TODO :  add a constructor generating the schedule
    ///         This requires RefData and full support of Date/Roll conventions
    ///         which is not currently available in QValuation
    ///         For the time being an EventScheduleGenerator is implemented in
    ///         the interface layer using some Cortex code.
    /// NB :    Cortex introduces a dummy event date as the first fixing date
    ///         in the trade there is no event date before that.
    ///         We chose not to do so.
    class EventSchedule
    {
    public:
        /// simple constructor
        EventSchedule(std::vector<Date> const& event_dates);
        
        /// constructor with all the data
        EventSchedule(std::vector<Date>    const&    effective_event_dates,
                      std::vector<Date>    const&    effective_fee_dates,
                      std::vector<double>  const&    exercise_fees,
                      bool                            payCoupon = false,
                      bool                            optionOnly = false,
                      bool                            linearZero = false);
        
        
        /// constructor with all the data
        EventSchedule(std::vector<Date>    const&   event_dates,
                      std::vector<bool>    const&   event_applicable,
                      std::vector<Date>    const&   fee_dates,
                      std::vector<double>  const&   exercise_fees,
                      bool                          payCoupon       = false,
                      bool                          optionOnly      = false,
                      bool                          linearZero      = false);
        
        /// Accessors
        std::size_t nb_events() const {return event_dates_.size(); }
        std::vector<Date> const& event_dates() const {return event_dates_; }
        std::vector<Date> const& fee_dates() const {return fee_dates_; }
        std::vector<double> const& fee_values() const {return fee_values_; }
        bool payCoupon() const {return payCoupon_; }
        bool optionOnly() const {return optionOnly_; }
        bool linearZero() const {return linearZero_; }
        
    private:
        /// Event schedule
        std::vector<Date>                                       event_dates_;
        /// Payment details
        std::vector<Date>                                       fee_dates_;
        std::vector<double>                                     fee_values_;
        bool                                                    payCoupon_;
        bool                                                    optionOnly_;
        bool                                                    linearZero_;
        ///
        //event_mapping_t                                      event_mapping_;
        //std::set<Date>                                          cf_before_event_indices_;
    };
    
    
    
} // namespace XQLib



#endif /* EventSchedule_hpp */
