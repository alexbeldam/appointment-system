#ifndef MOCK_HPP
#define MOCK_HPP

#include <string>
#include <vector>

long insert(const std::string& filename, const std::string& data);

std::string find(const std::string& filename, long id);

std::vector<std::string> findByColumn(const std::string& filename, size_t index,
                                      const std::string& value);

std::vector<std::string> findAll(const std::string& filename);

void update(const std::string& filename, long id, const std::string& data);

void remove(const std::string& filename, long id);

#endif