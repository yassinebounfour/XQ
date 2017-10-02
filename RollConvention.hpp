//
//  RollConvention.hpp
//  XQLib
//
//  Created by Yassine Bounfour on 14/06/2016.
//  Copyright © 2016 Yassine Bounfour. All rights reserved.
//

#ifndef RollConvention_hpp
#define RollConvention_hpp

#include <XQLib/Utils/Enum.hpp>
#include <XQLib/DateTime/Calendar.hpp>

namespace XQLib
{
    
    XQLIB_ENUM(Adjustment,
               NoAdjustment,
               Following,
               Preceding,
               ModifiedFollowing,
               ModifiedPreceding,
               Nearest,
               KoreaFollowing,
               ModifiedFollowingBiMon);
    
    typedef Adjustment::Enum RollConvention;
    
 
    
    
    class DateAdjustBase
    {
    public:
        DateAdjustBase() {}
        virtual ~DateAdjustBase() {}
        virtual Date adjust(Date const& d,Calendar const& cal) const = 0;
        virtual RollConvention get_convention() const = 0;
    };
 
    class DateAdjust : public DateAdjustBase
    {
    public:
        DateAdjust(RollConvention rc)  : roll_convention_(rc) {}
        virtual ~DateAdjust() {}
        virtual Date adjust(Date const& d,Calendar const& cal) const;
        virtual RollConvention get_convention() const { return roll_convention_; }
    private:
        RollConvention const roll_convention_;
    };
    
    void populate_korea_tags(std::vector<std::string> const& v);
    
    bool korea_tag_present(std::string const& str);
    
    Date roll_to_previous_eom(Date const& d, Calendar const& cal = Calendar(), RollConvention roll = Adjustment::NoAdjustment);

    Date roll_to_next_som(Date const& d, Calendar const& cal = Calendar(), RollConvention roll = Adjustment::NoAdjustment);

}

#endif /* RollConvention_hpp */
