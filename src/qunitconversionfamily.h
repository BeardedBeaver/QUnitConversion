#ifndef QUNITCONVERSIONFAMILY_H
#define QUNITCONVERSIONFAMILY_H

#include <cmath>
#include <map>
#include <stdexcept>

#include "qunitconversionrule.h"

/**
 * @brief The QUnitConversionFamily class is an internal class that provides
 * a conversion by holding all of the conversion rules for a single family
 */
template<class String>
class QUnitConversionFamily
{
public:
    QUnitConversionFamily() = default;

    QUnitConversionFamily(const String & familyName, const String & baseUnit)
    {
        m_family = familyName;
        m_baseUnit = baseUnit;
    }

    /**
     * @brief Adds a conversion rule to convertor
     * @param rule rule to add
     */
    void addConversionRule(const QUnitConversionRule<String> & rule)
    {
        if (m_rules.empty())
        {
            m_family = rule.family();
            m_baseUnit = rule.baseUnit();
        }
        else
        {
            if (m_family != rule.family() || m_baseUnit != rule.baseUnit())
                throw std::invalid_argument("Incorrect rule added to family");      // Don Corleone will be unhappy
        }
        m_rules.insert_or_assign(rule.unit(), rule);
    }

    /**
     * @brief Converts from in unit to out unit
     * @param in unit to convert from
     * @param out unit to convert to
     * @return QLinearFunction object containing conversion from in to out unit
     */
    QLinearFunction convert(const String & in, const String & out) const
    {
        if (m_rules.empty())
            return {};

        auto itIn  = m_rules.find(in);
        auto itOut = m_rules.find(out);

        if (in == m_baseUnit && itOut != m_rules.end())     // conversion from base unit to unit
            return itOut->second.convertFunction();
        if (itIn != m_rules.end() && out == m_baseUnit)    // conversion from unit to base unit
            return itIn->second.convertFunction().inverted();

        // conversion from one unit to another through the base unit if possible
        if (itIn == m_rules.end() || itOut == m_rules.end())
            return {};
        QLinearFunction inToBase  = itIn->second.convertFunction().inverted();
        QLinearFunction baseToOut = itOut->second.convertFunction();
        if (!inToBase.isValid() || !baseToOut.isValid())    // one of the conversions is not present
            return {};
        return QLinearFunction::combined(inToBase, baseToOut);
    }

    /**
     * @brief Converts a given value from in unit to out unit
     * @param value to convert
     * @param in unit to convert from
     * @param out unit to convert to
     * @return value converted to
     */
    double convert(double value, const String & in, const String & out) const
    {
        QLinearFunction function = convert(in, out);
        if (function.isValid())
            return function.y(value);
        return NAN;
    }

protected:
    std::map<String, QUnitConversionRule<String>> m_rules;  ///< Key is a unit, it's assumed that all rules have the same base unit
    String m_baseUnit;  ///< Base unit for this family
    String m_family;    ///< Family name
};

#endif // QUNITCONVERSIONFAMILY_H
