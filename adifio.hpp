#ifndef ADIFIO_HPP
#define ADIFIO_HPP

//
//  adifio.hpp
//
//  Author:
//       Tania - DF9RY <Tania@DF9RY.de>
//
//  Copyright (c) 2021 DF9RY
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <map>
#include <stdexcept>
#include <chrono>
#include <memory>
#include <cassert>
#include <sstream>
#include <functional>

#ifdef HAVE_FORMAT_H
#include <format>
#else
//#include "date/date.h"
#endif
//#include "utils/string_utils.hpp"

namespace ADIFIO
{

    /// <summary>
    /// This are the type an ADIF data item can have.
    /// </summary>
    typedef enum class Type {
        EOS,				// End of stream
        EOH,				// End of header
        EOR,				// End of record
        Default,			// Not specified
        Other,				// Unidentified type code
        AwardList,			// 'A' Deprecated
        CreditList,			// 'C'
        SponsoredAwardList,	// 'P'
        Boolean,			// 'B' ('Y' / 'N')
        Number,				// 'N'
        Date,				// 'D' ('YYYYMMDD')
        Time,				// 'T' ('HHMMSS' | 'HHMM')
        String,				// 'S' (ASCII)
        IntlString,			// 'I' (UTF-8)
        MultilineString,	// 'M' (Line break: 13 10)
        IntlMultilineString,// 'G'
        Enumeration,		// 'E'
        Location			// 'L' (XDDD MM.MMM)
    } Type_t;

    typedef std::unordered_map<std::string, Type_t> TypeDictionary_t;

    class Object {
    public:
        typedef enum class ObjectType {
            NIL, BOOL, DATE, DOUBLE, INT, TIME, STRING
        } ObjectType_t;

        static std::string getObjectTypeName(ObjectType ot);

        Object(): type{ObjectType::NIL}
        {
            pointer.reset();
        }

        Object(bool v): type{ObjectType::BOOL}
        {
            pointer.reset(new bool(v));
        }

        Object(const std::chrono::sys_days &v): type{ObjectType::DATE}
        {
            pointer.reset(new std::chrono::sys_days(v));
        }

        Object(double v): type{ObjectType::DOUBLE}
        {
            pointer.reset(new double(v));
        }

        Object(int64_t v): type{ObjectType::INT}
        {
            pointer.reset(new int64_t(v));
        }

        Object(const std::chrono::seconds &v): type{ObjectType::TIME}
        {
            pointer.reset(new std::chrono::seconds(v));
        }

        Object(const std::string &v): type{ObjectType::STRING}
        {
            pointer.reset(new std::string(v));
        }

        bool isSet() const {
            return (type != ObjectType::NIL);
        }

        bool asBool() const {
            assert(type == ObjectType::BOOL);
            return *static_cast<bool*>(pointer.get());
        }

        const std::chrono::sys_days &asDate() const {
            assert(type == ObjectType::DATE);
            return *static_cast<std::chrono::sys_days*>(pointer.get());
        }

        double asDouble() const {
            assert(type == ObjectType::DOUBLE);
            return *static_cast<double*>(pointer.get());
        }

        int64_t asInt() const {
            assert(type == ObjectType::INT);
            return *static_cast<int64_t*>(pointer.get());
        }

        const std::chrono::seconds &asTime() const {
            assert(type == ObjectType::TIME);
            return *static_cast<std::chrono::seconds*>(pointer.get());
        }

        const std::string &asString() const {
            assert(type == ObjectType::STRING);
            return *static_cast<std::string*>(pointer.get());
        }

        const std::string to_string() const {
            std::ostringstream oss;
            switch (type) {
            case Object::ObjectType::NIL:
                oss << "<NIL>";
                break;
            case Object::ObjectType::BOOL:
                oss << asBool();
                break;
            case Object::ObjectType::DATE:
#ifdef HAVE_FORMAT_H
                {
                    using namespace std::chrono;
                    oss << std::format("{:%F:}", asDate());
                }
#else
                {
                    using namespace date;
                    using namespace std::chrono;
                    oss << format("%F", asDate());
                }
#endif
                break;
            case Object::ObjectType::DOUBLE:
                oss << asDouble();
                break;
            case Object::ObjectType::INT:
                oss << asInt();
                break;
            case Object::ObjectType::TIME:
                {
                    long s{asTime().count()};
                    long hours{s / (24 * 60)}; s %= (24 * 60);
                    long mins{s / 60}; s %= 60;
                    long secs{s};
                    oss << (std::dec) << (std::setw(2)) << (std::setfill('0')) << hours << ":"
                        << (std::dec) << (std::setw(2)) << (std::setfill('0')) << mins  << ":"
                        << (std::dec) << (std::setw(2)) << (std::setfill('0')) << secs;
                }
                break;
            default:
                oss << asString();
            } // end switch //
            return oss.str();
        }

        const ObjectType_t type;

    private:
        std::shared_ptr<void> pointer{nullptr};
    };

    typedef std::map<std::string, Object> ObjectMap_t;

    typedef std::function<void(const ObjectMap_t &tokens, void *userData)> RecordCallback;

    class InvalidDataException : public std::runtime_error
    {
    public:
        InvalidDataException(const std::string &what): std::runtime_error(what) {}
    };

    class Token {
    public:
        static const int MAX_DATA_LEN = 8192;
        static Type getDefaultType(const std::string &key);

        char getTypeCode() { return typeCode; }
        Type getADIF_Type() { return type; }
        const std::string &getKey() { return key; }
        const std::string &getValue() { return value; }

        Token(char _typeCode, const std::string &_key, const std::string &_value = "") :
            typeCode{_typeCode},
            type{(_typeCode == '\0') ? Token::getDefaultType(_key) : getTypeForCode(_typeCode)},
            key{_key}, value{_value}
        {
        }

        Token(Type _type, const std::string &_key, const std::string &_value = "") :
            typeCode{getCodeForType(_type)},
            type{(_type == Type::Default) ? Token::getDefaultType(_key) : _type},
            key{_key}, value{_value}
        {
        }

        Object getAsObject() {
            switch (type) {
            case Type::Boolean:
                return Object(value == "y" || value == "Y");
            case Type::Date:
                {
                    std::istringstream in(value);
                    std::chrono::sys_days days;
                    in >> date::parse("%4Y%2m%2d", days); // "yyyyMMdd"
                    return Object(days);
                }
            case Type::Number:
                if (value.find('.') == std::string::npos) {
                    int64_t v{std::stoll(value)};
                    return Object(v);
                } else {
                    double v{std::stod(value)};
                    return Object(v);
                }
            case Type::Time:
                 {
                    int hour, min, sec;
                    long x{std::stol(value)};
                    if (value.length() ==  4) {
                        hour  = x / 1000; x %= 1000; hour *= 10;
                        hour += x /  100; x %=  100;
                        min   = x /   10; x %=   10; min  *= 10;
                        min  += x;
                        sec   = 0;
                    } else if (value.length() == 6) {
                        hour  = x / 100000; x %= 100000; hour *= 10;
                        hour += x /  10000; x %=  10000;
                        min   = x /   1000; x %=   1000; min  *= 10;
                        min  += x /    100; x %=    100;
                        sec   = x /     10; x %=     10; sec  *= 10;
                        sec  += x;
                    } else {
                        throw ::std::runtime_error("Invalid time value length: \"" + value + "\"");
                    }
                    if ((hour >= 24) || (min >= 60) || (sec >= 60)) {
                        throw ::std::runtime_error("Invalid time value: \"" + value + "\"");
                    }
                    long count{hour * 24 * 60 + min * 60 + sec};
                    std::chrono::seconds s(count);
                    return Object(s);
                }
            default:
                return Object(value);
            } // end switch //
        }

        static Type getTypeForCode(char code) {
            switch (code) {
            case 'a':
            case 'A':
                return Type::AwardList;
            case 'b':
            case 'B':
                return Type::Boolean;
            case 'c':
            case 'C':
                return Type::CreditList;
            case 'd':
            case 'D':
                return Type::Date;
            case 'e':
            case 'E':
                return Type::Enumeration;
            case 'i':
            case 'I':
                return Type::IntlString;
            case 'g':
            case 'G':
                return Type::IntlMultilineString;
            case 'l':
            case 'L':
                return Type::Location;
            case 'm':
            case 'M':
                return Type::MultilineString;
            case 'n':
            case 'N':
                return Type::Number;
            case 'p':
            case 'P':
                return Type::SponsoredAwardList;
            case 't':
            case 'T':
                return Type::Time;
            case '\0':
                return Type::Default;
            case '-':
                return Type::EOH;
            case '=':
                return Type::EOR;
            case '.':
                return Type::EOS;
            default:
                return Type::Other;
            }
        }

        static char getCodeForType(Type type) {
            switch (type) {
            case Type::AwardList:
                return 'A';
            case Type::Boolean:
                return 'B';
            case Type::CreditList:
                return 'C';
            case Type::Date:
                return 'D';
            case Type::Enumeration:
                return 'E';
            case Type::IntlString:
                return 'I';
            case Type::IntlMultilineString:
                return 'G';
            case Type::Location:
                return 'L';
            case Type::MultilineString:
                return 'M';
            case Type::Number:
                return 'N';
            case Type::SponsoredAwardList:
                return 'P';
            case Type::String:
                return 'S';
            case Type::Time:
                return 'T';
            case Type::Other:
                return '?';
            case Type::Default:
                return '\0';
            case Type::EOH:
                return '-';
            case Type::EOR:
                return '=';
            case Type::EOS:
                return '.';
            default:
                throw new InvalidDataException("ADIF Type " + std::to_string((int)type));
            } // end switch //
        } // end method //

        std::string to_string() const {
            return key + "=" + value;
        }

    private:
        char        typeCode;
        Type        type;
        std::string key;
        std::string value;
    }; // end class Token //

    class Reader
    {
    public:
        Reader();

        void readADI(std::istream &stream,
                     RecordCallback recordCB, void *recordUserData,
                     RecordCallback headerCB, void *headerUserData);

    private:
        ObjectMap_t readHeader(std::istream &stream, Token *first = nullptr);
        Token getNextToken(std::istream &stream);
        void skipToNextToken(std::istream &stream);
        int readByte(std::istream &stream);
        int peekByte(std::istream &stream);
    };

} // end namespace //


#endif // ADIFIO_HPP
