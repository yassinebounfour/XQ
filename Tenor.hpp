//
//  Tenor.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 09/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef Tenor_hpp
#define Tenor_hpp


#include <string>
#include <XQLib/DateTime/TimeUnit.hpp>

namespace XQLib
{
    
    typedef std::string Tenor; // Deserves to be replaced by a real class?
    
    /// Simple implementation
    /// TODO : take advantage of boost::duration
    Tenor add_tenor(Tenor const& tenor1, Tenor const& tenor2);
    
    
    /// \class Tenor2
    /// Simple implementation of a tenor for date arithmetics
    class Tenor2
    {
    public:
        Tenor2(TimeUnit::Enum unit, long count) : unit_(unit), count_(count) {}
        
        Tenor2(std::string const& tenor);
        
        std::string to_string() const;
        
        explicit operator std::string() const;
        
        Tenor2& operator+=(Tenor2 const& other);
        
        Tenor2& operator-=(Tenor2 const& other);

        /// accessors
        TimeUnit::Enum unit() const { return unit_; }
        long count() const { return count_; }
        
    private:
        TimeUnit::Enum      unit_;
        long                count_;
        //std::string         description_;
    };
    
    inline std::string tenor_to_string(Tenor2 const& tenor)
    {
        return tenor.to_string();
    }
    
    inline Tenor2 tenor_from_string(std::string const& tenor)
    {
        return Tenor2(tenor);
    }
}




#endif /* Tenor_hpp */
