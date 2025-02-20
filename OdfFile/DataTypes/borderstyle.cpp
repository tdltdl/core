﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include <boost/algorithm/string.hpp>

#include "borderstyle.h"

namespace cpdoccore { namespace odf_types { 

std::wostream & operator << (std::wostream & _Wostream, const border_style & borderStyle)
{
    if (borderStyle.is_none())
    {
        _Wostream << std::wstring(L"none");
        return _Wostream;
    }

    _Wostream << borderStyle.get_length();

    switch (borderStyle.get_style())
    {
        case border_style::none:            _Wostream << L" none "; break;
        case border_style::double_:         _Wostream << L" double "; break;
        case border_style::dotted:          _Wostream << L" dotted "; break;
        case border_style::dash:            _Wostream << L" dashed "; break;
        case border_style::dot_dash:        _Wostream << L" dash-dot "; break;
        case border_style::dot_dot_dash:    _Wostream << L" dash-dot-dot "; break;
        case border_style::fine_dashed:     _Wostream << L" fine-dashed "; break;
        case border_style::double_thin:     _Wostream << L" double-thin "; break;
        case border_style::solid:
        default:
            _Wostream <<  L" solid "; break;
    }
    _Wostream << borderStyle.get_color();
    return _Wostream;
}

bool operator == (border_style & _b1, border_style & _b2)
{
    if (_b1.is_none() && _b2.is_none()) return true;

    if (_b1.is_none() || _b2.is_none()) return false;

    if (    _b1.get_color()     == _b2.get_color()  &&
            _b1.get_length()    == _b2.get_length() &&
            _b1.get_style()     == _b2.get_style())     return true;

    return false;
}

border_style::border_style(const border_style & Value)
{
    color_ = Value.get_color();
    length_ = Value.get_length();
    style_ = Value.get_style();

	none_ = Value.is_none();

    initialized_ = true;
}
border_style& border_style::operator =(const border_style& Value)
{
    color_  = Value.get_color();
    length_ = Value.get_length();
    style_  = Value.get_style();

    none_   = Value.is_none();

    initialized_ = true;

    return *this;
}
border_style::border_style() : initialized_(false), none_(true), style_(none)
{
}

border_style::border_style(const std::wstring & Value) : initialized_(false), none_(false), style_(none)
{
    if (Value.empty()) return;

	std::wstring tmp =  boost::algorithm::trim_copy(Value);
    boost::algorithm::to_lower(tmp);
    
    if (L"none" == tmp/* || tmp.length() < 1*/)
    {
        none_ = true;
        style_ = none;
    }
    else
    {
		none_ = false;

        std::vector< std::wstring > splitted;
        boost::algorithm::split(splitted, Value, boost::algorithm::is_any_of(L" \t"), boost::algorithm::token_compress_on);

        try 
        {
            if (splitted.size() > 0)
                length_ = length::parse(splitted[0]);

            if (splitted.size() > 1)
            {
                if (splitted[1] == L"solid")        style_ = solid;
                if (splitted[1] == L"double")       style_ = double_;
                if (splitted[1] == L"dotted")       style_ = dotted;
                if (splitted[1] == L"dashed")       style_ = dash;
                if (splitted[1] == L"dot-dashed")   style_ = dot_dash;
                if (splitted[1] == L"dash-dot")     style_ = dot_dash;
                if (splitted[1] == L"dash-dot-dot") style_ = dot_dot_dash;
                if (splitted[1] == L"fine-dashed")  style_ = fine_dashed;
                if (splitted[1] == L"double-thin")  style_ = double_thin;                
            }

            if (splitted.size() > 2)
                color_ = color::parse(splitted[2]);
        }
        catch(...)
        {
        }
    }

    initialized_ = true;
}

border_style border_style::parse( const std::wstring & Value)
{
    return border_style(Value);
}

border_style::border_style(const color & color_,  const type & style_, const length & length_)
{
	this->color_	= color_;
	this->style_	= style_;
	this->length_	= length_;

    if (this->style_ == none)
        none_ = true;
	else 
		none_ = false;

    initialized_ = true;
}
bool border_style::initialized() const    { return initialized_; }
bool border_style::is_none()     const    { return none_; }

const length& border_style::get_length()const
{
    return length_;
}
const border_style::type& border_style::get_style() const
{
    return style_;
}
const color& border_style::get_color() const
{
    return color_;
}


}
}
