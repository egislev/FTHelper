#include "adifio.hpp"

namespace ADIFIO
{

static const std::map<Object::ObjectType, std::string> object_type_map {
    { Object::ObjectType::NIL,    "nil"    },
    { Object::ObjectType::BOOL,   "bool"   },
    { Object::ObjectType::DATE,   "date"   },
    { Object::ObjectType::DOUBLE, "double" },
    { Object::ObjectType::INT,    "int"    },
    { Object::ObjectType::TIME,   "time"   },
    { Object::ObjectType::STRING, "string" }
};

std::string Object::getObjectTypeName(ObjectType ot) {
    auto iter{object_type_map.find(ot)};
    return (iter != object_type_map.end()) ? iter->second : "undefined";
}

TypeDictionary_t defaultTypes {
    { "ADIF_VER",                    Type::String },
    { "CREATED_TIMESTAMP",           Type::String },
    { "PROGRAMID",         	         Type::String },
    { "PROGRAMVERSION",    	         Type::String },
    { "ADDRESS",           	         Type::MultilineString },
    { "ADDRESS_INTL",      	         Type::IntlMultilineString },
    { "AGE",               	         Type::Number },
    { "A_INDEX",           	         Type::Number },
    { "ANT_AZ",            	         Type::Number },
    { "ANT_EL",            	         Type::Number },
    { "ANT_PATH",                    Type::Enumeration },
    { "ARRL_SECT",                   Type::Enumeration },
    { "AWARD_SUBMITTED",             Type::SponsoredAwardList },
    { "BAND",                        Type::Enumeration },
    { "BAND_RX",                     Type::Enumeration },
    { "CALL",                        Type::String },
    { "CHECK",                       Type::String },
    { "CLASS",                       Type::String },
    { "CLUBLOG_QSO_UPLOAD_DATE",     Type::Enumeration },
    { "CLUBLOG_QSO_UPLOAD_STATUS",   Type::Enumeration },
    { "CNTY",              	         Type::Enumeration },
    { "COMMENT",           	         Type::String },
    { "COMMENT_INTL",      	         Type::IntlString },
    { "CONT",              	         Type::Enumeration },
    { "CONTACTED_OP",      	         Type::String },
    { "CONTEST_ID",        	         Type::String },
    { "COUNTRY",           	         Type::String },
    { "COUNTRY_INTL",      	         Type::IntlString },
    { "CQZ",               	         Type::Number },
    { "CREDIT_SUBMITTED",  	         Type::CreditList },
    { "CREDIT_GRANTED",    	         Type::CreditList },
    { "DISTANCE",          	         Type::Number },
    { "DXCC",              	         Type::Enumeration },
    { "EMAIL",             	         Type::String },
    { "EQ_CALL",           	         Type::String },
    { "EQSL_QSLRDATE",     	         Type::Date },
    { "EQSL_QSLSDATE",     	         Type::Date },
    { "EQSL_QSL_RCVD",     	         Type::Enumeration },
    { "EQSL_QSL_SENT",     	         Type::Enumeration },
    { "FISTS",             	         Type::String },
    { "FISTS_CC",          	         Type::String },
    { "FORCE_INIT",        	         Type::Boolean },
    { "FREQ",              	         Type::Number },
    { "FREQ_RX",           	         Type::Number },
    { "GRIDSQUARE",        	         Type::Other },
    { "GUEST_OP",          	         Type::String },
    { "HRDLOG_QSO_UPLOAD_DATE",      Type::Date },
    { "HRDLOG_QSO_UPLOAD_STATUS",    Type::Enumeration },
    { "IOTA",              	         Type::String },
    { "IOTA_ISLAND_ID",    	         Type::String },
    { "ITUZ",              	         Type::Number },
    { "K_INDEX",           	         Type::Number },
    { "LAT",               	         Type::Location },
    { "LON",               	         Type::Location },
    { "LOTW_QSLRDATE",     	         Type::Date },
    { "LOTW_QSLSDATE",     	         Type::Date },
    { "LOTW_QSL_RCVD",     	         Type::Enumeration },
    { "LOTW_QSL_SENT",     	         Type::Enumeration },
    { "MAX_BURSTS",        	         Type::Number },
    { "MODE",              	         Type::Enumeration },
    { "MS_SHOWER",         	         Type::String },
    { "MY_CITY",           	         Type::String },
    { "MY_CITY_INTL",      	         Type::IntlString },
    { "MY_CNTY",           	         Type::Enumeration },
    { "MY_COUNTRY",        	         Type::String },
    { "MY_COUNTRY_INTL",   	         Type::IntlString },
    { "MY_CQ_ZONE",        	         Type::Number },
    { "MY_DXCC",           	         Type::Enumeration },
    { "MY_FISTS",          	         Type::String },
    { "MY_GRIDSQUARE",     	         Type::Other },
    { "MY_IOTA",           	         Type::String },
    { "MY_IOTA_ISLAND_ID", 	         Type::String },
    { "MY_LAT",            	         Type::Location },
    { "MY_LON",            	         Type::Location },
    { "MY_NAME",           	         Type::String },
    { "MY_NAME_INTL",      	         Type::IntlString },
    { "MY_POSTAL_CODE",    	         Type::String },
    { "MY_POSTAL_CODE_INTL",         Type::IntlString },
    { "MY_RIG",            	         Type::String },
    { "MY_RIG_INTL",       	         Type::IntlString },
    { "MY_SIG",            	         Type::String },
    { "MY_SIG_INTL",       	         Type::IntlString },
    { "MY_SIG_INFO",       	         Type::String },
    { "MY_SIG_INFO_INTL",  	         Type::IntlString },
    { "MY_SOTA_REF",       	         Type::Other },
    { "MY_STATE",          	         Type::Enumeration },
    { "MY_STREET",         	         Type::String },
    { "MY_STREET_INTL",    	         Type::IntlString },
    { "MY_USACA_COUNTIES", 	         Type::Other },
    { "MY_VUCC_GRIDS",     	         Type::Other },
    { "NAME",              	         Type::String },
    { "NAME_INTL",         	         Type::IntlString },
    { "NOTES",             	         Type::MultilineString },
    { "NOTES_INTL",        	         Type::IntlMultilineString },
    { "NR_BURSTS",         	         Type::Number },
    { "NR_PINGS",          	         Type::Number },
    { "OPERATOR",          	         Type::String },
    { "OWNER_CALLSIGN",    	         Type::String },
    { "PFX",               	         Type::String },
    { "PRECEDENCE",        	         Type::String },
    { "PROP_MODE",         	         Type::Enumeration },
    { "PUBLIC_KEY",        	         Type::String },
    { "QRZCOM_QSO_UPLOAD_DATE", 	 Type::Date },
    { "QRZCOM_QSO_UPLOAD_STATUS",    Type::Enumeration },
    { "QSLMSG",            	         Type::MultilineString },
    { "QSLMSG_INTL",       	         Type::IntlMultilineString },
    { "QSLRDATE",          	         Type::Date },
    { "QSLSDATE",          	         Type::Date },
    { "QSL_RCVD",          	         Type::Enumeration },
    { "QSL_RCVD_VIA",      	         Type::Enumeration },
    { "QSL_SENT",          	         Type::Enumeration },
    { "QSL_SENT_VIA",      	         Type::Enumeration },
    { "QSL_VIA",           	         Type::String },
    { "QSO_COMPLETE",      	         Type::Enumeration },
    { "QSO_DATE",          	         Type::Date },
    { "QSO_DATE_OFF",      	         Type::Date },
    { "QSO_RANDOM",        	         Type::Boolean },
    { "QTH",               	         Type::String },
    { "QTH_INTL",          	         Type::IntlString },
    { "RIG",               	         Type::MultilineString },
    { "RIG_INTL",          	         Type::IntlMultilineString },
    { "RST_RCVD",          	         Type::String },
    { "RST_SENT",          	         Type::String },
    { "RX_PWR",            	         Type::Number },
    { "SAT_MODE",          	         Type::String },
    { "SAT_NAME",          	         Type::String },
    { "SFI",               	         Type::Number },
    { "SIG",               	         Type::String },
    { "SIG_INTL",          	         Type::IntlString },
    { "SIG_INFO",          	         Type::String },
    { "SIG_INFO_INTL",     	         Type::IntlString },
    { "SKCC",              	         Type::String },
    { "SOTA_REF",          	         Type::Other },
    { "SRX",               	         Type::Number },
    { "SRX_STRING",        	         Type::String },
    { "STATE",             	         Type::Enumeration },
    { "STATION_CALLSIGN",  	         Type::String },
    { "STX",               	         Type::Number },
    { "STX_STRING",        	         Type::String },
    { "SUBMODE",           	         Type::String },
    { "SWL",               	         Type::Boolean },
    { "TEN_TEN",           	         Type::Number },
    { "TIME_OFF",          	         Type::Time },
    { "TIME_ON",           	         Type::Time },
    { "TX_PWR",            	         Type::Number },
    { "VE_PROV",           	         Type::String },
    { "VUCC_GRIDS",        	         Type::Other },
    { "WEB",               	         Type::String }
};

Type Token::getDefaultType(const std::string &key) {
      auto iter{defaultTypes.find(key)};
      return (iter != defaultTypes.end()) ? iter->second : Type::Other;
}

Reader::Reader() {}

void Reader::readADI(std::istream &stream,
             RecordCallback recordCB, void *recordUserData,
             RecordCallback headerCB, void *headerUserData)
{
    int octet{stream.peek()};
    if (octet != '<') {
         ObjectMap_t tokens{readHeader(stream)};
         if (headerCB)
            headerCB(tokens, headerUserData);
    }

    // Start with records:
    Token t{getNextToken(stream)};
    if (t.getKey() == "ADIF_VER") { // Bug in some ADIF files!
         ObjectMap_t tokens{readHeader(stream)};
         if (headerCB)
            headerCB(tokens, headerUserData);
         t = getNextToken(stream);
    }

    ObjectMap_t r;
    while(true) {
        Type type = t.getADIF_Type();
        if (r.size() == 0) {
            if (type == Type::EOS)
                break;
            if (type == Type::EOR)
                throw std::runtime_error("Syntax error: Empty record");
        }
        if (type == Type::EOR) {
            if (recordCB)
                recordCB(r, recordUserData);
            r.clear();
            t = getNextToken(stream);
            continue;
        }
        if (type == Type::EOS) {
            throw std::runtime_error("Syntax error: End of stream in record");
        }
        r.emplace(t.getKey(), t.getAsObject());
        t = getNextToken(stream);
    } // end while //
}

ObjectMap_t Reader::readHeader(std::istream &stream, Token *first)
{
    ObjectMap_t record;
    if (first)
        record.emplace(first->getKey(), first->getAsObject());
    while (true) {
        Token ht = getNextToken(stream);
        if (ht.getADIF_Type() == Type::EOS)
            throw std::runtime_error("Syntax error: End of stream in header");
        if (ht.getADIF_Type() == Type::EOH)
            break;
        record.emplace(ht.getKey(), ht.getAsObject());
    } // end while //
    return record;
}

Token Reader::getNextToken(std::istream &stream) {
    skipToNextToken(stream);
    int octet{readByte(stream)};
    if (octet < 0)
         return Token(Type::EOS, "EOS");
    if (octet != '<')
         throw std::runtime_error("Syntax error: '<' expected");
    std::ostringstream sb;
    int _ch{readByte(stream)};
    while ((_ch >= 0) && (_ch != ':') && (_ch != '>')) {
         sb.put(static_cast<char>(_ch));
         _ch = readByte(stream);
    } // end while //
    std::string key{Utils::trim(Utils::toUpper(sb.str()))};
    char ch{static_cast<char>(_ch)};
    if (ch == '>') {
        readByte(stream);
        if (key == "EOH")
            return Token(Type::EOH, "EOH");
        if (key == "EOR")
            return Token(Type::EOR, "EOR");
        throw std::runtime_error("Syntax error: Unknown Token: " + key);
    }
    if (ch != ':')
        throw std::runtime_error("Syntax error: ':' expected");
    int len{0};
    ch = static_cast<int>(readByte(stream));
    while (std::isdigit(ch)) {
        len = 10 * len + ch - '0';
        ch = static_cast<int>(readByte(stream));
    } // end while //
    if ((len < 0) || (len > Token::MAX_DATA_LEN))
        throw std::runtime_error("Data field too long: " + std::to_string(len));
    char typeCode{'\0'};
    if (ch == ':')
        typeCode = static_cast<char>(readByte(stream));
    if (ch != '>')
        throw std::runtime_error("Syntax error: '>' expected");
    std::ostringstream oss;
    while (len--)
        oss.put(static_cast<char>(readByte(stream)));
    return Token(typeCode, key, oss.str());
}

void Reader::skipToNextToken(std::istream &stream) {
    while (true) {
        int octet = stream.peek();
        if ((octet < 0) || (octet == '<'))
            break;
        readByte(stream);
    } // end while //
}

int Reader::readByte(std::istream &stream) {
    try {
        return stream.get();
    }
    catch (...) {
        return -1;
    }
}

int Reader::peekByte(std::istream &stream) {
    try {
        return stream.peek();
    }
    catch (...) {
        return -1;
    }
}

} // end namespace ADIFIO //
