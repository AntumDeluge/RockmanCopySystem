#ifndef INIDICTIONARY_H
#define INIDICTIONARY_H

#include <map>
#include <string>

class IniDictionary {
  private:
  std::map<std::string, std::map<std::string, std::string> > m_dictionary;

  public:
  IniDictionary(const char* a_pFilename);
  bool getBoolValue(const char* a_pKey, const char* a_pSectionName, bool a_defaultValue);
  const char* getCStringValue(const char* a_pKey, const char* a_pSectionName, const char* a_defaultValue);
  int getIntValue(const char* a_pKey, const char* a_pSectionName, int a_defaultValue);
  void trim(std::string& a_string);
};

#endif