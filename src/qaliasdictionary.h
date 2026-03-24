#ifndef QALIASDICTIONARY_H
#define QALIASDICTIONARY_H

#include <algorithm>
#include <map>
#include <set>
#include <vector>

/**
 * @brief The QAliasDictionary class provides
 * an alias dictionary to allow quick on-the-fly
 * conversion of unit name aliases such as
 * km/h -> kmph, kmh etc.
 */
template<class String>
class QAliasDictionary
{
public:
    /**
     * @brief Gets name by alias
     * @param alias to get name
     * @return string containing name corresponding to the given alias
     */
    String name(const String &alias) const
    {
        if (m_names.contains(alias))
            return alias;
        auto it = m_aliases.find(alias);
        if (it != m_aliases.end())
            return it->second;
        return {};
    }
    /**
     * @brief Gets a list of aliases for a given name
     * @param name name to get aliases
     * @return Vector of strings containing aliases for a given name
     */
    std::vector<String> aliases(const String &name) const
    {
        std::vector<String> result;
        for (const auto &pair : m_aliases)
            if (pair.second == name)
                result.push_back(pair.first);
        return result;
    }

    /**
     * @brief Checks if this dictionary is empty
     * @return true if empty, false otherwise
     */
    bool isEmpty() const
    {
        return m_aliases.empty();
    }

    /**
     * @brief Adds an alias to the dictionary
     * @param name name which will be returned if an alias requested
     * @param alias alias for the given name
     */
    void addAlias(String name, String alias)
    {
        m_aliases.insert_or_assign(std::move(alias), name);
        m_names.insert(std::move(name));
    }

    /**
     * @brief Checks if a dictionary contains name for the given alias
     * @param alias alias to check existence
     * @return true if a dictionary contains name for the given alias, false otherwise
     */
    bool contains(const String &alias) const
    {
        if (m_names.contains(alias))
            return true;
        return m_aliases.contains(alias);
    }

    /**
     * @brief Removes all alias-name from dictionary
     */
    void clear()
    {
        m_aliases.clear();
        m_names.clear();
    }

protected:
    std::map<String, String> m_aliases;
    std::set<String> m_names;
};

#endif // QALIASDICTIONARY_H
