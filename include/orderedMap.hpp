/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   orderedMap.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/11 16:32:14 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/11 17:50:31 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ORDERED_MAP
# define ORDERED_MAP

#include <vector>
#include <unordered_map>

template<typename Key, typename Value>

class orderedMap
{
	private:
		std::vector<Key> keys;
		std::unordered_map<Key, Value> map;

	public:
		orderedMap() = default;
		~orderedMap() = default;

		const Value* begin() const
		{
			if (map.find(keys.at(0)) == map.end())
				return (nullptr);
			return (&map.at(keys.at(0)));
		}

		const Value* find(const Key& key) const
		{
			if (map.find(key) == map.end())
				return (nullptr);
			return (&map.at(key));
		}

		void insert(const Key& key, const Value& value)
		{
			if (map.find(key) == map.end())
				keys.push_back(key);
			map[key] = value;
		}

		void erase(const Key& key)
		{
			map.erase(key);
			keys.erase(std::remove(keys.begin(), keys.end(), key), keys.end());
		}

		Value& at(size_t pos) const
		{
			return (map.at(keys.at(pos)));
		}

		Value& operator[] (const Key& key)
		{
			if (map.find(key) == map.end())
				keys.push_back(key);
			return (map[key]);
		}

		const Value& operator[] (const Key& key) const
		{
			return (map.at(key));
		}

		size_t	size() const
		{
			return (map.size());
		}

		size_t	count(const Key& key) const
		{
			return (map.count(key));
		}
};

#endif